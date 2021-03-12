#ifndef MTK_CORE_VERSION_HPP
#define MTK_CORE_VERSION_HPP

#define MTK_MAJOR_VERSION 0
#define MTK_MINOR_VERSION 0

//! Combined version. Upper 16 bits is major, lower 16 bits is minor.
#define MTK_VERSION ((MTK_MAJOR_VERSION << 16) | MTK_MINOR_VERSION)

#endif
