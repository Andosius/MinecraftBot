#pragma once

// Local dependencies
#include "Utility.hpp"
#include "Application.hpp"

// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#include <fmt/format.h>
#pragma warning(pop)

#include <nlohmann/json.hpp>

// Standard Library
#include <mutex>
#include <fstream>
//======================================

void whitelist_handler(dpp::cluster& bot, const dpp::slashcommand_t& event);