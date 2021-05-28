#ifndef MTK_LINALG_MATRIX_HPP
#define MTK_LINALG_MATRIX_HPP

#include <mtk/core/array.hpp>
#include <mtk/core/assert.hpp>
#include <mtk/core/iterator_traits.hpp>
#include <mtk/core/preprocessor.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/require.hpp>
#include <mtk/core/impl/swap.hpp>
#include <mtk/ranges/range.hpp>
#include <mtk/linalg/fwd.hpp>

#include <cmath>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <optional>
#include <type_traits>

namespace mtk {

template<class Mat>
struct _linalg_traits { };

namespace impl_matrix {

template<class Iter
	,class ConstIter
	,size_t R
	,size_t C
	,matrix_options Opt
	,bool IsConst = std::is_same_v<Iter, ConstIter>>
class _vector_reference;


template<class Iter>
class _matrix_stride_iterator;

template<class Iter
	,class ConstIter
	,size_t R
	,size_t C
	,matrix_options Opt
	,bool IsRow>
class _matrix_vector_iterator;

} // namespace impl_matrix



template<class S
	,size_t R
	,size_t C
	,matrix_options Opt>
struct _linalg_traits<matrix<S, R, C, Opt>>
{
	using mat = matrix<S, R, C, Opt>;

	using value_type = S;
	using iterator = S*;
	using const_iterator = const S*;

	static constexpr size_t row_dimension = R;
	static constexpr size_t column_dimension = C;
	static constexpr matrix_options options = Opt;

	template<class Scalar
		,size_t Rows
		,size_t Cols
		,matrix_options Options>
	using matrix_type = matrix<Scalar, Rows, Cols, Options>;

	template<class Mat>
	static constexpr
	auto
	begin(Mat&& m)
	{
		return m.m_data.begin();
	}

	template<class Mat>
	static constexpr
	auto
	end(Mat&& m)
	{
		return m.m_data.end();
	}

	static constexpr
	auto
	rows(const mat& m)
	{
		if constexpr (R == dynamic_extent)
			return m.m_rows;
		else
			return R;
	}

	static constexpr
	auto
	columns(const mat& m)
	{
		if constexpr (C == dynamic_extent)
			return m.m_cols;
		else
			return C;
	}
};

template<class Iter
	,class ConstIter
	,size_t R
	,size_t C
	,matrix_options Opt
	,bool IsConst>
struct _linalg_traits<impl_matrix::_vector_reference<Iter, ConstIter, R, C, Opt, IsConst>>
{
	using mat = impl_matrix::_vector_reference<Iter, ConstIter, R, C, Opt, IsConst>;

	using value_type = iter::value_type<Iter>;
	using iterator = Iter;
	using const_iterator = ConstIter;

	static constexpr size_t row_dimension = R;
	static constexpr size_t column_dimension = C;
	static constexpr matrix_options options = Opt;

	template<class Scalar
		,size_t Rows
		,size_t Cols
		,matrix_options Options>
	using matrix_type = matrix<Scalar, Rows, Cols, Options>;

	template<class Mat>
	static constexpr
	auto
	begin(Mat&& m)
	{
		return m.m_iter;
	}

	template<class Mat>
	static constexpr
	auto
	end(Mat&& m)
	{
		return m.m_iter + rows(m)*columns(m);
	}

	static constexpr
	auto
	rows(const mat& m)
	{
		if constexpr (R == dynamic_extent)
			return m.m_size;
		else
			return R;
	}

	static constexpr
	auto
	columns(const mat& m)
	{
		if constexpr (C == dynamic_extent)
			return m.m_size;
		else
			return C;
	}
};

template<class MatA
	,class MatB>
struct _is_matrix_compatible
{
	using mat_a = std::decay_t<MatA>;
	using mat_b = std::decay_t<MatB>;

	static constexpr bool is_vector = ((mat_a::row_dimension == 1) || (mat_a::column_dimension == 1)) &&
		((mat_b::row_dimension == 1) || (mat_b::column_dimension == 1));

	static constexpr auto a_max_dim = ((mat_a::row_dimension >= mat_a::column_dimension) ? mat_a::row_dimension : mat_a::column_dimension);
	static constexpr auto b_max_dim = ((mat_b::row_dimension >= mat_b::column_dimension) ? mat_b::row_dimension : mat_b::column_dimension);
	static constexpr bool is_vec_comp = (a_max_dim == b_max_dim);
	static constexpr bool is_mat_comp = ((mat_a::row_dimension == mat_b::row_dimension) && (mat_a::column_dimension == mat_b::column_dimension));
	static constexpr bool value = (is_vector ? is_vec_comp : is_mat_comp) && std::is_same_v<typename mat_a::value_type, typename mat_b::value_type>;
};

template<class Mat>
constexpr
Mat
_make_matrix(size_t rows, size_t cols)
{
	MTK_IGNORE(rows, cols);
	constexpr auto row_dim = std::decay_t<Mat>::row_dimension;
	constexpr auto col_dim = std::decay_t<Mat>::column_dimension;
	if constexpr ((row_dim == dynamic_extent) && (col_dim == dynamic_extent))
		return Mat(rows, cols);
	else if constexpr (row_dim == dynamic_extent)
		return Mat(rows);
	else if constexpr (col_dim == dynamic_extent)
		return Mat(cols);
	else
		return Mat();
}


template<class Derived>
class _matrix_base
{
public:
	using _traits = _linalg_traits<Derived>;

	using value_type = typename _traits::value_type;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	using iterator = typename _traits::iterator;
	using const_iterator = typename _traits::const_iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	using reference = std::remove_reference_t<decltype(*mtk::_declval<iterator>())>&;
	using const_reference = const value_type&;
	using pointer = std::remove_reference_t<reference>*;
	using const_pointer = const value_type*;

	static constexpr
	size_type
	row_dimension = _traits::row_dimension;

	static constexpr
	size_type
	column_dimension = _traits::column_dimension;

	static constexpr
	matrix_options
	options = _traits::options;

	using row_iterator = impl_matrix::_matrix_vector_iterator<iterator, const_iterator, row_dimension, column_dimension, options, true>;
	using const_row_iterator = impl_matrix::_matrix_vector_iterator<const_iterator, const_iterator, row_dimension, column_dimension, options, true>;
	using column_iterator = impl_matrix::_matrix_vector_iterator<iterator, const_iterator, row_dimension, column_dimension, options, false>;
	using const_column_iterator = impl_matrix::_matrix_vector_iterator<const_iterator, const_iterator, row_dimension, column_dimension, options, false>;
	using reverse_row_iterator = std::reverse_iterator<row_iterator>;
	using const_reverse_row_iterator = std::reverse_iterator<const_row_iterator>;
	using reverse_column_iterator = std::reverse_iterator<column_iterator>;
	using const_reverse_column_iterator = std::reverse_iterator<const_column_iterator>;

	using row_vector_type = decltype(*mtk::_declval<row_iterator>());
	using const_row_vector_type = decltype(*mtk::_declval<const_row_iterator>());
	using column_vector_type = decltype(*mtk::_declval<column_iterator>());
	using const_column_vector_type = decltype(*mtk::_declval<const_column_iterator>());

	static constexpr
	bool
	_is_column_major = ((options & matrix_options::column_major) == matrix_options::column_major);

	static constexpr
	bool
	_is_vector = ((row_dimension == 1) || (column_dimension == 1));



	constexpr
	size_type
	rows() const
	{
		return _traits::rows(static_cast<const Derived&>(*this));
	}

	constexpr
	size_type
	columns() const
	{
		return _traits::columns(static_cast<const Derived&>(*this));
	}

	constexpr
	size_type
	size() const
	{
		return this->rows()*this->columns();
	}

	constexpr
	size_type
	max_size() const
	{
		return this->size();
	}

	[[nodiscard]]
	constexpr
	bool
	empty() const
	{
		return (this->size() == size_type());
	}



	constexpr
	iterator
	begin()
	{
		return _traits::begin(static_cast<Derived&>(*this));
	}

	constexpr
	const_iterator
	begin() const
	{
		return _traits::begin(static_cast<const Derived&>(*this));
	}

	constexpr
	const_iterator
	cbegin() const
	{
		return this->begin();
	}

	constexpr
	iterator
	end()
	{
		return _traits::end(static_cast<Derived&>(*this));
	}

	constexpr
	const_iterator
	end() const
	{
		return _traits::end(static_cast<const Derived&>(*this));
	}

