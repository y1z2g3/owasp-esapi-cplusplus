/**
 * OWASP Enterprise Security API (ESAPI)
 *
 * This file is part of the Open Web Application Security Project (OWASP)
 * Enterprise Security API (ESAPI) project. For details, please see
 * http://www.owasp.org/index.php/ESAPI.
 *
 * Copyright (c) 2011 - The OWASP Foundation
 */

#pragma once

#include "EsapiCommon.h"
#include "Encryptor.h"
#include "crypto/PlainText.h"
#include "crypto/CipherText.h"
#include "crypto/SecretKey.h"
#include "crypto/MessageDigest.h"
#include "crypto/Crypto++Common.h"
#include "errors/EncryptionException.h"
#include "errors/InvalidArgumentException.h"

#include <string>

// Must be consistent with JavaEncryptor.java.
// http://owasp-esapi-java.googlecode.com/svn/trunk/src/main/java/org/owasp/esapi/reference/crypto/JavaEncryptor.java

namespace esapi
{
  class ESAPI_EXPORT DefaultEncryptor : public Encryptor
  {
  public:

    static String DefaultDigestAlgorithm();
    static unsigned int DefaultDigestIterations();

  public:

    /**
     * {@inheritDoc}
     * 
     * Hashes the data using the specified algorithm and the Java MessageDigest class. This method
     * first adds the salt, a separator (L":"), and the data, and then rehashes the specified number of iterations
     * in order to help strengthen weak passwords.
     */
    virtual String hash(const String &plaintext, const String &salt, unsigned int iterations = DefaultDigestIterations());

    virtual CipherText encrypt(const PlainText& plainText);

    virtual CipherText encrypt(const SecretKey& secretKey, const PlainText& plainText);

    virtual PlainText decrypt(const CipherText& /*cipherText*/)
    {
      return PlainText();
    }

    virtual PlainText decrypt(const SecretKey& secretKey, const CipherText& /*cipherText*/)
    {
      return PlainText();
    }

    virtual String sign(const String & /*message*/)
      {
        return String();
      }

    virtual bool verifySignature(const String &, const String &)
    {
      return false;
    }

    virtual String seal(const String &, long) throw (IntegrityException)
      {
        return String();
      }

    virtual String unseal(const String &)
      {
        return String();
      }

    virtual bool verifySeal(const String &)
    {
      return false;
    }

    virtual long getRelativeTimeStamp(long /*timeStamp*/)
    {
      return 0;
    }

    virtual long getTimeStamp()
    {
      return 0;
    }

  public:
    explicit DefaultEncryptor() { }
    virtual ~DefaultEncryptor() { }
  };
} // NAMESPACE

