#pragma once

#include "../common.hpp"

#include "../Device/d3d.hpp"
#include "../Device/platform.hpp"
#include "../Device/input.hpp"
#include "../Interface/i_execute.hpp"
#include "../System/program.hpp"

#define DEBUG

#ifdef DEBUG
#include "../debug.hpp"
#endif

bool should_quit = false;

void Run(string title, int window_width, int window_height, Execute::I_Execute* exe)
{
	SDL2 sdl2;
	Input input;
	D3D directX;

	//Init device
	{
		init(&sdl2);
		init(&input);
		directX.init();
	}

	//Init program
	Program program(&directX);
	{
		program.init();
	}


	//Loop
	{
		float color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		
		while (!should_quit) {
			Event::process_pending_events(&input);

		directX.get_context()->ClearRenderTargetView(directX.get_back_buffer_rtv(), color);
		directX.get_context()->ClearDepthStencilView(directX.depth_stencil_view, D3D11_CLEAR_DEPTH, 1, 0);

		program.update();
		program.render();

		directX.get_swap_chain()->Present(1, 0);
		}

	}

	//Destroy
	{
		program.destroy();
		directX.destroy();
		cleanup(&input);
		cleanup(&sdl2);
	}

#ifdef DEBUG
	D3D::ReportLiveObjects();
#endif
}