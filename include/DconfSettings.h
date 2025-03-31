#ifndef DCONFSETTINGS_H
#define DCONFSETTINGS_H
#include <string>
#include <fstream>
#include <unistd.h>
#include "Utilities.h"

namespace KeyBinder
{
    class DconfSettings
    {

    private:
        const std::string custom_keybindings_dot_schema_path = "org.gnome.settings-daemon.plugins.media-keys";
        const std::string custom_keybindings_slash_schema_path = "/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/";

        std::string dconf_backup_store;

    public:
        std::string name_prepend = "custom_";

        DconfSettings()
        {
            // Backup dconf settings
            dconf_backup_store = dconfBackup();
        }

        ~DconfSettings();

        std::string dconfBackup()
        {
            const std::string cmd = "dconf dump /";
            std::string result = exec(cmd);
            return result;
        }

        void dconfRestore()
        {
            std::string temp_file = "/tmp/dconf_backup_" + std::to_string(getpid()) + ".ini";
            std::ofstream ofs(temp_file);
            ofs << dconf_backup_store;
            ofs.close();

            const std::string cmd = "dconf load / < " + temp_file;
            exec(cmd);

            std::remove(temp_file.c_str());
        }

        std::string getCustomKeybindings()
        {
            std::string key_list = exec("gsettings get " + custom_keybindings_dot_schema_path + " custom-keybindings");

            return key_list;
        }

        void addKeyBinding(const std::string &name, const std::string &key_command, const std::string &binding)
        {
            // get the current list
            std::string bindings_list = getCustomKeybindings();

            // check if first item in list
            std::string add_comma;
            bindings_list == "[]" ? add_comma = "" : add_comma = ",";

            std::string new_key = name_prepend + name;
            // append new key to list
            bindings_list.insert(bindings_list.length() - 1, add_comma + custom_keybindings_slash_schema_path + new_key + "/'");

            // set the new list
            std::string cmd = "gsettings set " + custom_keybindings_dot_schema_path + " custom-keybindings " + bindings_list;
            exec(cmd);

            // set name
            cmd = "gsettings set " + custom_keybindings_dot_schema_path + " custom-keybindings:" + custom_keybindings_slash_schema_path + new_key + " name '" + name + "'";
            exec(cmd);

            // set command
            cmd = "gsettings set " + custom_keybindings_dot_schema_path + " custom-keybindings:" + custom_keybindings_slash_schema_path + new_key + " name '" + key_command + "'";
            exec(cmd);

            // set keybinding
            cmd = "gsettings set " + custom_keybindings_dot_schema_path + " custom-keybindings:" + custom_keybindings_slash_schema_path + new_key + " name '" + binding + "'";
            exec(cmd);
        }
    };
}

#endif // DCONFSETTINGS_H