#pragma once

#include "IGeometry.h"

namespace Geo
{
	class Sphere : public IGeometry
	{
	public:
		virtual void Start(GeometryOption option) override;
		virtual void Update() override;
		virtual void Render() override;

	};
}
