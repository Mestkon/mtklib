#include <mtk/core/os.hpp>

#if defined(MTK_OS_WINDOWS)
	#include <../src/mtk/network/windows/address.inl>
	#include <../src/mtk/network/windows/interface.inl>

#elif defined(MTK_OS_UNIX)
	#include <../src/mtk/network/unix/address.inl>
	#include <../src/mtk/network/unix/interface.inl>
#else
	#error Networking not implemented on current operating system
#endif
