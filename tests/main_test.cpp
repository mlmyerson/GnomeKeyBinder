#define BOOST_TEST_MODULE MyTestModule
#include <boost/test/included/unit_test.hpp>  // Use the header-only version
#include <iostream> 

BOOST_AUTO_TEST_CASE(test_example)
{
    int x = 42;
    int y = 42;
    BOOST_TEST(x == y);
}