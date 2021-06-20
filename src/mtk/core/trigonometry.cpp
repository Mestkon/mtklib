#include <mtk/core/trigonometry.hpp>

#include <mtk/core/types.hpp>

#include <cmath>

namespace mtk {
namespace impl_core {
namespace trigonometry {

template<class T>
T _sin(T val)
{
	return std::sin(val);
}

template<class T>
T _cos(T val)
{
	return std::cos(val);
}

template<class T>
T _tan(T val)
{
	return std::tan(val);
}

template<class T>
T _asin(T val)
{
	return std::asin(val);
}

template<class T>
T _acos(T val)
{
	return std::acos(val);
}

template<class T>
T _atan(T val)
{
	return std::atan(val);
}

template<class T>
T _atan2(T y, T x)
{
	return std::atan2(y, x);
}

template float _sin<float>(float);
template double _sin<double>(double);
template ldouble _sin<ldouble>(ldouble);
template float _cos<float>(float);
template double _cos<double>(double);
template ldouble _cos<ldouble>(ldouble);
template float _tan<float>(float);
template double _tan<double>(double);
template ldouble _tan<ldouble>(ldouble);

template float _asin<float>(float);
template double _asin<double>(double);
template ldouble _asin<ldouble>(ldouble);
template float _acos<float>(float);
template double _acos<double>(double);
template ldouble _acos<ldouble>(ldouble);
template float _atan<float>(float);
template double _atan<double>(double);
template ldouble _atan<ldouble>(ldouble);
template float _atan2<float>(float, float);
template double _atan2<double>(double, double);
template ldouble _atan2<ldouble>(ldouble, ldouble);

} // namespace trigonometry
} // namespace impl_core
} // namespace mtk
