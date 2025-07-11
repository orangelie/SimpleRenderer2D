#pragma once

struct ConstantSetting
{
	Matrix World;
};

class Camera
{
private:
	struct CPUConstantBuffer
	{
		Matrix World;
		Matrix VP;
	};

	friend class RenderEngine;
	void Start();

	friend class RenderEngine;
	void Render();

public:
	void PushConstantBuffer(ConstantSetting setting);

	Vec2 position;

private:
	ComPtr<ID3D11Buffer> _transformBuffer = nullptr;
	CPUConstantBuffer _cpuTransformData = {};
	Matrix _view, _projection;

};
