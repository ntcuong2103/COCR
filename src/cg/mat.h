#ifndef MAT_H
#define MAT_H
#include "cg.h"
#include "xyz.h"
#include "ray.h"
#include "curve.h"
class Mat {
public:
	virtual bool reflect(
		const Ray& in, const Intersection& hit, XYZ& _loss, Ray& out
	)const = 0;
	virtual XYZ emit()const;//����Lamp�ϣ����صƹ���ɫ��һ����Ϸ���0
};

//��ɢ�����
class Cloth :public Mat {
public:
	Cloth(const XYZ& _loss);
	bool reflect(
		const Ray& in, const Intersection& hit, XYZ& _loss, Ray& out
	)const override;
private:
	XYZ loss;//��ӳ�Բ�ͬ��ɫ�����ն�
};

//ɢ��+�������
class Mirror : public Mat {
public:
	Mirror(const XYZ& _loss, real _rough = 0.0);
	bool reflect(
		const Ray& in, const Intersection& hit, XYZ& _loss, Ray& out
	)const override;
private:
	XYZ loss;
	real rough;//ɢ��̶�
};

//ɢ��+����+�������
class Glass : public Mat {
public:
	Glass(const XYZ& _loss, real _rough = 0.0, real _n = 1.0);
	bool reflect(
		const Ray& in, const Intersection& hit, XYZ& _loss, Ray& out
	)const override;
private:
	XYZ loss;
	real rough;//ɢ��̶�
	real n;//������
};

//��Դ����
class Lamp :public Mat {
public:
	Lamp(const XYZ& _color);
	bool reflect(
		const Ray& in, const Intersection& hit, XYZ& _loss, Ray& out
	)const override;
	XYZ emit()const override;
private:
	XYZ color;
};

#endif // !MAT_H