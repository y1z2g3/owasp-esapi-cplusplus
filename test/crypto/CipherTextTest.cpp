/*
 * OWASP Enterprise Security API (ESAPI)
 *
 * This file is part of the Open Web Application Security Project (OWASP)
 * Enterprise Security API (ESAPI) project. For details, please see
 * <a href="http://www.owasp.org/index.php/ESAPI">http://www.owasp.org/index.php/ESAPI</a>.
 *
 * Copyright (c) 2011 - The OWASP Foundation
 *
 * @author Kevin Wall, kevin.w.wall@gmail.com
 * @author Jeffrey Walton, noloader@gmail.com
 * @author David Anderson, david.anderson@aspectsecurity.com
 */

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#if !defined(ESAPI_OS_WINDOWS)
# define BOOST_TEST_DYN_LINK
# include <boost/test/unit_test.hpp>
using namespace boost::unit_test;
#endif

#include "EsapiCommon.h"

// auto_ptr is deprecated in C++0X
#if defined(ESAPI_CPLUSPLUS_UNIQUE_PTR)
# define THE_AUTO_PTR  std::unique_ptr
#else
# define THE_AUTO_PTR  std::auto_ptr
#endif

#include <string>
using std::string;

#include <crypto/CipherText.h>
using esapi::CipherText;

BOOST_AUTO_TEST_CASE( VerifyCipherText )
{
  BOOST_MESSAGE( "Verifying CipherText class" );
}