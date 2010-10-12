include(${QT_USE_FILE})
set(MathTools_qtcolorcombo_SRCS
  qtcolorcombobox.cpp
)

set(MathTools_qtcolorcombo_MOC_HDRS
  qtcolorcombobox.hpp
)

#include_directories(${CMAKE_CURRENT_BINARY_DIR})
qt4_wrap_cpp(MathTools_qtcolorcombo_MOC_SRCS ${MathTools_qtcolorcombo_MOC_HDRS})
add_library(MathTools_qtcolorcombo ${MathTools_qtcolorcombo_SRCS} ${MathTools_qtcolorcombo_MOC_SRCS})