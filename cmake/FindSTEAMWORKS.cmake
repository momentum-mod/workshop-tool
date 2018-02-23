# - Try to find the steamworks library
#
# Once done, this will define:
#
#  STEAMWORKS_INCLUDE_DIR - the Steamworks include directory
#  STEAMWORKS_LIBRARIES - The libraries needed to use Steamworks

if (NOT STEAMWORKS_INCLUDE_DIR OR NOT STEAMWORKS_LIBRARIES)
	set(LIB_SEARCH_PATHS
		${CMAKE_SOURCE_DIR}/steamworks/sdk/redistributable_bin
        ${CMAKE_SOURCE_DIR}/steamworks/sdk/public/steam/lib
        DOC "library path for Steamworks"
	)
	FIND_PATH(STEAMWORKS_INCLUDE_DIR steam_api.h
		${CMAKE_SOURCE_DIR}/steamworks/sdk/public/steam
		DOC "Include path for Steamworks"
	)

	if (MSVC)
		if (BIT_32) #Need a better way of determining bits needed. Maybe have (LIBRARIES_32_BIT and LIBRARIES_64_BIT)?
			FIND_LIBRARY(STEAMWORKS_LIBRARY NAMES steam_api
				PATHS
		        ${LIB_SEARCH_PATHS}
			)
			MESSAGE("32 bit steam")
		else ()
			FIND_LIBRARY(STEAMWORKS_LIBRARY NAMES steam_api64
				PATHS
				${LIB_SEARCH_PATHS}/win64
			)
			MESSAGE("64 bit steam")
		endif()
	elseif (APPLE)
		FIND_LIBRARY(STEAMWORKS_LIBRARY NAMES libsteam_api
			PATHS
			${LIB_SEARCH_PATHS}/osx32
            MESSAGE("32 bit steam")
		)
	elseif (UNIX)
		if (BIT_32) #Need a better way of determining bits needed. Maybe have (LIBRARIES_32_BIT and LIBRARIES_64_BIT)?
			FIND_LIBRARY(STEAMWORKS_LIBRARY NAMES libsteam_api
				PATHS
				${LIB_SEARCH_PATHS}/linux32
			)
			MESSAGE("32 bit steam")
		else ()
			FIND_LIBRARY(STEAMWORKS_LIBRARY NAMES libsteam_api
				PATHS
				${LIB_SEARCH_PATHS}/linux64
			)
			MESSAGE("64 bit steam")
		endif()
	endif ()
	if (STEAMWORKS_LIBRARY)
		set(STEAMWORKS_LIBRARIES ${STEAMWORKS_LIBRARY})
	endif ()
	MARK_AS_ADVANCED(STEAMWORKS_INCLUDE_DIR STEAMWORKS_LIBRARIES)
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(STEAMWORKS DEFAULT_MSG STEAMWORKS_INCLUDE_DIR STEAMWORKS_LIBRARIES)