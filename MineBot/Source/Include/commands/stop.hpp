#pragma once

// Local dependencies


// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#pragma warning(pop)

// Standard Library


//======================================

/**
* @brief The stop command handler.
*
* Stops the minecraft server.
*/
void stop_handler(dpp::cluster& bot, const dpp::slashcommand_t& event);
