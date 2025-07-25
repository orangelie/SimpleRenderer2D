#pragma once

#include "Shader.h"
#include "Camera.h"

struct GeometryOption
{
	union
	{
		Vec2 size;
		float radius;
	};

	Color color;
	Vec2 position;

	GeometryOption() {}
};

namespace Geo
{
	struct CPUVertexData
	{
		Vec2 position = {};
		Vec2 uv = {};
	};

	class IGeometry
	{
	public:
		virtual void Start(GeometryOption option) = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;

		void SetTexture(TEXTURES texture) { _texId = texture; }

		Vec2 position = {};
		Vec2 scale = { 1.0F, 1.0F };
		float rotation = 0.0F; // degrees

	protected:
		TEXTURES _texId = TEXTURES::RECT1;

		GeometryOption _option;

		ConstantSetting _settings = {};
		ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
		ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

		int _indexCount;

	};
}
