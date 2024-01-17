// Local dependencies
#include "commands/op.hpp"
#include "CommandMutex.hpp"

// External dependencies

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

	std::string username;
	bool is_known = false;
	bool is_operator = false;

	std::vector<Operator> operators = Utility::GetServerOperators();
	std::vector<DiscordUser> users = Utility::GetKnownUsers();

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].DiscordId == account_id)
		{
			username = users[i].MinecraftName;
			is_known = true;
			break;
		}
	}

	if (!is_known)
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

	for (size_t i = 0; i < operators.size(); i++)
	{
		if (operators[i].Name == username)
		{
			is_operator = true;
			break;
		}
	}

	if (is_operator)
	{
		Utility::SendMinecraftCommand(fmt::format("op {0}", username));
	}
	else
	{
		Utility::SendMinecraftCommand(fmt::format("deop {0}", username));
	}

	event.edit_original_response(
		dpp::message().add_embed(
			dpp::embed().
			set_color(dpp::colors::green).
			set_title("Befehl übertragen!").
			set_description("Der Server wird den Befehl so bald wie möglich ausführen.")
		)
	);
}
