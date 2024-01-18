// Local dependencies
#include "commands/start.hpp"
#include "CommandMutex.hpp"
#include "Utility.hpp"

// External dependencies
#pragma warning(push, 0)
#include <fmt/format.h>
#pragma warning(pop)

// Standard Library
#include <mutex>
#include <chrono>

//======================================


static std::chrono::system_clock::time_point s_StartAvailable = std::chrono::system_clock::now();


void start_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
	event.thinking();
	std::lock_guard<std::mutex> lock(s_CommandMutex);


	if (Utility::IsServerRunning())
	{
		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::red).
				set_title("Server läuft bereits!").
				set_description("Der Server kann nicht gestartet werden, da dieser bereits aktiv ist.")
			)
		);
		return;
	}


	if (std::chrono::system_clock::now() >= s_StartAvailable)
	{
		s_StartAvailable = std::chrono::system_clock::now() + std::chrono::minutes(2);
		Utility::SendCommand("systemctl start minecraft");

		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::green).
				set_title("Server gestartet!").
				set_description("Der Server wird gestartet.")
			)
		);
		return;
	}
	
	event.edit_original_response(
		dpp::message().add_embed(
			dpp::embed().
			set_color(dpp::colors::yellow).
			set_title("Server kann nicht gestartet werden!").
			set_description("Der Befehl hat eine Abklingzeit von 2 Minuten.")
		)
	);
	return;
}
