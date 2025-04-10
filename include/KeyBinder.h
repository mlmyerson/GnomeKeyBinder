#ifndef KEY_BINDER_H
#define KEY_BINDER_H

#include <string>

namespace KeyBinder
{
    class KeyBinder
    {
        public:
            KeyBinder();
            ~KeyBinder();

            std::string getCustomKeysPath() const;
            std::string getCustomKeySubKeys(const std::string &name) const;

            void setCustomKeybinding(const std::string &name);
            void setCustomKeybindingSubkeys(const std::string &name, const std::string &key_command, const std::string &binding);

            void removeCustomKeybinding(const std::string &name);
            void removeCustomKeyBindingSubKey(const std::string &name, const std::string &subkey);

            void editCustomKeyBinding(const std::string &old_name, const std::string &new_name);
    };
}

#endif