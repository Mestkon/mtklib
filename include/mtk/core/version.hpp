#ifndef MTK_CORE_VERSION_HPP
#define MTK_CORE_VERSION_HPP

//! @file
//! Contains version info.

//! @addtogroup core
//! @{

//! @brief Major version.
//!
//! @code
//! #include <mtk/core/version.hpp>
//! @endcode
#define MTK_MAJOR_VERSION (0ul)
//! @brief Minor version.
//!
//! @code
//! #include <mtk/core/version.hpp>
//! @endcode
#define MTK_MINOR_VERSION (1ul)

//! @brief Combines version to single number.
//!
//! @code
//! #include <mtk/core/version.hpp>
//! @endcode
#define MTK_MAKE_VERSION(MAJOR, MINOR) ((MAJOR << 16ul) | MINOR)

//! @brief Combined version.
//!
//! @code
//! #include <mtk/core/version.hpp>
//! @endcode
#define MTK_VERSION (MTK_MAKE_VERSION(MTK_MAJOR_VERSION, MTK_MINOR_VERSION))

//! @}

#endif
