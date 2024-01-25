#pragma once

// Local dependencies


// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#pragma warning(pop)

// Standard Library


//======================================

/**
* @brief The ping command handler.
* 
* Displays how long it takes to send commands to the discord API.
*/
void ping_handler(dpp::cluster& bot, const dpp::slashcommand_t& event);
