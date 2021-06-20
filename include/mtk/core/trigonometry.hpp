#ifndef MTK_CORE_TRIGONOMETRY_HPP
#define MTK_CORE_TRIGONOMETRY_HPP

//! @file
//! Contains type safe trigonometric functions.

#include <mtk/core/math.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {
namespace impl_core {
namespace trigonometry {

template<class T>
T _sin(T val);
template<class T>
T _cos(T val);
template<class T>
T _tan(T val);

template<class T>
T _asin(T val);
template<class T>
T _acos(T val);
template<class T>
T _atan(T val);
template<class T>
T _atan2(T y, T x);

} // namespace trigonometry
} // namespace impl_core

//! @addtogroup core
//! @{

//! @brief Base class for trigonometric angles.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
//!
//! - @ref radians_t
//! - @ref degrees_t
//!
//! @pre T must be a floating point type.
template<class T
	,class AngleTraits>
class basic_angle
{
public:
	static_assert(std::is_floating_point_v<T>, "T must be a floating point type");

	//! typedef.
	using value_type = T;
	//! typedef.
	using angle_traits = AngleTraits;

	//! Amount of angle in a whole cycle.
	static constexpr
	T
	period = AngleTraits::period;

	//! Default constructor, initializes to zero.
	constexpr
	basic_angle() noexcept :
		m_val()
	{ }

	//! Constructs a basic_angle initialized to value.
	explicit constexpr
	basic_angle(T value) noexcept :
		m_val(value)
	{ }

	//! Returns the current angle.
	explicit constexpr
	operator T() const noexcept
	{
		return m_val;
	}

	//! Returns the current angle.
	constexpr
	const T&
	value() const noexcept
	{
		return m_val;
	}

	//! Returns the current angle.
	constexpr
	T&
	value() noexcept
	{
		return m_val;
	}

private:
	T m_val;
};

//! @}

//! @brief Compares 2 basic_angle types.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
bool
operator==(basic_angle<T, Traits> lhs, basic_angle<T, Traits> rhs) noexcept
{
	return (lhs.value() == rhs.value());
}

//! @brief Compares 2 basic_angle types.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
bool
operator!=(basic_angle<T, Traits> lhs, basic_angle<T, Traits> rhs) noexcept
{
	return (lhs.value() != rhs.value());
}

//! @brief Compares 2 basic_angle types.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
bool
operator<(basic_angle<T, Traits> lhs, basic_angle<T, Traits> rhs) noexcept
{
	return (lhs.value() < rhs.value());
}

//! @brief Compares 2 basic_angle types.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
bool
operator>(basic_angle<T, Traits> lhs, basic_angle<T, Traits> rhs) noexcept
{
	return (lhs.value() > rhs.value());
}

//! @brief Compares 2 basic_angle types.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
bool
operator<=(basic_angle<T, Traits> lhs, basic_angle<T, Traits> rhs) noexcept
{
	return (lhs.value() <= rhs.value());
}

//! @brief Compares 2 basic_angle types.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
bool
operator>=(basic_angle<T, Traits> lhs, basic_angle<T, Traits> rhs) noexcept
{
	return (lhs.value() >= rhs.value());
}

//! @brief Returns a copy of rhs.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>
operator+(basic_angle<T, Traits> rhs) noexcept
{
	return rhs;
}

//! @brief Returns -angle.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>
operator-(basic_angle<T, Traits> rhs) noexcept
{
	return basic_angle<T, Traits>(-rhs.value());
}

//! @brief Adds rhs to lhs and returns lhs.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>&
operator+=(basic_angle<T, Traits>& lhs, basic_angle<T, Traits> rhs) noexcept
{
	lhs.value() += rhs.value();
	return lhs;
}

//! @brief Subtracts rhs from lhs and returns lhs.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>&
operator-=(basic_angle<T, Traits>& lhs, basic_angle<T, Traits> rhs) noexcept
{
	lhs.value() -= rhs.value();
	return lhs;
}

