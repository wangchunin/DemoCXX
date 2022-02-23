add_library(MathFunctions SHARED mysqrt.cxx)

install (TARGETS MathFunctions DESTINATION lib)
install (FILES MathFunctions.h DESTINATION include)