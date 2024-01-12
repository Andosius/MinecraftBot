// Local dependencies
#include "commands/blocker.hpp"

// External dependencies

// Standard Library

//======================================


static std::vector<std::string> s_Blockers = {};
static std::mutex s_BlockerMutex = std::mutex();



bool IsInteractionBlocked()
{
	std::lock_guard l(s_BlockerMutex);
	return s_Blockers.size() != 0;
}

void blocker_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
	std::lock_guard l(s_BlockerMutex);
	dpp::interaction interaction = event.command;
	dpp::command_interaction cmd_data = interaction.get_command_interaction();

	dpp::command_data_option& subcommand = cmd_data.options[0];
	std::string action = subcommand.name;

	event.thinking();

	std::string answer;

	if (action == "list")
	{
		answer = "Currently available interaction blockers:\n\n";

		if (s_Blockers.size() != 0)
		{
			for (int i = 0; i < s_Blockers.size(); i++)
			{
				answer += fmt::format("{0} -> Reason: {1}", i + 1, s_Blockers[i]);
			}
			answer += "\n\n";
		}

		answer += fmt::format("{0} interaction blockers available.", s_Blockers.size());

		event.edit_original_response(answer);
	}
	else if (action == "add")
	{
		dpp::command_value blocker_param = event.get_parameter("reason");
		std::string reason = std::get<std::string>(blocker_param);

		if (reason != "")
		{
			s_Blockers.push_back(reason);
			answer = fmt::format("Successfully added \"{0}\" to the blocker list!", reason);

			event.edit_original_response(
				dpp::message().
				add_embed(
					dpp::embed().
					set_color(dpp::colors::green).
					set_title("Added item to blocker list!").
					set_description(answer)
				)
			);
		}
		else
		{
			event.edit_original_response(
				dpp::message().
				add_embed(
					dpp::embed().
					set_color(dpp::colors::red).
					set_title("Failed to add item to blocker list!").
					set_description("\"reason\" can't be empty!")
				)
			);
		}
	}
	else if (action == "remove")
	{
		dpp::command_value blocker_param = event.get_parameter("number");
		uint64_t number = std::get<int64_t>(blocker_param);
		number -= 1;

		if (number >= s_Blockers.size() || number < 0)
		{
			event.edit_original_response(
				dpp::message().
				add_embed(
					dpp::embed().
					set_color(dpp::colors::red).
					set_title("Failed to remove item from blocker list!").
					set_description("\"number\" doesn't match any interaction blocker index!")
				)
			);
		}
		else
		{
			answer = fmt::format("Successfully removed {} from the list!", s_Blockers[number]);

			s_Blockers.erase(s_Blockers.begin() + number);

			event.edit_original_response(
				dpp::message().
				add_embed(
					dpp::embed().
					set_color(dpp::colors::green).
					set_title("Removed item from blocker list!").
					set_description(answer)
				)
			);
		}
	}

}
