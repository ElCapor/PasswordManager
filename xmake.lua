add_rules("mode.release", "mode.debug")

set_languages("cxx17")

target("core")
    set_kind("static")
    add_files("core/*.cpp")
    add_includedirs("core/include")
    add_defines("BINARY_VERBOSE", "BINARY_WARN_EMPTY", "ERROR_LEVEL=0")

target("cli")
    add_deps("core")
    set_kind("binary")
    add_files("cli/*.cpp")
    add_includedirs("cli/include")
    add_includedirs("core/include")

    local remove = true
    if remove then 
        after_build(function (target)
            os.rm(target:targetdir().."/res/master.bin")
            os.rm(target:targetdir().."/res/accounts.bin")
            os.cp(os.scriptdir().."/res", target:targetdir())
        end)
    end