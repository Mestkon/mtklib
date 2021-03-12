cmake_minimum_required(VERSION 3.5)

project (mtklib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${CMAKE_PROJECT_NAME} STATIC "")
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DMTK_DEBUG)
endif()

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    include/mtk/core/assert.hpp
    include/mtk/core/guarded_ptr.hpp
    include/mtk/core/not_nullptr.hpp
    include/mtk/core/nullptr_exception.hpp
    include/mtk/core/pointer.hpp
    include/mtk/core/types.hpp
    include/mtk/core/version.hpp
    include/mtk/core/impl/pointer_validators.hpp
    include/mtk/core/impl/require.hpp

    src/mtk/core/assert.cpp
    src/mtk/core/nullptr_exception.cpp
    src/mtk/core/pointer_validators.cpp
)
