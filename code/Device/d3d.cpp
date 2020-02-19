#pragma once

#include "d3d.hpp"
#include "../common.hpp"

#include <assert.h>
#include <stdio.h>

extern HWND hwnd;

void D3D::init()
{
	create_device_and_swap_chain();
	create_back_buffer_rtv();

	init_rasterizer_state(device, rasterizer_states);
	context->RSSetState(rasterizer_states[(u32)Rasterizer_State_Type::CW_Default]);

	init_sampler_state(device, sampler_states);
	context->PSSetSamplers(0, max_count_sampler_state, &sampler_states[0]);

	HRESULT hr;
	//Create Depth stencil buffer
	{
		D3D11_TEXTURE2D_DESC depth_buffer_desc = { 0 };

		depth_buffer_desc.Width = 1280;
		depth_buffer_desc.Height = 720;
		depth_buffer_desc.MipLevels = 1;
		depth_buffer_desc.ArraySize = 1;
		depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_buffer_desc.SampleDesc.Count = 1;
		depth_buffer_desc.SampleDesc.Quality = 0;
		depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_buffer_desc.CPUAccessFlags = 0;
		depth_buffer_desc.MiscFlags = 0;

		hr = device->CreateTexture2D(&depth_buffer_desc, nullptr, &depth_stencil_buffer);
		assert(SUCCEEDED(hr));
	}


	//Create depth stencil
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc = { 0 };

		// Set up the description of the stencil state.
		depth_stencil_desc.DepthEnable = true;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

		depth_stencil_desc.StencilEnable = true;
		depth_stencil_desc.StencilReadMask = 0xFF;
		depth_stencil_desc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		hr = device->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state);
		assert(SUCCEEDED(hr));
	}

	//Create depth stencil view
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depths_stencil_view_desc;
		ZeroMemory(&depths_stencil_view_desc, sizeof(depths_stencil_view_desc));

		// Set up the depth stencil view description.
		depths_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depths_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depths_stencil_view_desc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(depth_stencil_buffer, &depths_stencil_view_desc, &depth_stencil_view);
		assert(SUCCEEDED(hr));
	}

	context->OMSetRenderTargets(1, &back_buffer_rtv, depth_stencil_view);

	//Create Viewport
	{
		D3D11_VIEWPORT viewport = { 0 };
		viewport.Width = 1280.0f;
		viewport.Height = 720.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		//Set Viewport
		context->RSSetViewports(1, &viewport);
	}
}

void D3D::create_device_and_swap_chain()
{
	HRESULT result;
	// Create Device
	{
		ComPtr<ID3D11Device> temp_device;
		ComPtr<ID3D11DeviceContext> temp_context;
		D3D_FEATURE_LEVEL level[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
		UINT device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
		device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, device_flags, level, ARRAYSIZE(level), D3D11_SDK_VERSION, &temp_device, 0, &temp_context);
		assert(SUCCEEDED(result));

		// Get 1.1 interface of D3D11 Device and Context
		result = temp_device->QueryInterface(__uuidof(ID3D11Device1), (void**)&device);
		assert(SUCCEEDED(result));

		result = temp_context->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&context);
		assert(SUCCEEDED(result));
	}

	// Get DXGI Factory (needed to create Swap Chain)
	{
		ComPtr<IDXGIDevice1> dxgi_device;
		result = device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgi_device);
		assert(SUCCEEDED(result));

		ComPtr<IDXGIAdapter> dxgi_adapter;
		result = dxgi_device->GetAdapter(&dxgi_adapter);
		assert(SUCCEEDED(result));

		result = dxgi_adapter->GetParent(__uuidof(IDXGIFactory2), (void**)&factory);
		assert(SUCCEEDED(result));
	}

	// Create Swap Chain
	{
		DXGI_SWAP_CHAIN_DESC1 desc;
		desc.Width = 0; // use window width
		desc.Height = 0; // use window height
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		desc.Stereo = FALSE;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 2;
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.Flags = 0;

		if (!hwnd) printf("we need hwnd to create swap chain!\n");

		result = factory->CreateSwapChainForHwnd(device, hwnd, &desc, 0, 0, &swap_chain);

		assert(SUCCEEDED(result));
	}
}

void D3D::create_back_buffer_rtv()
{
	ComPtr<ID3D11Texture2D> buffer;

	HRESULT result;

	result = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
	assert(SUCCEEDED(result));

	result = device->CreateRenderTargetView(buffer.Get(), 0, &back_buffer_rtv);
	assert(SUCCEEDED(result));
}

void D3D::destroy()
{
	SAFE_RELEASE(depth_stencil_buffer);
	SAFE_RELEASE(depth_stencil_state);
	SAFE_RELEASE(depth_stencil_view);

	SAFE_RELEASE(back_buffer_rtv);
	SAFE_RELEASE(swap_chain);
	SAFE_RELEASE(factory);
	SAFE_RELEASE(context);
	SAFE_RELEASE(device);

	cleanup_rasterizer_state(&rasterizer_states[0]);
	cleanup_sampler_state(&sampler_states[0]);
}

void D3D::set_rasterizer_state(Rasterizer_State_Type type)
{
	context->RSSetState(rasterizer_states[(u32)type]);
}


#ifdef DEBUG
void D3D::ReportLiveObjects()
{
	using fPtrDXGIGetDebugInterface = HRESULT(__stdcall*)(const IID&, void**);

	HMODULE hMod = GetModuleHandle(L"Dxgidebug.dll");
	fPtrDXGIGetDebugInterface DXGIGetDebugInterface = (fPtrDXGIGetDebugInterface)GetProcAddress(hMod, "DXGIGetDebugInterface");

	IDXGIDebug* debug = nullptr;
	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&debug);

	debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);
	debug->Release();
}
#endif // DEBUG