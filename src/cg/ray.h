#ifndef RAY_H
#define RAY_H
#include "cg.h"
#include "xyz.h"
class Ray {
public:
	Ray();
	Ray(const XYZ& _a, const XYZ& _b);
	XYZ from() const;//���
	XYZ direction() const;//����
	XYZ to(const real t) const;//�յ�
private:
	//r=a+tb
	XYZ a;
	XYZ b;
};

#endif // !RAY_H