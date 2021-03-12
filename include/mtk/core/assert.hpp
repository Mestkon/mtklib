#ifndef MTK_CORE_ASSERT_HPP
#define MTK_CORE_ASSERT_HPP

namespace mtk {

[[noreturn]]
void
_assertion_handler(const char* what, const char* file, int line) noexcept;

#define MTK_IMPL_LIVE_ASSERT(...) static_cast<void>(!static_cast<bool>(__VA_ARGS__) && (mtk::_assertion_handler(#__VA_ARGS__, __FILE__, __LINE__), 1))
#define MTK_IMPL_DUD_ASSERT(...) static_cast<void>(0)

#ifdef MTK_DEBUG
	#define MTK_IMPL_ASSERT(...) MTK_IMPL_LIVE_ASSERT(__VA_ARGS__)
#else
	#define MTK_IMPL_ASSERT(...) MTK_IMPL_DUD_ASSERT(__VA_ARGS__)
#endif



#define MTK_ASSERT(...) MTK_IMPL_ASSERT(__VA_ARGS__)

} // namespace mtk

#endif
