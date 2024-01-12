// Local dependencies
#include "commands/stop.hpp"

// External dependencies

// Standard Library

//======================================


static std::mutex s_StopMutex = std::mutex();
static std::chrono::system_clock::time_point s_StopAvailable = std::chrono::system_clock::now();


void stop_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
	std::lock_guard l(s_StopMutex);
	event.thinking();

	if (IsServerRunning())
	{
		if (std::chrono::system_clock::now() >= s_StopAvailable)
		{
			s_StopAvailable = std::chrono::system_clock::now() + std::chrono::minutes(2);

			Application::SendCommand("systemctl stop minecraft");

			//Application* app = Application::Get();

			//Application::SendCommand(fmt::format("/usr/bin/screen -p 0 -S {0} -X eval 'stuff \"save-all\"\\015'", app->m_RemoteScreenName));
			//Application::SendCommand(fmt::format("/usr/bin/screen -p 0 -S {0} -X eval 'stuff \"stop\"\\015'", app->m_RemoteScreenName));

			event.edit_original_response(
				dpp::message().add_embed(
					dpp::embed().
					set_color(dpp::colors::green).
					set_title("Stopping server!").
					set_description("This may take a while...")
				)
			);
		}
		else
		{
			event.edit_original_response(
				dpp::message().add_embed(
					dpp::embed().
					set_color(dpp::colors::yellow).
					set_title("Command on cooldown!").
					set_description("This command needs two minutes to cool down!")
				)
			);
		}
	}
	else
	{
		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::red).
				set_title("Server is not running!").
				set_description("Can't stop server as it is already offline!")
			)
		);
	}
}
