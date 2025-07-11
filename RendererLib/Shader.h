#pragma once

class Texture;

enum class TEXTURES : int32
{
	RECT1 = 0,
	RECT2,
	CIRCLE,
};

class Shader
{
private:
	struct ShaderConstant
	{

	};

public:
	void Start();
	void Render();

	void PushConstantBuffer();
	void SetTexture(TEXTURES texture) { _textureId = texture; }

private:
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;

private:
	// Texture
	static constexpr int MAX_TEXTURE_COUNT = 3;
	shared_ptr<Texture> _textures[MAX_TEXTURE_COUNT] = {};
	int _realTextureCount = 0;
	TEXTURES _textureId = TEXTURES::RECT1;

	ComPtr<ID3D11SamplerState> _samplerState = nullptr;

};
