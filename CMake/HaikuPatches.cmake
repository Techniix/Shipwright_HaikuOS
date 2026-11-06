# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Techniix (Ship of Harkinian Haiku port)
#
# Applies the Haiku changes to the libultraship submodule.
#
# libultraship is a submodule pointing at upstream, so the Haiku support for it
# lives in scripts/haiku/patches/ and is applied here at configure time. That
# keeps the submodule on an upstream commit and makes upstream syncs simple.

set(_lus_dir "${CMAKE_CURRENT_SOURCE_DIR}/libultraship")
file(GLOB _haiku_lus_patches "${CMAKE_CURRENT_SOURCE_DIR}/scripts/haiku/patches/libultraship-*.patch")
list(SORT _haiku_lus_patches)

find_package(Git REQUIRED)

foreach(_patch IN LISTS _haiku_lus_patches)
    get_filename_component(_patch_name "${_patch}" NAME)

    # Already applied? (reverse applies cleanly)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} -C "${_lus_dir}" apply --reverse --check "${_patch}"
        RESULT_VARIABLE _already OUTPUT_QUIET ERROR_QUIET
    )
    if(_already EQUAL 0)
        message(STATUS "Haiku: ${_patch_name} already applied to libultraship")
        continue()
    endif()

    execute_process(
        COMMAND ${GIT_EXECUTABLE} -C "${_lus_dir}" apply --whitespace=nowarn "${_patch}"
        RESULT_VARIABLE _applied ERROR_VARIABLE _apply_err
    )
    if(NOT _applied EQUAL 0)
        message(FATAL_ERROR
            "Haiku: could not apply ${_patch_name} to libultraship.\n"
            "The submodule is probably on a different commit than the patch was made for.\n"
            "Run `git submodule update --init` to reset it, or refresh the patch.\n${_apply_err}")
    endif()
    message(STATUS "Haiku: applied ${_patch_name} to libultraship")
endforeach()
