#pragma once

#include "HelperInclude.h"

class HelloTriangle : public IGameInterface
{
public:
	virtual void Start() override;
	virtual void Update(float dt) override;
	virtual void Render() override;

	shared_ptr<Geo::IGeometry> _obj = {};

};

