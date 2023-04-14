add_subdirectory(liba)

if(NOT HAS_CJSON_H)
  add_subdirectory(cjson)
endif()

if(NOT HAS_GETOPT_H)
  add_subdirectory(getopt)
endif()

if(NOT HAS_SQLITE3_H)
  add_subdirectory(sqlite3)
endif()
