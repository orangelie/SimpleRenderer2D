#include "pch.h"
#include "Camera.h"

void Camera::Start()
{
    // CB
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = sizeof(CPUConstantBuffer);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    CHECK(DEVICE->CreateBuffer(&desc, nullptr, _transformBuffer.GetAddressOf()));
}

void Camera::PushConstantBuffer(ConstantSetting setting)
{
    Vec3 camPosition = Vec3(position.x, position.y, 0.0F);
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

void Camera::Render()
{
    DC->VSSetConstantBuffers(0, 1, _transformBuffer.GetAddressOf());
}
