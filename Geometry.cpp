#include <Engine/Geometry.h>
//Eigen library for dense vectors
#include <Dense>

using namespace Eigen;

namespace Engine::Geometry;

//Triangle Class, Collection of 3 3D points
//Manage points, calcuate normal
class Triangle {
	public:
		Triangle(Vector3f A, Vector3f B, Vector3f C);
		Vector3f normal();
		Vector3f getA() { return A; }
		Vector3f getB() { return B; }
		Vector3f getC() { return C; }
		Vector3f setA(Vector3f P1) { A = P1; }
		Vector3f setB(Vector3f P2) { B = P2; }
		Vector3f setC(Vector3f P3) { C = P3; }
	private:
		Vector3f A, B, C;
};

Triangle::Triangle(Vector3f P1, Vector3f P2, Vector3f P3) {
	A = P1;
	B = P2;
	C = P3;
}

Triangle::normal() {
	return (B - A).cross(C - A).normalized();
}


