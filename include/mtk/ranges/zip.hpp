#ifndef MTK_RANGES_ZIP_HPP
#define MTK_RANGES_ZIP_HPP

#include <mtk/core/iterator_traits.hpp>
#include <mtk/core/types.hpp>
#include <mtk/ranges/range.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/require.hpp>

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mtk {
namespace impl_zip {

template<class Tuple,
	size_t... Ids>
constexpr
auto
_deref(Tuple&& t, std::index_sequence<Ids...>)
{
	return std::tuple<decltype(*std::get<Ids>(t))...>(*std::get<Ids>(t)...);
}

template<size_t Idx = 0
	,class... Iters>
constexpr
void
_increment(std::tuple<Iters...>& t)
{
	++std::get<Idx>(t);
	if constexpr (Idx + 1 < sizeof...(Iters))
		_increment<Idx + 1>(t);
}

template<class... Iters>
class _zip_iterator
{
public:
	using value_type = std::tuple<std::remove_reference_t<decltype(*mtk::_declval<Iters>())>...>;
	using reference = std::tuple<decltype(*mtk::_declval<Iters>())...>;
	using pointer = struct {
		value_type value;
		constexpr
		const value_type*
		operator->() const { return &this->value; }
	};
	using difference_type = ptrdiff_t;
	using iterator_category = std::input_iterator_tag;

	constexpr
	_zip_iterator(Iters... iters) :
		m_iters(iters...)
	{ }

	constexpr
	reference
	operator*() const
	{
		return impl_zip::_deref(m_iters, std::make_index_sequence<sizeof...(Iters)>());
	}

	constexpr
	pointer
	operator->() const
	{
		return pointer{**this};
	}

	friend constexpr
	_zip_iterator&
	operator++(_zip_iterator& rhs)
	{
		impl_zip::_increment(rhs.m_iters);
		return rhs;
	}

	friend constexpr
	_zip_iterator
	operator++(_zip_iterator& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend constexpr
	bool
	operator==(const _zip_iterator& lhs, const _zip_iterator& rhs)
	{
		return (std::get<0>(lhs.m_iters) == std::get<0>(rhs.m_iters));
	}

	friend constexpr
	bool
	operator!=(const _zip_iterator& lhs, const _zip_iterator& rhs)
	{
		return (std::get<0>(lhs.m_iters) != std::get<0>(rhs.m_iters));
	}

private:
	std::tuple<Iters...> m_iters;
};

} // namespace impl_zip



template<class... Conts
#ifndef MTK_DOXYGEN
	,_require<(is_input_iterator_v<typename std::decay_t<Conts>::iterator> && ...)> = 0
	,_require<(sizeof...(Conts) > 0)> = 0
#endif
>
constexpr
auto
zip(Conts&& ...conts)
{
	using std::begin;
	using std::end;
	impl_zip::_zip_iterator first(begin(conts)...);
	impl_zip::_zip_iterator last(end(conts)...);
	return mtk::range(first, last);
}

template<class Cont
#ifndef MTK_DOXYGEN
	,_require<is_input_iterator_v<typename std::decay_t<Cont>::iterator>> = 0
#endif
>
constexpr
auto
enumerate(Cont&& c)
{
	using std::size;
	return mtk::zip(mtk::range(size(c)), c);
}


} // namespace mtk

#endif
