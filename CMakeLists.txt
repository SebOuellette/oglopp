cmake_minimum_required(VERSION 4.0.1)
project(oglopp
	VERSION 1.0.1
	LANGUAGES C CXX
)

# PIC everywhere
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS ON)
# Safe linking
set(CMAKE_INSTALL_RPATH "/usr/local/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

#set(CMAKE_VERBOSE_MAKEFILE TRUE)

# Output dirs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include paths
include_directories(
	${CMAKE_SOURCE_DIR}/Headers
	${CMAKE_SOURCE_DIR}/glad/include
)

# Collect sources
file(GLOB_RECURSE OGL_SOURCES
	${CMAKE_SOURCE_DIR}/Sources/*.cpp
)
set(GLAD_SOURCE ${CMAKE_SOURCE_DIR}/glad/src/gl.c)

# Shared library target
add_library(${PROJECT_NAME} SHARED
	${OGL_SOURCES}
	${GLAD_SOURCE}
)

# Export macro for Windows
target_compile_definitions(${PROJECT_NAME} PRIVATE OGL_BUILD_DLL)

# Link dependencies
find_library(GLFW_LIB NAMES glfw glfw3 PATHS /usr/lib/ NO_DEFAULT_PATH REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIB})
if (UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE dl pthread)
endif()

# SONAME / versioning (Linux)
set_target_properties(${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	OUTPUT_NAME "${PROJECT_NAME}"
)

# Install rules
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers and glad include
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Headers/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/glad/include/glad DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# Examples
set(example_targets)
file(GLOB EXAMPLE_SRCS ${CMAKE_SOURCE_DIR}/Examples/*.cpp)
foreach(src IN LISTS EXAMPLE_SRCS)
	get_filename_component(name ${src} NAME_WE)
	set(exname ogl-${name})
	add_executable(${exname} EXCLUDE_FROM_ALL ${src})
	target_link_options(${exname} PRIVATE -Wl,-rpath,${CMAKE_INSTALL_RPATH})
	target_link_libraries(${exname} PRIVATE "${GLFW_LIB}")
	target_link_libraries(${exname} PRIVATE ${PROJECT_NAME})
	#target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR}/Headers)
	unset(${PROJECT_NAME} CACHE)
	list(APPEND example_targets ${exname})
endforeach()

# All examples
add_custom_target(examples
	DEPENDS ${example_targets}
	COMMENT "Building all example executables"
)

#if (TARGET examples)
#	install(TARGETS ${example_targets} DESTINATION ${CMAKE_INSTALL_BINDIR})
#endif()

# Strip target
add_custom_target(strip
	COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${PROJECT_NAME}>
	COMMENT "Stripping shared library"
)

# Uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY
)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# (Optional) Doxygen target if you have docs/Doxyfile
find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
	add_custom_target(docs
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/docs/Doxyfile
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Generating Doxygen documentation"
	)
endif()
