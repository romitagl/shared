// Copyright (C) 2018 Gian Luigi Romita. All rights reserved.

#include "sslcrypto.h"

#include <openssl/conf.h>
// SSL I/F for all symmetric encryption algorithms
#include <openssl/evp.h>
#include <openssl/err.h>

#include <sstream>

namespace ssl_crypto
{
    typedef unsigned char byte;

    // constant definitions
    static std::string IV = "4DBA325EAA8DBC69"; // initialization vector 16 bytes randomly generated

    // multithreading locks
    static pthread_mutex_t *lock_cs = NULL;
    static long *lock_count = NULL;

    // multithreading callback
    void pthreads_thread_id(CRYPTO_THREADID *tid)
    {
        CRYPTO_THREADID_set_numeric(tid, (unsigned long)pthread_self());
    }

    // multithreading callback
    void pthreads_locking_callback(int mode, int type, const char *file, int line)
    {
        if (mode & CRYPTO_LOCK) {
            pthread_mutex_lock(&(lock_cs[type]));
            lock_count[type]++;
        } else {
            pthread_mutex_unlock(&(lock_cs[type]));
        }
    }

    // SSLCrypto implementation   
    SSLCrypto::SSLCrypto()
    {       
        // multithreading initialization
        thread_setup();

        // Load the human readable error strings for libcrypto 
        ERR_load_crypto_strings();

        // Load all digest and cipher algorithms 
        OpenSSL_add_all_ciphers();
        //OpenSSL_add_all_algorithms();

        // Load config file, and other important initialisation 
        OPENSSL_config(NULL);
    }

    SSLCrypto::~SSLCrypto()
    {
        cleanup();
    }

    SSLCrypto& SSLCrypto::Instance()
    {
        static SSLCrypto instance;
        return instance;
    }

