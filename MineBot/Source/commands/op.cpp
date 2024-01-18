// Local dependencies
#include "commands/op.hpp"
#include "CommandMutex.hpp"
#include "Utility.hpp"

// External dependencies
#pragma warning(push, 0)
#include <fmt/format.h>
#pragma warning(pop)
// Standard Library

//======================================


void op_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
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

	uint64_t account_id = static_cast<uint64_t>(event.command.get_issuing_user().id);


	DiscordUser user = Utility::GetKnownDiscordUserByAccountID(account_id);
	if (!user.IsValid())
	{
		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::red).
				set_title("Account ist nicht bekannt!").
				set_description("Der Befehl kann nicht ausgeführt werden, da der Account nicht auf der Whitelist steht.")
			)
		);
		return;
	}

	Operator op = Utility::GetMinecraftOperatorByUsername(user.MinecraftName);
	if (!op.IsValid())
	{
		Utility::SendMinecraftCommand(fmt::format("op {0}", user.MinecraftName));
		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::green).
				set_title("Befehl übertragen!").
				set_description("Der Server wird dich der Operator Rolle hinzufügen.")
			)
		);
		return;
	}


	Utility::SendMinecraftCommand(fmt::format("deop {0}", user.MinecraftName));
	event.edit_original_response(
		dpp::message().add_embed(
			dpp::embed().
			set_color(dpp::colors::green).
			set_title("Befehl übertragen!").
			set_description("Der Server wird dich der Operator Rolle entfernen.")
		)
	);
	return;
}
