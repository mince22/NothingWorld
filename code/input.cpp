#include "input.hpp"

void init(Input *input) {
	for (auto& it : input->command) it = 0;
}

void cleanup(Input *input) {
	// @Note:
	// this function is currently empty for now, when we add dynamically allocated
	// things later such as memory for controller or memory for key mapping
	// file, we deallocate those in here.

}