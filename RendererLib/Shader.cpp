#include "pch.h"
#include "Shader.h"
#include "GameLoader.h"
#include "RenderEngine.h"

void Shader::Start()
{
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

    // CB
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = sizeof(CPUConstantBuffer);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    CHECK(DEVICE->CreateBuffer(&desc, nullptr, _transformBuffer.GetAddressOf()));
}

void Shader::Render()
{
    DC->IASetInputLayout(_inputLayout.Get());

    DC->VSSetShader(_vertexShader.Get(), nullptr, 0);
    DC->VSSetConstantBuffers(0, 1, _transformBuffer.GetAddressOf());

    DC->PSSetShader(_pixelShader.Get(), nullptr, 0);
}

void Shader::PushConstantBuffer(ConstantSetting setting)
{
    Vec3 camPosition = { 0.0F, 0.0F, -10.0F };
    const Vec3 focusPosition = camPosition + Vec3(0.0F, 0.0F, 1.0F);

    _view = XMMatrixLookAtLH(camPosition, focusPosition, Vec3::Up);
    _projection = XMMatrixOrthographicLH(WIDTH, HEIGHT, 0.1F, 100.0F);

    _cpuTransformData.World = setting.World;
    _cpuTransformData.VP = _view * _projection;

    D3D11_MAPPED_SUBRESOURCE subResource = {};
    DC->Map(_transformBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, &_cpuTransformData, sizeof(CPUConstantBuffer));
    DC->Unmap(_transformBuffer.Get(), 0);
}
