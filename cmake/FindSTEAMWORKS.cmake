# - Try to find the steamworks library
#
# Once done, this will define:
#
#  STEAMWORKS_INCLUDE_DIR - the Steamworks include directory
#  STEAMWORKS_LIBRARIES - The libraries needed to use Steamworks

include(FindPackageHandleStandardArgs)

if (NOT STEAWORKS_FOUND)
    set(STEAMWORKS_PATH ${CMAKE_SOURCE_DIR}/steamworks/sdk)

    # find library
    find_path(STEAMWORKS_INCLUDE_DIR 
        NAMES steam_api.h
        PATH_SUFFIXES "public/steam"
        PATHS ${STEAMWORKS_PATH}
    )

    if (MSVC)
        if (BIT_32)
            set(PATH_SUFFIX "")
        else()
            set(PATH_SUFFIX "win64")
        endif()

    elseif (APPLE)
        set(PATH_SUFFIX "osx")

    else ()
        if (BIT_32)
            set(PATH_SUFFIX "linux32")
        else()
            set(PATH_SUFFIX "linux64")
        endif()
    endif()
    
    find_library(STEAMWORKS_LIBRARY 
        NAMES steam_api
        PATH_SUFFIXES  "redistributable_bin/${PATH_SUFFIX}"
        PATHS ${STEAMWORKS_PATH}
    )
endif ()

find_package_handle_standard_args(STEAMWORKS 
    FOUND_VAR 
        STEAMWORKS_FOUND 
    REQUIRED_VARS 
        STEAMWORKS_INCLUDE_DIR STEAMWORKS_LIBRARY
)