    void SSLCrypto::thread_setup()
    {
        // multithreading initialization
        // OpenSSL initialization has to be performed only ONCE as the it's loaded globally in the target executable
        if(CRYPTO_THREADID_get_callback() != NULL)
        {
            lock_cs = (pthread_mutex_t*)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
            lock_count = (long int*)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(long));
            for (size_t i = 0; i < CRYPTO_num_locks(); i++) {
                lock_count[i] = 0;
                pthread_mutex_init(&(lock_cs[i]), NULL);
            }

            // CRYPTO_set_locking_callback() is available in all versions of SSLeay and OpenSSL.
            // CRYPTO_num_locks() was added in OpenSSL 0.9.4. All functions dealing with dynamic locks were added in OpenSSL 0.9.5b-dev.
            // CRYPTO_THREADID and associated functions were introduced in OpenSSL 1.0.0 to replace (actually, deprecate) the previous
            // CRYPTO_set_id_callback(), CRYPTO_get_id_callback(), and CRYPTO_thread_id() functions
            // which assumed thread IDs to always be represented by 'unsigned long'. 
            CRYPTO_THREADID_set_callback(pthreads_thread_id);
            CRYPTO_set_locking_callback(pthreads_locking_callback);  // this performs also OPENSSL_init 
        }
        //else // ALREADY SET
    }

    bool SSLCrypto::ValidateCryptoParams(const std::string& ciphername, const std::string& key, std::string& error)
    {
        static const std::string fname = "SSLCrypto::ValidateKey ";

        if(!key.empty() && !ciphername.empty())
        {
            // check for valid key
            {
                const EVP_CIPHER *cipher = EVP_get_cipherbyname(ciphername.c_str());
                if(!cipher)
                {
                    error = fname + "Error validating encryption params: ciphername: " + ciphername;
                    return false;
                }

                EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
                if(!ctx)
                { 
                    error = fname + "Error validating encryption params: CIPHER_CTX";
                    return false;
                }

                if(1 != EVP_EncryptInit_ex(ctx, cipher, NULL, NULL, (byte*)&IV[0]))
                { 
                    error = fname + "Error validating encryption params: EncryptInit";
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }

                int IV_size = EVP_CIPHER_CTX_iv_length(ctx);
                if(IV_size > 0 && (IV_size != IV.size()))
                {
                    std::stringstream iv_len;
                    iv_len << IV_size;
                    error = fname + " FAILED - cipher: " + ciphername + ", IV length not correct. Expected: " + iv_len.str();
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }

                // check the key length for the current cipher
                if(!EVP_CIPHER_CTX_set_key_length(ctx, key.size()))
                {
                    std::stringstream key_len;
                    key_len << EVP_CIPHER_key_length(cipher);
                    error = fname + " FAILED - cipher: " + ciphername + ", key length not correct. Expected: " + key_len.str();
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }

                if(1 != EVP_EncryptInit_ex(ctx, cipher, NULL, (byte*)&key[0], (byte*)&IV[0]))
                { 
                    error = fname + " Error validating encryption params: EncryptInit";
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }

                EVP_CIPHER_CTX_cleanup(ctx);
                EVP_CIPHER_CTX_free(ctx);
            }

            return true;
        }

        error = fname + " FAILED - empty param found - cipher:" + ciphername + " , key:" + key;
        return false;
    }

    void SSLCrypto::thread_cleanup(void)
    {
        if(!lock_cs) //not initialezed
        {
            return;
        }

        CRYPTO_set_locking_callback(NULL);

        for (size_t i=0; i<CRYPTO_num_locks(); i++)
        {
            pthread_mutex_destroy(&(lock_cs[i]));
        }
        OPENSSL_free(lock_cs);
    }

    void SSLCrypto::cleanup()
    {
        /* Removes all digests and ciphers */
        EVP_cleanup();

        /* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g. base64 transformations */
        CRYPTO_cleanup_all_ex_data();

        ERR_remove_thread_state(NULL);

        /* Remove error strings */
        ERR_free_strings();

        // multithreading cleanup
        thread_cleanup();    
    }

    // Encrypting consists of the following stages:
    //      Setting up a context
    //      Initialising the encryption operation
    //      Providing plaintext bytes to be encrypted
    //      Finalising the encryption operation
    bool SSLCrypto::Encrypt(const std::string& ciphername, const std::string& key, const std::string& plaintext, std::string& ciphertext, std::string& error)
    {
        static const std::string fname = "SSLCrypto::Encrypt ";

        const EVP_CIPHER *cipher = EVP_get_cipherbyname(ciphername.c_str());
        if(!cipher)
        {
            error = fname + "Error loading cipher: " + ciphername + ". Check crypto configuration";
            return false;
        }

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        // Create and initialise the context 
        if(!ctx)
        { 
            error = fname + "Impossible create cipher context";
            return false;
        }

        ciphertext.clear();

        // Initialise the encryption operation. IMPORTANT - ensure you use a key
        // and IV size appropriate for your cipher
        // The IV size for *most* modes is the same as the block size. For AES this is 128 bits
        if(1 != EVP_EncryptInit_ex(ctx, cipher, NULL, (byte*)&key[0], (byte*)&IV[0]))
        { 
            error = fname + "Failed to initialise the encryption operation";
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        // Recovered text expands up to EVP_MAX_BLOCK_LENGTH        
        //ciphertext.resize(plaintext.size()+EVP_MAX_BLOCK_LENGTH);
        int cipher_block_size = EVP_CIPHER_block_size(cipher);
        ciphertext.resize(plaintext.size() + cipher_block_size);
        int len = (int)ciphertext.size();

        // By default encryption operations are padded using standard block padding and the padding is checked and removed when decrypting.
        // If the pad parameter is zero then no padding is performed, 
        // the total amount of data encrypted or decrypted must then be a multiple of the block size or an error will occur

        // Provide the message to be encrypted, and obtain the encrypted output.
        // EVP_EncryptUpdate can be called multiple times if necessary
        if(1 != EVP_EncryptUpdate(ctx, (byte*)&ciphertext[0], &len, (const byte*)&plaintext[0], plaintext.size()))
        { 
            error = fname + "Failed to perform EVP_EncryptUpdate";
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        // Finalise the encryption. Further ciphertext bytes may be written at this stage.
        int ciphertext_len = (int)ciphertext.size() - len;
        if(1 != EVP_EncryptFinal_ex(ctx, (byte*)&ciphertext[0]+ len, &ciphertext_len))
        { 
            error = fname + "Failed to finalise the encryption";
            EVP_CIPHER_CTX_free(ctx);       
            return false;
        }
        ciphertext_len += len;        
        ciphertext.resize(ciphertext_len);

        // Clean up 
        EVP_CIPHER_CTX_cleanup(ctx);
        EVP_CIPHER_CTX_free(ctx);

        return true;
    }


    bool SSLCrypto::Decrypt(const std::string& ciphername, const std::string& key, const std::string& ciphertext, std::string& plaintext, std::string& error)
    {
        static const std::string fname = "SSLCrypto::Decrypt ";

        const EVP_CIPHER *cipher = EVP_get_cipherbyname(ciphername.c_str());
        if(!cipher)
        {
            error = fname + "Error loading cipher: " + ciphername + ". Check crypto configuration";
            return false;
        }

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        // Create and initialise the context 
        if(!ctx)
        { 
            error = fname + "Impossible create cipher context";
            return false;
        }

        plaintext.clear();

        // Initialise the encryption operation. IMPORTANT - ensure you use a key
        // and IV size appropriate for your cipher
        // The IV size for *most* modes is the same as the block size. For AES this is 128 bits
        if(1 != EVP_DecryptInit_ex(ctx, cipher, NULL, (byte*)&key[0], (byte*)&IV[0]))
        { 
            error = fname + "Failed to initialise the decryption operation";
            EVP_CIPHER_CTX_free(ctx);          
            return false;
        }

        // Recovered text expands up to EVP_MAX_BLOCK_LENGTH        
        plaintext.resize(ciphertext.size() + EVP_MAX_BLOCK_LENGTH);
        int len = (int)ciphertext.size();

        // By default encryption operations are padded using standard block padding and the padding is checked and removed when decrypting.
        // If the pad parameter is zero then no padding is performed, 
        // the total amount of data encrypted or decrypted must then be a multiple of the block size or an error will occur.

        // Provide the message to be encrypted, and obtain the encrypted output.
        // EVP_EncryptUpdate can be called multiple times if necessary
        if(1 != EVP_DecryptUpdate(ctx, (byte*)&plaintext[0], &len, (const byte*)&ciphertext[0], ciphertext.size()))
        { 
            error = fname + "Failed to perform EVP_DecryptUpdate";
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        // Finalise the encryption. Further ciphertext bytes may be written at this stage.
        int plaintext_len = (int)plaintext.size() - len;
        if(1 != EVP_DecryptFinal_ex(ctx, (byte*)&plaintext[0]+ len, &plaintext_len))
        { 
            error = fname + "Failed to finalise the decryption";
            EVP_CIPHER_CTX_free(ctx);        
            return false;
        }
        plaintext_len += len;        
        plaintext.resize(plaintext_len);

        // Clean up 
        EVP_CIPHER_CTX_cleanup(ctx);
        EVP_CIPHER_CTX_free(ctx);

        return true;
    }
}
