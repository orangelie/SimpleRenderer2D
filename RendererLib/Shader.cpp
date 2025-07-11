#include "pch.h"
#include "Shader.h"
#include "GameLoader.h"
#include "RenderEngine.h"
#include "Texture.h"

void Shader::Start()
{
    // 몇가지 테스트 텍스쳐 준비
    {
        wstring path = L"../Shaders/";
        wstring filenames[MAX_TEXTURE_COUNT] =
        { L"rectangle.png", L"rectangle_wood.png", L"sphere.png" };
        _realTextureCount = _countof(filenames);

        assert(_realTextureCount <= MAX_TEXTURE_COUNT);

        for (int i = 0; i < _realTextureCount; ++i)
        {
            _textures[i] = make_shared<Texture>();
            _textures[i]->LoadTextureFromFile(path + filenames[i]);
        }


        D3D11_SAMPLER_DESC desc = {};
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MipLODBias = 0.0F;
        desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        desc.MinLOD = 0.0F;
        desc.MaxLOD = 0.0F;

        DEVICE->CreateSamplerState(&desc, _samplerState.GetAddressOf());
    }

    const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    ComPtr<ID3DBlob> vsBlob, psBlob;
    CHECK(D3DCompileFromFile(L"../Shaders/basic.fx", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", compileFlag, 0, &vsBlob, nullptr));
    CHECK(D3DCompileFromFile(L"../Shaders/basic.fx", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", compileFlag, 0, &psBlob, nullptr));

    CHECK(DEVICE->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf()));
    CHECK(DEVICE->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf()));

    D3D11_INPUT_ELEMENT_DESC inputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    CHECK(DEVICE->CreateInputLayout(inputLayout, _countof(inputLayout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), _inputLayout.GetAddressOf()));
}

void Shader::Render()
{
    DC->IASetInputLayout(_inputLayout.Get());

    DC->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

    switch (_textureId)
    {
    case TEXTURES::RECT1:
        DC->PSSetShaderResources(0, 1, _textures[0]->GeSRV().GetAddressOf());
        break;
    case TEXTURES::RECT2:
        DC->PSSetShaderResources(0, 1, _textures[1]->GeSRV().GetAddressOf());
        break;
    case TEXTURES::CIRCLE:
        DC->PSSetShaderResources(0, 1, _textures[2]->GeSRV().GetAddressOf());
        break;
    }

    DC->VSSetShader(_vertexShader.Get(), nullptr, 0);
    DC->PSSetShader(_pixelShader.Get(), nullptr, 0);
}

void Shader::PushConstantBuffer()
{
    
}