	constexpr
	const_iterator
	cend() const
	{
		return this->end();
	}

	constexpr
	reverse_iterator
	rbegin()
	{
		return reverse_iterator{this->end()};
	}

	constexpr
	const_reverse_iterator
	rbegin() const
	{
		return const_reverse_iterator{this->end()};
	}

	constexpr
	const_reverse_iterator
	crbegin() const
	{
		return this->rbegin();
	}

	constexpr
	reverse_iterator
	rend()
	{
		return reverse_iterator{this->begin()};
	}

	constexpr
	const_reverse_iterator
	rend() const
	{
		return const_reverse_iterator{this->begin()};
	}

	constexpr
	const_reverse_iterator
	crend() const
	{
		return this->rend();
	}



	constexpr
	row_iterator
	begin_rows()
	{
		return row_iterator(this->begin(), this->rows(), this->columns(), 0);
	}

	constexpr
	const_row_iterator
	begin_rows() const
	{
		return const_row_iterator(this->begin(), this->rows(), this->columns(), 0);
	}

	constexpr
	const_row_iterator
	cbegin_rows() const
	{
		return this->begin_rows();
	}

	constexpr
	row_iterator
	end_rows()
	{
		return row_iterator(this->begin(), this->rows(), this->columns(), this->rows());
	}

	constexpr
	const_row_iterator
	end_rows() const
	{
		return const_row_iterator(this->begin(), this->rows(), this->columns(), this->rows());
	}

	constexpr
	const_row_iterator
	cend_rows() const
	{
		return this->end_rows();
	}

	constexpr
	reverse_row_iterator
	rbegin_rows()
	{
		return reverse_row_iterator{this->end_rows()};
	}

	constexpr
	const_reverse_row_iterator
	rbegin_rows() const
	{
		return const_reverse_row_iterator{this->end_rows()};
	}

	constexpr
	const_reverse_row_iterator
	crbegin_rows() const
	{
		return this->rbegin_rows();
	}

	constexpr
	reverse_row_iterator
	rend_rows()
	{
		return reverse_row_iterator{this->begin_rows()};
	}

	constexpr
	const_reverse_row_iterator
	rend_rows() const
	{
		return const_reverse_row_iterator{this->begin_rows()};
	}

	constexpr
	const_reverse_row_iterator
	crend_rows() const
	{
		return this->rend_rows();
	}



	constexpr
	column_iterator
	begin_columns()
	{
		return column_iterator(this->begin(), this->rows(), this->columns(), 0);
	}

	constexpr
	const_column_iterator
	begin_columns() const
	{
		return const_column_iterator(this->begin(), this->rows(), this->columns(), 0);
	}

	constexpr
	const_column_iterator
	cbegin_columns() const
	{
		return this->begin_columns();
	}

	constexpr
	column_iterator
	end_columns()
	{
		return column_iterator(this->begin(), this->rows(), this->columns(), this->columns());
	}

	constexpr
	const_column_iterator
	end_columns() const
	{
		return const_column_iterator(this->begin(), this->rows(), this->columns(), this->columns());
	}

	constexpr
	const_column_iterator
	cend_columns() const
	{
		return this->end_columns();
	}

	constexpr
	reverse_column_iterator
	rbegin_columns()
	{
		return reverse_column_iterator{this->end_columns()};
	}

	constexpr
	const_reverse_column_iterator
	rbegin_columns() const
	{
		return const_reverse_column_iterator{this->end_columns()};
	}

	constexpr
	const_reverse_column_iterator
	crbegin_columns() const
	{
		return this->rbegin_columns();
	}

	constexpr
	reverse_column_iterator
	rend_columns()
	{
		return reverse_column_iterator{this->begin_columns()};
	}

	constexpr
	const_reverse_column_iterator
	rend_columns() const
	{
		return const_reverse_column_iterator{this->begin_columns()};
	}

	constexpr
	const_reverse_column_iterator
	crend_columns() const
	{
		return this->rend_columns();
	}



	constexpr
	auto
	all_rows()
	{
		return mtk::range(this->begin_rows(), this->end_rows());
	}

	constexpr
	auto
	all_rows() const
	{
		return mtk::range(this->begin_rows(), this->end_rows());
	}

	constexpr
	auto
	all_columns()
	{
		return mtk::range(this->begin_columns(), this->end_columns());
	}

	constexpr
	auto
	all_columns() const
	{
		return mtk::range(this->begin_columns(), this->end_columns());
	}



	constexpr
	reference
	front()
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	constexpr
	const_reference
	front() const
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	constexpr
	reference
	back()
	{
		MTK_ASSERT(!this->empty());
		return *std::prev(this->end());
	}

	constexpr
	const_reference
	back() const
	{
		MTK_ASSERT(!this->empty());
		return *std::prev(this->end());
	}

	constexpr
	reference
	value(size_type idx)
	{
		MTK_ASSERT(idx < this->size());
		return *(this->begin() + idx);
	}

	constexpr
	const_reference
	value(size_type idx) const
	{
		MTK_ASSERT(idx < this->size());
		return *(this->begin() + idx);
	}

	constexpr
	reference
	value(size_type row, size_type column)
	{
		MTK_ASSERT(row < this->rows());
		MTK_ASSERT(column < this->columns());
		if constexpr (_is_column_major) {
			return *(this->begin() + row + column*this->rows());
		} else {
			return *(this->begin() + row*this->columns() + column);
		}
	}

	constexpr
	const_reference
	value(size_type row, size_type column) const
	{
		MTK_ASSERT(row < this->rows());
		MTK_ASSERT(column < this->columns());
		if constexpr (_is_column_major) {
			return *(this->begin() + row + column*this->rows());
		} else {
			return *(this->begin() + row*this->columns() + column);
		}
	}

	constexpr
	row_vector_type
	row(size_type idx)
	{
		return *(this->begin_rows() + idx);
	}

	constexpr
	const_row_vector_type
	row(size_type idx) const
	{
		return *(this->begin_rows() + idx);
	}

	constexpr
	column_vector_type
	column(size_type idx)
	{
		return *(this->begin_columns() + idx);
	}

	constexpr
	const_column_vector_type
	column(size_type idx) const
	{
		return *(this->begin_columns() + idx);
	}

	constexpr
	reference
	operator()(size_type idx)
	{
		return this->value(idx);
	}

	constexpr
	const_reference
	operator()(size_type idx) const
	{
		return this->value(idx);
	}

	constexpr
	reference
	operator()(size_type row, size_type column)
	{
		return this->value(row, column);
	}

	constexpr
	const_reference
	operator()(size_type row, size_type column) const
	{
		return this->value(row, column);
	}

	constexpr
	auto
	operator[](size_type idx)
	{
		if constexpr (_is_vector) {
			MTK_ASSERT(idx < this->size());
			return *(this->begin() + idx);
		} else {
			MTK_ASSERT(idx < this->rows());
			return *(this->begin_rows() + idx);
		}
	}

	constexpr
	auto
	operator[](size_type idx) const
	{
		if constexpr (_is_vector) {
			MTK_ASSERT(idx < this->size());
			return *(this->begin() + idx);
		} else {
			MTK_ASSERT(idx < this->rows());
			return *(this->begin_rows() + idx);
		}
	}



	constexpr
	auto
	transposed() const
	{
		using ret_type = typename _linalg_traits<Derived>::template matrix_type<value_type, column_dimension, row_dimension, options>;

		const auto rs = this->rows();
		const auto cs = this->columns();
		auto ret = mtk::_make_matrix<ret_type>(cs, rs);
		for (size_type r = 0; r < rs; ++r) {
			for (size_type c = 0; c < cs; ++c) {
				ret.value(c, r) = this->value(r, c);
			}
		}

		return ret;
	}



#ifndef MTK_DOXYGEN
	template<bool IsRef = !std::is_pointer_v<iterator>
		,_require<IsRef == !std::is_pointer_v<iterator>> = 0
		,_require<!IsRef> = 0>
#endif
	constexpr
	void
	swap(_matrix_base& other) noexcept
	{
		auto& self = static_cast<Derived&>(*this);
		auto& oth = static_cast<Derived&>(other);
		mtk::_swap(self, other);
	}

#ifndef MTK_DOXYGEN
	template<class Other
		,_require<_is_matrix_compatible<Derived, Other>::value> = 0>
	constexpr
	void
	swap(_matrix_base<Other>& other)
	{
		auto& self = static_cast<Derived&>(*this);
		auto& oth = static_cast<Other&>(other);

		matrix<value_type, row_dimension, column_dimension, options | Other::options> tmp = mtk::_move(self);
		self = mtk::_move(oth);
		oth = mtk::_move(tmp);
	}

