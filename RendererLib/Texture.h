#pragma once

class Texture
{
public:
	void LoadTextureFromFile(wstring_view filename);
	ComPtr<ID3D11ShaderResourceView> GeSRV() { return _shaderResourveView; }

private:
	ComPtr<ID3D11ShaderResourceView> _shaderResourveView = nullptr;
	Vec2 _size = { 0.0F, 0.0F };
	DirectX::ScratchImage _img = {};

};
