#include "shader.hpp"
#include "../common.hpp"
#include "../vertex_layout.hpp"

#include <d3dcompiler.h>
#include <stdio.h>

Shader::Shader() {

}

Shader::Shader(ID3D11Device1 *device, ID3D11DeviceContext1 *context) {
	this->device = device;
	this->context = context;
}

void Shader::init(ID3D11Device1 * device, ID3D11DeviceContext1 * context)
{
	this->device = device;
	this->context = context;
}

HRESULT Shader::create_shader(Shader_Type type, LPCWSTR source_file, LPCSTR shader_profile)
{
	HRESULT hr;
	LPCSTR entry_point;

	//Set shader information according to type.
	switch (type) {
	case Shader_Type::VERTEX_SHADER: {
		entry_point = "VS";
		compile_shader(source_file, entry_point, shader_profile, &blob_shader);

		hr = device->CreateVertexShader(
			blob_shader->GetBufferPointer()
			, blob_shader->GetBufferSize()
			, nullptr
			, &vertex_shader);

		if (FAILED(hr))
		{
			printf("Creating vertex shader failed.\n");
			return hr;
		}
		else
			printf("Creating vertex shader completed.\n");

		create_input_layout(blob_shader);
	} break;


	case Shader_Type::PIXEL_SHADER: {
		entry_point = "PS";
		compile_shader(source_file, entry_point, shader_profile, &blob_shader);

		hr = device->CreatePixelShader(
			blob_shader->GetBufferPointer()
			, blob_shader->GetBufferSize()
			, nullptr
			, &pixel_shader);

		if (FAILED(hr))
		{

			return hr;
		}
		else
			printf("Creating pixel shader completed.\n");
	} break;

	default:
		break;
	}

	if (FAILED(hr))
	{

	}

	if (blob_shader)
		blob_shader->Release();

	return hr;
}

void Shader::create_input_layout(ID3DBlob * blob_vertex_shader)
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC test_layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
			  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
			  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
			  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
			  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
			  D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};

	int num_element = sizeof(test_layout) / sizeof(test_layout[0]);
	hr = device->CreateInputLayout(test_layout, num_element, blob_shader->GetBufferPointer(), blob_shader->GetBufferSize(), &input_layout);
	
	if (SUCCEEDED(hr)) {

	}
	else
		printf("Creating input layout completed.\n");
}

HRESULT Shader::compile_shader(LPCWSTR file, LPCSTR entry_point, LPCSTR shader_version, ID3DBlob ** shader)
{
	if (!file || !entry_point || !shader_version || !shader)
		return E_INVALIDARG;

	*shader = nullptr;

	u32 flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	//ID3DBlob is interface used to return data of arbitrary length.
	ID3DBlob* blob_shader;
	ID3DBlob* blob_error;

	HRESULT hr;

	//If shader contains a #include, must set pInclude to D3D_COMPILE_STANDARD_FILE_INCLUDE.
	hr = D3DCompileFromFile(file, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point, shader_version, flags, NULL, &blob_shader, &blob_error);

	if (FAILED(hr)) {
		//If there is no error, blob_error is NULL.
		if (blob_error) {
			OutputDebugStringA((char*)blob_error->GetBufferPointer());
			printf((char*)blob_error->GetBufferPointer());
			blob_error->Release();
		}

		if (blob_shader) {
			blob_shader->Release();
		}
	}

	printf("Complete compiling shader\n");

	*shader = blob_shader;

	return hr;
}

void Shader::render()
{
	context->VSSetShader(vertex_shader, nullptr, 0);
	context->PSSetShader(pixel_shader, nullptr, 0);
	context->IASetInputLayout(input_layout);
}

void Shader::destroy()
{
	if (vertex_shader)
		vertex_shader->Release();

	if (pixel_shader)
		pixel_shader->Release();

	if (input_layout)
		input_layout->Release();
}

void init(Shader * shader)
{

}

void cleanup(Shader * shader)
{
	if (shader->vertex_shader)
		shader->vertex_shader->Release();

	if (shader->pixel_shader)
		shader->pixel_shader->Release();

	if (shader->input_layout)
		shader->input_layout->Release();
}
