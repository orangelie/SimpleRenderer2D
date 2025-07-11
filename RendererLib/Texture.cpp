#include "pch.h"
#include "Texture.h"

void Texture::LoadTextureFromFile(wstring_view filename)
{
	DirectX::TexMetadata md;
	CHECK(::LoadFromWICFile(filename.data(), WIC_FLAGS_NONE, &md, _img));

	CHECK(::CreateShaderResourceView(DEVICE.Get(), _img.GetImages(), _img.GetImageCount(), md, _shaderResourveView.GetAddressOf()));

	_size.x = md.width;
	_size.y = md.height;
}
