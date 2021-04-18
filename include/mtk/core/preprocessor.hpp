#ifndef MTK_CORE_PREPROCESSOR_HPP
#define MTK_CORE_PREPROCESSOR_HPP

namespace mtk {

template<class... Args>
constexpr
void
_no_op(Args&& ...) noexcept { }

#define MTK_NO_OP mtk::_no_op()

#define MTK_IGNORE(...) mtk::_no_op(__VA_ARGS__)

#define MTK_STRINGIFY_TEXT(X) #X

#define MTK_STRINGIFY(X) MTK_STRINGIFY_TEXT(X)

#define MTK_CONCAT_TEXT(A, B) A##B

#define MTK_CONCAT(A, B) MTK_CONCAT_TEXT(A, B)

#define MTK_RETURNS(X) noexcept(noexcept(X)) -> decltype(X) { return X; }

} // namespace mtk

#endif
