#ifndef MTK_CORE_OS_HPP
#define MTK_CORE_OS_HPP

//! @file
//! Contains macros to determine the operating system

namespace mtk {

//! @addtogroup core
//! @{

#if !defined(MTK_OS_WINDOWS) && \
	!defined(MTK_OS_IOS) && \
	!defined(MTK_OS_MACOS) && \
	!defined(MTK_OS_ANDROID) && \
	!defined(MTK_OS_LINUX) && \
	!defined(MTK_OS_UNIX)

	#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
		#define MTK_OS_WINDOWS
	#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
		#include <TargetConditionals.h>
		#if TARGET_OS_IPHONE
			#define MTK_OS_IOS
		#elif TARGET_OS_MAC
			#define MTK_OS_MACOS
		#endif
	#elif defined(__ANDROID__)
		#define MTK_OS_ANDROID
	#elif defined(__linux__) || defined(linux) || defined(__linux)
		#define MTK_OS_LINUX
	#endif

	#if defined(__unix__) || defined(__unix)
		#define MTK_OS_UNIX
	#endif
#endif

#ifdef MTK_DOXYGEN
	//! @brief Defined if compilation target is a windows system.
	//!
	//! @code
	//! #include <mtk/core/os.hpp>
	//! @endcode
	//!
	//! Can be overriden by globally defining any of the MTK_OS_* macros.
	#define MTK_OS_WINDOWS
	//! @brief Defined if compilation target is an IOS system.
	//!
	//! @code
	//! #include <mtk/core/os.hpp>
	//! @endcode
	//!
	//! Can be overriden by globally defining any of the MTK_OS_* macros.
	#define MTK_OS_IOS
	//! @brief Defined if compilation target is a MacOS system.
	//!
	//! @code
	//! #include <mtk/core/os.hpp>
	//! @endcode
	//!
	//! Can be overriden by globally defining any of the MTK_OS_* macros.
	#define MTK_OS_MACOS
	//! @brief Defined if compilation target is an android system.
	//!
	//! @code
	//! #include <mtk/core/os.hpp>
	//! @endcode
	//!
	//! Can be overriden by globally defining any of the MTK_OS_* macros.
	#define MTK_OS_ANDROID
	//! @brief Defined if compilation target is a linux system.
	//!
	//! @code
	//! #include <mtk/core/os.hpp>
	//! @endcode
	//!
	//! Can be overriden by globally defining any of the MTK_OS_* macros.
	#define MTK_OS_LINUX
	//! @brief Defined if compilation target is a unix system.
	//!
	//! @code
	//! #include <mtk/core/os.hpp>
	//! @endcode
	//!
	//! Can be overriden by globally defining any of the MTK_OS_* macros.
	#define MTK_OS_UNIX

#endif

//! @brief Returns a human readable string of the compilation target operating system.
//!
//! @code
//! #include <mtk/core/os.hpp>
//! @endcode
const char*
get_os_string() noexcept;

//! @}

} // namespace mtk

#endif
