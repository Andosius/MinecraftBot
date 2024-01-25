#pragma once

// Local dependencies


// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#pragma warning(pop)

// Standard Library


//======================================

/**
* @brief The op command handler.
* 
* Sends the op command based on registration-level (whitelisted).
*/
void op_handler(dpp::cluster& bot, const dpp::slashcommand_t& event);
