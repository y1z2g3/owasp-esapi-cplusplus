/**
 * OWASP Enterprise Security API (ESAPI)
 *
 * This file is part of the Open Web Application Security Project (OWASP)
 * Enterprise Security API (ESAPI) project. For details, please see
 * http://www.owasp.org/index.php/ESAPI.
 *
 * Copyright (c) 2011 - The OWASP Foundation
 *
 * @author Kevin Wall, kevin.w.wall@gmail.com
 * @author Jeffrey Walton, noloader@gmail.com
 *
 */

#pragma once

#include "EsapiCommon.h"
#include "util/Mutex.h"
#include "crypto/SecretKey.h"
#include "errors/EncryptionException.h"
#include "errors/InvalidArgumentException.h"

#include <string>
#include <vector>

ESAPI_MS_WARNING_PUSH(3)
#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
ESAPI_MS_WARNING_POP()

// Crypto++ is MT safe at the class level, meaning it does not share data amoung
// instances. If a Global PRNG is provided, we must take care to ensure only one 
// thread is operating on it at a time since there's only one set of data within
// the class (ie, there is no thread local storage). To date, we only support
// Windows, Linux, and Apple due to Mutex and MutexAutoLock.

namespace esapi
{
  /**
   * This class implements functionality similar to Java's SecureRandom for consistency
   * http://download.oracle.com/javase/6/docs/api/java/security/SecureRandom.html
   */
  class ESAPI_EXPORT SecureRandom
  {
  public:
    // Retrieve a reference to the global PRNG.
    static SecureRandom& GlobalSecureRandom() throw();

    // Create an instance PRNG. Throws an EncryptionException if the
    // class fails to initialize its lock for thread safe operations.
    explicit SecureRandom() throw(EncryptionException);

    // Create an instance PRNG with a seed. Throws an InvalidArgumentException
    // if the arguments are not valid and an EncryptionException if
    // the class fails to initialize its lock for thread safe operations.
    explicit SecureRandom(const byte* seed, size_t size) throw(InvalidArgumentException, EncryptionException);

    // Create an instance PRNG with a seed. Throws an EncryptionException if
    // the class fails to initialize its lock for thread safe operations.
    explicit SecureRandom(const std::vector<byte>& seed) throw(EncryptionException);

    // Standard destructor.
    virtual ~SecureRandom() throw();

    // Returns the name of the algorithm implemented by this SecureRandom object.
    virtual std::string getAlgorithm() const throw();

    // Generates a user-specified number of random bytes. Throws an
    // InvalidArgumentException if the arguments are not valid or and
    // EncryptionException for a failure to generate the requested
    // number of bytes.
    void nextBytes(byte* bytes, size_t size) throw(InvalidArgumentException, EncryptionException);   

    // Generates a user-specified number of random bytes. Throws an
    // EncryptionException if the arguments are not valid or there is
    // a failure to generate the requested number of bytes.
    void nextBytes(std::vector<byte>& bytes) throw(EncryptionException);

    // Reseeds this random object. Throws an InvalidArgumentException
    // if the arguments are not valid and EncryptionException if there
    // is a failure incorporating the entropy.
    void setSeed(const byte* seed, size_t size) throw(InvalidArgumentException, EncryptionException);

    // Reseeds this random object. Throws an EncryptionException
    // if the arguments are not valid or there is a failure
    // incorporating the entropy.
    void setSeed(const std::vector<byte>& seed) throw(EncryptionException);

    // Reseeds this random object, using the bytes contained in
    // the given long seed. Throws an EncryptionException
    // if there is a failure incorporating the entropy.
    void setSeed(long seed) throw(EncryptionException);

  protected:

    // Crypto++ is MT safe at the class level, meaning it does not share data amoung
    // instances. If a Global PRNG is provided, we must take care to ensure only one 
    // thread is operating on it at a time since there's only one set of data within
    // the class (ie, there is no thread local storage). To date, we only support
    // Windows, Linux, and Apple due to Mutex and MutexAutoLock.

    // An instance PRNG used as a seeder
    CryptoPP::AutoSeededX917RNG<CryptoPP::AES> prng;   

    // Lock for using our PRNG and the Crypto++ PRNG
    Mutex m_mutex;
  };

}; // NAMESPACE esapi