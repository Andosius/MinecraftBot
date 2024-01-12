// Local dependencies
#include "commands/ping.hpp"

// External dependencies

// Standard Library

//======================================


void ping_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
	double ping = (bot.get_shard(0)->websocket_ping + bot.rest_ping) * 1000;

	event.reply(
		dpp::message().add_embed(
			dpp::embed().
			set_color(dpp::colors::aquamarine).
			set_title("Pong!").
			set_description(
				fmt::format("{0} the ping is: {1:.02f} ms!", event.command.get_issuing_user().get_mention(), ping)
			)
		)
	);
}
