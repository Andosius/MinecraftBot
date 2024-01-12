#pragma once

// Local dependencies
#include "Application.hpp"

// External dependencies
#pragma warning(push, 0)
#include <fmt/format.h>
#pragma warning(pop)

#include <nlohmann/json.hpp>

// Standard Library
#include <regex>
//======================================


bool IsServerRunning();
bool IsValidMinecraftName(const std::string& name);
std::string GetAccountsContent();
void WriteAccounts(nlohmann::json accounts);
nlohmann::json GetAccounts();
bool IsUserWhitelisted(const uint64_t& account_id, size_t& idx);
