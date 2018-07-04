// Copyright (C) 2018 Gian Luigi Romita. All rights reserved.

#ifndef _SSL_CRYPTO_H_
#define _SSL_CRYPTO_H_

#include <string>

namespace ssl_crypto
{
    // OpenSSL provides two primary libraries: libssl and libcrypto. 
    // The libcrypto library provides the fundamental cryptographic routines used 
    // by libssl. You can however use libcrypto without using libssl. 
    class SSLCrypto
    {
    public:
        /**
        * SSLCrypto API - Initialize
        *
        * create singleton, perform initialization and return instance
        */
        static SSLCrypto& Instance();

        /**
        * SSLCrypto API - Encrypt
        *
        * @param ciphername - input cipher 
        * @param key - input key used for crypto operation
        * @param plaintext - input plain binary buffer to encrypt         
        * @param ciphertext - output encrypted binary buffer 
        * @param error - output error string set on failure
        * @return bool - true on success, false on failure
        */
        bool Encrypt(const std::string& ciphername, const std::string& key, const std::string& plaintext, std::string& ciphertext, std::string& error);

        /**
        * SSLCrypto API - Decrypt
        *
        * @param ciphername - input cipher 
        * @param key - input key used for crypto operation
        * @param ciphertext - input encrypted binary buffer to decrypt
        * @param plaintext - output plain binary buffer 
        * @param error - output error string set on failure
        * @return bool - true on success, false on failure
        */
        bool Decrypt(const std::string& ciphername, const std::string& key, const std::string& ciphertext, std::string& plaintext, std::string& error);

        /**
        * SSLCrypto API - ValidateKey
        *
        * @param key - input key used for crypto operation
        * @param ciphername - input cipher 
        * @param error - output error string set on failure
        * @return bool - true on success, false on failure
        */
        bool ValidateCryptoParams(const std::string& ciphername, const std::string& key, std::string& error);

        private:
            SSLCrypto();    
            SSLCrypto(const SSLCrypto&);
            const SSLCrypto& operator=(const SSLCrypto&);
            ~SSLCrypto();

        private:
            void cleanup();
            void thread_setup();
            void thread_cleanup(void);  
    };
}

#endif //_SSL_CRYPTO_H_

