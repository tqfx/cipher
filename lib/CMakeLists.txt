if(NOT unofficial-sqlite3_FOUND)
  add_subdirectory(sqlite3)
endif()

if(NOT cJSON_FOUND)
  add_subdirectory(cjson)
endif()
