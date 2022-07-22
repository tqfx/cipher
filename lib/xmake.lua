target("cjson")

    set_default(false)

    set_kind("static")

    add_includedirs("cjson", {public = true})

    add_files("cjson/*.c")

target_end()

target("getopt")

    set_default(false)

    set_kind("static")

    add_includedirs("getopt", {public = true})

    add_files("getopt/*.c")

target_end()

target("sqlite3")

    set_default(false)

    set_kind("static")

    add_defines("SQLITE_ENABLE_UNLOCK_NOTIFY", "SQLITE_ENABLE_COLUMN_METADATA")

    add_includedirs("sqlite3", {public = true})

    add_files("sqlite3/*.c")

    if is_plat("windows", "mingw") then
        add_defines("SQLITE_OS_WIN=1")
    else
        add_defines("SQLITE_OS_UNIX=1")
    end

    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
    end

target_end()
