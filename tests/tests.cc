#include <iostream>
#include "../parser.h"
#include "../parser.cc"
#define BOOST_TEST_MODULE tests

//#include "QuickStart.hpp"

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>



BOOST_AUTO_TEST_SUITE(QuickStartSuite) // use this to define a "group" of testcases to be run as 1

BOOST_AUTO_TEST_CASE(AdditionTest)
{
    BOOST_CHECK_EQUAL(2+1,3);
}

BOOST_AUTO_TEST_CASE(SubtractionTest)
{
    BOOST_CHECK_EQUAL(2-1,1);
}

BOOST_AUTO_TEST_SUITE_END()