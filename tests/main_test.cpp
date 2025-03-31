#define BOOST_TEST_MODULE MyTestModule
#include <boost/test/included/unit_test.hpp> // Use the header-only version
#include <iostream>
#include "DconfSettings.h"
#include "Utilities.h"

BOOST_AUTO_TEST_CASE(test_gsettings_access)
{
    // Check if gsettings exists
    int which_result = system("which gsettings > /dev/null 2>&1");
    BOOST_TEST_REQUIRE(which_result == 0, "gsettings command not found in PATH");

    // Check if gsettings is executable
    int gsettings_result = system("gsettings list-schemas > /dev/null 2>&1");
    BOOST_TEST_REQUIRE(gsettings_result == 0, "gsettings command failed to execute properly");
}

//test dconf backup and restore 
BOOST_AUTO_TEST_CASE(test_key_view)
{
    //backup dconf
    KeyBinder::DconfSettings dconf_settings;

    //add a setting to gsettings
    dconf_settings.addKeyBinding("test_keybinding", "echo 'Hello World'", "<Ctrl>h");
    
    //check if key was added to custom-keybindings

    //check if the name, command, and binding were set correctly

    //restore dconf

}
