cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{VCPKG_INSTALLATION_ROOT} AND VCPKG_TARGET_TRIPLET)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

if("${VCPKG_TARGET_TRIPLET}" MATCHES "x86-mingw(.*)")
  include(cmake/mingw32.cmake)
elseif("${VCPKG_TARGET_TRIPLET}" MATCHES "x64-mingw(.*)")
  include(cmake/mingw64.cmake)
endif()

project(cipher VERSION 0.1.1 LANGUAGES C DESCRIPTION "A cipher generator" HOMEPAGE_URL "https://github.com/tqfx/cipher.git")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Specifies the build type on single-configuration generators." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

include(CheckIncludeFiles)
check_include_files(getopt.h HAS_GETOPT_H)

if(BUILD_SHARED_LIBS)
  check_include_files(sqlite3.h HAS_SQLITE3_H)
  check_include_files(cjson/cJSON.h HAS_CJSON_H)
endif()

add_subdirectory(lib)

include(cmake/core.cmake)
include(cmake/target.cmake)
include(cmake/package.cmake)
include(cmake/standard.cmake)

option(ENABLE_DOXYGEN "Enable doxygen" ON)
option(ENABLE_WARNINGS "Enable warnings" OFF)
option(ENABLE_SANITIZE "Enable sanitize" OFF)
option(ENABLE_CPPCHECK "Enable cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
option(ENABLE_IYWU "Enable include-what-you-use" OFF)
option(ENABLE_IPO "Enable interprocedural optimization" OFF)

if(ENABLE_WARNINGS)
  include(cmake/warnings.cmake)
endif()

if(ENABLE_SANITIZE)
  include(cmake/sanitize.cmake)
endif()

if(ENABLE_DOXYGEN)
  find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)
endif()

if(ENABLE_IPO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ENABLE_IPO)
endif()

file(GLOB SOURCES include/cipher/*.h src/*.[ch])

add_library(cipher ${SOURCES})
add_library(${PROJECT_NAME}::cipher ALIAS cipher)
target_compile_definitions(cipher PUBLIC _CRT_SECURE_NO_WARNINGS)
target_include_directories(cipher PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/lib>
)
target_link_libraries(cipher PUBLIC sqlite3 cjson alib)
target_library_options(cipher)

file(GLOB_RECURSE SOURCES src/cli/*.[ch])

add_executable(cipher-cli ${SOURCES})
set_target_properties(cipher-cli PROPERTIES RUNTIME_OUTPUT_NAME cipher)
target_compile_definitions(cipher-cli PRIVATE _POSIX_C_SOURCE=200809L)
target_link_libraries(cipher-cli PRIVATE cipher
  $<$<NOT:$<BOOL:${HAS_GETOPT_H}>>:getopt>
)
target_executable_options(cipher-cli)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
  DESTINATION include FILES_MATCHING
  REGEX ".*\.\(h\|hh\|hxx\|hpp\)$"
)
install(TARGETS cipher cipher-cli
  EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(EXPORT ${PROJECT_NAME}-targets
  DESTINATION lib/cmake/${PROJECT_NAME}
  FILE ${PROJECT_NAME}-targets.cmake
  NAMESPACE ${PROJECT_NAME}::
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
  DESTINATION lib/pkgconfig
)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.pc
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
  @ONLY
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(test)
endif()

package_cpack(${CMAKE_CURRENT_LIST_DIR})

if(CLANG_FORMAT)
  file(GLOB_RECURSE SOURCES include/*.h* src/*.[ch]* test/*.[ch]*)
  add_custom_target(format # https://clang.llvm.org/docs/ClangFormat.html
    COMMAND ${CLANG_FORMAT} --style=file -i ${SOURCES} --verbose
    COMMENT "Formatting code using ClangFormat"
  )
endif()

if(DOXYGEN_FOUND)
  # Project related configuration options
  set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)

  # Build related configuration options
  # Configuration options related to warning and progress messages
  set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
  set(DOXYGEN_WARN_NO_PARAMDOC YES)
  set(DOXYGEN_WARN_LOGFILE doc.log)

  # Configuration options related to the input files
  set(DOXYGEN_RECURSIVE YES)
  set(DOXYGEN_EXCLUDE_SYMBOLS "__*")

  # Configuration options related to source browsing
  # Configuration options related to the alphabetical class index
  # Configuration options related to the HTML output
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_USE_MATHJAX YES)

  # Configuration options related to the LaTeX output
  # Configuration options related to the RTF output
  # Configuration options related to the man page output
  # Configuration options related to the XML output
  # Configuration options related to the DOCBOOK output
  # Configuration options for the AutoGen Definitions output
  # Configuration options related to Sqlite3 output
  # Configuration options related to the Perl module output
  # Configuration options related to the preprocessor
  set(DOXYGEN_MACRO_EXPANSION YES)
  set(DOXYGEN_EXPAND_ONLY_PREDEF YES)
  set(DOXYGEN_PREDEFINED
    "__STDC_VERSION__=201112L"
    "__cplusplus=201703L"
    "__STDC_HOSTED__=1"
  )

  # Configuration options related to external references
  # Configuration options related to the dot tool
  if(Doxygen_dot_FOUND)
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_INTERACTIVE_SVG YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
  endif()

  # https://www.doxygen.nl/manual/config.html
  doxygen_add_docs(docs WORKING_DIRECTORY
    ${CMAKE_CURRENT_LIST_DIR}/include/cipher
    ${CMAKE_CURRENT_LIST_DIR}/include/cipher
    ${CMAKE_CURRENT_LIST_DIR}/README.md
  )
endif()
