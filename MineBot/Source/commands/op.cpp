// Local dependencies
#include "commands/op.hpp"

// External dependencies

// Standard Library

//======================================


void DeOP(const std::string& username, const dpp::slashcommand_t& event)
{
	Application::SendCommand(fmt::format("/usr/bin/screen -p 0 -S {0} -X eval 'stuff \"deop {1}\"\\015'", Application::Get()->m_RemoteScreenName, username));

	event.edit_original_response(
		dpp::message().add_embed(
			dpp::embed().
			set_color(dpp::colors::green).
			set_title("Server received the command!").
			set_description("The server will remove you as an operator soon!")
		)
	);
}

void OP(const std::string& username, const dpp::slashcommand_t& event)
{
	Application::SendCommand(fmt::format("/usr/bin/screen -p 0 -S {0} -X eval 'stuff \"op {1}\"\\015'", Application::Get()->m_RemoteScreenName, username));

	event.edit_original_response(
		dpp::message().add_embed(
			dpp::embed().
			set_color(dpp::colors::green).
			set_title("Server received the command!").
			set_description("The server will make you an operator soon!")
		)
	);
}

void op_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
	std::lock_guard l(Application::Get()->m_AccountsMutex);

	event.thinking();

	if (IsServerRunning())
	{
		uint64_t account_id = (uint64_t)event.command.get_issuing_user().id;
		nlohmann::json accounts = GetAccounts();

		size_t idx = -1;
		IsUserWhitelisted(account_id, idx);

		if (idx != -1)
		{
			std::string username = accounts[idx]["name"].get<std::string>();
			std::string ops_data = Application::SendCommand(fmt::format("cat {}/ops.nlohmann::json", Application::Get()->m_RemoteScreenLocation));
			nlohmann::json ops = nlohmann::json::parse(ops_data);

			bool found = false;
			for (int i = 0; i < ops.size(); i++)
			{
				if (ops[i]["name"].get<std::string>() == username)
				{
					found = true;
					break;
				}
			}

			if (found)
				DeOP(username, event);
			else
				OP(username, event);
		}
		else
		{
			event.edit_original_response(
				dpp::message().add_embed(
					dpp::embed().
					set_color(dpp::colors::red).
					set_title("Account not whitelisted!").
					set_description("Can't execute this command as you can't connect anyway!")
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
				set_description("Can't execute this command as the server is not reachable!")
			)
		);
	}
}