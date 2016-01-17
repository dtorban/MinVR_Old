cmake_minimum_required (VERSION 2.6)

project (minvr_example)

find_package(OpenGL REQUIRED)

set(ALL_LIBS
	${OPENGL_LIBRARY}
  )
  
  FIND_PACKAGE(Qt4 COMPONENTS QtMain QtCore QtGui QtNetwork QtOpenGL REQUIRED)

  IF(QT_FOUND)
    INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
    INCLUDE(${QT_USE_FILE})
    ADD_DEFINITIONS(${QT_DEFINITIONS})
  ELSE()
    MESSAGE(FATAL_ERROR "QT 4.8 or greater is required to build SCIRun")
  ENDIF()

  ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
  
SET(QT_USE_QTOPENGL TRUE)
  
  
#---------------- OS specific settings -------------
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
	set(ALL_LIBS ${ALL_LIBS} ${COCOA_LIB} ${IOKIT_LIB} ${CORE_FOUNDATION_FRAMEWORK} ${CORE_VIDEO_FRAMEWORK})
	message(STATUS "${CORE_VIDEO_FRAMEWORK}")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(Threads)
	find_package(X11)
	set(ALL_LIBS ${ALL_LIBS} ${CMAKE_THREAD_LIBS_INIT} rt Xrandr Xxf86vm Xi m dl ${X11_LIBRARIES})
endif()

#----------------------Application------------------------------

add_executable (${PROJECT_NAME} main.cpp )
target_link_libraries(${PROJECT_NAME} VRMain VRPlugin VRDisplay VRIndex VRMath VRNet ${QT_LIBRARIES} ${ALL_LIBS})

install( TARGETS ${PROJECT_NAME}
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
         RUNTIME DESTINATION bin)