//! @brief Returns the sum of lhs and rhs.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>
operator+(basic_angle<T, Traits> lhs, basic_angle<T, Traits> rhs) noexcept
{
	return (lhs += rhs);
}

//! @brief Returns the difference between lhs and rhs.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>
operator-(basic_angle<T, Traits> lhs, basic_angle<T, Traits> rhs) noexcept
{
	return (lhs -= rhs);
}

//! @brief Multiplies lhs by rhs and returns lhs.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>&
operator*=(basic_angle<T, Traits>& lhs, typename basic_angle<T, Traits>::value_type rhs) noexcept
{
	lhs.value() *= rhs;
	return lhs;
}

//! @brief Divieds lhs by rhs and returns lhs.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>&
operator/=(basic_angle<T, Traits>& lhs, typename basic_angle<T, Traits>::value_type rhs) noexcept
{
	lhs.value() /= rhs;
	return lhs;
}

//! @brief Returns the product of lhs and rhs.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>
operator*(basic_angle<T, Traits> lhs, typename basic_angle<T, Traits>::value_type rhs) noexcept
{
	return (lhs *= rhs);
}

//! @brief Returns the product of lhs and rhs.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>
operator*(typename basic_angle<T, Traits>::value_type lhs, basic_angle<T, Traits> rhs) noexcept
{
	return (rhs *= lhs);
}

//! @brief Returns the quotient of lhs and rhs.
//!
//! @relates basic_angle
template<class T
	,class Traits>
constexpr
basic_angle<T, Traits>
operator/(basic_angle<T, Traits> lhs, typename basic_angle<T, Traits>::value_type rhs) noexcept
{
	return (lhs /= rhs);
}



//! @addtogroup core
//! @{

template<class T>
struct _radians_traits
{
	static constexpr
	T
	period = T(2)*math::pi<T>;
};

template<class T>
struct _degrees_traits
{
	static constexpr
	T
	period = T(360);
};

//! @brief Templated radian typedef.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
template<class T>
using radians_t = basic_angle<T, _radians_traits<T>>;
//! @brief Templated degree typedef.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
template<class T>
using degrees_t = basic_angle<T, _degrees_traits<T>>;

//! @brief typedef for float radians.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
using radiansf = radians_t<float>;
//! @brief typedef for double radians.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
using degreesf = degrees_t<float>;
//! @brief typedef for float degrees.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
using radians = radians_t<double>;
//! @brief typedef for double degrees.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
using degrees = degrees_t<double>;

inline namespace literals {

//! Inline. Contains literals for basic_angle.
inline namespace trigonometry_literals {

//! @addtogroup core
//! @{

//! @brief Returns the value as radians.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
constexpr
radians
operator""_rad(ldouble val) noexcept
{
	return radians(static_cast<double>(val));
}

//! @brief Returns the value as radians.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
constexpr
radians
operator""_rad(ullong val) noexcept
{
	return radians(static_cast<double>(val));
}

//! @brief Returns the value as radians.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
constexpr
degrees
operator""_deg(ldouble val) noexcept
{
	return degrees(static_cast<double>(val));
}

//! @brief Returns the value as radians.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
constexpr
degrees
operator""_deg(ullong val) noexcept
{
	return degrees(static_cast<double>(val));
}

//! @}

} // namespace trigonometry_literals
} // namespace literals



template<class ToAngle
	,class FromAngle>
constexpr
ToAngle
_convert_angle(FromAngle angle) noexcept
{
	if constexpr (std::is_same_v<ToAngle, FromAngle>) {
		return angle;
	} else {
		constexpr auto factor = ToAngle::period / FromAngle::period;;
		return ToAngle(angle.value()*factor);
	}
}

//! @brief Returns angle as radians.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
template<class T,
	class Traits>
constexpr
radians_t<T>
to_radians(basic_angle<T, Traits> angle) noexcept
{
	return mtk::_convert_angle<radians_t<T>>(angle);
}

