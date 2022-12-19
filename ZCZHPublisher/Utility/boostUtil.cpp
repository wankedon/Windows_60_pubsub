#include "boostUtil.h"
#include <boost/histogram.hpp>

float calcBaselineLevel(int binCount, std::vector<float>::iterator start, std::vector<float>::iterator stop)
{
	using namespace boost::histogram;
	//����ֱ��ͼ����
	auto pr = std::minmax_element(start, stop);
	auto hist = make_histogram(axis::regular<float, use_default, use_default, axis::option::none_t>(binCount, *pr.first - 1e-6, *pr.second + 1e-6));

	//�������
	std::for_each(start, stop, std::ref(hist));
	//�����°��Ჿ�ֵ����������cell
	auto maxHitLowerPart = std::max_element(hist.begin(), hist.begin() + hist.size() / 2);
	assert(*maxHitLowerPart > 0);	//������Ч
	auto lbin = hist.axis().bin(maxHitLowerPart - hist.begin());
	float acc = 0;
	for (auto y = start; y < stop; y++)
	{
		if (*y >= lbin.lower() && *y < lbin.upper())
		{
			acc += *y;
		}
	}
	return acc / *maxHitLowerPart;
}