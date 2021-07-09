#include <mtk/core/os.hpp>

namespace mtk {

const char*
get_os_string() noexcept
{
#if defined(MTK_OS_WINDOWS)
	return "Windows";
#elif defined(MTK_OS_IOS)
	return "IOS";
#elif defined(MTK_OS_MACOS)
	return "MacOS";
#elif defined(MTK_OS_ANDROID)
	return "Android";
#elif defined(MTK_OS_LINUX)
	return "Linux";
#elif defined(MTK_OS_UNIX)
	return "Unix";
#else
	return "Unknown";
#endif
}



} // namespace mtk
