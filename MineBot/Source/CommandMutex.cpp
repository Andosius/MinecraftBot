// Local dependencies
#include "CommandMutex.hpp"

// External dependencies

// Standard Library

//======================================



std::vector<std::string> s_Blockers = {};
std::mutex s_CommandMutex = std::mutex();
bool s_CanExecuteCommands = true;

