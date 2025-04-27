#define BOOST_TEST_MODULE MyTestModule
#include <boost/test/included/unit_test.hpp> // Use the header-only version
#include <iostream>
#include "KeyBinder.h"
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
    GnomeKeyBinder::KeyBinder Binder;
    std::regex pattern(R"(\[.*\])");
    std::smatch match;
    std::string result = Binder.getCustomKeysPath();
    bool search = std::regex_search(result, match, pattern);
    BOOST_TEST(search, "Key paths list not returned.");
}

// Set and remove a custom keybinding
BOOST_AUTO_TEST_CASE(test_custom_keybinding)
{
    std::string keybinding_name = "custom_test";
    GnomeKeyBinder::KeyBinder Binder;

    try
    {
        Binder.setCustomKeybinding(keybinding_name);
        std::string result = Binder.getCustomKeysPath();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        BOOST_TEST(false, "Keybinding name already exists in the list.");
    }

    std::string result = Binder.getCustomKeysPath();
    BOOST_TEST(result.find(keybinding_name) != std::string::npos, "Keybinding name not found");

    Binder.removeCustomKeybinding(keybinding_name);
    result = Binder.getCustomKeysPath();
    BOOST_TEST(result.find(keybinding_name) == std::string::npos, "Keybinding name found after removal");
}

// Set and remove subkeys of a custom keybinding
BOOST_AUTO_TEST_CASE(test_custom_subkeys)
{
    std::string keybinding_name = "custom_test";
    std::string command = "echo test_command";
    std::string binding = "<Ctrl>h";
    GnomeKeyBinder::KeyBinder Binder;

    Binder.setCustomKeybinding(keybinding_name);
    Binder.setCustomKeybindingSubkeys(keybinding_name, command, binding);
    std::string result = GnomeKeyBinder::exec(Binder.getCustomKeySubKeys(keybinding_name).c_str());

    bool found_name = result.find("name " + keybinding_name) != std::string::npos;
    bool found_command = result.find("command " + command) != std::string::npos;
    bool found_binding = result.find("binding " + binding) != std::string::npos;
    BOOST_TEST(found_name, "Keybinding name not found");
    BOOST_TEST(found_command, "Command not found");
    BOOST_TEST(found_binding, "Binding pattern not found");

    Binder.removeCustomKeyBindingSubKey(keybinding_name, keybinding_name);
    Binder.removeCustomKeyBindingSubKey(keybinding_name, command);
    Binder.removeCustomKeyBindingSubKey(keybinding_name, binding);

    found_name = result.find("name " + keybinding_name) != std::string::npos;
    found_command = result.find("command " + command) != std::string::npos;
    found_binding = result.find("binding " + binding) != std::string::npos;
    BOOST_TEST(!found_name, "Keybinding name was found after removal");
    BOOST_TEST(!found_command, "Command was found after removal");
    BOOST_TEST(!found_command, "Command was found after removal");
}

BOOST_AUTO_TEST_CASE(test_edit_keybinding)
{
    std::string keybinding_name_1 = "custom_test_1";
    std::string keybinding_name_2 = "custom_test_2";

    GnomeKeyBinder::KeyBinder Binder;
    Binder.setCustomKeybinding(keybinding_name_1);
    Binder.editCustomKeyBinding(keybinding_name_1, keybinding_name_2);

    std::string result = GnomeKeyBinder::exec(Binder.getCustomKeysPath().c_str());
    BOOST_TEST(result.find(keybinding_name_1) == std::string::npos, "Original Keybinding name found after replacement");
    BOOST_TEST(result.find(keybinding_name_2) != std::string::npos, "New Keybinding name not found");
}

BOOST_AUTO_TEST_CASE(test_edit_subkey)
{
    std::string keybinding_name_1 = "custom_test_1";
    std::string command = "echo test_command";
    std::string binding = "<Ctrl>h";
    GnomeKeyBinder::KeyBinder Binder;

    Binder.setCustomKeybinding(keybinding_name_1);
    Binder.setCustomKeybindingSubkeys(keybinding_name_1, command, binding);
    std::string result = GnomeKeyBinder::exec(Binder.getCustomKeySubKeys(keybinding_name_1).c_str());

    // ensure the keybinding and subkeys are set
    bool found_name = result.find("name " + keybinding_name_1) != std::string::npos;
    bool found_command = result.find("command " + command) != std::string::npos;
    bool found_binding = result.find("binding " + binding) != std::string::npos;
    BOOST_TEST(found_name, "Keybinding name not found");
    BOOST_TEST(found_command, "Command not found");
    BOOST_TEST(found_binding, "Binding pattern not found");

    // edit the name
    std::string keybinding_name_2 = "custom_test_2";
    Binder.editCustomKeyBinding(keybinding_name_1, keybinding_name_2);
    // edit the subkeys
    command = "echo test_command_2";
    binding = "<Ctrl>j";
    Binder.setCustomKeybindingSubkeys(keybinding_name_2, command, binding);

    // ensure the keybinding and subkeys are set
    found_name = result.find("name " + keybinding_name_2) != std::string::npos;
    found_command = result.find("command " + command) != std::string::npos;
    found_binding = result.find("binding " + binding) != std::string::npos;
    BOOST_TEST(found_name, "New Keybinding name not found");
    BOOST_TEST(found_command, "New Command not found");
    BOOST_TEST(found_binding, "New Binding pattern not found");
}

BOOST_AUTO_TEST_SUITE_END()