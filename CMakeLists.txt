CMAKE_MINIMUM_REQUIRED(VERSION 3.16.3)
PROJECT(angel)

INCLUDE (cmake/utils.cmake)

SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(/usr/local/include)
LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/local/)

FIND_LIBRARY(YAMLCPP yaml-cpp)
MESSAGE("***", ${YAMLCPP})

SET(LIB_SRC
    angel/log.cpp
    angel/util.cpp
    angel/config.cpp
    )

ADD_LIBRARY(angel SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(angel)
#ADD_LIBRARY(angel_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(angel_static PROPERTIES OUTPUT_NAME "angel")

ADD_EXECUTABLE(test tests/log_test.cpp)
ADD_DEPENDENCIES(test angel)
force_redefine_file_macro_for_sources(test)
TARGET_LINK_LIBRARIES(angel libmysqlclient.so)
TARGET_LINK_LIBRARIES(test angel ${YAMLCPP})
target_link_libraries (angel pthread) 


ADD_EXECUTABLE(config_test tests/config_test.cpp)
ADD_DEPENDENCIES(config_test angel)
force_redefine_file_macro_for_sources(config_test)
TARGET_LINK_LIBRARIES(angel libmysqlclient.so)
TARGET_LINK_LIBRARIES(config_test angel ${YAMLCPP})


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


