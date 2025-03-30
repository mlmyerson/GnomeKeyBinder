#ifndef KEYBINDMANAGER_H
#define KEYBINDMANAGER_H

#include <string> 
#include <vector> 

class KeyBindManager
{
public:
    KeyBindManager();
    ~KeyBindManager();

private:
    std::string exec(const char*);
    std::string removeWhitespace(std::string str);
    std::string getSubSchemas();
    std::vector<std::string> getUserInput();
};

#endif