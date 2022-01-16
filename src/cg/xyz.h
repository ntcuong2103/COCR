#ifndef XYZ_H
#define XYZ_H
#include "cg.h"
#include <vector>
#include <iostream>

class XYZ;
namespace xyz {
	std::vector<XYZ> mul3x3(const std::vector<XYZ> a, const std::vector<XYZ>b);
	XYZ mul3x1(const std::vector<XYZ> a, const XYZ b);
	std::vector<XYZ> inverse3x3(const std::vector<XYZ> a);
	//�����Ե�Ϊ���İ�����k��ӳ
	XYZ cast(const XYZ& _from, real _k, const XYZ& _center);
	//���㴹ֱ���߰�����k��ӳ
	XYZ cast(const XYZ& _from, real _k, const XYZ& _p1, const XYZ& _p2);
	//���㴹ֱ���水����k��ӳ
	XYZ cast(const XYZ& _from, real _k, const XYZ& _p1, const XYZ& _p2, const XYZ& _p3);
	//�������ϵĴ���
	XYZ foot(const XYZ& _from, const XYZ& _p1, const XYZ& _p2);
	//���䣬���ط���ⷽ��
	XYZ reflect(const XYZ& _in, const XYZ& _n);
	//���䣬ȫ���䷵��false��n1��n2��ʾ���ֽ��ʵ������ʣ�Ĭ��n1=1�����䷽�����out
	bool refract(const XYZ& _in, const XYZ& _n, XYZ& _out, const real _n2, const real _n1 = 1.0);
	//���ص�λ������ָ���ڲ�����������
	XYZ randAtNormSphere();
	//����XYƽ��ĵ�λ����
	XYZ randAtNormXY();
};
class XYZ {
public:
	XYZ(real _x = 0, real _y = 0, real _z = 0);
	XYZ(const XYZ& xyz);
	//Լ��XYZ�޸�����
	real x()const;
	real y()const;
	real z()const;
	//Լ��ͨ��RGB�޸�ֵ
	real& r();
	real& g();
	real& b();

	real operator[](int i) const;
	real& operator[](int i);
	const XYZ& operator+() const;
	XYZ operator-() const;
	XYZ& operator+=(const XYZ& _xyz);
	XYZ& operator-=(const XYZ& _xyz);
	XYZ& operator*=(const XYZ& _xyz);
	XYZ& operator/=(const XYZ& _xyz);
	XYZ& operator*=(const real _t);
	XYZ& operator/=(const real _t);

	const real* data()const;
	real length()const;
	XYZ& abs();
	XYZ& norm();
	real dot(const XYZ& _xyz)const;
	XYZ cross(const XYZ& _xyz)const;

	friend bool operator==(const XYZ& _xyz0, const XYZ& _xyz);
	friend bool operator!=(const XYZ& _xyz0, const XYZ& _xyz);
	friend std::istream& operator>>(std::istream& _in, XYZ& _t);
	friend std::ostream& operator<<(std::ostream& _out, const XYZ& _t);
	friend XYZ operator+(const XYZ& _xyz0, const XYZ& _xyz);
	friend XYZ operator-(const XYZ& _xyz0, const XYZ& _xyz);
	friend XYZ operator*(const XYZ& _xyz0, const XYZ& _xyz);
	friend XYZ operator/(const XYZ& _xyz0, const XYZ& _xyz);
	friend XYZ operator*(real _t, const XYZ& _xyz);
	friend XYZ operator/(XYZ _xyz, real _t);
	friend XYZ operator*(const XYZ& _xyz, real _t);
private:
	real ptr[3];
};

real len(const XYZ& _xyz);
XYZ abs(const XYZ& _xyz);
XYZ norm(const XYZ& _xyz);
real dot(const XYZ& _xyz0, const XYZ& _xyz);
XYZ cross(const XYZ& _xyz0, const XYZ& _xyz);

#endif // !XYZ_H
