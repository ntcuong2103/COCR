#include "mat.h"
using namespace std;

Cloth::Cloth(const XYZ& _loss) : loss(_loss) {

}

bool Cloth::reflect(
	const Ray& in, const Intersection& hit, XYZ& _loss, Ray& out
)const {
	//�����ⷴ����ɢ��
	out = Ray(
		hit.p,
		xyz::randAtNormSphere() + hit.n//Լ��������
	);
	_loss = loss;
	return true;
}

Mirror::Mirror(const XYZ& _loss, real _rough) : loss(_loss), rough(_rough) {
}

bool Mirror::reflect(
	const Ray& in, const Intersection& hit, XYZ& _loss, Ray& out
)const {
	XYZ reflected = xyz::reflect(in.direction(), hit.n);
	out = Ray(
		hit.p,
		reflected								//��׼���䣬ģ��1
		+ rough * xyz::randAtNormSphere()		//���ɢ�䣬ģ��rough*[0,1)
	);
	_loss = loss;
	return (dot(out.direction(), hit.n) > 0);
}

Glass::Glass(const XYZ& _loss, real _rough, real _n) : loss(_loss), rough(_rough), n(_n) {

}

bool Glass::reflect(
	const Ray& in, const Intersection& hit, XYZ& _loss, Ray& out
)const {
	XYZ _n;
	real _ni;
	_loss = XYZ(1.0, 1.0, 1.0);
	if (dot(in.direction(), hit.n) > 0) {//����
		_n = -hit.n;
		_ni = 1.0 / n;
	}
	else {//����
		_n = hit.n;
		_ni = n;
	}
	//�����ʱ��û���Ƿ���
	XYZ refracted;
	if (xyz::refract(in.direction(), _n, refracted, _ni)) {
		out = Ray(hit.p, refracted);
		return true;
	}
	//ȫ����
	XYZ reflected = xyz::reflect(in.direction(), hit.n);
	out = Ray(
		hit.p,
		reflected + rough * xyz::randAtNormSphere()
	);
	_loss = loss;
	return (dot(out.direction(), hit.n) > 0);
}

Lamp::Lamp(const XYZ& _color) :color(_color) {
}

bool Lamp::reflect(const Ray& in, const Intersection& hit, XYZ& _loss, Ray& out) const {
	return false;
}

XYZ Lamp::emit() const {
	return color;
}

XYZ Mat::emit() const {
	return XYZ(0, 0, 0);
}
