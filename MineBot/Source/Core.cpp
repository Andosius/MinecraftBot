// Local dependencies
#include "Core.hpp"
#include "Log.hpp"
#include "Config.hpp"
#include "Commands.hpp"
#include "CommandMutex.hpp"

// External dependencies


// Standard Library
#include <string>
#include <memory>

//======================================


Core::Core()
	: m_Bot(AppSettings->Token)
{
	RegisterCallbacks();
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

				for (auto& command : BotCommands)
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
				if (s_Blockers.size() > 0)
				{
					event.reply("This interaction is currently blocked and can't be performed!");
					return;
				}
			}
			// End - Interaction Blocker


			/* Check for commands that exist in our command map */
			auto command_iter = BotCommands.find(cmd_data.name);

			if (command_iter != BotCommands.end()) {
				/* If we find a matching command, call its handler function,
				 * passing in this event's information and the bot cluster
				 * as references.
				 */
				command_iter->second.function(m_Bot, event);
			}
		}
	);
}

void Core::Run()
{
	m_Bot.start(dpp::st_wait);
}

void Core::Stop()
{
	m_Bot.shutdown();
}
