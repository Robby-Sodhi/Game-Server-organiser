#ifndef COMMANDSHPP
#define COMMANDSHPP
#include <map>
#include "command.hpp"

class CommandsClass {
private:
	std::vector<Command>M_commandsAndSettings;
public:
	CommandsClass();
	bool setCommand(std::string name, std::string value);
	std::string commandString();
};



#endif // !COMMANDSHPP