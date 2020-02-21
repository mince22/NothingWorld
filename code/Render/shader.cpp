#pragma once
#include "shader.hpp"
#include "../common.hpp"
#include "../vertex_layout.hpp"
#include "render_system.hpp"

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

HRESULT Shader::create_shader(Shader_Profile type, LPCWSTR source_file, LPCSTR shader_profile)
{
	HRESULT hr;
	LPCSTR entry_point;

	//Set shader information according to type.
	switch (type) {
	case Shader_Profile::VERTEX_SHADER: {
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


	case Shader_Profile::PIXEL_SHADER: {
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

Shader* Shader::create_shader(Shader_Type type)
{
	wstring shader_extension = L".hlsl";

	switch (type)
	{
	case Shader_Type::Model_Instance:
	{
		wstring source_file = SHADER_PATH + shader_type_name[u32(type)] + shader_extension;
		//wstring test = L"../_asset/Shader/" + shader_type_name[u32(type)] + shader_extension;
		create_shader(Shader_Profile::VERTEX_SHADER, source_file.c_str(), "vs_5_0");
		create_shader(Shader_Profile::PIXEL_SHADER, source_file.c_str(), "ps_5_0");
	}
		break;
	case Shader_Type::Max:
		break;
	default:
		break;
	}
	return this;
}

void Shader::create_input_layout(ID3DBlob * blob_vertex_shader)
{
	HRESULT hr;
	hr = D3DReflect
	(
		blob_vertex_shader->GetBufferPointer()
		, blob_vertex_shader->GetBufferSize()
		, IID_ID3D11ShaderReflection
		, (void**)&reflection
	);
	assert(SUCCEEDED(hr));

	D3D11_SHADER_DESC shader_desc;
	reflection->GetDesc(&shader_desc);

	vector<D3D11_INPUT_ELEMENT_DESC> input_layout_desc;
	for (u32 ii = 0; ii < shader_desc.InputParameters; ii++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC param_desc;
		reflection->GetInputParameterDesc(ii, &param_desc);

		D3D11_INPUT_ELEMENT_DESC element_desc;
		element_desc.SemanticName = param_desc.SemanticName;
		element_desc.SemanticIndex = param_desc.SemanticIndex;
		element_desc.InputSlot = 0;
		element_desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		element_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		element_desc.InstanceDataStepRate = 0;

		if (param_desc.Mask == 1)
		{
			if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				element_desc.Format = DXGI_FORMAT_R32_UINT;
			else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				element_desc.Format = DXGI_FORMAT_R32_SINT;
			else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				element_desc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (param_desc.Mask <= 3)
		{
			if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				element_desc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				element_desc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				element_desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (param_desc.Mask <= 7)
		{
			if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				element_desc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				element_desc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				element_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (param_desc.Mask <= 15)
		{
			if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				element_desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				element_desc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				element_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		string name = param_desc.SemanticName;
		transform(name.begin(), name.end(), name.begin(), toupper);

		if (name == "POSITION")
			element_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		else if (name == "INSTANCE")
		{
			element_desc.InputSlot = 1;
			element_desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			element_desc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			element_desc.InstanceDataStepRate = 1;
		}

		input_layout_desc.push_back(element_desc);
	}
	
	hr = device->CreateInputLayout(&input_layout_desc[0], input_layout_desc.size(), blob_shader->GetBufferPointer(), blob_shader->GetBufferSize(), &input_layout);
	
	if (SUCCEEDED(hr)) {

	}
	else
		printf("Creating input layout completed.\n");

	reflection->Release();
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
