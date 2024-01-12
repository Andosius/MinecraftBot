// Local dependencies
#include "commands/start.hpp"

// External dependencies

// Standard Library

//======================================

static std::mutex s_StartMutex = std::mutex();
static std::chrono::system_clock::time_point s_StartAvailable = std::chrono::system_clock::now();



void start_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
	std::lock_guard l(s_StartMutex);

	event.thinking();

	if (IsServerRunning())
	{
		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::red).
				set_title("Server is already running!").
				set_description("Can't start server as it is already running!")
			)
		);
	}
	else
	{
		if (std::chrono::system_clock::now() >= s_StartAvailable)
		{
			s_StartAvailable = std::chrono::system_clock::now() + std::chrono::minutes(2);

			Application::SendCommand("systemctl start minecraft");

			//Application* app = Application::Get();

			//Application::SendCommand(fmt::format("cd {0} && /usr/bin/screen -dmS {1} {2}", app->m_RemoteScreenLocation, app->m_RemoteScreenName, app->m_RemoteScreenExecutable));

			event.edit_original_response(
				dpp::message().add_embed(
					dpp::embed().
					set_color(dpp::colors::green).
					set_title("Server started!").
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
}
