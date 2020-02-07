#pragma once

#include "directX.hpp"
#include "../common.hpp"

#include <assert.h>
#include <stdio.h>

extern HWND hwnd;

void DirectX::init()
{
	create_device_and_swap_chain();
	create_back_buffer_rtv();
}

void DirectX::create_device_and_swap_chain()
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

void DirectX::create_back_buffer_rtv()
{
	ComPtr<ID3D11Texture2D> buffer;

	HRESULT result;

	result = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
	assert(SUCCEEDED(result));

	result = device->CreateRenderTargetView(buffer.Get(), 0, &back_buffer_rtv);
	assert(SUCCEEDED(result));
}

void DirectX::destroy()
{
	SAFE_RELEASE(back_buffer_rtv);
	SAFE_RELEASE(swap_chain);
	SAFE_RELEASE(factory);
	SAFE_RELEASE(context);
	SAFE_RELEASE(device);
}