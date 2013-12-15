#include "ltguard/guard.h"

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Guard
#include <boost/test/unit_test.hpp>

#include <ts/ts.h>
#include <ts/mock.h>

BOOST_AUTO_TEST_CASE( my_test1 )
{
  tsmock_init("abc", 1, 2, 3);
  BOOST_CHECK_EQUAL(2+2, 4);
}

BOOST_AUTO_TEST_CASE( my_test2 )
{
  BOOST_CHECK_EQUAL(2+2, 4);
}

BOOST_AUTO_TEST_CASE( my_test3 )
{
  BOOST_CHECK_EQUAL(2+2, 4);
}