	template<class Other
		,_require<_is_matrix_compatible<Derived, Other>::value> = 0
		,_require<!std::is_pointer_v<typename Other::iterator>> = 0>
	constexpr
	void
	swap(_matrix_base<Other>&& other)
	{
		this->swap(other);
	}
#endif

protected:
	template<class RowIter>
	constexpr
	void
	_assign_rows(RowIter row_first)
	{
		auto row_it = this->begin_rows();
		const auto row_end = this->end_rows();
		while (row_it != row_end) {
			auto first = row_first->begin();
			auto it = row_it->begin();
			const auto end = row_it->end();
			while (it != end) {
				*(it++) = *(first++);
			}
			++row_it;
			++row_first;
		}
	}

	template<class Iter>
	constexpr
	void
	_assign(Iter first)
	{
		if constexpr (_is_vector) {
			auto it = this->begin();
			const auto end = this->end();
			while (it != end) {
				*(it++) = *(first++);
			}
		} else {
			auto row_it = this->begin_rows();
			const auto row_end = this->end_rows();
			while (row_it != row_end) {
				auto it = row_it->begin();
				const auto end = row_it->end();
				while (it != end) {
					*(it++) = *(first++);
				}
				++row_it;
			}
		}
	}
};



template<class Derived
	,class Base>
class _matrix_data_base :
	public Base
{
public:
	using typename Base::pointer;
	using typename Base::const_pointer;

	pointer
	data()
	{
		return this->begin();
	}

	const_pointer
	data() const
	{
		return this->begin();
	}
};



template<class Derived
	,class Base>
class _matrix_square_base :
	public Base
{
public:
	using typename Base::value_type;
	using typename Base::size_type;
	using typename Base::iterator;
	using typename Base::const_iterator;

	using Base::row_dimension;
	using Base::options;
	using Base::_is_column_major;

	static constexpr
	size_type
	dimension = row_dimension;

	using diagonal_vector_type = impl_matrix::_vector_reference<impl_matrix::_matrix_stride_iterator<iterator>, impl_matrix::_matrix_stride_iterator<const_iterator>, dimension, 1, options>;
	using const_diagonal_vector_type = impl_matrix::_vector_reference<impl_matrix::_matrix_stride_iterator<const_iterator>, impl_matrix::_matrix_stride_iterator<const_iterator>, dimension, 1, options>;


	constexpr
	size_type
	order() const
	{
		return this->rows();
	}

	constexpr
	diagonal_vector_type
	diagonal()
	{
		auto it = impl_matrix::_matrix_stride_iterator<iterator>(this->begin(), this->order() + 1, 0);
		return diagonal_vector_type(it, this->order());
	}

	constexpr
	const_diagonal_vector_type
	diagonal() const
	{
		auto it = impl_matrix::_matrix_stride_iterator<const_iterator>(this->begin(), this->order() + 1, 0);
		return const_diagonal_vector_type(it, this->order());
	}

	constexpr
	value_type
	trace() const
	{
		value_type ret = 0;
		const auto ord = this->order();
		for (size_type i = 0; i < ord; ++i) {
			ret += this->value(i, i);
		}

		return ret;
	}

	constexpr
	bool
	is_identity() const
	{
		constexpr value_type diag_value = value_type(1);
		constexpr value_type off_diag_value = value_type(0);
		const auto ord = this->order();

		for (size_type outer = 0; outer < ord; ++outer) {
			for (size_type inner = 0; inner < ord; ++inner) {
				if constexpr (_is_column_major) {
					if (this->value(inner, outer) != (outer == inner) ? diag_value : off_diag_value)
						return false;
				} else {
					if (this->value(outer, inner) != (outer == inner ? diag_value : off_diag_value))
						return false;
				}
			}
		}

		return true;
	}

	constexpr
	void
	to_identity()
	{
		constexpr value_type diag_value = value_type(1);
		constexpr value_type off_diag_value = value_type(0);
		const auto ord = this->order();

		for (size_type outer = 0; outer < ord; ++outer) {
			for (size_type inner = 0; inner < ord; ++inner) {
				if constexpr (_is_column_major) {
					this->value(inner, outer) = (outer == inner ? diag_value : off_diag_value);
				} else {
					this->value(outer, inner) = (outer == inner ? diag_value : off_diag_value);
				}
			}
		}
	}

	constexpr
	void
	transpose()
	{
		const auto ord = this->order();
		for (size_type row = 1; row < ord; ++row) {
			for (size_type col = 0; col < row; ++col) {
				mtk::_swap(this->value(row, col), this->value(col, row));
			}
		}
	}
};



template<class Derived
	,class Base>
class _matrix_detinv_base :
	public Base
{
public:
	using typename Base::value_type;
	using Base::dimension;
	using Base::options;

	constexpr
	value_type
	determinant() const
	{
		if constexpr (dimension == 2) {
			return (this->value(0, 0)*this->value(1, 1) - this->value(0, 1)*this->value(1, 0));

		} else if constexpr (dimension == 3) {
			return (
				this->value(0, 0)*(
					this->value(1, 1)*this->value(2, 2) - this->value(1, 2)*this->value(2, 1))
				+ this->value(0, 1)*(
					this->value(1, 2)*this->value(2, 0) - this->value(1, 0)*this->value(2, 2))
				+ this->value(0, 2)*(
					this->value(1, 0)*this->value(2, 1) - this->value(1, 1)*this->value(2, 0)));

		} else if constexpr (dimension == 4) {
			return (
				this->value(0, 0)*(
					+ this->value(1, 1)*(
						this->value(2, 2)*this->value(3, 3) - this->value(2, 3)*this->value(3, 2))
					+ this->value(1, 2)*(
						this->value(2, 3)*this->value(3, 1) - this->value(2, 1)*this->value(3, 3))
					+ this->value(1, 3)*(
						this->value(2, 1)*this->value(3, 2) - this->value(2, 2)*this->value(3, 1)))
				+ this->value(0, 1)*(
					+ this->value(1, 0)*(
						this->value(2, 3)*this->value(3, 2) - this->value(2, 2)*this->value(3, 3))
					+ this->value(1, 2)*(
						this->value(2, 0)*this->value(3, 3) - this->value(2, 3)*this->value(3, 0))
					+ this->value(1, 3)*(
						this->value(2, 2)*this->value(3, 0) - this->value(2, 0)*this->value(3, 2)))
				+ this->value(0, 2)*(
					+ this->value(1, 0)*(
						this->value(2, 1)*this->value(3, 3) - this->value(2, 3)*this->value(3, 1))
					+ this->value(1, 1)*(
						this->value(2, 3)*this->value(3, 0) - this->value(2, 0)*this->value(3, 3))
					+ this->value(1, 3)*(
						this->value(2, 0)*this->value(3, 1) - this->value(2, 1)*this->value(3, 0)))
				+ this->value(0, 3)*(
					+ this->value(1, 0)*(
						this->value(2, 2)*this->value(3, 1) - this->value(2, 1)*this->value(3, 2))
					+ this->value(1, 1)*(
						this->value(2, 0)*this->value(3, 2) - this->value(2, 2)*this->value(3, 0))
					+ this->value(1, 2)*(
						this->value(2, 1)*this->value(3, 0) - this->value(2, 0)*this->value(3, 1))));

		} else {
			return value_type();
		}
	}

	constexpr
	bool
	is_invertible() const
	{
		const auto det = this->determinant();
		const auto det_pos = (det < value_type() ? -det : det);
		return (det_pos > std::numeric_limits<value_type>::epsilon());
	}


	constexpr
	auto
	inverted() const
	{
		using val_type = std::conditional_t<std::is_integral_v<value_type>, double, value_type>;
		using ret_mat = typename _linalg_traits<Derived>::template matrix_type<value_type, dimension, dimension, options>;

		if constexpr (dimension == 2) {
			const auto det = this->determinant();
			const auto pos_det = (det < val_type() ? -det : det);
			if (pos_det <=  std::numeric_limits<value_type>::epsilon())
				return std::optional<ret_mat>();

			const auto det_inv = value_type(1) / det;
			const auto m11 =  static_cast<val_type>(this->value(1, 1))*det_inv;
			const auto m12 = -static_cast<val_type>(this->value(0, 1))*det_inv;
			const auto m21 = -static_cast<val_type>(this->value(1, 0))*det_inv;
			const auto m22 =  static_cast<val_type>(this->value(0, 0))*det_inv;
			return std::optional<ret_mat>(ret_mat{m11, m12, m21, m22});

		} else if constexpr (dimension == 3) {
			const auto c0 = static_cast<value_type>(this->value(1, 0)*this->value(2, 1) - this->value(1, 1)*this->value(2, 0));
			const auto c1 = static_cast<value_type>(this->value(1, 1)*this->value(2, 2) - this->value(1, 2)*this->value(2, 1));
			const auto c2 = static_cast<value_type>(this->value(1, 2)*this->value(2, 0) - this->value(1, 0)*this->value(2, 2));

			const auto det = this->value(0, 0)*c1 + this->value(0, 1)*c2 + this->value(0, 2)*c0;
			const auto pos_det = (det < val_type() ? -det : det);
			if (pos_det <=  std::numeric_limits<value_type>::epsilon())
				return std::optional<ret_mat>();

			const auto det_inv = value_type(1) / det;
			const auto m11 = c1*det_inv;
			const auto m12 = -static_cast<value_type>(this->value(0, 1)*this->value(2, 2) - this->value(0, 2)*this->value(2, 1))*det_inv;
			const auto m13 = static_cast<value_type>(this->value(0, 1)*this->value(1, 2) - this->value(0, 2)*this->value(1, 1))*det_inv;
			const auto m21 = c2*det_inv;
			const auto m22 = static_cast<value_type>(this->value(0, 0)*this->value(2, 2) - this->value(0, 2)*this->value(2, 0))*det_inv;
			const auto m23 = -static_cast<value_type>(this->value(0, 0)*this->value(1, 2) - this->value(0, 2)*this->value(1, 0))*det_inv;
			const auto m31 = c0*det_inv;
			const auto m32 = -static_cast<value_type>(this->value(0, 0)*this->value(2, 1) - this->value(0, 1)*this->value(2, 0))*det_inv;
			const auto m33 = static_cast<value_type>(this->value(0, 0)*this->value(1, 1) - this->value(0, 1)*this->value(1, 0))*det_inv;
			return std::optional<ret_mat>(ret_mat{m11, m12, m13, m21, m22, m23, m31, m32, m33});

		} else if constexpr (dimension == 4) {
			const auto s0 = static_cast<value_type>(this->value(0, 0)*this->value(1, 1) - this->value(1, 0)*this->value(0, 1));
			const auto s1 = static_cast<value_type>(this->value(0, 0)*this->value(1, 2) - this->value(1, 0)*this->value(0, 2));
			const auto s2 = static_cast<value_type>(this->value(0, 0)*this->value(1, 3) - this->value(1, 0)*this->value(0, 3));
			const auto s3 = static_cast<value_type>(this->value(0, 1)*this->value(1, 2) - this->value(1, 1)*this->value(0, 2));
			const auto s4 = static_cast<value_type>(this->value(0, 1)*this->value(1, 3) - this->value(1, 1)*this->value(0, 3));
			const auto s5 = static_cast<value_type>(this->value(0, 2)*this->value(1, 3) - this->value(1, 2)*this->value(0, 3));

			const auto c0 = static_cast<value_type>(this->value(2, 0)*this->value(3, 1) - this->value(3, 0)*this->value(2, 1));
			const auto c1 = static_cast<value_type>(this->value(2, 0)*this->value(3, 2) - this->value(3, 0)*this->value(2, 2));
			const auto c2 = static_cast<value_type>(this->value(2, 0)*this->value(3, 3) - this->value(3, 0)*this->value(2, 3));
			const auto c3 = static_cast<value_type>(this->value(2, 1)*this->value(3, 2) - this->value(3, 1)*this->value(2, 2));
			const auto c4 = static_cast<value_type>(this->value(2, 1)*this->value(3, 3) - this->value(3, 1)*this->value(2, 3));
			const auto c5 = static_cast<value_type>(this->value(2, 2)*this->value(3, 3) - this->value(3, 2)*this->value(2, 3));

			const auto det = s0*c5 - s1*c4 + s2*c3 + s3*c2 - s4*c1 + s5*c0;
			const auto pos_det = (det < val_type() ? -det : det);
			if (pos_det <=  std::numeric_limits<value_type>::epsilon())
				return std::optional<ret_mat>();

			const auto det_inv = value_type(1) / det;
			const auto m11 = ( this->value(1, 1)*c5 - this->value(1, 2)*c4 + this->value(1, 3)*c3)*det_inv;
			const auto m12 = (-this->value(0, 1)*c5 + this->value(0, 2)*c4 - this->value(0, 3)*c3)*det_inv;
			const auto m13 = ( this->value(3, 1)*s5 - this->value(3, 2)*s4 + this->value(3, 3)*s3)*det_inv;
			const auto m14 = (-this->value(2, 1)*s5 + this->value(2, 2)*s4 - this->value(2, 3)*s3)*det_inv;
			const auto m21 = (-this->value(1, 0)*c5 + this->value(1, 2)*c2 - this->value(1, 3)*c1)*det_inv;
			const auto m22 = ( this->value(0, 0)*c5 - this->value(0, 2)*c2 + this->value(0, 3)*c1)*det_inv;
			const auto m23 = (-this->value(3, 0)*s5 + this->value(3, 2)*s2 - this->value(3, 3)*s1)*det_inv;
			const auto m24 = ( this->value(2, 0)*s5 - this->value(2, 2)*s2 + this->value(2, 3)*s1)*det_inv;
			const auto m31 = ( this->value(1, 0)*c4 - this->value(1, 1)*c2 + this->value(1, 3)*c0)*det_inv;
			const auto m32 = (-this->value(0, 0)*c4 + this->value(0, 1)*c2 - this->value(0, 3)*c0)*det_inv;
			const auto m33 = ( this->value(3, 0)*s4 - this->value(3, 1)*s2 + this->value(3, 3)*s0)*det_inv;
			const auto m34 = (-this->value(2, 0)*s4 + this->value(2, 1)*s2 - this->value(2, 3)*s0)*det_inv;
			const auto m41 = (-this->value(1, 0)*c3 + this->value(1, 1)*c1 - this->value(1, 2)*c0)*det_inv;
			const auto m42 = ( this->value(0, 0)*c3 - this->value(0, 1)*c1 + this->value(0, 2)*c0)*det_inv;
			const auto m43 = (-this->value(3, 0)*s3 + this->value(3, 1)*s1 - this->value(3, 2)*s0)*det_inv;
			const auto m44 = ( this->value(2, 0)*s3 - this->value(2, 1)*s1 + this->value(2, 2)*s0)*det_inv;
			return std::optional<ret_mat>(ret_mat{m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44});
		} else {
			return std::optional<ret_mat>();
		}
	}
};



template<class Derived
	,class Base>
class _matrix_detinv_fp_base :
	public Base
{
public:
	constexpr
	void
	invert()
	{
		auto inv = this->inverted();
		if (inv)
			static_cast<Derived&>(*this) = mtk::_move(*inv);
		else
			this->to_identity();
	}
};



template<class Derived
	,class Base>
class _matrix_vector_base :
	public Base
{
public:
	using typename Base::value_type;
	using typename Base::size_type;
	using norm_type = std::conditional_t<std::is_floating_point_v<value_type>, value_type, double>;

	using Base::row_dimension;
	using Base::column_dimension;
	using Base::options;

	norm_type
	norm() const
	{
		return std::sqrt(this->norm_squared());
	}

	constexpr
	norm_type
	norm_squared() const
	{
		return this->dot(*this);
	}

	template<class Other
		,_require<_is_matrix_compatible<Derived, Other>::value> = 0>
	constexpr
	value_type
	dot(const _matrix_base<Other>& other) const
	{
		MTK_ASSERT(this->size() == other.size());

		value_type sum = {};
		const size_type sz = this->size();
		for (size_type i = 0; i < sz; ++i) {
			sum += this->value(i)*other.value(i);
		}

		return sum;
	}

	auto
	normalized() const
	{
		using ret_type = typename _linalg_traits<Derived>::template matrix_type<norm_type, row_dimension, column_dimension, options>;
		ret_type ret(*this);
		return ret /= this->norm();
	}
};



template<class Derived
	,class Base>
class _matrix_vector_normalize_base :
	public Base
{
public:
	void
	normalize()
	{
		(*this) /= this->norm();
	}
};



template<class Derived
	,class Base>
class _matrix_vector_cross_base :
	public Base
{
public:
	using typename Base::value_type;

	using Base::row_dimension;
	using Base::column_dimension;
	using Base::options;

	template<class Other
#ifndef MTK_DOXYGEN
		,_require<_is_matrix_compatible<Derived, Other>::value> = 0
#endif
	>
	constexpr
	auto
	cross(const _matrix_base<Other>& other) const
	{
		using ret_type1 = typename _linalg_traits<Derived>::template matrix_type<value_type, row_dimension, column_dimension, options | Other::options>;
		using ret_type2 = typename _linalg_traits<Other>::template matrix_type<value_type, row_dimension, column_dimension, options | Other::options>;
		using mat_type = matrix<value_type, row_dimension, column_dimension, options | Other::options>;
		using ret_type = std::conditional_t<std::is_same_v<ret_type1, mat_type>, ret_type2, ret_type1>;
		return ret_type(
			this->value(1)*other.value(2) - this->value(2)*other.value(1),
			this->value(2)*other.value(0) - this->value(0)*other.value(2),
			this->value(0)*other.value(1) - this->value(1)*other.value(0)
		);
	}
};



template<class Derived
	,class Base>
class _matrix_vector2_accessors :
	public Base
{
public:
	using typename Base::reference;
	using typename Base::const_reference;

	constexpr
	reference
	x()
	{
		return this->value(0);
	}

	constexpr
	const_reference
	x() const
	{
		return this->value(0);
	}

	constexpr
	reference
	y()
	{
		return this->value(1);
	}

	constexpr
	const_reference
	y() const
	{
		return this->value(1);
	}
};

template<class Derived
	,class Base>
class _matrix_vector3_accessors :
	public Base
{
public:
	using typename Base::reference;
	using typename Base::const_reference;

	constexpr
	reference
	z()
	{
		return this->value(2);
	}

	constexpr
	const_reference
	z() const
	{
		return this->value(2);
	}
};

template<class Derived
	,class Base>
class _matrix_vector4_accessors :
	public Base
{
public:
	using typename Base::reference;
	using typename Base::const_reference;

	constexpr
	reference
	w()
	{
		return this->value(3);
	}

	constexpr
	const_reference
	w() const
	{
		return this->value(3);
	}
};



template<class Derived>
struct _matrix_base_selector
{
	using value_type = typename _linalg_traits<Derived>::value_type;
	using iterator = typename _linalg_traits<Derived>::iterator;
	static constexpr auto row_dim = _linalg_traits<Derived>::row_dimension;
	static constexpr auto col_dim = _linalg_traits<Derived>::column_dimension;

