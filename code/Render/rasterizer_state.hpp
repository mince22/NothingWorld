#pragma once

#include "../common.hpp"
#include <d3d11_1.h>
#include <assert.h>

enum class Rasterizer_State_Type : unsigned int
{
	CW_Default = 0,
	CW_Wireframe,
	CW_Disable_Culling,
	MAX
};

constexpr u32 max_count_rasterizer_state = (u32)Rasterizer_State_Type::MAX;

void init_rasterizer_state(ID3D11Device1* device, std::array<ID3D11RasterizerState*, (u32)Rasterizer_State_Type::MAX>& rasterizer_states)
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC desc;
	desc.AntialiasedLineEnable = false;
	desc.CullMode = D3D11_CULL_BACK;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;

	ID3D11RasterizerState* rasterizer;
	hr = device->CreateRasterizerState(&desc, &rasterizer);
	assert(SUCCEEDED(hr));
	rasterizer_states[(u32)Rasterizer_State_Type::CW_Default] = rasterizer;

	desc.FillMode = D3D11_FILL_WIREFRAME;
	hr = device->CreateRasterizerState(&desc, &rasterizer);
	assert(SUCCEEDED(hr));
	rasterizer_states[(u32)Rasterizer_State_Type::CW_Wireframe] = rasterizer;

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	hr = device->CreateRasterizerState(&desc, &rasterizer);
	assert(SUCCEEDED(hr));
	rasterizer_states[(u32)Rasterizer_State_Type::CW_Disable_Culling] = rasterizer;
}

void cleanup_rasterizer_state(ID3D11RasterizerState** rasterizer_state) 
{
	for (u32 ii = 0; ii < (u32)Rasterizer_State_Type::MAX; ii++)
		rasterizer_state[ii]->Release();
}
//
//struct Rasterizer_State {
//	ID3D11RasterizerState1* rasterizer_state[(unsigned int)Rasterizer_State_Type::MAX];
//	
//	void init_rasterizer_state(ID3D11Device1* device) {
//		HRESULT hr;
//
//		D3D11_RASTERIZER_DESC1 raster_desc;
//		raster_desc.AntialiasedLineEnable = false;
//		raster_desc.CullMode = D3D11_CULL_BACK;
//		raster_desc.DepthBias = 0;
//		raster_desc.DepthBiasClamp = 0.0f;
//		raster_desc.DepthClipEnable = true;
//		raster_desc.FillMode = D3D11_FILL_SOLID;
//		raster_desc.FrontCounterClockwise = false;
//		raster_desc.MultisampleEnable = false;
//		raster_desc.ScissorEnable = false;
//		raster_desc.SlopeScaledDepthBias = 0.0f;
//
//		hr = device->CreateRasterizerState1(&raster_desc, &(rasterizer_state[(u32)Rasterizer_State_Type::CW_Default]));
//		assert(SUCCEEDED(hr));
//
//		raster_desc.FillMode = D3D11_FILL_WIREFRAME;
//		hr = device->CreateRasterizerState1(&raster_desc, &(rasterizer_state[(u32)Rasterizer_State_Type::CW_Wireframe]));
//		assert(SUCCEEDED(hr));
//
//		raster_desc.FillMode = D3D11_FILL_SOLID;
//		raster_desc.CullMode = D3D11_CULL_NONE;
//		hr = device->CreateRasterizerState1(&raster_desc, &(rasterizer_state[(u32)Rasterizer_State_Type::CW_Disable_Culling]));
//		assert(SUCCEEDED(hr));
//	};
//
//	void destroy() {
//		for (u32 ii = 0; ii < (u32)Rasterizer_State_Type::MAX; ii++)
//			rasterizer_state[ii]->Release();
//	}
//};