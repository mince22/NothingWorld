#pragma once

#include "../common.hpp"
#include <d3d11_1.h>
#include <assert.h>

enum class Sampler_State_Type : unsigned int
{
	Anisotropic = 0,
	Linear,
	Point,
	Shadow_Less_Equal_Comp,
	Shadow_Greater_Equal_Comp,
	Shadow_Linear,
	MAX
};

constexpr u32 max_count_sampler_state = (u32)Sampler_State_Type::MAX;

void init_sampler_state(ID3D11Device1* device, std::array<ID3D11SamplerState*, (u32)Sampler_State_Type::MAX>& sampler_states)
{
	HRESULT hr;

	//Texture sampler
	D3D11_SAMPLER_DESC desc;
	memset(&desc, 0, sizeof(D3D11_SAMPLER_DESC));

	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MaxAnisotropy = 16;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = -D3D11_FLOAT32_MAX;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* sampler;
	hr = device->CreateSamplerState(&desc, &sampler);
	assert(SUCCEEDED(hr));
	sampler_states[(u32)Sampler_State_Type::Anisotropic] = sampler;

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = device->CreateSamplerState(&desc, &sampler);
	assert(SUCCEEDED(hr));
	sampler_states[(u32)Sampler_State_Type::Linear] = sampler;

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = device->CreateSamplerState(&desc, &sampler);
	assert(SUCCEEDED(hr));
	sampler_states[(u32)Sampler_State_Type::Point] = sampler;

	//Shadow sampler state
	{
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 1.0f;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = 0;

		hr = device->CreateSamplerState(&desc, &sampler);
		assert(SUCCEEDED(hr));
		sampler_states[(u32)Sampler_State_Type::Shadow_Less_Equal_Comp] = sampler;
	}
	
	{
		desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0.0f;
		desc.ComparisonFunc = D3D11_COMPARISON_GREATER;

		hr = device->CreateSamplerState(&desc, &sampler);
		assert(SUCCEEDED(hr));
		sampler_states[(u32)Sampler_State_Type::Shadow_Greater_Equal_Comp] = sampler;
	}

	{
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 0;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;//D3D11_COMPARISON_ALWAYS;
		desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = 0;

		hr = device->CreateSamplerState(&desc, &sampler);
		assert(SUCCEEDED(hr));
		sampler_states[(u32)Sampler_State_Type::Shadow_Linear] = sampler;
	}
}

void cleanup_sampler_state(ID3D11SamplerState** sampler_state)
{
	for (u32 ii = 0; ii < (u32)Sampler_State_Type::MAX; ii++)
		sampler_state[ii]->Release();
}