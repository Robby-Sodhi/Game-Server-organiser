#ifndef COMMANDHPP
#define COMMANDHPP
#include <string>
#include <vector>
class Command {
private:
	std::string M_name;
	std::string M_value;
	std::vector<std::string> M_possibleValues{};
public:
	Command(std::string name, std::string value, std::vector<std::string> possibleValues);
	Command(std::string name, std::string value);
	Command(std::string name, bool place);
	Command(std::string name, const char* value) : Command(name, std::string(value)) {}
	bool setValue(std::string value);
	std::pair<std::string, std::string> getNameValuePair();

};


#endif // !COMMANDHPP