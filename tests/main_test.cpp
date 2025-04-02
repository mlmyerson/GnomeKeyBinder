#define BOOST_TEST_MODULE MyTestModule
#include <boost/test/included/unit_test.hpp> // Use the header-only version
#include <iostream>
#include "DconfSettings.h"
#include "Utilities.h"

BOOST_AUTO_TEST_SUITE(main_test_suite)

BOOST_AUTO_TEST_CASE(test_gsettings_access)
{
    // Check if gsettings exists
    int which_result = system("which gsettings > /dev/null 2>&1");
    BOOST_TEST_REQUIRE(which_result == 0, "gsettings command not found in PATH");

    // Check if gsettings is executable
    int gsettings_result = system("gsettings list-schemas > /dev/null 2>&1");
    BOOST_TEST_REQUIRE(gsettings_result == 0, "gsettings command failed to execute properly");
}

// test dconf backup and restore
BOOST_AUTO_TEST_CASE(test_dconf_backup)
{
    // backup dconf
    KeyBinder::DconfSettings dconf_settings;

    // add a setting to gsettings
    std::string name = "test_keybinding";
    std::string key_cmd = "echo Hello World";
    std::string key_bind = "<Ctrl>h";
    dconf_settings.addKeyBinding(name, key_cmd, key_bind);

    // check if key was added to custom-keybindings
    std::string key_list = dconf_settings.getCustomKeybindings();

    // check if key was removed from custom-keybindings
    std::cout << "debug key_list: " << key_list << std::endl;

    BOOST_TEST_REQUIRE(key_list.find(dconf_settings.name_prepend + name) != std::string::npos, "Keybinding not found in custom-keybindings");

    // check if the name, command, and binding were set correctly
    std::string result = KeyBinder::exec("gsettings list-recursively org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/" + dconf_settings.name_prepend + name + "/");

    BOOST_TEST_REQUIRE(result.find("name '" + name + "'") != std::string::npos, "Keybinding name not found");

    BOOST_TEST_REQUIRE(result.find("command '" + key_cmd + "'") != std::string::npos, "Keybinding command not found");

    BOOST_TEST_REQUIRE(result.find("binding '" + key_bind + "'") != std::string::npos, "Keybinding binding not found");

    // restore dconf
    dconf_settings.dconfRestore();


    // check if key was removed from custom-keybindings
    key_list = dconf_settings.getCustomKeybindings();
    std::cout << "debug key_list: " << key_list << std::endl;

    BOOST_TEST_REQUIRE(key_list.find(dconf_settings.name_prepend + name) == std::string::npos, "Keybinding was found in custom-keybindings after restore");

}


BOOST_AUTO_TEST_SUITE_END()