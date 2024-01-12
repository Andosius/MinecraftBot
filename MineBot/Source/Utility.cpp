// Local dependencies
#include "Utility.hpp"

// External dependencies

// Standard Library

//======================================


bool IsServerRunning()
{
	std::string result = Application::SendCommand(fmt::format("screen -ls | grep \"{}\"", Application::Get()->m_RemoteScreenName));

	if (result == "")
		return false;

	return true;
}

bool IsValidMinecraftName(const std::string& name)
{
	// Check for name length (3-16 chars)
	if (name.length() < 3 || name.length() > 16) {
		return false;
	}

	// A-Z, a-z, 0-9 and _ are allowed
	std::regex regex("[A-Za-z0-9_]+");
	if (!std::regex_match(name, regex)) {
		return false;
	}

	return true;
}

std::string GetAccountsContent()
{
	std::string content = "";
	std::ifstream input_file("accounts.nlohmann::json", std::ios_base::app);

	std::string line;
	while (std::getline(input_file, line))
		content += line;

	if (content == "")
		return "[]";

	return content;
}

void WriteAccounts(nlohmann::json accounts)
{
	std::ofstream output_file("accounts.nlohmann::json", std::ios_base::trunc);
	output_file << accounts.dump() << std::endl;
}

nlohmann::json GetAccounts()
{
	nlohmann::json accounts = nlohmann::json::parse(GetAccountsContent());
	return accounts;
}

bool IsUserWhitelisted(const uint64_t& account_id, size_t& idx)
{
	bool found = false;

	nlohmann::json accounts = GetAccounts();

	for (int i = 0; i < accounts.size(); i++)
	{
		if (accounts[i]["account_id"].get<uint64_t>() == account_id)
		{
			idx = i;
			found = true;
			break;
		}
	}

	return found;
}
