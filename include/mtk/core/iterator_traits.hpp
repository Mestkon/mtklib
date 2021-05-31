#ifndef MTK_CORE_ITERATOR_TRAITS_HPP
#define MTK_CORE_ITERATOR_TRAITS_HPP

//! @file
//! Contains traits to check if a type satisfies
//! the different iterator named requirements.

#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/require.hpp>

#include <iterator>
#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

//! @brief Contains typedefs for the std::iterator_traits types.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
namespace iter {

//! @addtogroup core
//! @{

//! @brief Typedef
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class Iter>
using value_type = typename std::iterator_traits<Iter>::value_type;

//! @brief Typedef
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class Iter>
using reference = typename std::iterator_traits<Iter>::reference;

//! @brief Typedef
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class Iter>
using pointer = typename std::iterator_traits<Iter>::pointer;

//! @brief Typedef
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class Iter>
using difference_type = typename std::iterator_traits<Iter>::difference_type;

//! @brief Typedef
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class Iter>
using iterator_category = typename std::iterator_traits<Iter>::iterator_category;

//! @}

} // namespace iter

namespace impl_core {
namespace iterator_traits {

template<class T
	,class = void>
struct _is_iterator_helper : std::false_type { };

template<class T>
struct _is_iterator_helper<T
	,_void_t<void
		,_require<std::is_copy_constructible_v<T>>
		,_require<std::is_copy_assignable_v<T>>
		,_require<std::is_destructible_v<T>>
		,_require<std::is_swappable_v<T>>
		,iter::value_type<T>
		,iter::difference_type<T>
		,iter::reference<T>
		,iter::pointer<T>
		,iter::iterator_category<T>
		,decltype(*mtk::_declval<T&>())
		,_require<std::is_same_v<decltype(++mtk::_declval<T&>()), T&>>
	>> : std::true_type { };



template<class T
	,class = void>
struct _has_indirection : std::false_type { };

template<class T>
struct _has_indirection<T*> : std::true_type { };

template<class T>
struct _has_indirection<T
	,_void_t<void
		,decltype(mtk::_declval<T&>().operator->())
	>> : std::true_type { };

template<class T
	,class = void>
struct _is_input_iterator_helper : std::false_type { };

template<class T>
struct _is_input_iterator_helper<T
	,_void_t<void
		,_require<_is_iterator_helper<T>::value>
		,_require<std::is_base_of_v<std::input_iterator_tag, iter::iterator_category<T>>>
		,_require<std::is_convertible_v<decltype(mtk::_declval<const T>() == mtk::_declval<const T>()), bool>>
		,_require<std::is_constructible_v<bool, decltype(mtk::_declval<const T>() != mtk::_declval<const T>())>>
		,_require<std::is_same_v<decltype(*mtk::_declval<const T>()), iter::reference<T>>>
		,_require<_has_indirection<T>::value>
		,_require<std::is_convertible_v<iter::reference<T>, iter::value_type<T>>>
		,_require<std::is_convertible_v<decltype(*mtk::_declval<T&>()++), iter::value_type<T>>>
	>> : std::true_type { };



template<class T
	,class OutputT
	,class = void>
struct _is_output_iterator_helper : std::false_type { };

template<class T
	,class OutputT>
struct _is_output_iterator_helper<T
	,OutputT
	,_void_t<void
		,_require<_is_iterator_helper<T>::value>
		,_require<std::is_class_v<T> || std::is_pointer_v<T>>
		,decltype(*(mtk::_declval<T&>()) = mtk::_declval<OutputT>())
		,_require<std::is_convertible_v<decltype(mtk::_declval<T&>()++), const T&>>
		,decltype(*mtk::_declval<T&>()++ = mtk::_declval<OutputT>())
	>> : std::true_type { };



template<class T
	,class = void>
struct _is_forward_iterator_helper : std::false_type { };

template<class T>
struct _is_forward_iterator_helper<T
	,_void_t<void
		,_require<_is_input_iterator_helper<T>::value>
		,_require<std::is_base_of_v<std::forward_iterator_tag, iter::iterator_category<T>>>
		,_require<std::is_default_constructible_v<T>>
		,_require<std::is_same_v<iter::reference<T>, iter::value_type<T>&>
			|| std::is_same_v<iter::reference<T>, const iter::value_type<T>&>>
		,_require<std::is_same_v<decltype(mtk::_declval<T&>()++), T>>
		,_require<std::is_same_v<decltype(*mtk::_declval<T&>()++), iter::reference<T>>>
	>> : std::true_type { };



template<class T
	,class = void>
struct _is_bidirectional_iterator_helper : std::false_type { };

template<class T>
struct _is_bidirectional_iterator_helper<T
	,_void_t<void
		,_require<_is_forward_iterator_helper<T>::value>
		,_require<std::is_base_of_v<std::bidirectional_iterator_tag, iter::iterator_category<T>>>
		,_require<std::is_same_v<decltype(--mtk::_declval<T&>()), T&>>
		,_require<std::is_convertible_v<decltype(mtk::_declval<T&>()--), const T&>>
		,_require<std::is_same_v<decltype(*mtk::_declval<T&>()--), iter::reference<T>>>
	>> : std::true_type { };



template<class T
	,class = void>
struct _is_random_access_iterator_helper : std::false_type { };

template<class T>
struct _is_random_access_iterator_helper<T
	,_void_t<void
		,_require<_is_bidirectional_iterator_helper<T>::value>
		,_require<std::is_base_of_v<std::random_access_iterator_tag, iter::iterator_category<T>>>
		,_require<std::is_same_v<decltype(mtk::_declval<T&>() += mtk::_declval<iter::difference_type<T>>()), T&>>
		,_require<std::is_same_v<decltype(mtk::_declval<const T>() + mtk::_declval<iter::difference_type<T>>()), T>>
		,_require<std::is_same_v<decltype(mtk::_declval<iter::difference_type<T>>() + mtk::_declval<const T>()), T>>
		,_require<std::is_same_v<decltype(mtk::_declval<T&>() -= mtk::_declval<iter::difference_type<T>>()), T&>>
		,_require<std::is_same_v<decltype(mtk::_declval<const T>() - mtk::_declval<iter::difference_type<T>>()), T>>
		,_require<std::is_same_v<decltype(mtk::_declval<const T>() - mtk::_declval<const T>()), iter::difference_type<T>>>
		,_require<std::is_convertible_v<decltype(mtk::_declval<const T>()[mtk::_declval<iter::difference_type<T>>()]), iter::reference<T>>>
		,_require<std::is_constructible_v<bool, decltype(mtk::_declval<const T>() < mtk::_declval<const T>())>>
		,_require<std::is_constructible_v<bool, decltype(mtk::_declval<const T>() > mtk::_declval<const T>())>>
		,_require<std::is_constructible_v<bool, decltype(mtk::_declval<const T>() >= mtk::_declval<const T>())>>
		,_require<std::is_constructible_v<bool, decltype(mtk::_declval<const T>() <= mtk::_declval<const T>())>>
	>> : std::true_type { };

} // namespace iterator_traits
} // namespace impl_core

