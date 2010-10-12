include(${QT_USE_FILE})
set(MathTools_SRCS
  MathTools.cpp
  MathFunction.cpp
  main.cpp
  Plotter.cpp
  parser.cpp
  parser_helper.cpp
)

set(MathTools_MOC_HDRS
  MathTools.hpp
  Plotter.hpp
)

set(MathTools_UIS
  mathtools.ui
)

add_subdirectory(qtcolorcombobox)

qt4_wrap_ui(MathTools_UIS_H ${MathTools_UIS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
qt4_wrap_cpp(MathTools_MOC_SRCS ${MathTools_MOC_HDRS})
add_executable(MathTools ${MathTools_SRCS} ${MathTools_UIS_H} ${MathTools_MOC_SRCS})
target_link_libraries(MathTools ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} MathTools_qtcolorcombo)
