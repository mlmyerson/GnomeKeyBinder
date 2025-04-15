#include <iostream>
#include "KeyBinder.h"
#include "Utilities.h"


std::string GnomeKeyBinder::KeyBinder::getPathByName(const std::string &name) const
{
    std::string paths = getCustomKeysPath();
    // search string for name
    size_t pos = paths.find(name);
    // reverse search from name to the first comma or beginning of list
    size_t reverse_index = paths.rfind(",", pos);
    if (reverse_index == std::string::npos)
    {
        // open bracket at position 0
        reverse_index = 0;
    }
    // substring from reverse index to the length of the name
    std::string path = paths.substr(reverse_index, pos + name.length());

    // remove apostrophes
    path.erase(std::remove(path.begin(), path.end(), '\''), path.end());

    return path;
}

std::string GnomeKeyBinder::KeyBinder::getCustomKeysPath() const
{
    return exec("gsettings get " + dot_schema_path + " custom-keybindings");
}

std::string GnomeKeyBinder::KeyBinder::getCustomKeySubKeys(const std::string &name) const
{
    std::string exec_cmd = "gsettings list-recursively " + dot_schema_path + ":" + getPathByName(name);
    return exec(exec_cmd); 
}

void GnomeKeyBinder::KeyBinder::setCustomKeybinding(const std::string &name)
{
    std::string bindings_list = getCustomKeysPath();
    bool empty_list = false;
    auto brack1_pos = bindings_list.find("[");
    auto brack2_pos = bindings_list.find("]");

    if (brack1_pos == std::string::npos || brack2_pos == std::string::npos)
    {
        std::cerr << "Error: Invalid format for custom keybindings list." << std::endl;
        return;
        // check if brackets are enxt to each other "[]"
    }
    else if (brack2_pos - brack1_pos == 1)
    {
        empty_list = true;
    }

    // check if first item in list
    std::string add_comma;
    empty_list ? add_comma = "" : add_comma = ",";
}

void GnomeKeyBinder::KeyBinder::setCustomKeybindingSubkeys(const std::string &name, const std::string &key_command, const std::string &binding)
{
    std::string slash_path = ":" + getPathByName(name);
    exec("gsettings set " + dot_schema_path + slash_path + " " + "name" + " '" + name + "'");
    exec("gsettings set " + dot_schema_path + slash_path + " " + "command" + " '" + key_command+ "'");
    exec("gsettings set " + dot_schema_path + slash_path + " " + "binding" + " '" + binding + "'");
}

void GnomeKeyBinder::KeyBinder::removeCustomKeybinding(const std::string &name)
{
    // Implementation to remove a custom keybinding
}

void GnomeKeyBinder::KeyBinder::removeCustomKeyBindingSubKey(const std::string &name, const std::string &subkey)
{
    // Implementation to remove a custom keybinding's subkeys
}

void GnomeKeyBinder::KeyBinder::editCustomKeyBinding(const std::string &old_name, const std::string &new_name)
{
    // Implementation to edit a custom keybinding's name
}