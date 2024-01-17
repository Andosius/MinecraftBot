#pragma once

// Local dependencies


// External dependencies


// Standard Library
#include <vector>
#include <string>
#include <mutex>

//======================================


extern std::vector<std::string> s_Blockers;
extern std::mutex s_CommandMutex;
extern bool s_CanExecuteCommands;