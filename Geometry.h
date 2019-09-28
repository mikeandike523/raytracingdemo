#pragma once
#include <Dense>
using namespace Eigen;

namespace Engine {
	namespace Geometry{

		class Triangle {
			Triangle(Vector3f A, Vector3f B, Vector3f C);
			//Vector3f normal();
			Vector3f getA();
			Vector3f getB();
			Vector3f getC();
			Vector3f setA(Vector3f P1);
			Vector3f setB(Vector3f P2);
			Vector3f setC(Vector3f P3);
		};
		Matrix4f createRotationMatrix(Vector3f axis, float angle) {
			Matrix4f m;
			axis = axis.normalized();
			float s = sin(angle);
			float c = cos(angle);
			float oc = 1.0 - c;

			m << oc * axis(0) * axis(0) + c, oc * axis(0) * axis(1) - axis(2) * s, oc * axis(2) * axis(0) + axis(1) * s, 0.0,
				oc * axis(0) * axis(1) + axis(2) * s, oc * axis(1) * axis(1) + c, oc * axis(1) * axis(2) - axis(0) * s, 0.0,
				oc * axis(2) * axis(0) - axis(1) * s, oc * axis(1) * axis(2) + axis(0) * s, oc * axis(2) * axis(2) + c, 0.0,
				0.0, 0.0, 0.0, 1.0;

			return m;
		}
	}
}