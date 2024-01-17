#pragma once

// Local dependencies
#include "Utility.hpp"

// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#include <fmt/format.h>
#pragma warning(pop)

// Standard Library

//======================================


void op_handler(dpp::cluster& bot, const dpp::slashcommand_t& event);
