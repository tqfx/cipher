function(unit_test source target)
  set(TARGET test-${target})
  add_executable(${TARGET} ${source})
  set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${target})
  target_link_libraries(${TARGET} PRIVATE cksum)
  add_test(NAME ${TARGET} COMMAND $<TARGET_FILE:${TARGET}> ${ARGN})
endfunction()

unit_test(crc.c crc crc_tab.c)

unit_test(hash.c hash)
unit_test(hmac.c hmac)
