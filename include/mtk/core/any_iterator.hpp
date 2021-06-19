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
	unique_ptr<abstract_any_iterator>
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
class any_iterator final :
	public abstract_any_iterator<T>
{
public:
	any_iterator(Iter iter) :
		m_iter(iter)
	{ }

	~any_iterator() override = default;

	unique_ptr<abstract_any_iterator<T>>
	clone() const override
	{
		return mtk::make_unique<any_iterator>(m_iter);
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
			const auto* p = dynamic_cast<const any_iterator*>(&other);
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
		const auto* p = dynamic_cast<const any_iterator*>(&other);
		if (!p)
			return false;

		return (m_iter == p->m_iter);
	}

	bool
	compare_less_than(const abstract_any_iterator<T>& other) const override
	{
		if constexpr (is_random_access_iterator_v<Iter>) {
			const auto* p = dynamic_cast<const any_iterator*>(&other);
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

template<class T>
class any_iterator_storage
{
public:
	any_iterator_storage() :
		m_ptr()
	{ }

	template<class Iter>
	any_iterator_storage(Iter iter) :
		m_ptr(mtk::make_unique<any_iterator<Iter, T>>(iter))
	{ }

	any_iterator_storage(const any_iterator_storage& other) :
		m_ptr(other.m_ptr ? other.m_ptr->clone() : nullptr)
	{ }

	any_iterator_storage(any_iterator_storage&& other) noexcept :
		m_ptr(mtk::_move(other.m_ptr))
	{ }

	any_iterator_storage&
	operator=(any_iterator_storage rhs) noexcept
	{
		this->swap(rhs);
		return *this;
	}

	T
	dereference() const
	{
		return m_ptr->dereference();
	}

	auto
	address() const
	{
		if constexpr (std::is_reference_v<T>) {
			return mtk::_addressof(**this);
		} else {
			struct {
				T value;
				const T*
				operator->() const { return mtk::_addressof(this->value); }
			} ret {this->dereference()};
			return ret;
		}
	}

	T
	dereference(ptrdiff_t idx) const
	{
		return m_ptr->dereference(idx);
	}

	void
	increment()
	{
		m_ptr->increment();
	}

	void
	decrement()
	{
		m_ptr->decrement();
	}

	void
	increment(ptrdiff_t n)
	{
		m_ptr->increment(n);
	}

	void
	decrement(ptrdiff_t n)
	{
		m_ptr->decrement(n);
	}

	ptrdiff_t
	difference(const any_iterator_storage& other) const
	{
		if (!m_ptr || !other.m_ptr)
			return 0;

		return m_ptr->difference(*other.m_ptr);
	}

	bool
	compare_equal(const any_iterator_storage& other) const
	{
		if (bool(m_ptr) ^ bool(other.m_ptr))
			return false;

		if (!m_ptr && !other.m_ptr)
			return true;

		return m_ptr->compare_equal(*other.m_ptr);
	}

	bool
	compare_lt(const any_iterator_storage& other) const
	{
		if (!m_ptr || !other.m_ptr)
			return false;

		return m_ptr->compare_less_than(*other.m_ptr);
	}

	bool
	compare_gt(const any_iterator_storage& other) const
	{
		if (!m_ptr || !other.m_ptr)
			return false;

		return other.m_ptr->compare_less_than(*m_ptr);
	}

	bool
	compare_lte(const any_iterator_storage& other) const
	{
		if (!m_ptr || !other.m_ptr)
			return false;

		return !other.m_ptr->compare_less_than(*m_ptr);
	}

	bool
	compare_gte(const any_iterator_storage& other) const
	{
		if (!m_ptr || !other.m_ptr)
			return false;

		return !m_ptr->compare_less_than(*other.m_ptr);
	}

	void
	swap(any_iterator_storage& other) noexcept
	{
		swap(m_ptr, other.m_ptr);
	}

private:
	unique_ptr<abstract_any_iterator<T>, asserting_pointer_validator> m_ptr;
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
	using pointer = decltype(impl_core::any_iterator::any_iterator_storage<value_type>().address());
	using difference_type = ptrdiff_t;
	using iterator_category = std::input_iterator_tag;

	template<class Iter
#ifndef MTK_DOXYGEN
		,_require<is_input_iterator_v<Iter>> = 0
		,_require<std::is_same_v<iter::value_type<Iter>, value_type>> = 0
#endif
	>
	any_input_iterator(Iter iter) :
		m_storage(mtk::_move(iter))
	{ }

	reference
	operator*() const
	{
		return m_storage.dereference();
	}

	pointer
	operator->() const
	{
		return m_storage.address();
	}

	void
	swap(any_input_iterator& other) noexcept
	{
		m_storage.swap(other.m_storage);
	}



	friend
	any_input_iterator&
	operator++(any_input_iterator& rhs)
	{
		rhs.m_storage.increment();
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
		return lhs.m_storage.compare_equal(rhs.m_storage);
	}

	friend
	bool
	operator!=(const any_input_iterator& lhs, const any_input_iterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	impl_core::any_iterator::any_iterator_storage<value_type> m_storage;
};

template<class T>
void
swap(any_input_iterator<T>& a, any_input_iterator<T>& b) noexcept
{
	a.swap(b);
}



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
		m_storage()
	{ }

	template<class Iter
#ifndef MTK_DOXYGEN
		,_require<is_forward_iterator_v<Iter>> = 0
		,_require<std::is_convertible_v<iter::reference<Iter>, reference>> = 0
#endif
	>
	any_forward_iterator(Iter iter) :
		m_storage(mtk::_move(iter))
	{ }

	reference
	operator*() const
	{
		return m_storage.dereference();
	}

	pointer
	operator->() const
	{
		return m_storage.address();
	}

	void
	swap(any_forward_iterator& other) noexcept
	{
		m_storage.swap(other.m_storage);
	}



	friend
	any_forward_iterator&
	operator++(any_forward_iterator& rhs)
	{
		rhs.m_storage.increment();
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
		return lhs.m_storage.compare_equal(rhs.m_storage);
	}

	friend
	bool
	operator!=(const any_forward_iterator& lhs, const any_forward_iterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	impl_core::any_iterator::any_iterator_storage<reference> m_storage;
};

template<class T>
void
swap(any_forward_iterator<T>& a, any_forward_iterator<T>& b) noexcept
{
	a.swap(b);
}



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
		m_storage()
	{ }

	template<class Iter
#ifndef MTK_DOXYGEN
		,_require<is_bidirectional_iterator_v<Iter>> = 0
		,_require<std::is_convertible_v<iter::reference<Iter>, reference>> = 0
#endif
	>
	any_bidirectional_iterator(Iter iter) :
		m_storage(mtk::_move(iter))
	{ }

	reference
	operator*() const
	{
		return m_storage.dereference();
	}

	pointer
	operator->() const
	{
		return m_storage.address();
	}

	void
	swap(any_bidirectional_iterator& other) noexcept
	{
		m_storage.swap(other.m_storage);
	}



	friend
	any_bidirectional_iterator&
	operator++(any_bidirectional_iterator& rhs)
	{
		rhs.m_storage.increment();
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
		rhs.m_storage.decrement();
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
		return lhs.m_storage.compare_equal(rhs.m_storage);
	}

	friend
	bool
	operator!=(const any_bidirectional_iterator& lhs, const any_bidirectional_iterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	impl_core::any_iterator::any_iterator_storage<reference> m_storage;
};

template<class T>
void
swap(any_bidirectional_iterator<T>& a, any_bidirectional_iterator<T>& b) noexcept
{
	a.swap(b);
}


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
		m_storage()
	{ }

	template<class Iter
#ifndef MTK_DOXYGEN
		,_require<is_random_access_iterator_v<Iter>> = 0
		,_require<std::is_convertible_v<iter::reference<Iter>, reference>> = 0
#endif
	>
	any_random_access_iterator(Iter iter) :
		m_storage(mtk::_move(iter))
	{ }

	reference
	operator*() const
	{
		return m_storage.dereference();
	}

	pointer
	operator->() const
	{
		return m_storage.address();
	}

	reference
	operator[](difference_type idx) const
	{
		return m_storage.dereference(idx);
	}

	void
	swap(any_random_access_iterator& other) noexcept
	{
		m_storage.swap(other.m_storage);
	}



	friend
	any_random_access_iterator&
	operator++(any_random_access_iterator& rhs)
	{
		rhs.m_storage.increment();
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
		rhs.m_ptr.decrement();
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
		lhs.m_storage.increment(rhs);
		return lhs;
	}

	friend
	any_random_access_iterator&
	operator-=(any_random_access_iterator& lhs, difference_type rhs)
	{
		lhs.m_storage.decrement(rhs);
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
		return lhs.m_storage.difference(rhs.m_storage);
	}

	friend
	bool
	operator==(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		return lhs.m_storage.compare_equal(rhs.m_storage);
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
		return lhs.m_storage.compare_lt(rhs.m_storage);
	}

	friend
	bool
	operator>(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		return lhs.m_storage.compare_gt(rhs.m_storage);
	}

	friend
	bool
	operator<=(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		return lhs.m_storage.compare_lte(rhs.m_storage);
	}

	friend
	bool
	operator>=(const any_random_access_iterator& lhs, const any_random_access_iterator& rhs)
	{
		return lhs.m_storage.compare_gte(rhs.m_storage);
	}

private:
	impl_core::any_iterator::any_iterator_storage<reference> m_storage;
};

template<class T>
void
swap(any_random_access_iterator<T>& a, any_random_access_iterator<T>& b) noexcept
{
	a.swap(b);
}

} // namespace mtk

#endif