	static constexpr bool is_continuous = std::is_pointer_v<iterator>;
	static constexpr bool is_square = (row_dim == col_dim) && (row_dim != 1) && (row_dim != dynamic_extent);
	static constexpr bool has_det = (row_dim >= 2) && (row_dim <= 4);
	static constexpr bool is_vector = ((row_dim == 1) || (col_dim == 1));
	static constexpr bool is_vec3 = (row_dim*col_dim == 3);
	static constexpr bool has_xy = ((row_dim*col_dim >= 2) && (row_dim*col_dim <= 4));
	static constexpr bool has_z = ((row_dim*col_dim >= 3) && (row_dim*col_dim <= 4));
	static constexpr bool has_w = (row_dim*col_dim == 4);

	using base1 = _matrix_base<Derived>;
	using base2 = std::conditional_t<is_continuous,
		_matrix_data_base<Derived, base1>,
		base1>;
	using base3 = std::conditional_t<is_square,
		_matrix_square_base<Derived, base2>,
		base2>;
	using base4 = std::conditional_t<is_square && has_det,
		_matrix_detinv_base<Derived, base3>,
		base3>;
	using base5 = std::conditional_t<is_square && has_det && std::is_floating_point_v<value_type>,
		_matrix_detinv_fp_base<Derived, base4>,
		base4>;
	using base6 = std::conditional_t<is_vector,
		_matrix_vector_base<Derived, base5>,
		base5>;
	using base7 = std::conditional_t<is_vector && std::is_floating_point_v<value_type>,
		_matrix_vector_normalize_base<Derived, base6>,
		base6>;
	using base8 = std::conditional_t<is_vector && is_vec3,
		_matrix_vector_cross_base<Derived, base7>,
		base7>;
	using base9 = std::conditional_t<is_vector && has_xy,
		_matrix_vector2_accessors<Derived, base8>,
		base8>;
	using base10 = std::conditional_t<is_vector && has_z,
		_matrix_vector3_accessors<Derived, base9>,
		base9>;
	using base11 = std::conditional_t<is_vector && has_w,
		_matrix_vector4_accessors<Derived, base10>,
		base10>;

