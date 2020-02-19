#pragma once

#include <d3d11_1.h>
#include <dxgi.h>

enum class Shader_Type {
	VERTEX_SHADER,
	PIXEL_SHADER,
};

struct Shader {
	Shader_Type shader_type;

	ID3D11Device1 *device;
	ID3D11DeviceContext1 *context;

	ID3D11VertexShader *vertex_shader;
	ID3D11PixelShader *pixel_shader;
	ID3D11InputLayout *input_layout;

	ID3DBlob *blob_shader;

	Shader();

	Shader(ID3D11Device1 *device, ID3D11DeviceContext1 *context);

	void init(ID3D11Device1 *device, ID3D11DeviceContext1 *context);

	HRESULT create_shader(Shader_Type type, LPCWSTR source_file, LPCSTR shader_profile);
	
	void create_input_layout(ID3DBlob* blob_vertex_shader);

	HRESULT compile_shader(_In_ LPCWSTR file, _In_ LPCSTR entry_point, _In_ LPCSTR shader_version, _Outptr_ ID3DBlob** shader);

	void render();

	void destroy();
};

void init(Shader *shader);

void cleanup(Shader *shader);
