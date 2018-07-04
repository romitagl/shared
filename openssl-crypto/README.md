Copyright (c) 2018 Gian Luigi Romita (romitagl@gmail.com)

# openssl-crypto
Wrapper on the OpenSSL crypto library to easily perform Encryption/Decryption operations and validate encryption configuration.
- sslcrypto.h offers a high level, easy-to-use I/F to the lower level OpenSSL EVP.
- main.cpp shows how to integrate the sslcrypto module in your code and perform crypto operations
## Dependencies:
- OpenSSL library (https://github.com/openssl/openssl)
## To compile:
- create a symbolic link to the openssl folder (eg. 'ln -s /your_openssl_path openssl')
- make
## To run:
- execute the 'run_sslcrypto' sample file that performs an encryption and decryption test after validating the crypto parameters.
## Platform supported:
- Tested on Linux Red Hat 4.8.5 - GCC 4.8.5
