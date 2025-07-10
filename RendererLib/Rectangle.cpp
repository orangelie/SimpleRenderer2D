#include "pch.h"
#include "Rectangle.h"
#include "GameLoader.h"
#include "RenderEngine.h"
#include "Shader.h"

void Geo::Rectangle::Start(GeometryOption option)
{
	position = option.position;

	vector<CPUVertexData> vertices;
	{
		vertices.assign(4, {});

		vertices[0] = { { -1.0F, 1.0F }, { 0.0F, 0.0F } };
		vertices[1] = { { 1.0F, 1.0F }, { 1.0F, 0.0F } };
		vertices[2] = { { 1.0F, -1.0F }, { 1.0F, 1.0F } };
		vertices[3] = { { -1.0F, -1.0F }, { 0.0F, 1.0F } };
	}

	D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.ByteWidth = vertices.size() * sizeof(CPUVertexData);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices.data();

	CHECK(DEVICE->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf()));


	vector<uint32> indices;
	{
		indices.assign(6, {});

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;

		_indexCount = indices.size();
	}

	desc = {};
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = (uint32)(sizeof(uint32) * indices.size());

	data = {};
	data.pSysMem = indices.data();

	CHECK(DEVICE->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf()));
}

void Geo::Rectangle::Update()
{
	Matrix matScale = Matrix::CreateScale(Vec3(scale.x, scale.y, 1.0F));
	Matrix matRotation = Matrix::CreateRotationZ(rotation * (XM_PI / 180.0F));
	Matrix matTranslation = Matrix::CreateTranslation(Vec3(position.x, position.y, 1.0F));

	_settings.World = matScale * matRotation * matTranslation;

	SHADER->PushConstantBuffer(_settings);
}

void Geo::Rectangle::Render()
{
	uint32 stride = sizeof(CPUVertexData);
	uint32 offset = 0;

	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DC->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	DC->DrawIndexedInstanced(_indexCount, 1, 0, 0, 0);
}
