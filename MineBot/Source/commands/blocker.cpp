// Local dependencies
#include "commands/blocker.hpp"
#include "CommandMutex.hpp"

// External dependencies

// Standard Library

//======================================


void blocker_handler(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
	event.thinking();
	std::lock_guard<std::mutex> lock(s_CommandMutex);


	dpp::interaction interaction = event.command;
	dpp::command_interaction cmd_data = interaction.get_command_interaction();

	dpp::command_data_option& subcommand = cmd_data.options[0];
	std::string action = subcommand.name;


	if (action == "list")
	{
		std::string answer = "";

		if (s_Blockers.size() > 0)
		{
			for (size_t i = 0; i < s_Blockers.size(); i++)
			{
				answer += fmt::format("{0:>2} -> Grund: {1}\n", (i + 1), s_Blockers[i]);
			}

			answer += "\n\n";
		}

		answer += fmt::format("Es sind {0} Blocker aktiv!", s_Blockers.size());


		event.edit_original_response(
			dpp::message().add_embed(
				dpp::embed().
				set_color(dpp::colors::green).
				set_title("Blocker-Übersicht").
				set_description(answer)
			)
		);
		return;
	}
	else if (action == "add")
	{
		dpp::command_value blocker_param = event.get_parameter("reason");
		std::string reason = std::get<std::string>(blocker_param);

		if (reason == "")
		{
			event.edit_original_response(
				dpp::message().
				add_embed(
					dpp::embed().
					set_color(dpp::colors::red).
					set_title("Blocker kann nicht hinzugefügt werden!").
					set_description("Der Grund darf nicht leer sein.")
				)
			);
			return;
		}

		s_Blockers.push_back(reason);

		event.edit_original_response(
			dpp::message().
			add_embed(
				dpp::embed().
				set_color(dpp::colors::green).
				set_title("Blocker hinzugefügt!").
				set_description(fmt::format("Der Blocker \"{}\" wurde hinzugefügt.", reason))
			)
		);
		return;
	}
	else if (action == "remove")
	{
		dpp::command_value blocker_param = event.get_parameter("number");
		uint64_t number = std::get<int64_t>(blocker_param);

		number -= 1;

		if (number >= 0 && number < s_Blockers.size())
		{
			event.edit_original_response(
				dpp::message().
				add_embed(
					dpp::embed().
					set_color(dpp::colors::green).
					set_title("Blocker entfernt!").
					set_description(fmt::format("Der Blocker \"{}\" wurde entfernt.", s_Blockers[number]))
				)
			);

			s_Blockers.erase(s_Blockers.begin() + number);
			return;
		}
		else
		{
			event.edit_original_response(
				dpp::message().
				add_embed(
					dpp::embed().
					set_color(dpp::colors::green).
					set_title("Blocker kann nicht entfernt werden!").
					set_description(fmt::format("Der Blocker an der Stelle {} existiert nicht.", (number + 1)))
				)
			);
			return;
		}
	}
}
