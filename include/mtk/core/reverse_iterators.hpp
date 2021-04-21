#ifndef MTK_CORE_REVERSE_ITERATORS_HPP
#define MTK_CORE_REVERSE_ITERATORS_HPP

#include <mtk/core/types.hpp>
#include <mtk/core/impl/require.hpp>

#include <iterator>
#include <type_traits>

namespace mtk {

template<class T
	,size_t N>
class array;

template<class T
	,size_t N>
constexpr
auto
rbegin(array<T, N>& a)
{
	return std::reverse_iterator(a.end());
}

template<class T
	,size_t N>
constexpr
auto
rbegin(const array<T, N>& a)
{
	return std::reverse_iterator(a.end());
}

template<class T
	,size_t N>
constexpr
auto
rend(array<T, N>& a)
{
	return std::reverse_iterator(a.begin());
}

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

template<class T>
constexpr
auto
rbegin(span<T>& s)
{
	return std::reverse_iterator(s.end());
}

template<class T>
constexpr
auto
rbegin(const span<T>& s)
{
	return std::reverse_iterator(s.end());
}

template<class T>
constexpr
auto
rend(span<T>& s)
{
	return std::reverse_iterator(s.begin());
}

template<class T>
constexpr
auto
rend(const span<T>& s)
{
	return std::reverse_iterator(s.begin());
}



class zstring_view;

template<class T = zstring_view
	,_require<std::is_same_v<T, zstring_view>> = 0>
constexpr
auto
rbegin(T& zv)
{
	return std::reverse_iterator(zv.end());
}

template<class T = zstring_view
	,_require<std::is_same_v<T, zstring_view>> = 0>
constexpr
auto
rbegin(const T& zv)
{
	return std::reverse_iterator(zv.end());
}

template<class T = zstring_view
	,_require<std::is_same_v<T, zstring_view>> = 0>
constexpr
auto
rend(T& zv)
{
	return std::reverse_iterator(zv.begin());
}

template<class T = zstring_view
	,_require<std::is_same_v<T, zstring_view>> = 0>
constexpr
auto
rend(const T& zv)
{
	return std::reverse_iterator(zv.rbegin());
}



} // namespace mtk

#endif
