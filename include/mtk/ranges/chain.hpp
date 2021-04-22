#ifndef MTK_RANGES_CHAIN_HPP
#define MTK_RANGES_CHAIN_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/iterator_traits.hpp>
#include <mtk/core/types.hpp>
#include <mtk/ranges/range.hpp>
#include <mtk/core/impl/addressof.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/require.hpp>

#include <iterator>
#include <tuple>
#include <type_traits>

namespace mtk {
namespace impl_chain {

template<size_t Idx
	,class... Iters>
constexpr
auto
_deref(const std::tuple<Iters...>& iters, const std::tuple<Iters...>& ends)
{
	if (std::get<Idx>(iters) != std::get<Idx>(ends))
		return *std::get<Idx>(iters);

	if constexpr (Idx + 1 < sizeof...(Iters))
		return _deref<Idx + 1>(iters, ends);

	MTK_ASSERT(false);
}

template<size_t Idx
	,class... Iters>
constexpr
void
_increment(std::tuple<Iters...>& iters, std::tuple<Iters...>& ends)
{
	if (std::get<Idx>(iters) != std::get<Idx>(ends))
		++std::get<Idx>(iters);
	else if constexpr (Idx + 1 < sizeof...(Iters))
		_increment<Idx + 1>(iters, ends);
}

template<class... Iters>
class _chain_iterator
{
public:
	using value_type = std::decay_t<decltype(*std::get<0>(mtk::_declval<std::tuple<Iters...>&>()))>;
	using reference = value_type;
	using pointer = struct {
		value_type value;
		constexpr
		const value_type*
		operator->() const { return mtk::_addressof(this->value); }
	};
	using difference_type = ptrdiff_t;
	using iterator_category = std::input_iterator_tag;

	constexpr
	_chain_iterator(std::tuple<Iters...> firsts, std::tuple<Iters...> lasts) :
		m_iters(mtk::_move(firsts)),
		m_ends(mtk::_move(lasts))
	{ }

	constexpr
	reference
	operator*() const
	{
		return impl_chain::_deref<0>(m_iters, m_ends);
	}

	constexpr
	pointer
	operator->() const
	{
		return pointer{**this};
	}

	friend constexpr
	_chain_iterator&
	operator++(_chain_iterator& rhs)
	{
		impl_chain::_increment<0>(rhs.m_iters, rhs.m_ends);
		return rhs;
	}

	friend constexpr
	_chain_iterator
	operator++(_chain_iterator& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend constexpr
	bool
	operator==(const _chain_iterator& lhs, const _chain_iterator& rhs)
	{
		return (lhs.m_iters == rhs.m_iters);
	}

	friend constexpr
	bool
	operator!=(const _chain_iterator& lhs, const _chain_iterator& rhs)
	{
		return (lhs.m_iters != rhs.m_iters);
	}

private:
	std::tuple<Iters...> m_iters;
	std::tuple<Iters...> m_ends;
};



template<class T
	,class... Ts>
struct _is_all_same
{
	static constexpr bool value = (std::is_same_v<T, Ts> && ...);
};

} // namespace impl_chain



template<class... Conts
#ifndef MTK_DOXYGEN
	,_require<(sizeof...(Conts) > 0)> = 0
	,_require<(is_input_iterator_v<typename std::decay_t<Conts>::iterator> && ...)> = 0
	,_require<impl_chain::_is_all_same<typename std::decay_t<Conts>::value_type...>::value> = 0
#endif
>
constexpr
auto
chain(Conts&& ...conts)
{
	using std::begin;
	using std::end;
	std::tuple begins(begin(conts)...);
	std::tuple ends(end(conts)...);
	impl_chain::_chain_iterator first(mtk::_move(begins), ends);
	impl_chain::_chain_iterator last(ends, mtk::_move(ends));
	return mtk::range(first, last);
}

} // namespace mtk

#endif
