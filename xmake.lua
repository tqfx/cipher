set_project("cipher")

set_version("0.1.0", {build = "%Y%m%d%H%M"})

set_description("A cipher generator")

set_languages("c11", "c++17")

includes("check_cincludes.lua")

configvar_check_cincludes("HAS_GETOPT_H", "getopt.h")

add_rules("mode.check", "mode.debug", "mode.release")

target("cipher")

    if not get_config("__HAS_GETOPT_H") then
        add_deps("getopt")
    end

    add_deps("sqlite3", "cjson", "a")

    set_warnings("everything")

    set_kind("binary")

    add_defines("_POSIX_C_SOURCE=200809L")

    add_includedirs("lib", {private = true})

    add_includedirs("include", {public = true})

    add_headerfiles("include/(**.h)")

    add_files("src/**.c")

    add_cxflags("-Wno-reserved-identifier")
    add_cxflags("-Wno-used-but-marked-unused")
    add_cflags("-Wno-declaration-after-statement")
    add_cxxflags("-Wno-c++98-compat-pedantic")

target_end()

-- include library sources
includes("lib")
