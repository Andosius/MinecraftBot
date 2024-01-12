#pragma once

// Local dependencies
#include "Utility.hpp"
#include "Application.hpp"

// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#include <fmt/format.h>
#pragma warning(pop)

// Standard Library
#include <mutex>

//======================================


void blocker_handler(dpp::cluster& bot, const dpp::slashcommand_t& event);
bool IsInteractionBlocked();
