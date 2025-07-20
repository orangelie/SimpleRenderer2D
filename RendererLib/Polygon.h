#pragma once

#include "IGeometry.h"

namespace Geo
{
	class Polygon : public IGeometry
	{
	public:
		void SetPoints(vector<Vec2> pts) { _pts = pts; }

		virtual void Start(GeometryOption option) override;
		virtual void Update() override;
		virtual void Render() override;

	private:
		vector<Vec2> _pts = {};
		int32 _vertexCount = 0;

	};
}