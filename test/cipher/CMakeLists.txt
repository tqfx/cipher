function(unit_test source target)
  set(TARGET test-${target})
  add_executable(${TARGET} ${source})
  set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${target})
  target_link_libraries(${TARGET} PRIVATE cipher)
  add_test(NAME ${TARGET} COMMAND $<TARGET_FILE:${TARGET}> ${ARGN})
endfunction()

unit_test(cipher.c cipher)

unit_test(word.c word)
unit_test(info.c info)

unit_test(sqlite.c sqlite)
unit_test(json.c json)
