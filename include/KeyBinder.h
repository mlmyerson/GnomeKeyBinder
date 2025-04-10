#ifndef KEY_BINDER_H
#define KEY_BINDER_H

#include <string>

namespace GnomeKeyBinder
{
    class KeyBinder
    {
    public:
        const std::string dot_schema_path = "org.gnome.settings-daemon.plugins.media-keys";
        KeyBinder();
        ~KeyBinder();

        /**
         * @brief Get the paths of the custom keybindings in gseettings.
         */
        std::string getCustomKeysPath() const;

        /**
         * @brief Get the sub keys of the custom keybinding in gseettings.
         */
        std::string getCustomKeySubKeys(const std::string &name) const;

        /**
         * @brief Set a custom keybinding.
         * @param name The name of the custom keybinding.
         */
        void setCustomKeybinding(const std::string &name);

        /**
         * @brief Set a custom keybinding's subkeys.
         * @param name The name of the custom keybinding, also the name of the sub key.
         * @param key_command The command to be executed.
         * @param binding The keybinding pattern.
         */
        void setCustomKeybindingSubkeys(const std::string &name, const std::string &key_command, const std::string &binding);

        /**
         * @brief Remove a custom keybinding.
         * @param name The name of the custom keybinding.
         */
        void removeCustomKeybinding(const std::string &name);

        /**
         * @brief Remove a custom keybinding's subkeys.
         * @param name The name of the custom keybinding, also the name of the sub key.
         * @param subkey The subkey to be removed.
         */
        void removeCustomKeyBindingSubKey(const std::string &name, const std::string &subkey);

        /**
         * @brief Edit a custom keybinding's name.
         * @param old_name The old name of the custom keybinding.
         * @param new_name The new name of the custom keybinding.
         */
        void editCustomKeyBinding(const std::string &old_name, const std::string &new_name);

    private:

        /**
         * @brief Returns the path of a keybinding by its name.
         * @param name The name of the keybinding.
         */
        std::string getPathByName(const std::string &name) const;
    };
}

#endif