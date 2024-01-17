// Local dependencies
#include "CommandMutex.hpp"

// External dependencies

// Standard Library

//======================================



static std::vector<std::string> s_Blockers = {};
static std::mutex s_BlockerMutex = std::mutex();
static bool s_CanExecuteCommands = true;
