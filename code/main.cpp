#define NOMINMAX

#include "graphics.cpp"
#include "platform.cpp"
#include "input.cpp"

#include "Device/directX.cpp"
#include "assimp/loader.cpp"

#include "Utility/BinaryFile.cpp"
#include "Utility/Path.cpp"
#include "Utility/String.cpp"
#include "Utility/Xml.cpp"

#define DEBUG

#ifdef DEBUG
#include "debug.hpp"
#endif

#include <stdio.h>

bool should_quit = false;
long use_count = 0;
/*
	tldr; avoid using global variables and prefer being clear about data flow

	consider not using any global defined variables to avoid confusion.
	this approach may benefits us with improved understandability. sicne
	codes in functions only contains what is known in the functions' scope,
	it can be easily understood by just reading things inside the function.
*/

void process_pending_events(Input *input) {

	SDL_Event event;
	for (;;) {

		int got_event = SDL_PollEvent(&event);

		if (!got_event) break;

		switch (event.type) {

		case SDL_KEYDOWN:
		case SDL_KEYUP: {
			auto key_code = event.key.keysym.sym;
			auto key_state = event.key.state;
			auto key_repeat = event.key.repeat;
			bool first_hit = (key_state == SDL_PRESSED && !key_repeat);

			if (key_code == SDLK_ESCAPE && first_hit) {
				input->command[Input::CMD_CHANGE_COLOR] = true;
			}

			if (key_code == SDLK_w && key_repeat) {
				input->command[Input::CMD_MOVEMENT] = true;
			}

			if (key_code == SDLK_s && key_repeat) {
				input->command[Input::CMD_MOVEMENT] = true;
			}

			if (key_code == SDLK_d && key_repeat) {
				input->command[Input::CMD_MOVEMENT] = true;
			}

			if (key_code == SDLK_a && key_repeat) {
				input->command[Input::CMD_MOVEMENT] = true;
			}

		} break;

		case SDL_QUIT: {
			should_quit = true;
			printf("should_quit is true.\n");
		} break;
		}
	}
}

void main_loop(Graphics* graphics, Input *input) {
	D3D11 &d3d11 = graphics->d3d11;

	float color[4] = { 0.5f, 0.5f, 0.1f, 1.0f };
	d3d11.context->ClearRenderTargetView(d3d11.render_target_view, color);
	d3d11.context->ClearDepthStencilView(d3d11.depth_stencil_view, D3D11_CLEAR_DEPTH, 1, 0);
	while (!should_quit) {
		/*
		this will used later for loop timing code:

		static u64 prev_count = SDL_GetPerformanceCounter();
		static u64 curr_count = SDL_GetPerformanceCounter();
		static u64 frequency = SDL_GetPerformanceFrequency();
		prev_count = curr_count;
		curr_count = SDL_GetPerformanceCounter();
		f64 elapsed_time = (f64)(curr_count - prev_count) / (f64)frequency;
		*/

		{
#ifdef DEBUG
			TIMED_BLOCK("Event & Input Precessing");
#endif
			zero_struct(input);
			process_pending_events(input);
		}

		d3d11.context->ClearRenderTargetView(d3d11.render_target_view, color);
		d3d11.context->ClearDepthStencilView(d3d11.depth_stencil_view, D3D11_CLEAR_DEPTH, 1, 0);

		d3d11.swap_chain->Present(1, 0);
	}
}

void main_loop_2(DirectX* directX, Input *input) {
	float color[4] = { 0.5f, 0.5f, 0.1f, 1.0f };
	directX->get_context()->ClearRenderTargetView(directX->get_back_buffer_rtv(), color);
	//directX->context->ClearDepthStencilView(d3d11.depth_stencil_view, D3D11_CLEAR_DEPTH, 1, 0);
	while (!should_quit) {
		/*
		this will used later for loop timing code:

		static u64 prev_count = SDL_GetPerformanceCounter();
		static u64 curr_count = SDL_GetPerformanceCounter();
		static u64 frequency = SDL_GetPerformanceFrequency();
		prev_count = curr_count;
		curr_count = SDL_GetPerformanceCounter();
		f64 elapsed_time = (f64)(curr_count - prev_count) / (f64)frequency;
		*/

		{
#ifdef DEBUG
			TIMED_BLOCK("Event & Input Precessing");
#endif
			zero_struct(input);
			process_pending_events(input);
		}

		directX->get_context()->ClearRenderTargetView(directX->get_back_buffer_rtv(), color);
		//directX->context->ClearDepthStencilView(d3d11.depth_stencil_view, D3D11_CLEAR_DEPTH, 1, 0);

		directX->get_swap_chain()->Present(1, 0);
	}
}

int main(int, char**)
{
	{
		SDL2 sdl2;
		Input input;
		Graphics graphics;
		DirectX directX;
		Assimp_Loader* kachujin_loader = new Assimp_Loader(
			L"_asset/fbx_model/Kachujin/Kachujin.fbx"
			, L"_model/Kachujin/"
			, L"Kachujin");
		kachujin_loader->export_material();
		kachujin_loader->export_mesh();

		init(&sdl2);
		init(&input);
		init(&graphics);
		directX.init();
		
		main_loop_2(&directX, &input);

		kachujin_loader->destroy();
		cleanup(&input);
		cleanup(&sdl2);
		cleanup(&graphics);
		directX.destroy();
	}

#ifdef DEBUG
	report_live_objects();
#endif
	return 0;
}