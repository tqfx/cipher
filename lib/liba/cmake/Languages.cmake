set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Enable/Disable output of compile commands during generation.")
mark_as_advanced(CMAKE_EXPORT_COMPILE_COMMANDS)
if(NOT DEFINED CACHE{LIBA_CXX})
  file(GLOB_RECURSE has_cxx *.cc *.hh *.cpp *.hpp *.cxx *.hxx *.ixx *.ipp *.tpp *.mpp *.cppm)
  if(has_cxx)
    option(LIBA_CXX "Enable/Disable C++" 1)
  else()
    option(LIBA_CXX "Enable/Disable C++" 0)
  endif()
  set(has_cxx)
endif()
list(FIND LANGUAGES CXX index)
if(${index} EQUAL -1 AND LIBA_CXX)
  list(INSERT LANGUAGES 0 CXX)
endif()
list(FIND LANGUAGES C index)
if(${index} EQUAL -1)
  list(INSERT LANGUAGES 0 C)
endif()
# default C standard
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 11)
endif()
if(NOT CMAKE_C_EXTENSIONS)
  set(CMAKE_C_EXTENSIONS 0)
endif()
if(NOT CMAKE_C_STANDARD_REQUIRED)
  set(CMAKE_C_STANDARD_REQUIRED 0)
endif()
# default C++ standard
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
endif()
if(NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS 0)
endif()
if(NOT CMAKE_CXX_STANDARD_REQUIRED)
  set(CMAKE_CXX_STANDARD_REQUIRED 0)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "TinyCC")
  string(FIND CMAKE_C_FLAGS "-std=c${CMAKE_C_STANDARD}" index)
  if(${index} EQUAL -1)
    string(STRIP "${CMAKE_C_FLAGS} -std=c${CMAKE_C_STANDARD}" CMAKE_C_FLAGS)
  endif()
endif()
set(index)
