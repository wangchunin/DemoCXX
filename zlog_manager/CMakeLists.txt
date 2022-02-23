add_library(zlog_manager SHARED log_manager.cxx)
set(EXTRA_LIBS
    pthread
    #"-Wl,--whole-archive" #告诉编译器，从这里开始，所有的库的内容都包含到so中，如果后续想把zlog和manager打包到一起，需要这两句话
    libzlog.a
    #"-Wl,--no-whole-archive" #告诉编译器，从这里开始，以后的库的内容不用都包含到so中 
)
target_link_libraries(zlog_manager
    ${EXTRA_LIBS}
)
install (TARGETS zlog_manager DESTINATION lib)
install (FILES log_manager.h DESTINATION include)