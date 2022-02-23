cmake_minimum_required (VERSION 3.10)
project (Tutorial)

# The version number.
set (Tutorial_VERSION_MAJOR 5)
set (Tutorial_VERSION_MINOR 0)


message (">>> value = ${PROJECT_SOURCE_DIR}")
message (">>> value = ${PROJECT_BINARY_DIR}")
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Tutorial.version"
  "${PROJECT_SOURCE_DIR}/Tutorial.h"
  )
# 安装路径
set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")

# 链接库路径操作
set(INSTALL_LIB_DIR $ORIGIN "${CMAKE_INSTALL_PREFIX}/lib") # 假设安装目录在编译目录的lib子目录内和当前文件夹，这里写成了列表的形式
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${INSTALL_LIB_DIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE) # 存在上面一句，此句好像没有用

# zlog
option (USE_ZLOG
        "Use tutorial provided math implementation" ON) 
if (USE_ZLOG)
  # 头文件和库文件是单独的
  include_directories ("${PROJECT_SOURCE_DIR}/zlog_manager/zlog/src/")
  link_directories("${PROJECT_SOURCE_DIR}/zlog_manager/zlog/src/")
  add_subdirectory (zlog_manager)
  set (EXTRA_LIBS ${EXTRA_LIBS} zlog_manager)
  message (">>> value = ${EXTRA_LIBS}")
endif (USE_ZLOG)


# 其他宏定义选项
option (USE_MYMATH 
        "Use tutorial provided math implementation" ON) 
if (USE_MYMATH)
  include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
  add_subdirectory (MathFunctions)
  set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
  message (">>> value = ${EXTRA_LIBS}")
endif (USE_MYMATH)


# 头文件路径
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/zlog_manager/")
# 因为是使用add_subdirectory的方式，所以库文件路径不用再加link_directories
add_executable (Tutorial tutorial.cxx)
target_link_libraries (Tutorial ${EXTRA_LIBS})

# 安装（库文件安装位置在子cmake文件中指定）
install (TARGETS Tutorial DESTINATION bin)
install (FILES "Tutorial.h" DESTINATION include)

# 测试模式
# enable_testing()
# #define a macro to simplify adding tests, then use it
# macro (do_test arg result)
#   add_test (TutorialComp${arg} Tutorial ${arg})
#   set_tests_properties (TutorialComp${arg}
#     PROPERTIES PASS_REGULAR_EXPRESSION ${result})
# endmacro (do_test)
 
# # do a bunch of result based tests
# do_test (25 "25 is 5")
# do_test (-25 "-25 is 0")