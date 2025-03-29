#include <iostream>
#include <array>
#include <memory>
#include <algorithm>

const std::string VIEW_SCHEMA = "--view-schema";
const std::string RESET_SCHEMA = "--reset-schema";

void usage()
{
  std::cout << "Options: \n"
            << "  " << VIEW_SCHEMA << ": View the keybindings in a schema\n"
            << "  " << RESET_SCHEMA << ": Reset the entire schema to empty\n"
            << std::endl;
}

// execute a system command and return the output
std::string exec(const char *cmd)
{
  std::array<char, 128> buffer;
  std::string result;
  std::string command(cmd);
  command += " 2>&1";

  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

  if (!pipe)
  {
    throw std::runtime_error("popen() failed!");
  }

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
  {
    result += buffer.data();
  }

  return result;
}

std::string removeWhitespace(std::string str)
{
  str.erase(std::remove_if(str.begin(), str.end(),
                           [](unsigned char c)
                           { return std::isspace(c); }),
            str.end());
  return str;
}

void printSubSchemas()
{
  // path to schema
  std::string get_sub_schemas_list = "gsettings get org.gnome.settings-daemon.plugins.media-keys custom-keybindings";
  // get the list of subschemas
  std::string sub_schema_list = exec(get_sub_schemas_list.c_str());
  std::cout << "Subschemas for " << "custom-keybindings" << ": " << sub_schema_list << std::endl;
}

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    usage();
    return 0;
  }

  if (argv[1] == VIEW_SCHEMA)
  {
    printSubSchemas();
  }
  else if (argv[1] == RESET_SCHEMA)
  {
    std::string reset_schema = "gsettings reset org.gnome.settings-daemon.plugins.media-keys custom-keybindings";
    exec(reset_schema.c_str());
    printSubSchemas();
  }

  // TODO
  // view the keybindings in a schema
  // delete entire root schema
  // CRUD operations on the keybinding's subschema

  // check if custom keybindings schema exists

  return 0;
}