#include <boost/process.hpp>
#include <boost/process/search_path.hpp>

// Replace your current exec function with this
std::string exec(const std::string& cmd) 
{
    namespace bp = boost::process;
    
    std::string result;
    bp::ipstream pipe_stream;
    bp::child c(cmd, bp::std_out > pipe_stream, bp::std_err > pipe_stream);
    
    std::string line;
    while (pipe_stream && std::getline(pipe_stream, line))
        result += line + "\n";
        
    c.wait();
    
    return result;
}