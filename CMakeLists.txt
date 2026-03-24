cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME mate)
project(${PROJECT_NAME} LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build shared libraries (.so)" ON)
option(BUILD_STATIC_LIBS "Build static libraries (.a)" ON)
option(INSTALL "Install libmate and executable." OFF)
option(ASAN "Enable Address Sanitizer" OFF)

add_compile_options(-Wall -Wextra -Wunused)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

if(ASAN)
  message(STATUS "AddressSanitizer enabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

if(INSTALL)
  include(GNUInstallDirs)
endif()

set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

set(LIB_SOURCES
    src/config.c
    src/debug.c
    src/environment.c
    src/error.c
    src/file.c
    src/interpreter.c
    src/hashmap.c
    src/lexer.c
    src/node.c
    src/parser.c
    src/native.c
    src/string.c
    src/vector.c
    src/util/native.c)

set(HEADERS
    include/mate/config.h
    include/mate/environment.h
    include/mate/error.h
    include/mate/file.h
    include/mate/hashmap.h
    include/mate/interpreter.h
    include/mate/lexer.h
    include/mate/node.h
    include/mate/parser.h
    include/mate/native.h
    include/mate/string.h
    include/mate/vector.h
    include/mate/util/native.h)

set(EXE_SOURCES
    src/main.c
    ${LIB_SOURCES}) # Add libs sources to executable, to avoid unused files removal.

if(BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME}_shared SHARED ${LIB_SOURCES} ${HEADERS})
  set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
  target_include_directories(${PROJECT_NAME}_shared
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  add_library(Mate::MateShared ALIAS ${PROJECT_NAME}_shared)
endif()

if(BUILD_STATIC_LIBS)
  add_library(${PROJECT_NAME}_static STATIC ${LIB_SOURCES} ${HEADERS})
  set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME}_static)
  target_include_directories(${PROJECT_NAME}_static
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  add_library(Mate::Mate ALIAS ${PROJECT_NAME}_static)
endif()

# Main Binary
add_executable(${PROJECT_NAME} ${EXE_SOURCES})
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  Mate::Mate)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME}
      PRIVATE
      -DDEBUG)
    message(STATUS "Debug Build")
endif()

if(INSTALL)
  include(GNUInstallDirs)

  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  if(BUILD_STATIC_LIBS)
    install(TARGETS ${PROJECT_NAME}_static
      EXPORT ${PROJECT_NAME}Targets
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  endif()

  if(BUILD_SHARED_LIBS)
    install(TARGETS ${PROJECT_NAME}_shared
      EXPORT ${PROJECT_NAME}Targets
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  endif()

  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

  install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE Mate::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
endif()