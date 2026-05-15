# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles/aicad_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/aicad_autogen.dir/ParseCache.txt"
  "aicad_autogen"
  )
endif()
