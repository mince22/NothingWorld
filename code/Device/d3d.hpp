#pragma once
#include "../common.hpp"
#include "../Render/rasterizer_state.hpp"
#include "../Render/sampler_state.hpp"
#include "../Render/shader.hpp"

#include <d3d11_1.h>
#include <dxgi.h>
#include <dxgidebug.h>

class D3D {
public:
	D3D() = default;
	//Disallow copy contructor
	D3D(const D3D&) = delete;
	//Disallow assign
	D3D& operator=(const D3D&) = delete;

	void init();

	void create_device_and_swap_chain();

	void create_back_buffer_rtv();

	void destroy();

	ID3D11Device1* get_device() const { return device; }
	ID3D11RenderTargetView* get_back_buffer_rtv() const { return back_buffer_rtv; }
	ID3D11DeviceContext1* get_context() const { return context; }
	IDXGISwapChain1* get_swap_chain() const { return swap_chain; }

	void set_rasterizer_state(Rasterizer_State_Type type);


#ifdef DEBUG
public:
	static void ReportLiveObjects();
#endif

private:
	ID3D11Device1 *device;
	ID3D11DeviceContext1 *context;
	IDXGIFactory2 *factory;
	IDXGISwapChain1 *swap_chain;
	ID3D11RenderTargetView *back_buffer_rtv;

public:
	ID3D11Texture2D* depth_stencil_buffer;
	ID3D11DepthStencilState* depth_stencil_state;
	ID3D11DepthStencilView* depth_stencil_view;

	std::array<ID3D11RasterizerState *, max_count_rasterizer_state> rasterizer_states;
	std::array<ID3D11SamplerState *, max_count_sampler_state> sampler_states;
};