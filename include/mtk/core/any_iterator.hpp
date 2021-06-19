#ifndef MTK_CORE_ANY_ITERATOR_HPP
#define MTK_CORE_ANY_ITERATOR_HPP

#include <mtk/core/iterator_traits.hpp>
#include <mtk/core/preprocessor.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/unique_ptr.hpp>
#include <mtk/core/impl/addressof.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/require.hpp>

#include <iterator>
#include <exception>
#include <type_traits>

namespace mtk {

class bad_any_iterator_operation :
	public std::exception
{
	const char*
	what() const noexcept override;
};

[[noreturn]]
void
_throw_bad_any_iterator_operation();



namespace impl_core {
namespace any_iterator {

template<class T>
class abstract_any_iterator
{
public:
	virtual
	~abstract_any_iterator() = default;

	virtual
	unique_ptr<abstract_any_iterator, asserting_pointer_validator>
	clone() const = 0;

	virtual
	T
	dereference() const = 0;

	virtual
	T
	dereference(ptrdiff_t idx) const = 0;

	virtual
	void
	increment() = 0;

	virtual
	void
	decrement() = 0;

	virtual
	void
	increment(ptrdiff_t n) = 0;

	virtual
	void
	decrement(ptrdiff_t n) = 0;

	virtual
	ptrdiff_t
	difference(const abstract_any_iterator<T>& other) const = 0;

	virtual
	bool
	compare_equal(const abstract_any_iterator& other) const = 0;

	virtual
	bool
	compare_less_than(const abstract_any_iterator& other) const = 0;
};

template<class Iter
	,class T>
class any_iterator_storage :
	public abstract_any_iterator<T>
{
public:
	any_iterator_storage(Iter iter) :
		m_iter(iter)
	{ }

	~any_iterator_storage() override = default;

	unique_ptr<abstract_any_iterator<T>, asserting_pointer_validator>
	clone() const override
	{
		return mtk::make_unique<any_iterator_storage>(m_iter);
	}

	T
	dereference() const override
	{
		return *m_iter;
	}

	T
	dereference(ptrdiff_t idx) const override
	{
		if constexpr (is_random_access_iterator_v<Iter>)
			return *(m_iter + static_cast<iter::difference_type<Iter>>(idx));
		else
			mtk::_throw_bad_any_iterator_operation();
	}

	void
	increment() override
	{
		++m_iter;
	}

	void
	decrement() override
	{
		if constexpr (is_bidirectional_iterator_v<Iter>)
			--m_iter;
		else
			mtk::_throw_bad_any_iterator_operation();
	}

	void
	increment(ptrdiff_t n) override
	{
		if constexpr (is_random_access_iterator_v<Iter>) {
			m_iter += static_cast<iter::difference_type<Iter>>(n);
		} else {
			MTK_IGNORE(n);
			mtk::_throw_bad_any_iterator_operation();
		}
	}

	void
	decrement(ptrdiff_t n) override
	{
		if constexpr (is_random_access_iterator_v<Iter>) {
			m_iter -= static_cast<iter::difference_type<Iter>>(n);
		} else {
			MTK_IGNORE(n);
			mtk::_throw_bad_any_iterator_operation();
		}
	}

	ptrdiff_t
	difference(const abstract_any_iterator<T>& other) const override
	{
		if constexpr(is_random_access_iterator_v<Iter>) {
			const auto* p = dynamic_cast<const any_iterator_storage*>(&other);
			if (!p)
				mtk::_throw_bad_any_iterator_operation();

			return static_cast<ptrdiff_t>(m_iter - p->m_iter);
		} else {
			MTK_IGNORE(other);
			mtk::_throw_bad_any_iterator_operation();
		}
	}

	bool
	compare_equal(const abstract_any_iterator<T>& other) const override
	{
		const auto* p = dynamic_cast<const any_iterator_storage*>(&other);
		if (!p)
			mtk::_throw_bad_any_iterator_operation();

		return (m_iter == p->m_iter);
	}

	bool
	compare_less_than(const abstract_any_iterator<T>& other) const override
	{
		if constexpr (is_random_access_iterator_v<Iter>) {
			const auto* p = dynamic_cast<const any_iterator_storage*>(&other);
			if (!p)
				mtk::_throw_bad_any_iterator_operation();

			return (m_iter < p->m_iter);
		} else {
			MTK_IGNORE(other);
			return false;
		}
	}

private:
	Iter m_iter;
};

} // namespace any_iterator
} // namespace impl_core



template<class T>
class any_input_iterator
{
public:
	static_assert(!std::is_reference_v<T>, "T must be non-reference");

