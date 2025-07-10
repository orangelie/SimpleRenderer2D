#pragma once

struct ConstantSetting
{
	Matrix World;
};

class Shader
{
private:
	struct CPUConstantBuffer
	{
		Matrix World;
		Matrix VP;
	};

public:
	void Start();
	void Render();

	void PushConstantBuffer(ConstantSetting setting);

private:
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;

	ComPtr<ID3D11Buffer> _transformBuffer = nullptr;
	CPUConstantBuffer _cpuTransformData = {};
	Matrix _view, _projection;

};
