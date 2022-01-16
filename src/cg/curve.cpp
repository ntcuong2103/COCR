#include "curve.h"
using namespace std;

CurveGroup::CurveGroup(const std::vector<const Curve*>& l) :objList(l) {}

bool CurveGroup::hitBy(
	const Ray& ray, Intersection& hit,
	real t_max, real t_min
) const {
	Intersection tmp;
	bool isHitted = false;
	for (auto& i : objList) {
		//�õ������hit�㣬��ϴ���
		if (i->hitBy(ray, tmp, t_max, t_min)) {
			isHitted = true;
			t_max = tmp.t;//���Ʋ����Ͻ�
			hit = tmp;
		}
	}
	return isHitted;
}

CurveGroup::CurveGroup() {}

CurveGroup::~CurveGroup() {

}
