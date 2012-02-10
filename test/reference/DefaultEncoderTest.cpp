/*
 * OWASP Enterprise Security API (ESAPI)
 *
 * This file is part of the Open Web Application Security Project (OWASP)
 * Enterprise Security API (ESAPI) project. For details, please see
 * http://www.owasp.org/index.php/ESAPI.
 *
 * Copyright (c) 2011 - The OWASP Foundation
 *
 * @author Daniel Amodio, dan.amodio@aspectsecurity.com
 *
 */

#if defined(_WIN32)
    #if defined(STATIC_TEST)
        // do not enable BOOST_TEST_DYN_LINK
    #elif defined(DLL_TEST)
        #define BOOST_TEST_DYN_LINK
    #else
        #error "For Windows you must define either STATIC_TEST or DLL_TEST"
    #endif
#else
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "EsapiCommon.h"
using esapi::Char;
using esapi::String;

#include "reference/DefaultEncoder.h"
#include "codecs/Codec.h"
#include "codecs/UnixCodec.h"
using esapi::UnixCodec;
using esapi::DefaultEncoder;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#if !defined(ESAPI_BUILD_RELEASE)
BOOST_AUTO_TEST_CASE( DefaultEncoderTestCase )
{
    // original 2012.01.29 jAHOLMES
	// esapi::DefaultEncoder de;
    esapi::Encoder & de = const_cast<esapi::Encoder &>( esapi::DefaultEncoder::getInstance() );

	shared_ptr<UnixCodec>uc1(new esapi::UnixCodec);

	String encoded = de.encodeForOS(uc1.get(), L"asdf<");
	BOOST_CHECK(encoded.compare(L"asdf\\<") == 0);

	shared_ptr<UnixCodec>uc2(new esapi::UnixCodec);
	BOOST_CHECK(de.encodeForOS(uc2.get(), L"sdf:ff").compare(L"sdf\\:ff")==0);

    // original 2012.01.29 jAHOLMES
	// encoded = de.encodeForBase64(L"asdf");
	encoded = de.encodeForBase64(L"asdf", false);
	BOOST_CHECK(encoded.compare(L"YXNkZg==") == 0); //base64 value of `asdf`
	BOOST_CHECK(de.decodeFromBase64(encoded).compare(L"asdf")==0);

	BOOST_CHECK(de.encodeForLDAP(L"asd\\f").compare(L"asd\\5cf")==0);

}
#endif

