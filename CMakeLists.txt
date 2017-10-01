cmake_minimum_required(VERSION 3.5)

project(BaseConverter)
enable_language(CXX)
enable_testing()

# Folders configuration
get_filename_component(root_dir             ${CMAKE_SOURCE_DIR}           REALPATH)
get_filename_component(SFML_dir             ${root_dir}/deps/SFML         REALPATH)
get_filename_component(imgui_dir            ${root_dir}/deps/imgui        REALPATH)
get_filename_component(imgui-sfml_dir       ${root_dir}/deps/imgui-sfml   REALPATH)

# By default build in Release mode
if( NOT CMAKE_BUILD_TYPE )
	set(CMAKE_BUILD_TYPE "Release")
endif()

# Add SFML targets
add_subdirectory(${SFML_dir})

# Add common CMake functions and macros
include(common.cmake)

# Dependencies check
directory_is_empty(is_empty "${SFML_dir}")
if(is_empty)
	message(FATAL_ERROR "SFML dependency is missing, maybe you didn't pull the git submodules")
endif()
directory_is_empty(is_empty "${imgui_dir}")
if(is_empty)
	message(FATAL_ERROR "ImGui dependency is missing, maybe you didn't pull the git submodules")
endif()
directory_is_empty(is_empty "${imgui-sfml_dir}")
if(is_empty)
	message(FATAL_ERROR "ImGui-SFML dependency is missing, maybe you didn't pull the git submodules")
endif()

# Add _d suffix in debug mode
set(CMAKE_DEBUG_POSTFIX _d)

# configure SFML folder in IDE
foreach(sfml_target IN ITEMS sfml-system sfml-network sfml-window sfml-graphics sfml-audio sfml-main)
	if(TARGET ${sfml_target})
		set_target_properties(${sfml_target} PROPERTIES FOLDER deps/SFML)
	endif()
endforeach()

# Configure ImGui for ImGui-SFML
message(STATUS "ImGui configuration for SFML")
if(NOT EXISTS "${imgui_dir}/CMake_ImGui_SFML_Config_Done")
	set(tmp ${CMAKE_DISABLE_SOURCE_CHANGES})
	set(CMAKE_DISABLE_SOURCE_CHANGES OFF)
	file(READ "${imgui-sfml_dir}/imconfig-SFML.h" imconfig-SFML_content)
	file(APPEND "${imgui_dir}/imconfig.h" "${imconfig-SFML_content}")
	file(WRITE "${imgui_dir}/CMake_ImGui_SFML_Config_Done" "")
	message(STATUS "ImGui configuration for SFML - done")
	set(CMAKE_DISABLE_SOURCE_CHANGES ${tmp})
else()
	message(STATUS "ImGui configuration for SFML - already done")
endif()

# Setup ImGui-SFML
get_files(
  files
  "${imgui_dir}"
  "${imgui-sfml_dir}"
)
make_target(
  ImGui-SFML "deps/ImGui-SFML" ${files}
  INCLUDES "${SFML_dir}/include/" "${imgui_dir}" "${imgui-sfml_dir}"
  OPTIONS cxx no_warnings
)
target_link_libraries(ImGui-SFML PRIVATE "sfml-system" "sfml-network" "sfml-window" "sfml-graphics" "sfml-audio")

# Setup BaseConverter
get_files(
  files
  "${root_dir}/src"
  "${root_dir}/include"
  OPTIONS recurse
)
make_target(
  BaseConverter "BaseConverter" ${files}
  INCLUDES "include/" "${SFML_dir}/include/" "${imgui_dir}" "${imgui-sfml_dir}"
  OPTIONS cxx executable
)
target_link_libraries(BaseConverter PRIVATE "sfml-system" "sfml-window" "sfml-graphics" "ImGui-SFML")
if(TARGET sfml-main)
	target_link_libraries(BaseConverter PRIVATE "sfml-main")
endif()
set_property(TARGET BaseConverter PROPERTY WIN32_EXECUTABLE ON)
set_property(TARGET BaseConverter PROPERTY CXX_STANDARD 11)
set_property(TARGET BaseConverter PROPERTY CXX_STANDARD_REQUIRED ON)

# Setup targets output, put exe and required SFML dll in the same folder
target_set_output_directory(sfml-system "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
target_set_output_directory(sfml-window "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
target_set_output_directory(sfml-graphics "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
target_set_output_directory(BaseConverter "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
