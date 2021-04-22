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
    include/mtk/core.hpp
    include/mtk/core/array.hpp
    include/mtk/core/assert.hpp
    include/mtk/core/byte_cast.hpp
    include/mtk/core/flag_operators.hpp
    include/mtk/core/iterator_traits.hpp
    include/mtk/core/math.hpp
    include/mtk/core/mem_cast.hpp
    include/mtk/core/narrow_cast.hpp
    include/mtk/core/not_null.hpp
    include/mtk/core/nullptr_exception.hpp
    include/mtk/core/overload.hpp
    include/mtk/core/pointer.hpp
    include/mtk/core/preprocessor.hpp
    include/mtk/core/reverse_iterators.hpp
    include/mtk/core/saturate_cast.hpp
    include/mtk/core/scope_exit.hpp
    include/mtk/core/span.hpp
    include/mtk/core/types.hpp
    include/mtk/core/unique_ptr.hpp
    include/mtk/core/version.hpp
    include/mtk/core/zstring_view.hpp
    include/mtk/core/impl/addressof.hpp
    include/mtk/core/impl/clamp.hpp
    include/mtk/core/impl/declval.hpp
    include/mtk/core/impl/move.hpp
    include/mtk/core/impl/pointer_validator.hpp
    include/mtk/core/impl/require.hpp
    include/mtk/core/impl/swap.hpp

    src/mtk/core/assert.cpp
    src/mtk/core/narrow_cast.cpp
    src/mtk/core/nullptr_exception.cpp
    src/mtk/core/scope_exit.cpp
    src/mtk/core/zstring_view.cpp
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    include/mtk/ranges.hpp
    include/mtk/ranges/chain.hpp
    include/mtk/ranges/filter.hpp
    include/mtk/ranges/generate.hpp
    include/mtk/ranges/range.hpp
    include/mtk/ranges/take.hpp
    include/mtk/ranges/transform.hpp
    include/mtk/ranges/zip.hpp
    include/mtk/ranges/impl/functor_storage.hpp
)
