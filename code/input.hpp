#pragma once

/*
	Input is used in logic code. command will be 'true' for one frame
	when matching key action is made. so we can use this like below:
		if (input->command[CMD_OPEN_CONSOLE]) {
			open_console_window();
		}
*/
struct Input {
	enum Command {
		CMD_CHANGE_COLOR, // for test
		CMD_MOVEMENT,
		CMD_ENUM_COUNT,
	};
	bool command[CMD_ENUM_COUNT];

};

void init(Input *input);
void cleanup(Input *input);