	using type = base11;
};



namespace impl_matrix {

template<class Iter>
class _matrix_stride_iterator
{
public:
	using value_type = iter::value_type<Iter>;
	using reference = iter::reference<Iter>;
	using pointer = iter::pointer<Iter>;
	using difference_type = iter::difference_type<Iter>;
	using iterator_category = std::random_access_iterator_tag;

	using _idx_type = std::make_unsigned_t<difference_type>;

	constexpr
	_matrix_stride_iterator() = default;

	_matrix_stride_iterator(Iter iter, _idx_type stride, _idx_type idx) :
		m_iter(iter),
		m_stride(stride),
		m_idx(idx)
	{ }

	template<class OtherIt
		,_require<std::is_convertible_v<OtherIt, Iter>> = 0>
	_matrix_stride_iterator(const _matrix_stride_iterator<OtherIt>& other) :
		m_iter(other.m_iter),
		m_stride(other.m_stride),
		m_idx(other.m_idx)
	{ }

	constexpr
	reference
	operator*() const
	{
		return *(m_iter + m_stride*m_idx);
	}

	constexpr
	Iter
	operator->() const
	{
		auto ret_iter = m_iter + m_stride*m_idx;
		return ret_iter;
	}

	constexpr
	reference
	operator[](difference_type idx) const
	{
		return *(*this + idx);
	}

	friend constexpr
	_matrix_stride_iterator&
	operator++(_matrix_stride_iterator& rhs)
	{
		++rhs.m_idx;
		return rhs;
	}

