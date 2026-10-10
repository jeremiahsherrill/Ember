#include <iostream>
#include <string>
#include <vector>

static void PrintHelp()
{
	std::cout <<
		"ember-cli — Ember Editor automation client\n"
		"\n"
		"Usage: ember-cli [--port N] <command> [args...]\n"
		"\n"
		"Commands (JSON-RPC; server starts in M2):\n"
		"  scene.new\n"
		"  scene.save\n"
		"  scene.describe\n"
		"  entity.create --name <name>\n"
		"  component.add --entity <id> --type <type>\n"
		"  property.set --entity <id> --component <type> --property <name> --value <v>\n"
		"  asset.import <path>\n"
		"  terrain.sculpt --brush <raise|lower|smooth|flatten|paint> --pos x,z --radius r --strength s\n"
		"  viewport.screenshot <path>\n"
		"  play.start | play.stop\n"
		"  project.export --platform <windows|linux|macos>\n"
		"  test.run\n"
		"\n"
		"M0 status: stub client. Connect implementation lands in M2 with the Automation server.\n";
}

int main(int argc, char** argv)
{
	std::vector<std::string> args(argv + 1, argv + argc);
	if (args.empty() || args[0] == "--help" || args[0] == "-h")
	{
		PrintHelp();
		return 0;
	}

	std::string port = "7777";
	std::vector<std::string> cmd;
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i] == "--port" && i + 1 < args.size())
		{
			port = args[++i];
			continue;
		}
		cmd.push_back(args[i]);
	}

	if (cmd.empty())
	{
		PrintHelp();
		return 1;
	}

	std::cout << "[ember-cli] Would send to localhost:" << port << " -> ";
	for (auto& c : cmd)
		std::cout << c << " ";
	std::cout << "\n[ember-cli] Automation server not yet available (arrives in M2).\n";
	return 0;
}
