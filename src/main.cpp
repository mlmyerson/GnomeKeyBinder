#include <iostream>
#include <array>
#include <memory>
#include <algorithm>
#include <vector>

const std::string HELP = "--help";
const std::string VIEW_SCHEMA = "--view-schema";
const std::string RESET_SCHEMA = "--reset-schema";
const std::string ADD_KEY = "--add-key";
const std::string REMOVE_KEY = "--remove-key";

void usage()
{
  std::cout << "Options: \n"
            << "  " << HELP << ": Show this help message\n"
            << "  " << VIEW_SCHEMA << ": View the keybindings in a schema\n"
            << "  " << RESET_SCHEMA << ": Reset the entire schema to empty\n"
            << "  " << ADD_KEY << ": Add a keybinding to the schema\n"
            << "  " << REMOVE_KEY << ": Remove a keybinding from the schema\n"
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

std::string getSubSchemas()
{
  // path to schema
  std::string get_sub_schemas_list = "gsettings get org.gnome.settings-daemon.plugins.media-keys custom-keybindings";
  // get the list of subschemas
  std::string sub_schema_list = exec(get_sub_schemas_list.c_str());
  return sub_schema_list;
}

// vector spaces for the user input: name, command, binding
std::vector<std::string> getUserInput()
{
  std::vector<std::string> user_input(3);

  // name - use name for both subschema and command
  std::cout << "Enter the name of the keybinding: " << std::endl;
  std::cin >> user_input[0];

  // command
  std::cout << "Enter the command used by the keybinding: " << std::endl;
  std::cin >> user_input[1];

  // binding
  std::cout << "Enter the key(s) used by the keybinding: " << std::endl;
  std::cin.ignore(); // Clear the newline character left in the input buffer
  std::getline(std::cin, user_input[2]);

  return user_input;
}


int main(int argc, char *argv[])
{

  if (argc < 2 || argv[1] == "--help")
  {
    usage();
    return 0;
  }

  if (argv[1] == VIEW_SCHEMA)
  {
    std::cout << "Subschemas for " << "custom-keybindings" << ": " << getSubSchemas() << std::endl;
  }
  else if (argv[1] == RESET_SCHEMA)
  {
    std::string reset_schema = "gsettings reset org.gnome.settings-daemon.plugins.media-keys custom-keybindings";
    exec(reset_schema.c_str());
    std::cout << "Subschemas for " << "custom-keybindings" << ": " << getSubSchemas() << std::endl;
  }
  else if (argv[1] == ADD_KEY)
  {
    std::cout << "Add keybinding to the schema" << std::endl;

    std::vector<std::string> user_input = getUserInput();
    std::string name = user_input[0];
    std::string command = user_input[1];  
    std::string binding = user_input[2];
    std::string new_list = getSubSchemas();
    std::string add_comma;

    //check if first item in list
    new_list == "[]" ? add_comma = "" : add_comma = ",";

    //setup pathing for the new keybinding
    std::string add_keybinding = add_comma + "gsettings set org.gnome.settings-daemon.plugins.media-keys custom-keybindings";
    std::string path_insert = "'/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/" + name + "/'";
    // insert before closing ]
    new_list.insert(new_list.length() - 1, path_insert);

    //set the keybinding
    std::string set_binding = add_keybinding + " " + new_list;
    exec(set_binding.c_str());

    //set the name
    exec((set_binding + " name '" + name + "'").c_str());
    //set the command
    exec((set_binding + " command '" + command + "'").c_str());
    //set the binding
    exec((set_binding + " binding '" + binding + "'").c_str());

    std::cout << "Keybinding added: " << name << std::endl;
  }
  else if (argv[1] == REMOVE_KEY)
  {
    std::cout << "Remove keybinding from the schema" << std::endl;
    
    // Ask for the name of the keybinding to remove
    std::string name;
    std::cout << "Enter the name of the keybinding to remove: " << std::endl;
    std::cin >> name;
    
    // Get current list of keybindings
    std::string current_list = getSubSchemas();
    
    // Create the path we want to remove
    std::string path_to_remove = "'/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/" + name + "/'";

    //clean up orphaned data in dconf
    std::string schema_path = "/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/" + name + "/";
    std::string cleanup_cmd = "gsettings reset-recursively org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:" + schema_path;
    std::cout << "Cleaning up orphaned data with command: " << cleanup_cmd << std::endl;
    exec(cleanup_cmd.c_str());
    
    // Check if this path exists in the current list
    if (current_list.find(path_to_remove) != std::string::npos) {
      // Remove the path from the list
      // First handle the case where it's the only item
      if (current_list == "[" + path_to_remove + "]") {
        current_list = "[]";
      }
      // Handle case where it's first in a list with multiple items
      else if (current_list.find(path_to_remove + ",") != std::string::npos) {
        size_t pos = current_list.find(path_to_remove + ",");
        current_list.erase(pos, path_to_remove.length() + 1);
      }
      // Handle case where it's anywhere else in the list
      else {
        size_t pos = current_list.find("," + path_to_remove);
        if (pos != std::string::npos) {
          current_list.erase(pos, path_to_remove.length() + 1);
        }
      }
      
      // Update the keybindings list
      std::string update_cmd = "gsettings set org.gnome.settings-daemon.plugins.media-keys custom-keybindings '" + current_list + "'";
      exec(update_cmd.c_str());
      
      std::cout << "Keybinding '" << name << "' removed." << std::endl;
    } else {
      std::cout << "Keybinding '" << name << "' not found." << std::endl;
    }
  }

  // TODO

  return 0;
}