//! @brief Returns angle as degrees.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
template<class T
	,class Traits>
constexpr
degrees_t<T>
to_degrees(basic_angle<T, Traits> angle) noexcept
{
	return mtk::_convert_angle<degrees_t<T>>(angle);
}


//! @brief Returns the sine of angle.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
template<class T
	,class Traits>
T
sin(basic_angle<T, Traits> angle)
{
	return mtk::impl_core::trigonometry::_sin(to_radians(angle).value());
}

//! @brief Returns the cosine of angle.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
template<class T
	,class Traits>
T
cos(basic_angle<T, Traits> angle)
{
	return mtk::impl_core::trigonometry::_cos(to_radians(angle).value());
}

//! @brief Returns the tangent of angle.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
template<class T
	,class Traits>
T
tan(basic_angle<T, Traits> angle)
{
	return mtk::impl_core::trigonometry::_tan(to_radians(angle).value());
}

//! @brief Returns the cosecant of angle.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
template<class T
	,class Traits>
T
csc(basic_angle<T, Traits> angle)
{
	return T(1) / mtk::sin(angle);
}

//! @brief Returns the secant of angle.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
template<class T
	,class Traits>
T
sec(basic_angle<T, Traits> angle)
{
	return T(1) / mtk::cos(angle);
}

//! @brief Returns the cotanget of angle.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
template<class T
	,class Traits>
T
cot(basic_angle<T, Traits> angle)
{
	return T(1) / mtk::tan(angle);
}



//! @brief Returns the arcsine of value.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
//!
//! @pre Float must be a floating point type.
template<class T
	,class Float = std::conditional_t<std::is_floating_point_v<T>, T, double>>
radians_t<Float>
asin(T value)
{
	return radians_t<Float>(mtk::impl_core::trigonometry::_asin(static_cast<Float>(value)));
}

//! @brief Returns the arccosine of value.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
//!
//! @pre Float must be a floating point type.
template<class T
	,class Float = std::conditional_t<std::is_floating_point_v<T>, T, double>>
radians_t<Float>
acos(T value)
{
	return radians_t<Float>(mtk::impl_core::trigonometry::_acos(static_cast<Float>(value)));
}

//! @brief Returns the arctangent of value.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
//!
//! @pre Float must be a floating point type.
template<class T
	,class Float = std::conditional_t<std::is_floating_point_v<T>, T, double>>
radians_t<Float>
atan(T value)
{
	return radians_t<Float>(mtk::impl_core::trigonometry::_atan(static_cast<Float>(value)));
}

//! @brief Returns the arctangent of y / x corrected for sector.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
//!
//! @pre Float must be a floating point type.
template<class T
	,class Float = std::conditional_t<std::is_floating_point_v<T>, T, double>>
radians_t<Float>
atan2(T y, T x)
{
	return radians_t<Float>(mtk::impl_core::trigonometry::_atan2(static_cast<Float>(y), static_cast<Float>(x)));
}

//! @brief Returns the arccosecant of value.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
//!
//! @pre Float must be a floating point type.
template<class T
	,class Float = std::conditional_t<std::is_floating_point_v<T>, T, double>>
radians_t<Float>
acsc(T value)
{
	return mtk::asin(T(1) / value);
}

//! @brief Returns the arcsecant of value.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
//!
//! @pre Float must be a floating point type.
template<class T
	,class Float = std::conditional_t<std::is_floating_point_v<T>, T, double>>
radians_t<Float>
asec(T value)
{
	return mtk::acos(T(1) / value);
}

//! @brief Returns the arccotangent of value.
//!
//! @code
//! #include <mtk/core/trigonometry.hpp>
//! @endcode
//!
//! @pre Float must be a floating point type.
template<class T
	,class Float = std::conditional_t<std::is_floating_point_v<T>, T, double>>
radians_t<Float>
acot(T value)
{
	return mtk::atan(T(1) / value);
}

//! @}

} // namespace mtk

#endif
