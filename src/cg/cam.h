#ifndef CAM_H
#define CAM_H
#include "cg.h"
#include "Ray.h"
#include "xyz.h"
#include "curve.h"
class Cam {
public:
	Cam(
		XYZ _eye,			//���߻�۵�
		XYZ _scene,			//�۽���
		XYZ _hair,			//ͷ����λ
		real _viewInDegree, //�ӽǴ�С�����Ƴ���ƽ��͹��߻�۵�ľ���
		real _w,			//������
		real _h				//������
	);
	Ray seeAt(real i, real j);//�����۲���
	XYZ collect(const Ray& in, CurveGroup& world, int depth);//�ռ����߻���
private:
	XYZ eye;
	XYZ leftButtom;
	XYZ right, up, in_n;
};



#endif // !CAM_H
