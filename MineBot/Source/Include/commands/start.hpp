#pragma once

// Local dependencies


// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#pragma warning(pop)

// Standard Library


//======================================

/**
* @brief The start command handler.
* 
* Starts the minecraft server.
*/
void start_handler(dpp::cluster& bot, const dpp::slashcommand_t& event);
