include(${QT_USE_FILE})
set(MathTools_SRCS
  MathTools.cpp
  functions_ginac.cpp
# (template definitions, included in the header file)
#    functor_builder.cpp
#    MathFunction.cpp
  main.cpp
  Plotter.cpp
  FunctionPlotter.cpp
  PlotTab.cpp  
  PlotterHelper.cpp
  ExportDialog.cpp
  Exporter.cpp
  BinomialDistTab.cpp
  BinomialDistPlotter.cpp
  HypTestTab.cpp
  HypTestPlotter.cpp
  ComplexPlotter.cpp
  ComplexTab.cpp
)

set(MathTools_MOC_HDRS
  MathTools.hpp
  PlotTab.hpp
  Plotter.hpp
  FunctionPlotter.hpp
  ExportDialog.hpp
  BinomialDistTab.hpp
  HypTestTab.hpp
  HypTestPlotter.hpp
  About.hpp
  ComplexTab.hpp
)

set(MathTools_UIS
  mathtoolsmain.ui
  plottab.ui
  exportdialog.ui
  binomialdist.ui
  hyptest.ui
  complextab.ui
  about.ui
)

add_subdirectory(qtcolorcombobox)

qt4_wrap_ui(MathTools_UIS_H ${MathTools_UIS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
qt4_wrap_cpp(MathTools_MOC_SRCS ${MathTools_MOC_HDRS})
add_executable(MathTools ${MathTools_SRCS} ${MathTools_UIS_H} ${MathTools_MOC_SRCS})
target_link_libraries(MathTools ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} MathTools_qtcolorcombo ginac gsl)
set_target_properties(MathTools PROPERTIES COMPILE_FLAGS "-frepo")

