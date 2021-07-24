cmake_minimum_required (VERSION 3.3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(sound_engine)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

file(GLOB_RECURSE SRC_ALL
	"common/*.h*"
	"common/*.c*"
	"external/*.h*"
	"external/*.c*"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/oggvorbis/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/mp3/pvmp3dec/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/mp3/pvmp3dec/src)

foreach(FILE ${SRC_ALL})
	# Get the directory of the source file
	get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
		
	# Remove common directory prefix to make the group
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

	# Make sure we are using windows slashes
	string(REPLACE "/" "\\" GROUP "${GROUP}")

	#include_directories(${PARENT_DIR})

	source_group("${GROUP}" FILES "${FILE}")
endforeach()

add_library(sound_engine ${SRC_ALL})

if(WIN32)
	add_definitions(-DX2AUDIO_SOUND)
elseif(UNIX)

	find_package(OpenAL REQUIRED)

	if (OPENAL_FOUND)
		include_directories(${OPENAL_INCLUDE_DIR})

		target_link_libraries(sound_engine
			${OPENAL_LIBRARY}
		)
		
		add_definitions(-DOPENAL_SOUND)
	else()
		message("can't find OpenAL library")
	endif()

else()
	add_definitions(-DDUMMY_SOUND)
endif()

#executable

project(tests CXX)

file(GLOB_RECURSE SRC_EXECUTABLE
	"tests/*.c*"
)

add_executable(tests ${SRC_EXECUTABLE})

target_link_libraries(tests sound_engine)

#copy to the main project folder
set(source_dir data)
set(destination_dir ${CMAKE_BINARY_DIR})

file( COPY ${source_dir}
      DESTINATION ${destination_dir}
)