	using value_type = std::remove_cv_t<T>;
	using reference = value_type;
	using pointer = struct {
		const value_type value;
		const value_type*
		operator->() const { return mtk::_addressof(value); }
	};
	using difference_type = ptrdiff_t;
	using iterator_category = std::input_iterator_tag;

	template<class Iter
		,_require<is_input_iterator_v<Iter>> = 0
		,_require<std::is_same_v<iter::value_type<Iter>, value_type>> = 0>
	any_input_iterator(Iter iter) :
		m_ptr(mtk::make_unique<impl_core::any_iterator::any_iterator_storage<Iter, iter::value_type<Iter>>>(iter))
	{ }

	any_input_iterator(const any_input_iterator& other) :
		m_ptr(other.m_ptr ? other.m_ptr->clone() : nullptr)
	{ }

	any_input_iterator(any_input_iterator&& other) :
		m_ptr(mtk::_move(other.m_ptr))
	{ }

	any_input_iterator&
	operator=(any_input_iterator rhs) noexcept
	{
		swap(m_ptr, rhs.m_ptr);
		return *this;
	}

	reference
	operator*() const
	{
		return m_ptr->dereference();
	}

	pointer
	operator->() const
	{
		return pointer{ **this };
	}

	friend
	any_input_iterator&
	operator++(any_input_iterator& rhs)
	{
		rhs.m_ptr->increment();
		return rhs;
	}

