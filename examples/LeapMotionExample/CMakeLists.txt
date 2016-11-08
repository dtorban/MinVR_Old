
project(LeapMotionExample)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

set(vr_example_source_files
  main.cpp
)

set(vr_example_config_files
  desktop.xml
)

set_source_files_properties(${vr_example_config_files} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group("Config Files" FILES ${vr_example_config_files})

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(Threads)
	find_package(X11)
	set(ALL_LIBS ${ALL_LIBS} ${CMAKE_THREAD_LIBS_INIT} rt Xrandr Xxf86vm Xi m dl ${X11_LIBRARIES})
endif()


add_executable(${PROJECT_NAME} ${vr_example_source_files} ${vr_example_config_files})

target_link_libraries(${PROJECT_NAME} MinVR ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${ALL_LIBS})

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "examples")

