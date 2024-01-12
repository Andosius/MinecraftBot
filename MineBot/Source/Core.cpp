// Local dependencies
#include "Core.hpp"
#include "Log.hpp"
#include "command.hpp"

#include "commands/blocker.hpp"


// External dependencies


// Standard Library
#include <string>
//======================================


#ifdef _WIN64
	#define pclose	_pclose
	#define popen	_popen
#endif

Core::Core(const std::string& token, CommandMap commands)
	: m_Bot(token), m_Commands(commands)
{
}

Core::~Core()
{
}

void Core::RegisterCallbacks()
{
	m_Bot.on_log([](const dpp::log_t& event)
		{
			switch (event.severity) {
			case dpp::ll_trace:
				LOG_TRACE(event.message);
				break;
			case dpp::ll_debug:
				LOG_DEBUG(event.message);
				break;
			case dpp::ll_info:
				LOG_INFO(event.message);
				break;
			case dpp::ll_warning:
				LOG_WARN(event.message);
				break;
			case dpp::ll_error:
				LOG_ERROR(event.message);
				break;
			case dpp::ll_critical:
			default:
				LOG_CRITICAL(event.message);
				break;
			}
		}
	);

	m_Bot.on_ready([this](const dpp::ready_t& event)
		{
			if (dpp::run_once<struct bulk_register_commands>())
			{
				std::vector<dpp::slashcommand> slash_commands;

				for (auto& command : m_Commands)
				{
					dpp::slashcommand c;
					c.set_name(command.first).
						set_description(command.second.description).
						set_application_id(m_Bot.me.id);
					c.options = command.second.parameters;
					slash_commands.push_back(c);
				}

				m_Bot.global_bulk_command_create(slash_commands);
			}
		}
	);

	m_Bot.on_slashcommand([this](const dpp::slashcommand_t& event)
		{
			dpp::command_interaction cmd_data = event.command.get_command_interaction();

			// Interaction Blocker
			if (cmd_data.name == "start" || cmd_data.name == "stop")
			{
				if (IsInteractionBlocked())
				{
					event.reply("This interaction is currently blocked and can't be performed!");
					return;
				}
			}
			// End - Interaction Blocker


			/* Check for commands that exist in our command map */
			auto command_iter = m_Commands.find(cmd_data.name);

			if (command_iter != m_Commands.end()) {
				/* If we find a matching command, call its handler function,
				 * passing in this event's information and the bot cluster
				 * as references.
				 */
				command_iter->second.function(m_Bot, event);
			}
		}
	);
}

void Core::SetRemoteAccessData(const std::string& host, const std::string& user, const std::string& pass)
{
	m_Hostname = host;
	m_Username = user;
	m_Password = pass;
}

void Core::Run()
{
	m_Bot.start(dpp::st_wait);
}

void Core::Stop()
{
	m_Bot.shutdown();
}

std::string Core::SendCommand(const std::string& command)
{
	std::array<char, 128> buffer;
	std::string result;

	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

	if (!pipe)
	{
		LOG_CRITICAL("popen() failed! Message: {}", command);
		return "";
	}

	while (fgets(buffer.data(), (int)buffer.size(), pipe.get()) != nullptr)
	{
		result += buffer.data();
	}

	return result;
}

/*
std::string Core::SendCommand(const std::string& command)
{
	ssh::Session session;
	std::string output;

	if (CreateRemoteConnection(session))
	{
		ssh::Channel channel(session);
		channel.openSession();

		channel.requestExec(command.c_str());

		char buffer[1024] = {};
		int nbytes = 0;

		while (!channel.isEof())
		{
			nbytes = channel.read(&buffer, sizeof(buffer), false, 1);
			if (nbytes > 0)
				output.append(buffer, nbytes);
		}

		channel.sendEof();
		channel.close();
	}
	else
	{
		LOG_CRITICAL("Unable to create connection!");
	}

	return output;
}

bool Core::CreateRemoteConnection(ssh::Session& session)
{
	session.setOption(SSH_OPTIONS_HOST, m_Hostname.c_str());
	session.setOption(SSH_OPTIONS_USER, m_Username.c_str());
	session.setOption(SSH_OPTIONS_PORT, 22);

	int err = session.connect();

	if (err == SSH_ERROR)
	{
		LOG_CRITICAL("SSH Error, can't create connection!");
		return false;
	}

	err = session.userauthPassword(m_Password.c_str());

	if (err == SSH_ERROR)
	{
		LOG_CRITICAL("SSH Error, can't authenticate with password!");
		return false;
	}

	return err == SSH_AUTH_SUCCESS;
}*/
