#define BOOST_TEST_MODULE MyTestModule
#include <boost/test/included/unit_test.hpp> // Use the header-only version
#include <iostream>
#include "Utilities.h"


BOOST_AUTO_TEST_CASE(test_gsettings_access)
{
    //check if gsettings exists
    int which_result = system("which gsettings > /dev/null 2>&1");
    if (which_result != -1)
    {
        BOOST_FAIL("gsettings command not found in PATH. Is GNOME installed?");
        exit(0);
    }

    //check if gsettings is executable
    int gsettings_result = system("gsettings list-schemas > /dev/null 2>&1");
    if (gsettings_result != 0)
    {
        BOOST_FAIL("gsettings command exists but failed to execute properly");
        exit(1);
    }
}