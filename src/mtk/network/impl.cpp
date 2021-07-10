#include <mtk/core/os.hpp>

#ifdef MTK_OS_WINDOWS
	#include <../src/mtk/network/windows/address.inl>
#elif defined(MTK_OS_UNIX)
	#include <../src/mtk/network/unix/address.inl>
#else
	#error Networking not implemented on current operating system
#endif