	friend constexpr
	_matrix_stride_iterator
	operator++(_matrix_stride_iterator& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend constexpr
	_matrix_stride_iterator&
	operator--(_matrix_stride_iterator& rhs)
	{
		--rhs.m_idx;
		return rhs;
	}

	friend constexpr
	_matrix_stride_iterator
	operator--(_matrix_stride_iterator& lhs, int)
	{
		auto cp = lhs;
		--lhs;
		return cp;
	}

	friend constexpr
	_matrix_stride_iterator&
	operator+=(_matrix_stride_iterator& lhs, difference_type rhs)
	{
		lhs.m_idx += rhs;
		return lhs;
	}

	friend constexpr
	_matrix_stride_iterator&
	operator-=(_matrix_stride_iterator& lhs, difference_type rhs)
	{
		lhs.m_idx -= rhs;
		return lhs;
	}

	friend constexpr
	_matrix_stride_iterator
	operator+(_matrix_stride_iterator lhs, difference_type rhs)
	{
		return (lhs += rhs);
	}

	friend constexpr
	_matrix_stride_iterator
	operator+(difference_type lhs, _matrix_stride_iterator rhs)
	{
		return (rhs += lhs);
	}

	friend constexpr
	_matrix_stride_iterator
	operator-(_matrix_stride_iterator lhs, difference_type rhs)
	{
		return (lhs -= rhs);
	}

	friend constexpr
	difference_type
	operator-(const _matrix_stride_iterator& lhs, const _matrix_stride_iterator& rhs)
	{
		return difference_type(lhs.m_idx) - difference_type(rhs.m_idx);
	}

	friend constexpr
	bool
	operator==(const _matrix_stride_iterator& lhs, const _matrix_stride_iterator& rhs)
	{
		return (lhs.m_idx == rhs.m_idx);
	}

	friend constexpr
	bool
	operator!=(const _matrix_stride_iterator& lhs, const _matrix_stride_iterator& rhs)
	{
		return (lhs.m_idx != rhs.m_idx);
	}

	friend constexpr
	bool
	operator<(const _matrix_stride_iterator& lhs, const _matrix_stride_iterator& rhs)
	{
		return (lhs.m_idx < rhs.m_idx);
	}

	friend constexpr
	bool
	operator>(const _matrix_stride_iterator& lhs, const _matrix_stride_iterator& rhs)
	{
		return (lhs.m_idx > rhs.m_idx);
	}

	friend constexpr
	bool
	operator<=(const _matrix_stride_iterator& lhs, const _matrix_stride_iterator& rhs)
	{
		return (lhs.m_idx <= rhs.m_idx);
	}

	friend constexpr
	bool
	operator>=(const _matrix_stride_iterator& lhs, const _matrix_stride_iterator& rhs)
	{
		return (lhs.m_idx >= rhs.m_idx);
	}

private:
	template<class OtherIter>
	friend class _matrix_stride_iterator;

	Iter m_iter;
	_idx_type m_stride;
	_idx_type m_idx;
};

template<class Iter
	,class ConstIter
	,size_t R
	,size_t C
	,matrix_options Opt
	,bool IsConst>
class _vector_reference :
	public _matrix_base_selector<_vector_reference<Iter, ConstIter, R, C, Opt, IsConst>>::type
{
	static_assert(R == 1 || C == 1);
public:

	constexpr
	_vector_reference(Iter iter, size_t size) :
		m_iter(iter),
		m_size(size)
	{ }

	auto operator=(const _vector_reference&) = delete;

private:
	friend class _linalg_traits<_vector_reference>;
	Iter m_iter;
	size_t m_size;
};

template<class Iter
	,class ConstIter
	,size_t R
	,size_t C
	,matrix_options Opt>
class _vector_reference<Iter, ConstIter, R, C, Opt, false> :
	public _matrix_base_selector<_vector_reference<Iter, ConstIter, R, C, Opt, false>>::type
{
	static_assert(R == 1 || C == 1);
public:

	constexpr
	_vector_reference(Iter iter, size_t size) :
		m_iter(iter),
		m_size(size)
	{ }

	constexpr
	_vector_reference&
	operator=(const _vector_reference& other)
	{
		this->_assign(other.begin());
		return *this;
	}

	template<class Other
		,_require<_is_matrix_compatible<_vector_reference, Other>::value> = 0>
	constexpr
	_vector_reference&
	operator=(const _matrix_base<Other>& other)
	{
		this->_assign(other.begin());
		return *this;
	}

private:
	friend class _linalg_traits<_vector_reference>;
	Iter m_iter;
	size_t m_size;
};

template<class Iter
	,class ConstIter
	,size_t R
	,size_t C
	,matrix_options Opt
	,bool IsRow>
class _matrix_vector_iterator
{
public:
	using value_type = _vector_reference<_matrix_stride_iterator<Iter>, _matrix_stride_iterator<ConstIter>, (IsRow ? 1 : R), (IsRow ? C : 1), Opt>;
	using reference = value_type;
	using pointer = struct {
		value_type value;
		constexpr
		const value_type*
		operator->() const { return &this->value; }
		constexpr
		value_type*
		operator->() { return &this->value; }
	};
	using difference_type = ptrdiff_t;
	using iterator_category = std::input_iterator_tag;

	static constexpr
	bool
	_is_column_major = ((Opt & matrix_options::column_major) == matrix_options::column_major);

	constexpr
	_matrix_vector_iterator() = default;

	constexpr
	_matrix_vector_iterator(Iter iter, size_t rows, size_t cols, size_t idx) :
		m_iter(iter),
		m_rows(rows),
		m_cols(cols),
		m_idx(idx)
	{ }

	template<class OtherIter
		,_require<std::is_convertible_v<OtherIter, Iter>> = 0>
	_matrix_vector_iterator(const _matrix_vector_iterator<OtherIter, ConstIter, R, C, Opt, IsRow>& other) :
		m_iter(other.m_iter),
		m_rows(other.m_rows),
		m_cols(other.m_cols),
		m_idx(other.m_idx)
	{ }

	constexpr
	reference
	operator*() const
	{
		if constexpr (IsRow)
			return this->_get_row();
		else
			return this->_get_col();
	}

	constexpr
	pointer
	operator->() const
	{
		return pointer{**this};
	}

	constexpr
	reference
	operator[](difference_type idx) const
	{
		return *(*this + idx);
	}

	friend constexpr
	_matrix_vector_iterator&
	operator++(_matrix_vector_iterator& rhs)
	{
		++rhs.m_idx;
		return rhs;
	}

