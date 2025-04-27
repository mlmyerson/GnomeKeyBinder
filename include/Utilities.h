#ifndef UTILITIES_H
#define UTILITIES_H

#include <boost/process.hpp>
#include <boost/process/search_path.hpp>

namespace GnomeKeyBinder
{

    /**
     * @brief Executes a shell command and returns the output as a string.
     * @param cmd The command to execute.
     */
    inline std::string exec(const std::string &cmd)
    {
        namespace bp = boost::process;

        std::string result;
        bp::ipstream out_stream;
        bp::ipstream err_stream;

        try
        {
            bp::child c(cmd, bp::std_out > out_stream, bp::std_err > err_stream);

            std::string line;
            while (out_stream && std::getline(out_stream, line))
                result += line + "\n";

            while (err_stream && std::getline(err_stream, line))
                result += "ERROR: " + line + "\n";

            c.wait();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error executing command: " << e.what() << std::endl;
        }

        return result;
    }

    /**
     * @brief Remove frist instance of a substring from a string.
     * @param str The string to modify.
     * @param substr The substring to remove.
     */
    inline std::string remove_first_instance(std::string str, const std::string &substr)
    {
        size_t pos = str.find(substr);
        if (pos != std::string::npos)
        {
            str.erase(pos, substr.length());
        }
        return str;
    }

    
}

#endif // UTILITIES_H