#pragma once
#define NOMINMAX

#include "common.hpp"

#include "assimp/loader.cpp"

#include "Component/transform.cpp"
#include "Component/model_component.cpp"

#include "Device/d3d.cpp"
#include "Device/platform.cpp"
#include "Device/input.cpp"

#include "Execute/test_model.cpp"
#include "Execute/test_object.cpp"

#include "Model/model.cpp"
#include "Model/model_bone.cpp"
#include "Model/model_material.cpp"

#include "Object/object.cpp"

#include "System/launcher.cpp"
#include "System/program.cpp"

#include "Render/render_system.cpp"
#include "Render/shader.cpp"

#include "Utility/BinaryFile.cpp"
#include "Utility/Path.cpp"
#include "Utility/String.cpp"
#include "Utility/Xml.cpp"
#include "Utility/WICTextureLoader.cpp"

#include <crtdbg.h>
#if _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#ifdef DEBUG
#include "debug.hpp"
#endif

#include <stdio.h>

/*
	tldr; avoid using global variables and prefer being clear about data flow

	consider not using any global defined variables to avoid confusion.
	this approach may benefits us with improved understandability. sicne
	codes in functions only contains what is known in the functions' scope,
	it can be easily understood by just reading things inside the function.
*/


void main_loop_2(D3D* directX, Input *input, Model* model) {
	float color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	directX->get_context()->ClearRenderTargetView(directX->get_back_buffer_rtv(), color);
	directX->get_context()->ClearDepthStencilView(directX->depth_stencil_view, D3D11_CLEAR_DEPTH, 1, 0);
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
			//process_pending_events(input);
		}


		directX->get_context()->ClearRenderTargetView(directX->get_back_buffer_rtv(), color);
		directX->get_context()->ClearDepthStencilView(directX->depth_stencil_view, D3D11_CLEAR_DEPTH, 1, 0);
		model->render();

		directX->get_swap_chain()->Present(1, 0);
	}
}

int main(int, char**)
{
//	{
//		SDL2 sdl2;
//		Input input;
//		D3D directX;
//
//		init(&sdl2);
//		init(&input);
//		directX.init();
//
//		/*Assimp_Loader* kachujin_loader = new Assimp_Loader(
//			L"_asset/fbx_model/Kachujin/Kachujin.fbx"
//			, L"_model/Kachujin/"
//			, L"Kachujin");*/
//		//kachujin_loader->export_material();
//		//kachujin_loader->export_mesh();
//
//		Model* kachujin = new Model(&directX);
//		kachujin->load_model(L"Kachujin");
//
//		World_Buffer* world_buffer;
//		world_buffer = new World_Buffer(directX.get_device(), directX.get_context());
//
//		Perspective_Buffer* perspective_buffer;
//		perspective_buffer = new Perspective_Buffer(directX.get_device(), 1280.0f, 720.0f);
//
//		Eye_Buffer* eye_buffer;
//		eye_buffer = new Eye_Buffer(directX.get_device(), directX.get_context(), { 0.0f, 200.0f, -500.0f });
//
//		directX.get_context()->VSSetConstantBuffers(0, 1, &world_buffer->buffer);
//		directX.get_context()->VSSetConstantBuffers(1, 1, &eye_buffer->buffer);
//		directX.get_context()->VSSetConstantBuffers(2, 1, &perspective_buffer->buffer);
//
//		main_loop_2(&directX, &input, kachujin);
//
//		//kachujin_loader->destroy();
//
//		cleanup_buffer(world_buffer->buffer);
//		cleanup_buffer(perspective_buffer->buffer);
//		cleanup_buffer(eye_buffer->buffer);
//		SAFE_DELETE(world_buffer);
//		SAFE_DELETE(perspective_buffer);
//		SAFE_DELETE(eye_buffer);
//
//		kachujin->destroy();
//		cleanup(&input);
//		cleanup(&sdl2);
//		directX.destroy();
//
//	}
//
//#ifdef DEBUG
//	//report_live_objects();
//#endif
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//_CrtSetBreakAlloc(2136);
	Run("JoonPortfolio", 1280, 720, nullptr);

	return 0;
}