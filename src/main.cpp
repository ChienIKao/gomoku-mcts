#include <windows.h>

#include <iostream>

#include "core/agent.h"

int main() {
	SetConsoleOutputCP(CP_UTF8);

	Agent agent = Agent();

	agent.game_start();

	return 0;
}
