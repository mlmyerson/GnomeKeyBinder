#ifndef UTILITIES_H
#define UTILITIES_H

#include <boost/process.hpp>
#include <boost/process/search_path.hpp>

namespace GnomeKeyBinder
{

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
}

#endif // UTILITIES_H