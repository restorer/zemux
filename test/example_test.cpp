#define BOOST_TEST_MODULE Example
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(example_test)
{
    int val = 42;
    BOOST_CHECK(val == 42);
}
