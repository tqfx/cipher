find_program(CMAKE_C_COMPILER clang)
find_program(CMAKE_CXX_COMPILER clang++)

if(WIN32)
  set(CMAKE_C_COMPILER_TARGET x86_64-w64-windows-gnu CACHE STRING "Specifies the C comipler target." FORCE)
  set(CMAKE_CXX_COMPILER_TARGET x86_64-w64-windows-gnu CACHE STRING "Specifies the C++ comipler target." FORCE)
endif()
