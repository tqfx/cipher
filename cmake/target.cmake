find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)
find_program(CLANG_FORMAT clang-format)
find_program(CLANG_TIDY clang-tidy)
find_program(CPPCHECK cppcheck)
find_program(CPPLINT cpplint)

function(target_library_options target)
  get_property(enabled_languages GLOBAL PROPERTY ENABLED_LANGUAGES)
  unset(languages)

  foreach(lang C CXX)
    if(${lang} IN_LIST enabled_languages)
      list(APPEND languages ${lang})
    endif()
  endforeach()

  foreach(lang ${languages})
    set_target_properties(${target} PROPERTIES ${lang}_VISIBILITY_PRESET hidden)

    if(ENABLE_IYWU AND INCLUDE_WHAT_YOU_USE)
      set_target_properties(${target} PROPERTIES ${lang}_INCLUDE_WHAT_YOU_USE "${INCLUDE_WHAT_YOU_USE}")
    endif()

    if(ENABLE_CLANG_TIDY AND CLANG_TIDY)
      set_target_properties(${target} PROPERTIES ${lang}_CLANG_TIDY "${CLANG_TIDY};--fix")
    endif()

    if(ENABLE_CPPCHECK AND CPPCHECK)
      set_target_properties(${target} PROPERTIES ${lang}_CPPCHECK "${CPPCHECK};--enable=warning,performance")
    endif()

    target_compile_options(${target} PRIVATE
      $<$<${lang}_COMPILER_ID:GNU,Clang,AppleClang,IntelLLVM>:-Wall -Wextra -Wpedantic>
      $<$<${lang}_COMPILER_ID:MSVC>:/W4 /sdl> $<$<COMPILE_LANGUAGE:${lang}>:${${lang}_FLAGS}>
    )
  endforeach()

  if(ENABLE_IPO)
    set_target_properties(${target} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
  endif()
endfunction()

function(target_executable_options target)
  get_property(enabled_languages GLOBAL PROPERTY ENABLED_LANGUAGES)
  unset(languages)

  foreach(lang C CXX)
    if(${lang} IN_LIST enabled_languages)
      list(APPEND languages ${lang})
    endif()
  endforeach()

  foreach(lang ${languages})
    if(ENABLE_IYWU AND INCLUDE_WHAT_YOU_USE)
      set_target_properties(${target} PROPERTIES ${lang}_INCLUDE_WHAT_YOU_USE "${INCLUDE_WHAT_YOU_USE}")
    endif()

    if(ENABLE_CLANG_TIDY AND CLANG_TIDY)
      set_target_properties(${target} PROPERTIES ${lang}_CLANG_TIDY "${CLANG_TIDY};--fix")
    endif()

    if(ENABLE_CPPCHECK AND CPPCHECK)
      set_target_properties(${target} PROPERTIES ${lang}_CPPCHECK "${CPPCHECK};--enable=warning,performance")
    endif()

    target_compile_options(${target} PRIVATE
      $<$<${lang}_COMPILER_ID:GNU,Clang,AppleClang,IntelLLVM>:-Wall -Wextra -Wpedantic>
      $<$<${lang}_COMPILER_ID:MSVC>:/W4 /sdl> $<$<COMPILE_LANGUAGE:${lang}>:${${lang}_FLAGS}>
    )

    target_link_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:${lang}>:${${lang}_FLAGS}>)

    if(MINGW)
      target_link_options(cipher-cli PRIVATE
        -static-libgcc $<$<COMPILE_LANGUAGE:CXX>:-static-libstdc++> -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive
      )
    endif()
  endforeach()

  if(ENABLE_IPO)
    set_target_properties(${target} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
  endif()
endfunction()
