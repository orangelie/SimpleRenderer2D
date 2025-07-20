#include "pch.h"
#include "01. HelloTriangle.h"
#include "Camera.h"
#include "GameLoader.h"
#include "RenderEngine.h"

#pragma warning(disable:4996)

// _obj_Update, Render이런걸 렌더러 내부에서 해주도록 바꿔주고, 오브젝트
// 삭제나 이런건 Destory로 ㄱㄱ

void HelloTriangle::Start()
{
	GeometryOption option = {};
	option.color = Color(1.0F, 0.0F, 1.0F, 1.0F);
	option.position = { 0.0F, 0.0F };
	option.size = { 1.0F, 1.0F };

	vector<Vec2> pts;
	{
		pts.push_back({});
	}

	_obj = make_shared<Geo::Polygon>();
	dynamic_cast<Geo::Polygon*>(_obj.get())->SetPoints(pts);
	_obj->Start(option);

	_obj->scale = { 32.0F, 32.0F };
	_obj->SetTexture(TEXTURES::RECT1);
}

void HelloTriangle::Update(float dt)
{
	_obj->Update();

	const float speed = 150.0F;
	if (INPUT->GetButton(KEY_TYPE::D))
		_obj->position += speed * dt * Vec2(1.0F, 0.0F);
	if (INPUT->GetButton(KEY_TYPE::A))
		_obj->position -= speed * dt * Vec2(1.0F, 0.0F);
	if (INPUT->GetButton(KEY_TYPE::W))
		_obj->position += speed * dt * Vec2(0.0F, 1.0F);
	if (INPUT->GetButton(KEY_TYPE::S))
		_obj->position -= speed * dt * Vec2(0.0F, 1.0F);

	if (INPUT->GetButton(KEY_TYPE::RIGHT))
		CAMERA->position += speed * dt * Vec2(1.0F, 0.0F);
	if (INPUT->GetButton(KEY_TYPE::LEFT))
		CAMERA->position -= speed * dt * Vec2(1.0F, 0.0F);
	if (INPUT->GetButton(KEY_TYPE::UP))
		CAMERA->position += speed * dt * Vec2(0.0F, 1.0F);
	if (INPUT->GetButton(KEY_TYPE::DOWN))
		CAMERA->position -= speed * dt * Vec2(0.0F, 1.0F);

	ImGui::Begin("UI");
	char buf[0x50] = {};
	sprintf(buf, "Object Position: (%f, %f)", _obj->position.x, _obj->position.y);
	ImGui::Text(buf);
	sprintf(buf, "Camera Position: (%f, %f)", CAMERA->position.x, CAMERA->position.y);
	ImGui::Text(buf);
	ImGui::End();
}

void HelloTriangle::Render()
{
	_obj->Render();
}
