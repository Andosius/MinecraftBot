#pragma once

// Local dependencies


// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#pragma warning(pop)

// Standard Library


//======================================

/**
* @brief The blocker command handler.
* 
* The blocker command enables the possibility to block all commands except itself.
* This is useful for maintenance purposes.
*/
void blocker_handler(dpp::cluster& bot, const dpp::slashcommand_t& event);
