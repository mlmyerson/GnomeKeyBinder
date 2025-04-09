#define BOOST_TEST_MODULE MyTestModule
#include <boost/test/included/unit_test.hpp> // Use the header-only version
#include <iostream>
#include "DconfSettings.h"
#include "Utilities.h"
#include <regex>

BOOST_AUTO_TEST_SUITE(main_test_suite)

// Check if gsettings is installed and executable
BOOST_AUTO_TEST_CASE(test_gsettings_access)
{
    // Check if gsettings exists
    int which_result = system("which gsettings > /dev/null 2>&1");
    BOOST_TEST_REQUIRE(which_result == 0, "gsettings command not found in PATH");

    // Check if gsettings is executable
    int gsettings_result = system("gsettings list-schemas > /dev/null 2>&1");
    BOOST_TEST_REQUIRE(gsettings_result == 0, "gsettings command failed to execute properly");
}

// List custom keys paths
BOOST_AUTO_TEST_CASE(test_get_custom_keys_path)
{
    KeyBinder::KeyBinder Binder;
    std::regex pattern(R"(\[.*\])");
    std::smatch match;
    std::string result = KeyBinder::exec(Binder.getCustomKeysPath().c_str());
    bool search = std::regex_search(result, match, pattern);
    BOOST_TEST(search, "Key paths list not returned.");
}

// Set and remove a custom keybinding
BOOST_AUTO_TEST_CASE(test_set_custom_keybinding)
{
    std::string keybinding_name = "custom_test";
    KeyBinder::KeyBinder Binder;

    Binder.setCustomKeybinding(keybinding_name);
    std::string result = KeyBinder::exec(Binder.getCustomKeysPath().c_str());
    BOOST_TEST(result.find(keybinding_name) != std::string::npos, "Keybinding name not found");

    Binder.removeCustomKeybinding(keybinding_name);
    result = KeyBinder::exec(Binder.getCustomKeysPath().c_str());
    BOOST_TEST(result.find(keybinding_name) == std::string::npos, "Keybinding name found after removal");
}





// // test dconf backup and restore
// BOOST_AUTO_TEST_CASE(test_dconf_backup)
// {
//     // backup dconf
//     KeyBinder::DconfSettings dconf_settings;

//     // add a setting to gsettings
//     std::string name = "test_keybinding";
//     std::string key_cmd = "echo Hello World";
//     std::string key_bind = "<Ctrl>h";
//     dconf_settings.addKeyBinding(name, key_cmd, key_bind);

//     // check if key was added to custom-keybindings
//     std::string key_list = dconf_settings.getCustomKeybindings();

//     // check if key was removed from custom-keybindings
//     std::cout << "debug key_list: " << key_list << std::endl;

//     BOOST_TEST(key_list.find(dconf_settings.name_prepend + name) != std::string::npos, "Keybinding not found in custom-keybindings");

//     // check if the name, command, and binding were set correctly
//     std::string result = KeyBinder::exec("gsettings list-recursively org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/" + dconf_settings.name_prepend + name + "/");

//     BOOST_TEST(result.find("name '" + name + "'") != std::string::npos, "Keybinding name not found");

//     BOOST_TEST(result.find("command '" + key_cmd + "'") != std::string::npos, "Keybinding command not found");

//     BOOST_TEST(result.find("binding '" + key_bind + "'") != std::string::npos, "Keybinding binding not found");

//     std::cout << "DEBUG RESTORING DCONF" << std::endl;
//     // restore dconf
//     dconf_settings.dconfRestore();


//     // check if key was removed from custom-keybindings
//     key_list = dconf_settings.getCustomKeybindings();
//     std::cout << "debug (after restore) key_list: " << key_list << std::endl;

//     BOOST_TEST(key_list.find(dconf_settings.name_prepend + name) == std::string::npos, "Keybinding was found in custom-keybindings after restore");

// }


BOOST_AUTO_TEST_SUITE_END()