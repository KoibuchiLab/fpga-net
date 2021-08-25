FILE(REMOVE_RECURSE
  "src/internal_config.h"
  "include/smpi/mpif.h"
  "include/simgrid/config.h"
  "bin/smpicc"
  "bin/smpicxx"
  "bin/smpiff"
  "bin/smpif90"
  "bin/smpirun"
  "bin/colorize"
  "bin/simgrid_update_xml"
  "examples/smpi/tracing/smpi_traced.trace"
  "src/cunit_unit.cpp"
  "src/dynar_unit.cpp"
  "src/dict_unit.cpp"
  "src/xbt_str_unit.cpp"
  "src/config_unit.cpp"
  "src/simgrid_units_main.c"
  "CMakeFiles/testall.dir/src/cunit_unit.cpp.o"
  "CMakeFiles/testall.dir/src/dynar_unit.cpp.o"
  "CMakeFiles/testall.dir/src/dict_unit.cpp.o"
  "CMakeFiles/testall.dir/src/xbt_str_unit.cpp.o"
  "CMakeFiles/testall.dir/src/config_unit.cpp.o"
  "CMakeFiles/testall.dir/src/simgrid_units_main.c.o"
  "testall.pdb"
  "testall"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C CXX)
  INCLUDE(CMakeFiles/testall.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
