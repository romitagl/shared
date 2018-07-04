// Copyright (C) 2018 Gian Luigi Romita. All rights reserved.

#include <iostream>
#include "sslcrypto.h"

using namespace ssl_crypto;

// this test executable provides usage examples of the SSLCrypto class
static const std::string key = "01234567890123456789012345678901";
static const std::string cipher = "aes-256-cbc";

int main()
{  
  std::cout << "Starting SSLCrypto tests..." << std::endl;

  SSLCrypto& crypto = SSLCrypto::Instance();

  std::string error;
  if(!crypto.ValidateCryptoParams(cipher, key, error))
  {
    std::cerr << error << std::endl;
    return EXIT_FAILURE;
  }

  std::string plaintext = "test string to encrypt";
  std::string ciphertext;
  if(!crypto.Encrypt(cipher, key, plaintext, ciphertext, error))
  {
    std::cerr << error << std::endl;
    return EXIT_FAILURE;
  }

  std::string decrypted_plaintext;
  if(!crypto.Decrypt(cipher, key, ciphertext, decrypted_plaintext, error))
  {
    std::cerr << error << std::endl;
    return EXIT_FAILURE;
  }

  if(plaintext != decrypted_plaintext)
  {
    std::cerr << "Decrypted buffer not matching what expected: \n" << plaintext << std::endl << decrypted_plaintext << std::endl;
    return EXIT_FAILURE;
  }
 
  std::cout << "SSLCrypto tests completed successfully!" << std::endl;
  return EXIT_SUCCESS;
}
