#ifndef CYLINDER_H
#define CYLINDER_H
#include "cg.h"
#include "xyz.h"
#include "curve.h"
#include "mat.h"
#include "sphere.h"
#include <vector>
class Tube : public Curve {
public:
	Tube(XYZ _o1, XYZ _o2, real _r, Mat* _m);
	//ԭ���ⷨ��
	//��Բ������Ϊԭ�㡢��ΪZ�ὨϵXYZ'
	//��XYZ��XYZ'��ȡ3���㣬��任����/��Ԫ����������ӳ�䵽�µ�����ϵ
	//��ֱ��ͶӰ��ƽ��Z=0��Z=h������ֱ�ߺ�Բ����Ľ��㣬�������ڲ��ڶ�������

	//ʵ�ʽⷨ��
	//��һ��С��ģ��Բ��,���߶�o1o2�ϼ��d����һ��С����ֱ�ߺ�С�򴮵Ľ���
	bool hitBy(const Ray& ray, Intersection& hit, real t_max = cg::_MAX_REAL, real t_min = cg::_MIN_REAL) const override;
private:
	Tube();
	XYZ o1, o2;		//������������
	real r;			//�뾶
	Mat* m;			//����
	std::vector<Sphere> ss;
};

#endif // !CYLINDER_H
