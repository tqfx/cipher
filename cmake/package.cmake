function(package_cpack root)
  set(SOURCE_IGNORE_FILES
    ${root}/.git
    ${root}/.cache
    ${root}/.vscode
    ${root}/.github
    ${root}/.gitignore
    ${root}/.gitmodules
    ${root}/.gitattributes
  )

  if(EXISTS ${root}/.gitignore)
    file(STRINGS ${root}/.gitignore IGNORE NEWLINE_CONSUME)
    string(REPLACE "\n" ";" IGNORE "${IGNORE}")
  else()
    set(IGNORE target cmake-build* build* *.egg-info dist)
  endif()

  foreach(ignore ${IGNORE})
    file(GLOB ignore ${ignore})
    list(APPEND SOURCE_IGNORE_FILES ${ignore})
  endforeach()

  include(InstallRequiredSystemLibraries)
  set(CPACK_SOURCE_IGNORE_FILES ${SOURCE_IGNORE_FILES})
  set(CPACK_RESOURCE_FILE_LICENSE ${root}/LICENSE.txt)
  set(CPACK_RESOURCE_FILE_README ${root}/README.md)
  include(CPack)
endfunction()
