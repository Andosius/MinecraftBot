// Local dependencies
#include "command.hpp"
#include "commands/ping.hpp"
#include "commands/whitelist.hpp"
#include "commands/start.hpp"
#include "commands/stop.hpp"
#include "commands/op.hpp"
#include "commands/blocker.hpp"

#include "Application.hpp"

// External dependencies
#include <dpp/dpp.h>

// Standard Library
#include <string>
//======================================


#define CONFIG_FILE			"config.json"


CommandMap commands = {
	{ "ping", { "Ping - pong!", ping_handler, {} 
			}
	},
	{"whitelist", {"Whitelist a player", whitelist_handler,
				{	
					dpp::command_option(dpp::co_sub_command, "add", "Whitelist your account", false).set_auto_complete(true).
						add_option(dpp::command_option(dpp::co_string, "username", "Your username", true)),
					dpp::command_option(dpp::co_sub_command, "remove", "Un-Whitelist your account", false).set_auto_complete(true)
				}
			}
	},
	{"start", {"Start the minecraft server", start_handler, {
															}
			}
	},
	{"stop", {"Stop the minecraft server", stop_handler, {
															}
			}
	},
	{"op", {"Give or remove your Operator rank", op_handler, {
															}

			}
	},
	{"blocker", {"Blocks server stop or start as long as an blocker exists", blocker_handler, 
				{
					dpp::command_option(dpp::co_sub_command, "list", "Lists all the blockers").set_auto_complete(true),
					dpp::command_option(dpp::co_sub_command, "add", "Add an interaction blocker", false).set_auto_complete(true).
						add_option(dpp::command_option(dpp::co_string, "reason", "Reason for interaction blocker", true)),
					dpp::command_option(dpp::co_sub_command, "remove", "Remove an interaction blocker", false).
						add_option(dpp::command_option(dpp::co_integer, "number", "Choose the interaction blocker you aim to delete", true))
				}
			}
	}
};


int main()
{
	try {
		Application app(CONFIG_FILE, commands);
		app.Run();
	}
	catch (nlohmann::detail::parse_error e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}