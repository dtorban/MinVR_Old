# This file is part of the MinVR cmake build system.  
# See the main MinVR/CMakeLists.txt file for authors, copyright, and license info.

# Builds itest-opengl-shaderpipeline-with-api.

# Note: This CMakeLists.txt is not intended to work on its own.  It inherits many settings 
# from the root CMakeLists.txt (build and install directories, compiling flags, #defines, etc.)


# By convention, interactive tests begin with "itest-"
project(itest-opengl-shaderpipeline-with-api)


# Source:
set (SOURCEFILES 
  main.cpp
)
set (HEADERFILES
)
set (CONFIGFILES
)

  add_definitions(-DWITH_HAPTICS)

# Define the target
add_executable(${PROJECT_NAME} ${HEADERFILES} ${SOURCEFILES})


# Setup dependency on libMinVR:C:\OpenHaptics\Developer\3.4.0\lib\x64\Release
target_include_directories(${PROJECT_NAME} PUBLIC ../../src
"$ENV{OH_SDK_BASE}/utilities/include"
"$ENV{OH_SDK_BASE}/include")

link_directories(
"$ENV{OH_SDK_BASE}/lib/x64/Release"
"$ENV{OH_SDK_BASE}/utilities/lib/x64/Release"
"$ENV{OH_SDK_BASE}/lib/x64/Release"
)

target_link_libraries(${PROJECT_NAME} PUBLIC MinVR)
target_link_libraries(${PROJECT_NAME} PUBLIC "$ENV{OH_SDK_BASE}/utilities/lib/x64/Release/hdu.lib")
target_link_libraries(${PROJECT_NAME} PUBLIC "$ENV{OH_SDK_BASE}/lib/x64/Release/hd.lib")

# Setup dependency on OpenGL:
include(AutoBuildOpenGL)
AutoBuild_use_package_OpenGL(${PROJECT_NAME} PUBLIC)


# Setup dependency on GLEW (not needed on OSX):
if (NOT APPLE)
  include(AutoBuildGLEW)
  AutoBuild_use_package_GLEW(${PROJECT_NAME} PUBLIC)
endif()


# Installation:
install( TARGETS ${PROJECT_NAME}
         RUNTIME DESTINATION ${INSTALL_BIN_DEST}
         COMPONENT ITests)



# For better organization when using an IDE with folder structures:
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests-interactive")
source_group("Header Files" FILES ${HEADERFILES})
set_source_files_properties(${CONFIGFILES} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Config Files" FILES ${CONFIGFILES})


