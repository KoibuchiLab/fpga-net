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
  "CMakeFiles/simgrid-colorizer"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/simgrid-colorizer.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)