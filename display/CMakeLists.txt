# Create a library from the source files in this directory.

project (VRDisplay)

set (${PROJECT_NAME}_source_files 
	VRDisplayNode.cpp
	renderers/concrete/VRCallbackRenderer.cpp
	renderers/VRRenderHandler.cpp
	renderers/VRRenderState.cpp
	nodes/console/VRConsoleDisplay.cpp
	nodes/graphics/VRGraphicsWindowNode.cpp
	nodes/graphics/camera/VROffAxisCameraNode.cpp
	nodes/graphics/stereo/VRStereoNode.cpp
	nodes/graphics/stereo/VRSideBySideNode.cpp
	nodes/graphics/structure/VRViewportNode.cpp
	nodes/graphics/structure/VRTile.cpp
	nodes/graphics/structure/VRTileNode.cpp
	nodes/scope/VRStateScopeNode.cpp
	factories/VRCompositeDisplayFactory.cpp
	factories/VRTypedDisplayFactory.cpp
)

add_library (${PROJECT_NAME} ${${PROJECT_NAME}_source_files})

install(DIRECTORY ${PROJECT_SOURCE_DIR}  DESTINATION "include" FILES_MATCHING PATTERN "*.h")

install( TARGETS ${PROJECT_NAME}
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
         RUNTIME DESTINATION bin)
