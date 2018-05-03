

find_path(LibWebSockets_INCLUDE_DIR 
    libwebsockets.h
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/include 
    ${CMAKE_INSTALL_PREFIX}/include/LibWebSockets
    $ENV{LibWebSockets_ROOT}/include 
    $ENV{LibWebSockets_ROOT}/include/LibWebSockets 
    /usr/local/include
    /usr/local/include/LibWebSockets
)


find_library(LibWebSockets_OPT_LIBRARIES 
  NAMES 
    libwebsockets.a 
    websockets.lib 
    websockets
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/lib 
    $ENV{LibWebSockets_ROOT}/lib 
    /usr/local/lib
)

message(lib sock:${LibWebSockets_OPT_LIBRARIES})
          
find_library(LibWebSockets_DEBUG_LIBRARIES 
  NAMES 
    libwebsocketsd.a 
    websocketsd.lib 
    websocketsd
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/lib 
    $ENV{LibWebSockets_ROOT}/lib 
    /usr/local/lib
)
 

if (LibWebSockets_OPT_LIBRARIES AND LibWebSockets_DEBUG_LIBRARIES)
    set(LibWebSockets_OPT_LIBRARIES optimized ${LibWebSockets_OPT_LIBRARIES})
    set(LibWebSockets_DEBUG_LIBRARIES debug ${LibWebSockets_DEBUG_LIBRARIES})
elseif(LibWebSockets_OPT_LIBRARIES AND NOT LibWebSockets_DEBUG_LIBRARIES)
    # if only opt is found, use it for both
    set(LibWebSockets_DEBUG_LIBRARIES debug ${LibWebSockets_OPT_LIBRARIES})
    set(LibWebSockets_OPT_LIBRARIES optimized ${LibWebSockets_OPT_LIBRARIES})
elseif(NOT LibWebSockets_OPT_LIBRARIES AND LibWebSockets_DEBUG_LIBRARIES)
    # if only debug is found, use it for both
    set(LibWebSockets_OPT_LIBRARIES optimized ${LibWebSockets_DEBUG_LIBRARIES})
    set(LibWebSockets_DEBUG_LIBRARIES debug ${LibWebSockets_DEBUG_LIBRARIES})
endif()


set(LibWebSockets_LIBRARIES ${LibWebSockets_OPT_LIBRARIES} ${LibWebSockets_DEBUG_LIBRARIES})

message(lib sock:${LibWebSockets_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    LibWebSockets
    DEFAULT_MSG
    LibWebSockets_INCLUDE_DIR
    LibWebSockets_OPT_LIBRARIES 
    LibWebSockets_DEBUG_LIBRARIES 
    LibWebSockets_LIBRARIES
)

mark_as_advanced(
    LibWebSockets_INCLUDE_DIR 
    LibWebSockets_OPT_LIBRARIES 
    LibWebSockets_DEBUG_LIBRARIES 
    LibWebSockets_LIBRARIES
)
