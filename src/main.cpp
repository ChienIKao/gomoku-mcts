#include <windows.h>

#include <iostream>

#include "core/agent.h"

int main() {
	SetConsoleOutputCP(CP_UTF8);

    printf("\nInput example: 5 10 (row = 5, col = 10)\n");
    printf("Black first\n");

	Agent agent = Agent();

	agent.game_start();

	return 0;
}
