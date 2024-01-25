#pragma once

// Local dependencies


// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#pragma warning(pop)

#include <nlohmann/json.hpp>

// Standard Library


//======================================

/**
* @brief The whitelist command handler.
*
* Links a minecraft user with a discord user.
*/
void whitelist_handler(dpp::cluster& bot, const dpp::slashcommand_t& event);