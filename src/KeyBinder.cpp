#include "KeyBinder.h"
#include "Utilities.h"


std::string GnomeKeyBinder::KeyBinder::getPathByName(const std::string &name) const
{
    std::string paths = getCustomKeysPath();
    //search string for name
    size_t pos = paths.find(name);
    //reverse search from name to the first comma or beginning of list
    size_t reverse_index = paths.rfind(",", pos);
    if (reverse_index == std::string::npos)
    {
        //open bracket at position 0
        reverse_index = 0;
    }
    //substring from reverse index to the length of the name
    std::string path = paths.substr(reverse_index, pos + name.length() - 1);
    return path;
}


std::string GnomeKeyBinder::KeyBinder::getCustomKeysPath() const
{
    return exec("gsettings get " + dot_schema_path + " custom-keybindings");
}

std::string GnomeKeyBinder::KeyBinder::getCustomKeySubKeys(const std::string &name) const
{
    return "/path/to/custom/keys/" + name; // Replace with actual path
}

void GnomeKeyBinder::KeyBinder::setCustomKeybinding(const std::string &name)
{
    // Implementation to set a custom keybinding
}

void GnomeKeyBinder::KeyBinder::setCustomKeybindingSubkeys(const std::string &name, const std::string &key_command, const std::string &binding)
{
    // Implementation to set a custom keybinding's subkeys
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