	friend constexpr
	_matrix_vector_iterator
	operator++(_matrix_vector_iterator& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend constexpr
	_matrix_vector_iterator&
	operator--(_matrix_vector_iterator& rhs)
	{
		--rhs.m_idx;
		return rhs;
	}

	friend constexpr
	_matrix_vector_iterator
	operator--(_matrix_vector_iterator& lhs, int)
	{
		auto cp = lhs;
		--lhs;
		return cp;
	}

	friend constexpr
	_matrix_vector_iterator&
	operator+=(_matrix_vector_iterator& lhs, difference_type rhs)
	{
		lhs.m_idx += rhs;
		return lhs;
	}

	friend constexpr
	_matrix_vector_iterator&
	operator-=(_matrix_vector_iterator& lhs, difference_type rhs)
	{
		lhs.m_idx -= rhs;
		return lhs;
	}

	friend constexpr
	_matrix_vector_iterator
	operator+(_matrix_vector_iterator lhs, difference_type rhs)
	{
		return (lhs += rhs);
	}

	friend constexpr
	_matrix_vector_iterator
	operator+(difference_type lhs, _matrix_vector_iterator rhs)
	{
		return (rhs += lhs);
	}

	friend constexpr
	_matrix_vector_iterator
	operator-(_matrix_vector_iterator lhs, difference_type rhs)
	{
		return (lhs -= rhs);
	}

	friend constexpr
	difference_type
	operator-(_matrix_vector_iterator lhs, _matrix_vector_iterator rhs)
	{
		return difference_type(lhs.m_idx) - difference_type(rhs.m_idx);
	}

	friend constexpr
	bool
	operator==(const _matrix_vector_iterator& lhs, const _matrix_vector_iterator& rhs)
	{
		return (lhs.m_idx == rhs.m_idx);
	}

	friend constexpr
	bool
	operator!=(const _matrix_vector_iterator& lhs, const _matrix_vector_iterator& rhs)
	{
		return (lhs.m_idx != rhs.m_idx);
	}

	friend constexpr
	bool
	operator<(const _matrix_vector_iterator& lhs, const _matrix_vector_iterator& rhs)
	{
		return (lhs.m_idx < rhs.m_idx);
	}

	friend constexpr
	bool
	operator>(const _matrix_vector_iterator& lhs, const _matrix_vector_iterator& rhs)
	{
		return (lhs.m_idx > rhs.m_idx);
	}

	friend constexpr
	bool
	operator<=(const _matrix_vector_iterator& lhs, const _matrix_vector_iterator& rhs)
	{
		return (lhs.m_idx <= rhs.m_idx);
	}

	friend constexpr
	bool
	operator>=(const _matrix_vector_iterator& lhs, const _matrix_vector_iterator& rhs)
	{
		return (lhs.m_idx >= rhs.m_idx);
	}

private:
	constexpr
	auto
	_rows() const
	{
		if constexpr (R == dynamic_extent)
			return m_rows;
		else
			return R;
	}

	constexpr
	auto
	_cols() const
	{
		if constexpr (C == dynamic_extent)
			return m_cols;
		else
			return C;
	}

	constexpr
	reference
	_get_row() const
	{
		if constexpr (_is_column_major) {
			auto first = m_iter + m_idx;
			auto it = _matrix_stride_iterator<Iter>(first, this->_rows(), 0);
			return reference(it, this->_cols());
		} else {
			auto first = m_iter + m_idx*this->_cols();
			auto it = _matrix_stride_iterator<Iter>(first, 1, 0);
			return reference(it, this->_cols());
		}
	}

	constexpr
	reference
	_get_col() const
	{
		if constexpr (_is_column_major) {
			auto first = m_iter + m_idx*this->_rows();
			auto it = _matrix_stride_iterator<Iter>(first, 1, 0);
			return reference(it, this->_rows());
		} else {
			auto first = m_iter + m_idx;
			auto it = _matrix_stride_iterator<Iter>(first, this->_cols(), 0);
			return reference(it, this->_rows());
		}
	}

	template<class OtherIter
		,class OtherConstIter
		,size_t OtherR
		,size_t OtherC
		,matrix_options OtherOpt
		,bool OtherIsR>
	friend class _matrix_vector_iterator;

	Iter m_iter;
	size_t m_rows;
	size_t m_cols;
	size_t m_idx;
};

} // namespace impl_matrix



template<class MatA
	,class MatB
#ifndef MTK_DOXYGEN
	,_require<std::is_base_of_v<_matrix_base<std::decay_t<MatA>>, std::decay_t<MatA>>> = 0
	,_require<std::is_base_of_v<_matrix_base<std::decay_t<MatB>>, std::decay_t<MatB>>> = 0
	,_require<_is_matrix_compatible<MatA, MatB>::value> = 0
	,decltype(mtk::_declval<MatA>().swap(mtk::_declval<MatB>()))* = nullptr
#endif
>
constexpr
void
swap(MatA&& a, MatB&& b)
{
	a.swap(b);
}



template<class Mat>
constexpr
auto
operator+(const _matrix_base<Mat>& rhs)
{
	using value_type = typename Mat::value_type;
	constexpr auto row_dim = Mat::row_dimension;
	constexpr auto col_dim = Mat::column_dimension;
	constexpr auto opt = Mat::options;
	using ret_type = typename _linalg_traits<Mat>::template matrix_type<value_type, row_dim, col_dim, opt>;
	return ret_type(rhs);
}

template<class Mat>
constexpr
auto
operator-(const _matrix_base<Mat>& rhs)
{
	using value_type = typename Mat::value_type;
	constexpr auto row_dim = Mat::row_dimension;
	constexpr auto col_dim = Mat::column_dimension;
	constexpr auto opt = Mat::options;
	using ret_type = typename _linalg_traits<Mat>::template matrix_type<value_type, row_dim, col_dim, opt>;
	ret_type ret(rhs);
	for (auto& el : ret)
		el = -el;

	return ret;
}

template<class MatA
	,class MatB
#ifndef MTK_DOXYGEN
	,_require<_is_matrix_compatible<MatA, MatB>::value> = 0
#endif
>
constexpr
MatA&
operator+=(_matrix_base<MatA>& lhs, const _matrix_base<MatB>& rhs)
{
	MTK_ASSERT(lhs.rows() == rhs.rows());
	MTK_ASSERT(lhs.columns() == rhs.columns());

	const auto rows = lhs.rows();
	const auto cols = lhs.columns();
	for (std::decay_t<decltype(rows)> row = 0; row < rows; ++row) {
		for (std::decay_t<decltype(cols)> col = 0; col < cols; ++col) {
			lhs.value(row, col) += rhs.value(row, col);
		}
	}
	return static_cast<MatA&>(lhs);
}

template<class MatA
	,class MatB
#ifndef MTK_DOXYGEN
	,_require<_is_matrix_compatible<MatA, MatB>::value> = 0
#endif
>
constexpr
MatA&
operator-=(_matrix_base<MatA>& lhs, const _matrix_base<MatB>& rhs)
{
	MTK_ASSERT(lhs.rows() == rhs.rows());
	MTK_ASSERT(lhs.columns() == rhs.columns());

	const auto rows = lhs.rows();
	const auto cols = lhs.columns();
	for (std::decay_t<decltype(rows)> row = 0; row < rows; ++row) {
		for (std::decay_t<decltype(cols)> col = 0; col < cols; ++col) {
			lhs.value(row, col) -= rhs.value(row, col);
		}
	}
	return static_cast<MatA&>(lhs);
}

template<class MatA
	,class MatB
#ifndef MTK_DOXYGEN
	,_require<_is_matrix_compatible<MatA, MatB>::value> = 0
#endif
>
constexpr
auto
operator+(const _matrix_base<MatA>& lhs, const _matrix_base<MatB>& rhs)
{
	using value_type = typename MatA::value_type;
	constexpr auto row_dim = MatA::row_dimension;
	constexpr auto col_dim = MatA::column_dimension;
	constexpr auto opt = MatA::options | MatB::options;
	using ret_type1 = typename _linalg_traits<MatA>::template matrix_type<value_type, row_dim, col_dim, opt>;
	using ret_type2 = typename _linalg_traits<MatB>::template matrix_type<value_type, row_dim, col_dim, opt>;
	using mat_type = matrix<value_type, row_dim, col_dim, opt>;
	using ret_type = std::conditional_t<std::is_same_v<ret_type1, mat_type>, ret_type2, ret_type1>;
	ret_type ret(lhs);
	return (ret += rhs);
}

template<class MatA
	,class MatB
#ifndef MTK_DOXYGEN
	,_require<_is_matrix_compatible<MatA, MatB>::value> = 0
#endif
>
constexpr
auto
operator-(const _matrix_base<MatA>& lhs, const _matrix_base<MatB>& rhs)
{
	using value_type = typename MatA::value_type;
	constexpr auto row_dim = MatA::row_dimension;
	constexpr auto col_dim = MatA::column_dimension;
	constexpr auto opt = MatA::options | MatB::options;
	using ret_type1 = typename _linalg_traits<MatA>::template matrix_type<value_type, row_dim, col_dim, opt>;
	using ret_type2 = typename _linalg_traits<MatB>::template matrix_type<value_type, row_dim, col_dim, opt>;
	using mat_type = matrix<value_type, row_dim, col_dim, opt>;
	using ret_type = std::conditional_t<std::is_same_v<ret_type1, mat_type>, ret_type2, ret_type1>;
	ret_type ret(lhs);
	return (ret -= rhs);
}

template<class Mat>
constexpr
Mat&
operator*=(_matrix_base<Mat>& lhs, typename Mat::value_type rhs)
{
	for (auto& el : lhs) {
		el *= rhs;
	}
	return static_cast<Mat&>(lhs);
}

template<class Mat>
constexpr
Mat&
operator/=(_matrix_base<Mat>& lhs, typename Mat::value_type rhs)
{
	for (auto& el : lhs) {
		el /= rhs;
	}
	return static_cast<Mat&>(lhs);
}

template<class Mat>
constexpr
auto
operator*(const _matrix_base<Mat>& lhs, typename Mat::value_type rhs)
{
	using ret_type = typename _linalg_traits<Mat>::template matrix_type<
		typename Mat::value_type,
		Mat::row_dimension,
		Mat::column_dimension,
		Mat::options>;
	ret_type ret(lhs);
	return (ret *= rhs);
}

template<class Mat>
constexpr
auto
operator*(typename Mat::value_type lhs, const _matrix_base<Mat>& rhs)
{
	using ret_type = typename _linalg_traits<Mat>::template matrix_type<
		typename Mat::value_type,
		Mat::row_dimension,
		Mat::column_dimension,
		Mat::options>;
	ret_type ret(rhs);
	return (rhs *= lhs);
}

template<class Mat>
constexpr
auto
operator/(const _matrix_base<Mat>& lhs, typename Mat::value_type rhs)
{
	using ret_type = typename _linalg_traits<Mat>::template matrix_type<
		typename Mat::value_type,
		Mat::row_dimension,
		Mat::column_dimension,
		Mat::options>;
	ret_type ret(lhs);
	return (ret /= rhs);
}

template<class MatA
	,class MatB
	,_require<std::is_same_v<typename MatA::value_type, typename MatB::value_type>> = 0
	,_require<(MatA::column_dimension == MatB::row_dimension)> = 0>
constexpr
auto
operator*(const _matrix_base<MatA>& lhs, const _matrix_base<MatB>& rhs)
{
	MTK_ASSERT(lhs.columns() == rhs.rows());

	using value_type = typename MatA::value_type;
	constexpr auto row_dim = MatA::row_dimension;
	constexpr auto col_dim = MatB::column_dimension;
	constexpr auto opt = MatA::options | MatB::options;
	using ret_type1 = typename _linalg_traits<MatA>::template matrix_type<value_type, row_dim, col_dim, opt>;
	using ret_type2 = typename _linalg_traits<MatB>::template matrix_type<value_type, row_dim, col_dim, opt>;
	using mat_type = matrix<value_type, row_dim, col_dim, opt>;
	using ret_type = std::conditional_t<std::is_same_v<ret_type1, mat_type>, ret_type2, ret_type1>;

	const auto rows = lhs.rows();
	const auto cols = rhs.columns();
	auto ret = mtk::_make_matrix<ret_type>(rows, cols);
	for (size_t row = 0; row < rows; ++row) {
		const auto row_vec = lhs.row(row);
		for (size_t col = 0; col < cols; ++col) {
			ret.value(row, col) = row_vec.dot(rhs.column(col));
		}
	}

	return ret;
}

template<class MatA
	,class MatB
	,_require<std::is_same_v<typename MatA::value_type, typename MatB::value_type>> = 0
	,_require<(MatA::column_dimension == MatB::dimension)> = 0>
constexpr
MatA&
operator*=(_matrix_base<MatA>& lhs, const _matrix_base<MatB>& rhs)
{
	using value_type = typename MatA::value_type;
	constexpr auto col_dim = MatA::column_dimension;
	constexpr auto opt = MatA::options | MatB::options;
	using tmp_vector_type1 = typename _linalg_traits<MatA>::template matrix_type<value_type, 1, col_dim, opt>;
	using tmp_vector_type2 = typename _linalg_traits<MatB>::template matrix_type<value_type, 1, col_dim, opt>;
	using mat_type = matrix<value_type, 1, col_dim, opt>;
	using tmp_vector_type = std::conditional_t<std::is_same_v<tmp_vector_type1, mat_type>, tmp_vector_type2, tmp_vector_type1>;

	const auto rows = lhs.rows();
	const auto cols = lhs.columns();
	auto tmp_vector = mtk::_make_matrix<tmp_vector_type>(1, cols);

	for (size_t row = 0; row < rows; ++row) {
		auto row_vec = lhs.row(row);
		for (size_t col = 0; col < cols; ++col) {
			tmp_vector.value(col) = row_vec.dot(rhs.column(col));
		}
		row_vec = tmp_vector;
	}

	return static_cast<MatA&>(lhs);
}



template<class S
	,size_t R
	,size_t C
	,matrix_options Opt>
class matrix :
	public _matrix_base_selector<matrix<S, R, C, Opt>>::type
{
public:
	constexpr
	matrix() :
		m_data()
	{ }

	template<class... Args
#ifndef MTK_DOXYGEN
		,_require<(sizeof...(Args) == R*C)> = 0
		,_require<(std::is_convertible_v<Args, S> && ...)> = 0
		,matrix_options O = Opt
		,_require<(O & matrix_options::column_major) != matrix_options::column_major> = 0
#endif
	>
	constexpr
	matrix(Args&& ...args) :
		m_data{static_cast<S>(mtk::_forward<Args>(args))...}
	{ }

#ifndef MTK_DOXYGEN
	template<class... Args
		,_require<(sizeof...(Args) == R*C)> = 0
		,_require<(std::is_convertible_v<Args, S> && ...)> = 0
		,matrix_options O = Opt
		,_require<(O & matrix_options::column_major) == matrix_options::column_major> = 0>
	constexpr
	matrix(Args&& ...args) :
		m_data{ }
	{
		std::initializer_list<S> ilist = {static_cast<S>(mtk::_forward<Args>(args))...};
		this->_assign(ilist.begin());
	}
#endif

	template<class Other
#ifndef MTK_DOXYGEN
		,_require<_is_matrix_compatible<matrix, Other>::value> = 0
#endif
	>
	constexpr
	matrix(const _matrix_base<Other>& other) :
		matrix()
	{
		if constexpr ((R == 1) || (C == 1))
			this->_assign(other.begin());
		else
			this->_assign_rows(other.begin_rows());
	}

private:
	friend struct _linalg_traits<matrix>;
	array<S, R*C> m_data;
};

template<class S
	,size_t R
	,matrix_options Opt>
class matrix<S, R, dynamic_extent, Opt> :
	public _matrix_base_selector<matrix<S, R, dynamic_extent, Opt>>::type
{
public:
	matrix() :
		m_data(),
		m_cols(0)
	{ }

	matrix(size_t cols) :
		m_data(R*cols),
		m_cols(cols)
	{ }

#ifndef MTK_DOXYGEN
	template<matrix_options O = Opt
		,_require<((O & matrix_options::column_major) != matrix_options::column_major)> = 0>
#endif
	matrix(std::initializer_list<S> args) :
		m_data(args),
		m_cols(args.size() / R)
	{
		MTK_ASSERT(m_cols*R == args.size());
	}

#ifndef MTK_DOXYGEN
	template<matrix_options O = Opt
		,_require<((O & matrix_options::column_major) == matrix_options::column_major)> = 0>
	matrix(std::initializer_list<S> args) :
		matrix(args.size() / R)
	{
		MTK_ASSERT(m_cols*R == args.size());
		this->_assign(args.begin());
	}
#endif

	template<class Other
#ifndef MTK_DOXYGEN
		,_require<_is_matrix_compatible<matrix, Other>::value> = 0
#endif
	>
	matrix(const _matrix_base<Other>& other) :
		matrix(other.columns())
	{
		if constexpr ((R == 1))
			this->_assign(other.begin());
		else
			this->_assign_rows(other.begin_rows());
	}

private:
	friend struct _linalg_traits<matrix>;
	array<S, dynamic_extent> m_data;
	size_t m_cols;
};

template<class S
	,size_t C
	,matrix_options Opt>
class matrix<S, dynamic_extent, C, Opt> :
	public _matrix_base_selector<matrix<S, dynamic_extent, C, Opt>>::type
{
public:
	matrix() :
		m_data(),
		m_rows(0)
	{ }

	matrix(size_t rows) :
		m_data(rows*C),
		m_rows(rows)
	{ }

#ifndef MTK_DOXYGEN
	template<matrix_options O = Opt
		,_require<((O & matrix_options::column_major) != matrix_options::column_major)> = 0>
#endif
	matrix(std::initializer_list<S> args) :
		m_data(args),
		m_rows(args.size() / C)
	{
		MTK_ASSERT(m_rows*C == args.size());
	}

#ifndef MTK_DOXYGEN
	template<matrix_options O = Opt
		,_require<((O & matrix_options::column_major) == matrix_options::column_major)> = 0>
	matrix(std::initializer_list<S> args) :
		matrix(args.size() / C)
	{
		MTK_ASSERT(m_rows*C == args.size());
		this->_assign(args.begin());
	}
#endif

	template<class Other
#ifndef MTK_DOXYGEN
		,_require<_is_matrix_compatible<matrix, Other>::value> = 0
#endif
	>
	matrix(const _matrix_base<Other>& other) :
		matrix(other.rows())
	{
		if constexpr ((C == 1))
			this->_assign(other.begin());
		else
			this->_assign_rows(other.begin_rows());
	}

private:
	friend struct _linalg_traits<matrix>;
	array<S, dynamic_extent> m_data;
	size_t m_rows;
};

template<class S
	,matrix_options Opt>
class matrix<S, dynamic_extent, dynamic_extent, Opt> :
	public _matrix_base_selector<matrix<S, dynamic_extent, dynamic_extent, Opt>>::type
{
public:
	matrix() :
		m_data(),
		m_rows(0),
		m_cols(0)
	{ }

	matrix(size_t rows, size_t cols) :
		m_data(rows*cols),
		m_rows(rows),
		m_cols(cols)
	{ }

	#ifndef MTK_DOXYGEN
	template<matrix_options O = Opt
		,_require<((O & matrix_options::column_major) != matrix_options::column_major)> = 0>
#endif
	matrix(size_t rows, size_t cols, std::initializer_list<S> args) :
		m_data(args),
		m_rows(rows),
		m_cols(cols)
	{
		MTK_ASSERT(m_rows*m_cols == args.size());
	}

#ifndef MTK_DOXYGEN
	template<matrix_options O = Opt
		,_require<((O & matrix_options::column_major) == matrix_options::column_major)> = 0>
	matrix(size_t rows, size_t cols, std::initializer_list<S> args) :
		matrix(rows, cols)
	{
		MTK_ASSERT(m_rows*m_cols == args.size());
		this->_assign(args.begin());
	}
#endif

	template<class Other
#ifndef MTK_DOXYGEN
		,_require<_is_matrix_compatible<matrix, Other>::value> = 0
#endif
	>
	matrix(const _matrix_base<Other>& other) :
		matrix(other.rows(), other.columns())
	{
		this->_assign_rows(other.begin_rows());
	}

private:
	friend struct _linalg_traits<matrix>;
	array<S, dynamic_extent> m_data;
	size_t m_rows;
	size_t m_cols;
};

} // namespace mtk

#endif
