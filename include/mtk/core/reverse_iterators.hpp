#ifndef MTK_CORE_REVERSE_ITERATORS_HPP
#define MTK_CORE_REVERSE_ITERATORS_HPP

//! @file
//! Defines free function reverse iterators for
//!  - mtk::array
//!  - mtk::span
//!  - mtk::zstring_view

#include <mtk/core/types.hpp>
#include <mtk/core/impl/require.hpp>

#include <iterator>
#include <type_traits>

namespace mtk {

template<class T
	,size_t N>
class array;

//! @brief Returns reverse iterator pointing to a.end().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates array
template<class T
	,size_t N>
constexpr
auto
rbegin(array<T, N>& a)
{
	return std::reverse_iterator(a.end());
}

//! @brief Returns reverse iterator pointing to a.end().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates array
template<class T
	,size_t N>
constexpr
auto
rbegin(const array<T, N>& a)
{
	return std::reverse_iterator(a.end());
}

//! @brief Returns reverse iterator pointing to a.begin().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates array
template<class T
	,size_t N>
constexpr
auto
rend(array<T, N>& a)
{
	return std::reverse_iterator(a.begin());
}

//! @brief Returns reverse iterator pointing to a.begin().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates array
template<class T
	,size_t N>
constexpr
auto
rend(const array<T, N>& a)
{
	return std::reverse_iterator(a.begin());
}



template<class T>
class span;

//! @brief Returns reverse iterator pointing to s.end().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates span
template<class T>
constexpr
auto
rbegin(span<T>& s)
{
	return std::reverse_iterator(s.end());
}

//! @brief Returns reverse iterator pointing to s.end().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates span
template<class T>
constexpr
auto
rbegin(const span<T>& s)
{
	return std::reverse_iterator(s.end());
}

//! @brief Returns reverse iterator pointing to s.begin().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates span
template<class T>
constexpr
auto
rend(span<T>& s)
{
	return std::reverse_iterator(s.begin());
}

//! @brief Returns reverse iterator pointing to s.begin().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates span
template<class T>
constexpr
auto
rend(const span<T>& s)
{
	return std::reverse_iterator(s.begin());
}



class zstring_view;

//! @brief Returns reverse iterator pointing to zv.end().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates zstring_view
#ifndef MTK_DOXYGEN
template<class T = zstring_view
	,_require<std::is_same_v<T, zstring_view>> = 0>
#endif
constexpr
auto
#ifndef MTK_DOXYGEN
rbegin(T& zv)
#else
rbegin(zstring_view& zv)
#endif
{
	return std::reverse_iterator(zv.end());
}

//! @brief Returns reverse iterator pointing to zv.end().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates zstring_view
#ifndef MTK_DOXYGEN
template<class T = zstring_view
	,_require<std::is_same_v<T, zstring_view>> = 0>
#endif
constexpr
auto
#ifndef MTK_DOXYGEN
rbegin(const T& zv)
#else
rbegin(const zstring_view& zv)
#endif
{
	return std::reverse_iterator(zv.end());
}

//! @brief Returns reverse iterator pointing to zv.begin().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates zstring_view
#ifndef MTK_DOXYGEN
template<class T = zstring_view
	,_require<std::is_same_v<T, zstring_view>> = 0>
#endif
constexpr
auto
#ifndef MTK_DOXYGEN
rend(T& zv)
#else
rend(zstring_view& zv)
#endif
{
	return std::reverse_iterator(zv.begin());
}

//! @brief Returns reverse iterator pointing to zv.begin().
//!
//! @code
//! #include <mtk/core/reverse_iterators.hpp>
//! @endcode
//!
//! @relates zstring_view
#ifndef MTK_DOXYGEN
template<class T = zstring_view
	,_require<std::is_same_v<T, zstring_view>> = 0>
#endif
constexpr
auto
#ifndef MTK_DOXYGEN
rend(const T& zv)
#else
rend(const zstring_view& zv)
#endif
{
	return std::reverse_iterator(zv.rbegin());
}

} // namespace mtk

#endif
