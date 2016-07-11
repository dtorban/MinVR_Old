project(MinVR_WebSockets)

set (SOURCEFILES 
  src/VRWebSocketsNode.cpp
  src/VRWebSocketsPlugin.cpp
)

set (HEADERFILES
  src/VRWebSocketsNode.h
)

source_group("Header Files" FILES ${HEADERFILES})

#------------------------------------------
# Download dependencies
#------------------------------------------

set(libwebsockets_checkout_Dir ${PROJECT_SOURCE_DIR}/deps/libwebsockets)
make_directory(${libwebsockets_checkout_Dir})
include(ExternalProject)

set(libwebsockets_build_dir ${CMAKE_BINARY_DIR}/plugins/${PROJECT_NAME}/libwebsockets)
set(libwebsockets_stamp_dir ${libwebsockets_build_dir}/stamp)
set(libwebsockets_tmp_dir ${libwebsockets_build_dir}/tmp)

set(libwebsockets_PREFIX "${libwebsockets_checkout_Dir}")
set(libwebsockets_INSTALL_DIR "${libwebsockets_build_dir}/install")

set(libwebsockets_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${libwebsockets_INSTALL_DIR} -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON -DLIBWEBSOCKETS_BUILD_DOCS=OFF -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
set(libwebsockets_DIR "${libwebsockets_INSTALL_DIR}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(libwebsockets_CMAKE_ARGS ${libwebsockets_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD="c++11" -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY="libc++")
endif()
   
ExternalProject_add(libwebsockets
		PREFIX ${libwebsockets_PREFIX}
		GIT_REPOSITORY https://github.com/warmcat/libwebsockets.git
		INSTALL_DIR ${libwebsockets_INSTALL_DIR}
		CMAKE_ARGS ${libwebsockets_CMAKE_ARGS}
		UPDATE_COMMAND ""
		DOWNLOAD_DIR ${libwebsockets_checkout_Dir}
		SOURCE_DIR ${libwebsockets_checkout_Dir}
		TMP_DIR ${libwebsockets_tmp_dir}
		STAMP_DIR ${libwebsockets_stamp_dir}
		BINARY_DIR ${libwebsockets_build_dir}
)
 
#set_property(TARGET libwebsockets PROPERTY FOLDER "Dependencies")

set(LIBWEBSOCKETS_INCLUDE_DIR ${libwebsockets_INSTALL_DIR}/include CACHE INTERNAL "Directory of LIBWEBSOCKETS header files")
#if(MSVC)
#	set(LIBWEBSOCKETS_LIBRARY ${libwebsockets_INSTALL_DIR}/lib/libwebsockets3.lib CACHE INTERNAL "LIBWEBSOCKETS lib file")
#else()
#	set(LIBWEBSOCKETS_LIBRARY ${libwebsockets_INSTALL_DIR}/lib/libwebsockets3.a CACHE INTERNAL "LIBWEBSOCKETS lib file")
#endif()	
set(LIBWEBSOCKETS_LIBRARY websockets)
link_directories(${libwebsockets_INSTALL_DIR}/lib)
include_directories(${LIBWEBSOCKETS_INCLUDE_DIR})

#------------------------------------------
# MinVR Dependencies
#------------------------------------------

link_directories(${CMAKE_BINARY_DIR}/lib)

#------------------------------------------
# Include Directories
#------------------------------------------

# Include Directories
include_directories (
  src
)

#------------------------------------------
# Specific preprocessor defines
#------------------------------------------

# Windows Section #
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(${WINDOWS_BINARIES} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	find_library(COCOA_LIB Cocoa)
	find_library(IOKIT_LIB IOKit)
	find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    	find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
	set(LIBS_ALL ${LIBS_ALL} ${COCOA_LIB} ${IOKIT_LIB} ${CORE_FOUNDATION_FRAMEWORK} ${CORE_VIDEO_FRAMEWORK})
	message(STATUS "${CORE_VIDEO_FRAMEWORK}")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(Threads)
	find_package(X11)
	set(LIBS_ALL ${LIBS_ALL} ${CMAKE_THREAD_LIBS_INIT} rt Xrandr Xxf86vm Xi m dl ${X11_LIBRARIES})
endif()

#------------------------------------------
# Set output directories to lib, and bin
#------------------------------------------

make_directory(${CMAKE_BINARY_DIR}/plugins/${PROJECT_NAME}/lib)
make_directory(${CMAKE_BINARY_DIR}/plugins/${PROJECT_NAME}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/${PROJECT_NAME}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/${PROJECT_NAME}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/${PROJECT_NAME}/bin)
foreach (CONF ${CMAKE_CONFIGURATION_TYPES})
	string (TOUPPER ${CONF} CONF)
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/plugins/${PROJECT_NAME}/bin)
	set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/plugins/${PROJECT_NAME}/lib)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/plugins/${PROJECT_NAME}/lib)
endforeach(CONF CMAKE_CONFIGURATION_TYPES)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "d")
endif()
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
set(CMAKE_MINSIZEREL_POSTFIX "s")

#set the build postfix extension according to the current configuration
if (CMAKE_BUILD_TYPE MATCHES "Release")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
else()
	set(CMAKE_BUILD_POSTFIX "")
endif()

#------------------------------------------
# Build Target
#------------------------------------------

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "${VRPlugins_install_dir}/${PROJECT_NAME}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


add_library ( ${PROJECT_NAME} SHARED ${HEADERFILES} ${SOURCEFILES} )
add_dependencies(${PROJECT_NAME} libwebsockets)
target_link_libraries(${PROJECT_NAME} ${LIBWEBSOCKETS_LIBRARY} MinVR ${LIBS_ALL})

#------------------------------------------
# Install Target
#------------------------------------------

install( TARGETS ${PROJECT_NAME}
         LIBRARY DESTINATION ${VRPlugins_install_dir}/${PROJECT_NAME}/lib
         ARCHIVE DESTINATION ${VRPlugins_install_dir}/${PROJECT_NAME}/lib
         RUNTIME DESTINATION ${VRPlugins_install_dir}/${PROJECT_NAME}/bin)
		 
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "plugins")
