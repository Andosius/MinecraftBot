// Local dependencies
#include "commands/whitelist.hpp"
#include "CommandMutex.hpp"
#include "Utility.hpp"

// External dependencies
#pragma warning(push, 0)
#include <fmt/format.h>
#pragma warning(pop)

// Standard Library
#include <mutex>
#include <fstream>

//======================================


void whitelist_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
	event.thinking();
	std::lock_guard<std::mutex> lock(s_CommandMutex);

	if (!Utility::IsServerRunning())
	{
		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::red).
				set_title("Server ist inaktiv!").
				set_description("Der Befehl kann nicht ausgeführt werden, da der Server inaktiv ist.")
			)
		);
		return;
	}


	dpp::interaction interaction = event.command;
	dpp::command_interaction cmd_data = interaction.get_command_interaction();

	dpp::command_data_option& subcommand = cmd_data.options[0];
	std::string action = subcommand.name;


	uint64_t account_id = static_cast<uint64_t>(event.command.get_issuing_user().id);


	if (action == "add")
	{
		dpp::command_value username_param = event.get_parameter("username");
		std::string username = std::get<std::string>(username_param);

		if (!Utility::IsValidMinecraftName(username))
		{
			event.edit_original_response(
				dpp::message().add_embed(
					dpp::embed().
					set_color(dpp::colors::red).
					set_title("Whitelist-Vorgang fehlgeschlagen!").
					set_description(fmt::format("Der Name \"{}\" enthält nicht zulässige Zeichen.", username))
				)
			);
			return;
		}

		DiscordUser user = Utility::GetKnownDiscordUserByAccountID(account_id);
		if (user.IsValid())
		{
			event.edit_original_response(
				dpp::message().add_embed(
					dpp::embed().
					set_color(dpp::colors::red).
					set_title("Whitelist-Vorgang abgebrochen!").
					set_description(fmt::format("Der Account \"{}\" ist bereits über dich freigeschaltet.", user.MinecraftName))
				)
			);
			return;
		}


		Utility::SendMinecraftCommand(fmt::format("whitelist add {}", username));
		Utility::AddDiscordUserToCache(DiscordUser(account_id, username));
		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::green).
				set_title("Whitelist-Vorgang erfolgreich!").
				set_description(fmt::format("Der Account \"{}\" wurde freigeschaltet.", username))
			)
		);
		return;
	}
	else // remove
	{
		DiscordUser user = Utility::GetKnownDiscordUserByAccountID(account_id);
		if (!user.IsValid())
		{
			event.edit_original_response(
				dpp::message().add_embed(
					dpp::embed().
					set_color(dpp::colors::red).
					set_title("Whitelist-Vorgang abgebrochen!").
					set_description("Dein Account ist mit keinem Minecraft Account verbunden.")
				)
			);
			return;
		}

		Utility::SendMinecraftCommand(fmt::format("whitelist remove {}", user.MinecraftName));
		Utility::RemoveDiscordUserFromCache(user);
		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::green).
				set_title("Whitelist-Vorgang erfolgreich!").
				set_description(fmt::format("Der Account \"{}\" wurde entfernt.", user.MinecraftName))
			)
		);
		return;
	}
}