#include "Command.hpp"

std::string ALLSTRINGSVALID = "*";
std::string DONTPLACE = "**"; //for commands like /cheats they take no arguments but if they're in the file its on and if they aren't its off
std::string PLACE = "***";
Command::Command(std::string name, std::string value, std::vector<std::string> possibleValues) //only a set number  of possible values
{
	M_name = name;
	M_possibleValues = possibleValues;
	if (!setValue(value)) {
		throw std::runtime_error("invalid command constructor");
	}
}
Command::Command(std::string name, std::string value) //all values are valid
{
	M_name = name;
	M_value = value;
	M_possibleValues.push_back(ALLSTRINGSVALID);
	if (!setValue(value)) {
		throw std::runtime_error("invalid command constructor");
	}
}
Command::Command(std::string name, bool place) //no values are valid
{
	M_name = name;
	M_value = "";
	if (!place) {
		M_possibleValues.push_back(DONTPLACE);
		return;
	}
	M_possibleValues.push_back(PLACE);

}

bool Command::setValue(std::string value)
{
	if (value.compare("place") == 0 && M_possibleValues[0].compare(DONTPLACE) == 0 || M_possibleValues[0].compare(PLACE) == 0) {
		M_possibleValues[0] = PLACE;
		return true;
	}
	if (M_possibleValues[0].compare(ALLSTRINGSVALID) == 0) { //all values are valid
		M_value = value;
		return true;
	}
	else if (M_possibleValues[0].compare(PLACE) == 0 || M_possibleValues[0].compare(DONTPLACE) == 0) { //no values are valid
		return false;
	}
	else {
		for (int i{ 0 }; i < M_possibleValues.size(); i++) { //only a set number  of possible values
			if (M_possibleValues[i].compare(value) == 0) {
				M_value = value;
				return true;
			}
		}
		return false;
	}


}

std::pair<std::string, std::string> Command::getNameValuePair()
{
	if (M_possibleValues[0].compare(DONTPLACE) == 0) {
		return std::pair<std::string, std::string>("", "");
	}
	return std::pair<std::string, std::string>(M_name, M_value);
}