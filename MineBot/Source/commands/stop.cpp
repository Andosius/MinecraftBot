// Local dependencies
#include "commands/stop.hpp"
#include "CommandMutex.hpp"

// External dependencies

// Standard Library

//======================================


static std::chrono::system_clock::time_point s_StopAvailable = std::chrono::system_clock::now();


void stop_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
	event.thinking();
	std::lock_guard<std::mutex> lock(s_CommandMutex);


	if (!Utility::IsServerRunning())
	{
		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::red).
				set_title("Server läuft bereits!").
				set_description("Der Server kann nicht gestoppt werden, da dieser bereits inaktiv ist.")
			)
		);
		return;
	}


	if (std::chrono::system_clock::now() >= s_StopAvailable)
	{
		s_StopAvailable = std::chrono::system_clock::now() + std::chrono::minutes(2);
		Utility::SendCommand("systemctl stop minecraft");

		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::green).
				set_title("Server gestoppt!").
				set_description("Der Server wird gestoppt.")
			)
		);
		return;
	}

	event.edit_original_response(
		dpp::message().add_embed(
			dpp::embed().
			set_color(dpp::colors::yellow).
			set_title("Server kann nicht gestoppt werden!").
			set_description("Der Befehl hat eine Abklingzeit von 2 Minuten.")
		)
	);
	return;
}