	friend
	any_input_iterator
	operator++(any_input_iterator& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend
	bool
	operator==(const any_input_iterator& lhs, const any_input_iterator& rhs)
	{
		if (bool(lhs.m_ptr) ^ bool(rhs.m_ptr))
			return false;

		if (!lhs.m_ptr && !rhs.m_ptr)
			return true;

		return lhs.m_ptr->compare_equal(*rhs.m_ptr);
	}

	friend
	bool
	operator!=(const any_input_iterator& lhs, const any_input_iterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	using _abstract_iter_type = impl_core::any_iterator::abstract_any_iterator<value_type>;
	unique_ptr<_abstract_iter_type, asserting_pointer_validator> m_ptr;
};



template<class T>
class any_forward_iterator
{
public:
	static_assert(!std::is_reference_v<T>, "T must be non-reference");

	using value_type = std::remove_cv_t<T>;
	using reference = T&;
	using pointer = T*;
	using difference_type = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	any_forward_iterator() :
		m_ptr()
	{ }

	template<class Iter
		,_require<is_forward_iterator_v<Iter>> = 0
		,_require<std::is_convertible_v<iter::reference<Iter>, reference>> = 0>
	any_forward_iterator(Iter iter) :
		m_ptr(mtk::make_unique<impl_core::any_iterator::any_iterator_storage<Iter, reference>>(iter))
	{ }

	any_forward_iterator(const any_forward_iterator& other) :
		m_ptr(other.m_ptr ? other.m_ptr->clone() : nullptr)
	{ }

	any_forward_iterator(any_forward_iterator&& other) :
		m_ptr(mtk::_move(other.m_ptr))
	{ }

	any_forward_iterator&
	operator=(any_forward_iterator rhs) noexcept
	{
		swap(m_ptr, rhs.m_ptr);
		return *this;
	}

	reference
	operator*() const
	{
		return m_ptr->dereference();
	}

	pointer
	operator->() const
	{
		return mtk::_addressof(**this);
	}

	friend
	any_forward_iterator&
	operator++(any_forward_iterator& rhs)
	{
		rhs.m_ptr->increment();
		return rhs;
	}

	friend
	any_forward_iterator
	operator++(any_forward_iterator& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend
	bool
	operator==(const any_forward_iterator& lhs, const any_forward_iterator& rhs)
	{
		if (bool(lhs.m_ptr) ^ bool(rhs.m_ptr))
			return false;

		if (!lhs.m_ptr && !rhs.m_ptr)
			return true;

		return lhs.m_ptr->compare_equal(*rhs.m_ptr);
	}

	friend
	bool
	operator!=(const any_forward_iterator& lhs, const any_forward_iterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	using _abstract_iter_type = impl_core::any_iterator::abstract_any_iterator<reference>;
	unique_ptr<_abstract_iter_type, asserting_pointer_validator> m_ptr;
};



template<class T>
class any_bidirectional_iterator
{
public:
	static_assert(!std::is_reference_v<T>, "T must be non-reference");

	using value_type = std::remove_cv_t<T>;
	using reference = T&;
	using pointer = T*;
	using difference_type = ptrdiff_t;
	using iterator_category = std::bidirectional_iterator_tag;

	any_bidirectional_iterator() :
		m_ptr()
	{ }

	template<class Iter
		,_require<is_bidirectional_iterator_v<Iter>> = 0
		,_require<std::is_convertible_v<iter::reference<Iter>, reference>> = 0>
	any_bidirectional_iterator(Iter iter) :
		m_ptr(mtk::make_unique<impl_core::any_iterator::any_iterator_storage<Iter, reference>>(iter))
	{ }

	any_bidirectional_iterator(const any_bidirectional_iterator& other) :
		m_ptr(other.m_ptr ? other.m_ptr->clone() : nullptr)
	{ }

	any_bidirectional_iterator(any_bidirectional_iterator&& other) :
		m_ptr(mtk::_move(other.m_ptr))
	{ }

	any_bidirectional_iterator&
	operator=(any_bidirectional_iterator rhs) noexcept
	{
		swap(m_ptr, rhs.m_ptr);
		return *this;
	}

	reference
	operator*() const
	{
		return m_ptr->dereference();
	}

	pointer
	operator->() const
	{
		return mtk::_addressof(**this);
	}

	friend
	any_bidirectional_iterator&
	operator++(any_bidirectional_iterator& rhs)
	{
		rhs.m_ptr->increment();
		return rhs;
	}

	friend
	any_bidirectional_iterator
	operator++(any_bidirectional_iterator& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend
	any_bidirectional_iterator&
	operator--(any_bidirectional_iterator& rhs)
	{
		rhs.m_ptr->decrement();
		return rhs;
	}

	friend
	any_bidirectional_iterator
	operator--(any_bidirectional_iterator& lhs, int)
	{
		auto cp = lhs;
		--lhs;
		return cp;
	}

	friend
	bool
	operator==(const any_bidirectional_iterator& lhs, const any_bidirectional_iterator& rhs)
	{
		if (bool(lhs.m_ptr) ^ bool(rhs.m_ptr))
			return false;

		if (!lhs.m_ptr && !rhs.m_ptr)
			return true;

		return lhs.m_ptr->compare_equal(*rhs.m_ptr);
	}

	friend
	bool
	operator!=(const any_bidirectional_iterator& lhs, const any_bidirectional_iterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	using _abstract_iter_type = impl_core::any_iterator::abstract_any_iterator<reference>;
	unique_ptr<_abstract_iter_type, asserting_pointer_validator> m_ptr;
};


template<class T>
class any_random_access_iterator
{
public:
	static_assert(!std::is_reference_v<T>, "T must be non-reference");

	using value_type = std::remove_cv_t<T>;
	using reference = T&;
	using pointer = T*;
	using difference_type = ptrdiff_t;
	using iterator_category = std::random_access_iterator_tag;

	any_random_access_iterator() :
		m_ptr()
	{ }

	template<class Iter
		,_require<is_random_access_iterator_v<Iter>> = 0
		,_require<std::is_convertible_v<iter::reference<Iter>, reference>> = 0>
	any_random_access_iterator(Iter iter) :
		m_ptr(mtk::make_unique<impl_core::any_iterator::any_iterator_storage<Iter, reference>>(iter))
	{ }

	any_random_access_iterator(const any_random_access_iterator& other) :
		m_ptr(other.m_ptr ? other.m_ptr->clone() : nullptr)
	{ }

	any_random_access_iterator(any_random_access_iterator&& other) :
		m_ptr(mtk::_move(other.m_ptr))
	{ }

	any_random_access_iterator&
	operator=(any_random_access_iterator rhs) noexcept
	{
		swap(m_ptr, rhs.m_ptr);
		return *this;
	}

	reference
	operator*() const
	{
		return m_ptr->dereference();
	}

	pointer
	operator->() const
	{
		return mtk::_addressof(**this);
	}

	reference
	operator[](difference_type idx) const
	{
		return m_ptr->dereference(idx);
	}

	friend
	any_random_access_iterator&
	operator++(any_random_access_iterator& rhs)
	{
		rhs.m_ptr->increment();
		return rhs;
	}

	friend
	any_random_access_iterator
	operator++(any_random_access_iterator& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend
	any_random_access_iterator&
	operator--(any_random_access_iterator& rhs)
	{
		rhs.m_ptr->decrement();
		return rhs;
	}

	friend
	any_random_access_iterator
	operator--(any_random_access_iterator& lhs, int)
	{
		auto cp = lhs;
		--lhs;
		return cp;
	}

	friend
	any_random_access_iterator&
	operator+=(any_random_access_iterator& lhs, difference_type rhs)
	{
		lhs.m_ptr->increment(rhs);
		return lhs;
	}

	friend
	any_random_access_iterator&
	operator-=(any_random_access_iterator& lhs, difference_type rhs)
	{
		lhs.m_ptr->decrement(rhs);
		return lhs;
	}

	friend
	any_random_access_iterator
	operator+(any_random_access_iterator lhs, difference_type rhs)
	{
		return lhs += rhs;
	}

	friend
	any_random_access_iterator
	operator+(difference_type lhs, any_random_access_iterator rhs)
	{
		return rhs += lhs;
	}

	friend
	any_random_access_iterator
	operator-(any_random_access_iterator lhs, difference_type rhs)
	{
		return lhs -= rhs;
	}

	friend
	difference_type
	operator-(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		return lhs.m_ptr->difference(*rhs.m_ptr);
	}

	friend
	bool
	operator==(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		if (bool(lhs.m_ptr) ^ bool(rhs.m_ptr))
			return false;

		if (!lhs.m_ptr && !rhs.m_ptr)
			return true;

		return lhs.m_ptr->compare_equal(*rhs.m_ptr);
	}

	friend
	bool
	operator!=(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		return !(lhs == rhs);
	}

	friend
	bool
	operator<(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		if (!lhs.m_ptr && !rhs.m_ptr)
			return false;

		return lhs.m_ptr->compare_less_than(*rhs.m_ptr);
	}

	friend
	bool
	operator>(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		if (!lhs.m_ptr && !rhs.m_ptr)
			return false;

		return rhs.m_ptr->compare_less_than(*lhs.m_ptr);
	}

	friend
	bool
	operator<=(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		if (!lhs.m_ptr && !rhs.m_ptr)
			return false;

		return !rhs.m_ptr->compare_less_than(*lhs.m_ptr);
	}

	friend
	bool
	operator>=(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		if (!lhs.m_ptr && !rhs.m_ptr)
			return false;

		return !lhs.m_ptr->compare_less_than(*rhs.m_ptr);
	}

private:
	using _abstract_iter_type = impl_core::any_iterator::abstract_any_iterator<reference>;
	unique_ptr<_abstract_iter_type, asserting_pointer_validator> m_ptr;
};

} // namespace mtk

#endif
