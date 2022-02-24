# 调用zlog的makefile文件
# zlog 位置
set(ZLOG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlog)
set(ZLOG_LIBRARY ${ZLOG_SOURCE_DIR}/src/libzlog.a)
add_custom_target(build_zlog ALL
    # 加入交叉编译参数
    COMMAND ${CMAKE_MAKE_PROGRAM} -e CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER}
    WORKING_DIRECTORY ${ZLOG_SOURCE_DIR}
    COMMENT "Original zlog makefile target")
add_library(zlog STATIC IMPORTED)
set_property(TARGET zlog APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(zlog PROPERTIES IMPORTED_LOCATION_NOCONFIG "${ZLOG_LIBRARY}")
add_dependencies(zlog build_zlog)
# zlog编译完成

# 头文件路径传递到父cmake文件中
set(ZLOG_INCLUDE_DIR "${ZLOG_SOURCE_DIR}/src/")
set(ZLOG_INCLUDE_DIR ${ZLOG_INCLUDE_DIR} PARENT_SCOPE)
# 每次连接都需要头文件，现在需要，最后连接还是需要，所以上传路径到父cmake中
include_directories (${ZLOG_INCLUDE_DIR})
add_library(zlog_manager SHARED log_manager.cxx)
set(EXTRA_LIBS
    pthread
    #"-Wl,--whole-archive" #告诉编译器，从这里开始，所有的库的内容都包含到so中，如果后续想把zlog和manager打包到一起，需要这两句话
    zlog
    #"-Wl,--no-whole-archive" #告诉编译器，从这里开始，以后的库的内容不用都包含到so中 
)
target_link_libraries(zlog_manager
    ${EXTRA_LIBS}
)
install (TARGETS zlog_manager DESTINATION lib)
install (FILES log_manager.h DESTINATION include)