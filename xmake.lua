set_project("rara")
set_version("0.0.0")

add_rules("mode.debug", "mode.release")
set_defaultarchs("x64")
set_languages("cxx2a")
set_defaultmode("debug")
set_toolchains("clang-cl")

add_requires("vcpkg::spdlog", { alias = "spdlog" })
add_requires("vcpkg::fmt", { alias = "fmt" })
add_requires("vcpkg::tl-expected", { alias = "tl-expected" })
add_requires("vcpkg::bext-ut", { alias = "bext-ut" })
add_requires("vcpkg::parallel-hashmap", { alias = "parallel-hashmap" })
--add_requires("vcpkg::llvm >=14.0.6", { alias = "llvm", configs = {
--    baseline = "c90d27a6f470da5f6d1b64d13a978ef493e9a3db",
--    default_features = false,
--    features = {
--        "default-options",
--        "default-targets",
--        "tools",
--        "utils"
--    },
--    ["default-features"] = false,
--} })

function p()
    -- vcpkg packages
    add_packages("spdlog", "fmt", "tl-expected", "parallel-hashmap")
    --add_packages("llvm")
    -- debug packages
    add_packages("catch2")

end

function f()
    add_files("src/*.cpp|main.cpp", "src/**/*.cpp")
end

target("rara")
set_arch("x64")
set_kind("binary")
add_includedirs("include", { public = true })
f()
add_files("src/main.cpp")
set_rundir("$(projectdir)")
p()
target_end()

target("rara-test")
add_defines("ENV_TEST")
set_arch("x64")
set_kind("binary")
add_includedirs("include", { public = true })
f()
add_files("tests/*.cpp|_*.cpp")
set_rundir("$(projectdir)")
p()
-- test packages
add_packages("bext-ut")
target_end()