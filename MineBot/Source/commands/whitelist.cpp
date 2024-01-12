// Local dependencies
#include "commands/whitelist.hpp"

// External dependencies

// Standard Library

//======================================



void WhitelistUser(const std::string& username, const dpp::slashcommand_t& event)
{
	nlohmann::json accounts = GetAccounts();
	nlohmann::json account;

	uint64_t account_id = (uint64_t)event.command.get_issuing_user().id;

	account["name"] = username;
	account["account_id"] = account_id;
	accounts.push_back(account);

	WriteAccounts(accounts);

	Application::SendCommand(fmt::format("/usr/bin/screen -p 0 -S {0} -X eval 'stuff \"whitelist add {1}\"\\015'", Application::Get()->m_RemoteScreenName, username));

	event.edit_original_response(
		dpp::message().add_embed(
			dpp::embed().
			set_color(dpp::colors::green).
			set_title("Whitelist command executed successfully!").
			set_description(fmt::format("Added {} to the whitelist, created a connection towards your Discord account.", username))
		)
	);
}

void UnWhitelistUser(size_t index, const dpp::slashcommand_t& event)
{
	nlohmann::json accounts = GetAccounts();
	std::string username = accounts[index].get<std::string>();

	accounts.erase(index);
	WriteAccounts(accounts);

	Application::SendCommand(fmt::format("/usr/bin/screen -p 0 -S {0} -X eval 'stuff \"whitelist remove {1}\"\\015'", Application::Get()->m_RemoteScreenName, username));

	event.edit_original_response(
		dpp::message().add_embed(
			dpp::embed().
			set_color(dpp::colors::green).
			set_title("Whitelist command executed successfully!").
			set_description(fmt::format("Removed {} from the whitelist, removed the connection towards your Discord account.", username))
		)
	);

}


void whitelist_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
	std::lock_guard l(Application::Get()->m_AccountsMutex);

	dpp::interaction interaction = event.command;
	dpp::command_interaction cmd_data = interaction.get_command_interaction();

	dpp::command_data_option& subcommand = cmd_data.options[0];
	std::string action = subcommand.name;

	event.thinking();

	uint64_t account_id = (uint64_t)event.command.get_issuing_user().id;
	size_t index = -1;

	if (!IsServerRunning())
	{
		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::red).
				set_title("Server offline!").
				set_description("Can't send command to server.")
			)
		);
		return;
	}

	if (action == "add")
	{
		dpp::command_value username_param = event.get_parameter("username");
		std::string username = std::get<std::string>(username_param);

		if (!IsValidMinecraftName(username))
		{
			event.edit_original_response(
				dpp::message().add_embed(
					dpp::embed().
					set_color(dpp::colors::red).
					set_title("Whitelist command failed.").
					set_description(fmt::format("Invalid username provided: {}", username))
				)
			);
			return;
		}

		if (!IsUserWhitelisted(account_id, index))
		{
			WhitelistUser(username, event);
			return;
		}
		else
		{
			event.edit_original_response(
				dpp::message().add_embed(
					dpp::embed().
					set_color(dpp::colors::yellow).
					set_title("Whitelist command failed.").
					set_description(fmt::format("You already have an account whitelisted - can't whitelist accounts for others!", username))
				)
			);
		}
	}
	else if (action == "remove")
	{
		if (IsUserWhitelisted(account_id, index))
		{
			UnWhitelistUser(index, event);
		}
		else
		{
			event.edit_original_response(
				dpp::message().add_embed(
					dpp::embed().
					set_color(dpp::colors::red).
					set_title("Whitelist command failed!").
					set_description("Can't find any account connected to your Discord account.")
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
				set_title("Whitelist command failed!").
				set_description(fmt::format("Invalid action: {}", action))
			)
		);
	}
}