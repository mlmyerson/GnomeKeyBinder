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
    public:
        DconfSettings();
        ~DconfSettings();

    private:
        std::string dconf_backup()
        {
            const std::string cmd = "dconf dump /";
            std::string result = exec(cmd);
            return result;
        }

        void dconf_restore()
        {
            std::string temp_file = "/tmp/dconf_backup_" + std::to_string(getpid()) + ".ini";
            std::ofstream ofs(temp_file);
            ofs << backup;
            ofs.close();
            
            const std::string cmd = "dconf load / < " + temp_file;
            exec(cmd);
            
            std::remove(temp_file.c_str());
        }

        std::string backup;
    };
}

#endif // DCONFSETTINGS_H