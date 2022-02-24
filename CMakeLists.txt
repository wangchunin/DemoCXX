cmake_minimum_required (VERSION 3.10)
# 交叉编译相关
# # 即你目标机target所在的操作系统名称，比如ARM或者Linux你就需要写"Linux",如果Windows平台你就写"Windows",如果你的嵌入式平台没有相关OS你即需要写成"Generic",只有当CMAKE_SYSTEM_NAME这个变量被设置了，CMake才认为此时正在交叉编译，它会额外设置一个变量CMAKE_CROSSCOMPILING为TRUE.
# SET(CMAKE_SYSTEM_NAME Linux)

# # 即C语言编译器，这里可以将变量设置成完整路径或者文件名，设置成完整路径有一个好处就是CMake会去这个路径下去寻找编译相关的其他工具比如linker,binutils等，如果你写的文件名带有arm-elf等等前缀，CMake会识别到并且去寻找相关的交叉编译器。
# SET(CMAKE_C_COMPILER   /opt/arm/usr/bin/ppc_74xx-gcc)
# SET(CMAKE_CXX_COMPILER /opt/arm/usr/bin/ppc_74xx-g++)

# # 代表了一系列的相关文件夹路径的根路径的变更，比如你设置了/opt/arm/,所有的Find_xxx.cmake都会优先根据这个路径下的/usr/lib,/lib等进行查找，然后才会去你自己的/usr/lib和/lib进行查找，如果你有一些库是不被包含在/opt/arm里面的，你也可以显示指定多个值给CMAKE_FIND_ROOT_PATH,比如set(CMAKE_FIND_ROOT_PATH /opt/arm /opt/inst)
# set(CMAKE_FIND_ROOT_PATH /opt/arm /opt/inst)
# SET(CMAKE_FIND_ROOT_PATH  /opt/arm/ppc_74xx /home/rickk/arm_inst)

# # 有三种取值，NEVER,ONLY,BOTH,第一个表示不在你CMAKE_FIND_ROOT_PATH下进行查找，第二个表示只在这个路径下查找，第三个表示先查找这个路径，再查找全局路径，对于这个变量来说，一般都是调用宿主机的程序，所以一般都设置成NEVER
# SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# # 对FIND_LIBRARY()起作用，表示在链接的时候的库的相关选项，因此这里需要设置成ONLY来保证我们的库是在交叉环境中找的.
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# # 对FIND_PATH()和FIND_FILE()起作用，一般来说也是ONLY,如果你想改变，一般也是在相关的FIND命令中增加option来改变局部设置，有NO_CMAKE_FIND_ROOT_PATH,ONLY_CMAKE_FIND_ROOT_PATH,BOTH_CMAKE_FIND_ROOT_PATH
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# # 对于Qt用户来说，需要更改相关的qmake命令切换成嵌入式版本，因此这里需要指定成相应的qmake路径（指定到qmake本身）
# SET(QT_QMAKE_EXECUTABLE /opt/qt-embedded/qmake)
# # 对于需要boost库的用户来说，相关的boost库路径配置也需要设置，因此这里的路径即ARM下的boost路径，里面有include和lib。
# SET(BOOST_ROOT /opt/boost_arm)
# cmake内置命令不区分大小写

# 项目名称
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
  add_subdirectory (zlog_manager)
  set (EXTRA_INCS ${EXTRA_INCS} ${ZLOG_INCLUDE_DIR})
  message (">>> value = ${EXTRA_INCS}")
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
include_directories("${EXTRA_INCS}")
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