//! @brief Type trait to check if a class T satisfies the
//! iterator named requirement.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T>
struct is_iterator
#ifndef MTK_DOXYGEN
	: impl_core::iterator_traits::_is_iterator_helper<T>
#endif
{ };

//! @brief Type trait to check if a class T satisfies the
//! input iterator named requirement.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T>
struct is_input_iterator
#ifndef MTK_DOXYGEN
	: impl_core::iterator_traits::_is_input_iterator_helper<T>
#endif
{ };

//! @brief Type trait to check if a class T satisfies the
//! output iterator named requirement for output type OutputT.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T
	,class OutputT>
struct is_output_iterator
#ifndef MTK_DOXYGEN
	: impl_core::iterator_traits::_is_output_iterator_helper<T, OutputT>
#endif
{ };

//! @brief Type trait to check if a class T satisfies the
//! forward iterator named requirement.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T>
struct is_forward_iterator
#ifndef MTK_DOXYGEN
	: impl_core::iterator_traits::_is_forward_iterator_helper<T>
#endif
{ };

//! @brief Type trait to check if a class T satisfies the
//! bidirectional iterator named requirement.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T>
struct is_bidirectional_iterator
#ifndef MTK_DOXYGEN
	: impl_core::iterator_traits::_is_bidirectional_iterator_helper<T>
#endif
{ };

//! @brief Type trait to check if a class T satisfies the
//! random access iterator named requirement.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T>
struct is_random_access_iterator
#ifndef MTK_DOXYGEN
	: impl_core::iterator_traits::_is_random_access_iterator_helper<T>
#endif
{ };


//! @brief Shortcut for is_iterator_t<T>::value.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T>
inline constexpr
bool
is_iterator_v = is_iterator<T>::value;

//! @brief Shortcut for is_input_iterator<T>::value
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T>
inline constexpr
bool
is_input_iterator_v = is_input_iterator<T>::value;

//! @brief Shortcut for is_output_iterator<T, OutputT>::value.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T
	,class OutputT>
inline constexpr
bool
is_output_iterator_v = is_output_iterator<T, OutputT>::value;

//! @brief Shortcut for is_forward_iterator<T>::value.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T>
inline constexpr
bool
is_forward_iterator_v = is_forward_iterator<T>::value;

//! @brief Shortcut for is_bidirectional_iterator<T>::value.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T>
inline constexpr
bool
is_bidirectional_iterator_v = is_bidirectional_iterator<T>::value;

//! @brief Shortcut for is_random_access_iterator<T>::value.
//!
//! @code
//! #include <mtk/core/iterator_traits.hpp>
//! @endcode
template<class T>
inline constexpr
bool
is_random_access_iterator_v = is_random_access_iterator<T>::value;

//! @}

} // namespace mtk

#endif
