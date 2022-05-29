cmake_minimum_required(VERSION 3.7)

project(fecalfunny)

#~ find_package(PkgConfig REQUIRED)
#~ pkg_check_modules(Vulkan REQUIRED vulkan)
find_package(Vulkan REQUIRED)

add_library(fecalfunny SHARED
    fecalfunny.cpp
    fecalfunny.h
)

if (MINGW)
    set_target_properties(fecalfunny PROPERTIES LINK_FLAGS "-lwinpthread")
endif()
