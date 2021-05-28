#ifndef MTK_LINALG_FWD_HPP
#define MTK_LINALG_FWD_HPP

#include <mtk/core/types.hpp>
#include <mtk/core/flag_operators.hpp>
#include <mtk/core/impl/dynamic_extent.hpp>

namespace mtk {

enum class matrix_options
{
	row_major		= 0,
	column_major	= (1 << 0)
};
MTK_DEFINE_FLAG_OPERATORS(matrix_options)



template<class Scalar
	,size_t Rows
	,size_t Columns
	,matrix_options Options = matrix_options::row_major>
class matrix;

using matrix2f = matrix<float, 2, 2>;
using matrix3f = matrix<float, 3, 3>;
using matrix4f = matrix<float, 4, 4>;
using matrixxf = matrix<float, dynamic_extent, dynamic_extent>;
using matrix2 = matrix<double, 2, 2>;
using matrix3 = matrix<double, 3, 3>;
using matrix4 = matrix<double, 4, 4>;
using matrixx = matrix<double, dynamic_extent, dynamic_extent>;
using matrix2i = matrix<int, 2, 2>;
using matrix3i = matrix<int, 3, 3>;
using matrix4i = matrix<int, 4, 4>;
using matrixxi = matrix<int, dynamic_extent, dynamic_extent>;



template<class Scalar
	,size_t Rows
	,matrix_options Options = matrix_options::row_major>
using vector = matrix<Scalar, Rows, 1, Options>;

using vector2f = vector<float, 2>;
using vector3f = vector<float, 3>;
using vector4f = vector<float, 4>;
using vectorxf = vector<float, dynamic_extent>;
using vector2 = vector<double, 2>;
using vector3 = vector<double, 3>;
using vector4 = vector<double, 4>;
using vectorx = vector<double, dynamic_extent>;
using vector2i = vector<int, 2>;
using vector3i = vector<int, 3>;
using vector4i = vector<int, 4>;
using vectorxi = vector<int, dynamic_extent>;



template<class Scalar
	,size_t Columns
	,matrix_options Options = matrix_options::row_major>
using row_vector = matrix<Scalar, 1, Columns, Options>;

using row_vector2f = row_vector<float, 2>;
using row_vector3f = row_vector<float, 3>;
using row_vector4f = row_vector<float, 4>;
using row_vectorxf = row_vector<float, dynamic_extent>;
using row_vector2 = row_vector<double, 2>;
using row_vector3 = row_vector<double, 3>;
using row_vector4 = row_vector<double, 4>;
using row_vectorx = row_vector<double, dynamic_extent>;
using row_vector2i = row_vector<int, 2>;
using row_vector3i = row_vector<int, 3>;
using row_vector4i = row_vector<int, 4>;
using row_vectorxi = row_vector<int, dynamic_extent>;

} // namespace mtk

#endif
