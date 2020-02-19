#pragma once
#include "Device/platform.hpp"
#include "common.hpp"
#include <stdio.h>

/*
	this code is experimental, error-prone and incomplete.
	use case looks like this :
		{
			TIMED_BLOCK("update player position")

			~ codes that update player position ~
		}
*/
#define TIMED_BLOCK(name) Timed_Block timed_block_##Number(name)
struct Timed_Block {
    Timed_Block(const char *name) {
    	time = __rdtsc();
    	block_name = name;
    }
    ~Timed_Block() {
    	// later if we have text rendering then we can see this on screen..
    	printf("%.6llf ms : %s\n", (double)(__rdtsc() - time) / (double)SDL_GetPerformanceFrequency(), block_name);
    }
    u64 time;
    const char* block_name;
};