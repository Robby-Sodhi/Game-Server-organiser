#include "commands.hpp"


CommandsClass::CommandsClass()
{

	M_commandsAndSettings.push_back(Command("bind", "0.0.0.0"));
	M_commandsAndSettings.push_back(Command("camera", "both", std::vector<std::string> {"first", "third", "both"}));
	M_commandsAndSettings.push_back(Command("cheats", true));
	M_commandsAndSettings.push_back(Command("filter", false));
	M_commandsAndSettings.push_back(Command("gamemode", "Survival", std::vector<std::string>{"Survival", "Horde", "Arena"}));
	M_commandsAndSettings.push_back(Command("gold", false));
	M_commandsAndSettings.push_back(Command("hide_admins", false));
	M_commandsAndSettings.push_back(Command("map", "washington", std::vector<std::string>{"washington", "russia", "elver", "germany", "france", "pei", "hawaii", "yukon"}));
	M_commandsAndSettings.push_back(Command("maxplayers", "24"));
	M_commandsAndSettings.push_back(Command("mode", "normal", std::vector<std::string>{"easy", "normal", "hard"}));
	M_commandsAndSettings.push_back(Command("name", "UnturnedServer"));
	M_commandsAndSettings.push_back(Command("owner", "123456789"));
	M_commandsAndSettings.push_back(Command("port", "27015"));
	M_commandsAndSettings.push_back(Command("pve", false));
	M_commandsAndSettings.push_back(Command("queue", "10"));
	M_commandsAndSettings.push_back(Command("sync", false));
	M_commandsAndSettings.push_back(Command("welcome", "Welcome to this Unturned Server (Please change commands.dat)"));


}

bool CommandsClass::setCommand(std::string name, std::string value)
{
	for (auto &command : M_commandsAndSettings) {
		if (command.getNameValuePairFORCED().first.compare(name) == 0) {
			if (command.setValue(value)) {
				return true;
			}
			return false;
		}
	}
	return false;
}

std::string CommandsClass::commandString()
{
	std::string commandString;
	for (auto command : M_commandsAndSettings) {
		commandString += command.getNameValuePair().first;
		commandString += " ";
		commandString += command.getNameValuePair().second;
		commandString += "\n";
	}
	return commandString;
}