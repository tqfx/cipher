get_property(enabled_languages GLOBAL PROPERTY ENABLED_LANGUAGES)

if(
  "${CMAKE_C_COMPILER_ID}" MATCHES "MSVC" OR
  "${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC"
)
  foreach(v /W4 /sdl)
    string_append(CMAKE_C_FLAGS ${v})
    string_append(CMAKE_CXX_FLAGS ${v})
  endforeach()
else()
  foreach(v -Wall -Wextra -Wpedantic)
    string_append(CMAKE_C_FLAGS ${v})
    string_append(CMAKE_CXX_FLAGS ${v})
  endforeach()
endif()

foreach(v ${C_FLAGS})
  string_append(CMAKE_C_FLAGS ${v})
endforeach()

foreach(v ${CXX_FLAGS})
  string_append(CMAKE_CXX_FLAGS ${v})
endforeach()

if(ENABLED_IPO)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

foreach(lang ${enabled_languages})
  if(ENABLE_IYWU AND INCLUDE_WHAT_YOU_USE)
    set(CMAKE_${lang}_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
  endif()

  if(ENABLE_CLANG_TIDY AND CLANG_TIDY)
    set(CMAKE_${lang}_CLANG_TIDY ${CLANG_TIDY} --fix)
  endif()

  if(ENABLE_CPPCHECK AND CPPCHECK)
    set(CMAKE_${lang}_CPPCHECK ${CPPCHECK} --enable=warning,performance)
  endif()

  if(CMAKE_${lang}_LINK_OPTIONS_NO_PIE AND NOT ANDROID)
    string_append(CMAKE_${lang}_LINK_FLAGS ${CMAKE_${lang}_LINK_OPTIONS_NO_PIE})
  endif()
endforeach()

add_subdirectory(cksum)
add_subdirectory(cipher)
