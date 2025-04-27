#include <iostream>
#include <string>
#include "KeyBinder.h"
#include "Utilities.h"

GnomeKeyBinder::KeyBinder::KeyBinder()
{
}

GnomeKeyBinder::KeyBinder::~KeyBinder()
{
}

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
        reverse_index = 1;
    }
    // substring from reverse index to the length of the name
    std::string path = paths.substr(reverse_index, pos + name.length()+1);
    path += "/";

    // remove apostrophes
    path.erase(std::remove(path.begin(), path.end(), '\''), path.end());

    return path;
}

std::string GnomeKeyBinder::KeyBinder::getCustomKeysPath() const
{
    std::string cmd = "gsettings get " + dot_schema_path + " custom-keybindings";
    std::string result = exec(cmd);
    result = remove_first_instance(result, "@as");
    return result;
}

std::string GnomeKeyBinder::KeyBinder::getCustomKeySubKeys(const std::string &name) const
{
    std::string exec_cmd = "gsettings list-recursively " + dot_schema_path + ":" + getPathByName(name);
    return exec(exec_cmd);
}

void GnomeKeyBinder::KeyBinder::setCustomKeybinding(const std::string &name)
{
    std::string bindings_list = getCustomKeysPath();

    // check if the name is already in the list
    if (bindings_list.find(name) != std::string::npos)
    {
        return;
    }

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

    std::string new_bindings_list = bindings_list.insert(brack2_pos, add_comma + "'" + slash_schema_path + name + "'");
    exec("gsettings set " + dot_schema_path + " custom-keybindings " + new_bindings_list);
}

void GnomeKeyBinder::KeyBinder::setCustomKeybindingSubkeys(const std::string &name, const std::string &key_command, const std::string &binding)
{
    std::string slash_path = ":" + getPathByName(name);
    std::string name_cmd = "gsettings set " + dot_schema_path + slash_path + " " + "name" + " '" + name + "'";
    std::string command_cmd = "gsettings set " + dot_schema_path + slash_path + " " + "command" + " '" + key_command + "'";
    std::string binding_cmd = "gsettings set " + dot_schema_path + slash_path + " " + "binding" + " '" + binding + "'";
    std::cout << "Name Command: " << name_cmd << std::endl; // DEBUG
    exec(name_cmd);
    exec(command_cmd);
    exec(binding_cmd);
}

void GnomeKeyBinder::KeyBinder::removeCustomKeybinding(const std::string &name)
{
    std::string paths = getCustomKeysPath();

    // check if paths is empty
    if (paths == "[]")
    {
        return;
    }

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
    size_t erase_pos = paths.find(path);

    if (erase_pos != std::string::npos)
    {
        paths.erase(erase_pos + 1, path.length());
    }

    if (paths.substr(1, 2) == ", ")
    {
        paths.erase(1, 2);
    }
    std::string cmd = "gsettings set " + dot_schema_path + " custom-keybindings " + paths;
    exec(cmd);
}

void GnomeKeyBinder::KeyBinder::removeCustomKeyBindingSubKey(const std::string &name, const std::string &subkey)
{
    std::string path = getPathByName(name);
    exec("gsettings reset " + dot_schema_path + ":" + path + " " + subkey);
}

void GnomeKeyBinder::KeyBinder::editCustomKeyBinding(const std::string &old_name, const std::string &new_name)
{
    // remove everything and reset everything
    // get subkeys first
    std::string subkeys = getCustomKeySubKeys(old_name);
    // search for each subkey
    /*
    org.gnome.settings-daemon.plugins.media-keys.custom-keybinding binding ''
    org.gnome.settings-daemon.plugins.media-keys.custom-keybinding command ''
    org.gnome.settings-daemon.plugins.media-keys.custom-keybinding name ''

    */
    size_t subkey_substr_pos = subkeys.find("name '");
    size_t next_apostophe = subkeys.find("'", subkey_substr_pos);
    std::string name_subkey = subkeys.substr(subkey_substr_pos, next_apostophe - subkey_substr_pos);

    subkey_substr_pos = subkeys.find("command '");
    next_apostophe = subkeys.find("'", subkey_substr_pos);
    std::string command_subkey = subkeys.substr(subkey_substr_pos, next_apostophe - subkey_substr_pos);

    subkey_substr_pos = subkeys.find("binding '");
    next_apostophe = subkeys.find("'", subkey_substr_pos);
    std::string binding_subkey = subkeys.substr(subkey_substr_pos, next_apostophe - subkey_substr_pos);

    // remove the old keybinding
    removeCustomKeybinding(old_name);

    // set the new keybinding
    setCustomKeybinding(new_name);
    setCustomKeybindingSubkeys(new_name, command_subkey, binding_subkey);
}