#include "pch.h"
#include "Circle.h"

void Geo::Circle::Start(GeometryOption option)
{
	position = option.position;

	const int sliceCount = 64;
	const float radius = 1.0F;

	vector<CPUVertexData> vertices;
	vertices.reserve(sliceCount + 2);

	// 중심점 추가
	vertices.push_back({ { 0.0F, 0.0F }, { 0.5F, 0.5F } });

	for (int i = 0; i <= sliceCount; ++i)
	{
		float angle = XM_2PI * i / sliceCount;
		float x = cosf(angle) * radius;
		float y = -sinf(angle) * radius;

		float u = (x + 1.0F) * 0.5F;  // [-1,1] -> [0,1]
		float v = (1.0F - y) * 0.5F;  // [-1,1] -> [0,1], Y축 반전

		vertices.push_back({ { x, y }, { u, v } });
	}

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = vertices.size() * sizeof(CPUVertexData);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices.data();

	CHECK(DEVICE->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf()));


	vector<uint32> indices;
	indices.reserve(sliceCount * 3);

	for (uint32 i = 1; i <= sliceCount; ++i)
	{
		indices.push_back(0);         // 중심점
		indices.push_back(i);         // 현재 꼭짓점
		indices.push_back(i + 1);     // 다음 꼭짓점
	}

	_indexCount = indices.size();

	desc = {};
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = (uint32)(sizeof(uint32) * indices.size());

	data = {};
	data.pSysMem = indices.data();

	CHECK(DEVICE->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf()));
}

void Geo::Circle::Update()
{
	Matrix matScale = Matrix::CreateScale(Vec3(scale.x, scale.y, 1.0F));
	Matrix matRotation = Matrix::CreateRotationZ(rotation * (XM_PI / 180.0F));
	Matrix matTranslation = Matrix::CreateTranslation(Vec3(position.x, position.y, 1.0F));

	_settings.World = matScale * matRotation * matTranslation;

	SHADER->PushConstantBuffer();
	CAMERA->PushConstantBuffer(_settings);
}

void Geo::Circle::Render()
{
	uint32 stride = sizeof(CPUVertexData);
	uint32 offset = 0;

	SHADER->SetTexture(_texId);

	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DC->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	DC->DrawIndexedInstanced(_indexCount, 1, 0, 0, 0);
}
