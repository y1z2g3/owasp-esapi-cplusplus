/*
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
 * @author David Anderson, david.anderson@aspectsecurity.com
 *
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test;

#include "EsapiCommon.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;
using std::istringstream;
using std::ostringstream;

#include <errno.h>

#include <crypto/SecureRandom.h>
using esapi::SecureRandom;

// Some worker thread stuff
static void DoWorkerThreadStuff();
static void* WorkerThreadProc(void* param);

static const unsigned int THREAD_COUNT = 64;

BOOST_AUTO_TEST_CASE( VerifySecureRandom )
{
  BOOST_MESSAGE( "Verifying SecureRandom with " << THREAD_COUNT << " threads" );

  DoWorkerThreadStuff();
}

#if defined(WIN32) || defined(_WIN32) 
void DoWorkerThreadStuff()
{
}
#elif defined(__linux) || defined(__linux__) || defined(__APPLE__)
void DoWorkerThreadStuff()
{
  pthread_t threads[THREAD_COUNT];

  // *** Worker Threads ***
  for(unsigned int i=0; i<THREAD_COUNT; i++)
    {
      int ret = pthread_create(&threads[i], nullptr, WorkerThreadProc, (void*)i);
      if(0 != ret /*success*/)
        {
          BOOST_ERROR( "pthread_create failed (thread " << i << "): " << strerror(errno) );
        }
    }

  for(unsigned int i=0; i<THREAD_COUNT; i++)
    {
      int ret = pthread_join(threads[i], nullptr);
      if(0 != ret /*success*/)
        {
          BOOST_ERROR( "pthread_join failed (thread " << i << "): " << strerror(errno) );
        }
    }

  BOOST_MESSAGE( "All threads completed successfully" );
}
#endif

static SecureRandom g_prng = SecureRandom::getInstance("HmacSHA-256");;

void* WorkerThreadProc(void* param)
{
  // give up the remainder of this time quantum to help
  // interleave thread creation and execution
#if defined(WIN32) || defined(_WIN32) 
  Sleep(0);
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
  sleep(0);
#endif

  byte random[8192];

  // This is the intended usage we envision - a single shared PRNG
  g_prng.nextBytes(random, sizeof(random));

  SecureRandom prng1;
  for (unsigned int i = 0; i < 64; i++)
    prng1.nextBytes(random, i+1);

  prng1.nextBytes(random, sizeof(random));

  SecureRandom prng2 = SecureRandom::getInstance("SHA-256");
  prng2.nextBytes(random, sizeof(random));

  for (unsigned int i = 0; i < 64; i++)
    prng2.setSeed(random, i+8);

  SecureRandom prng3 = prng1;
  for (unsigned int i = 0; i < 64; i++)
    prng3.nextBytes(random, i+1);

  // 1 and 3 are the same generators
  prng1.setSeed((int)(size_t)param);
  prng3.setSeed((int)(size_t)param);

  prng1.nextBytes(random, sizeof(random));
  prng3.nextBytes(random, sizeof(random));

  BOOST_CHECK(prng1.getAlgorithm() == prng3.getAlgorithm());

  SecureRandom prng4 = SecureRandom::getInstance("SHA-512");
  for (unsigned int i = 0; i < 64; i++)
  {
    prng4.setSeed(random, 128);
    prng4.nextBytes(random, sizeof(random));
  }

  BOOST_CHECK(prng2.getAlgorithm() != prng4.getAlgorithm());

  // 1, 3 and 5 are the same generators
  SecureRandom prng5(prng1);
  BOOST_CHECK(prng1.getAlgorithm() == prng5.getAlgorithm());
  BOOST_CHECK(prng3.getAlgorithm() == prng5.getAlgorithm());

  BOOST_MESSAGE( "Thread " << (size_t)param << " completed" );

  return (void*)0;
}
