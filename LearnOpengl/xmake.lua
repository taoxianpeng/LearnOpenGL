add_rules("mode.debug", "mode.release")
add_languages("c++20")
set_optimize("fastest")
add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")
add_requires("GL", "glfw", "glad", "assimp", "glew", "glm", "spdlog", "nlohmann_json")
add_requires("imgui", {configs = {glfw_opengl3 = true, opengl3 = true}})

-- 定义复制规则
rule("copy_config")
    on_build(function (target, rule)
        local output_dir = target:targetdir()
        if is_plat("linux") then
            os.cp("$(projectdir)/config_linux.json", output_dir)
        elseif is_plat("windows") then
            os.cp("$(projectdir)/config_win.json", output_dir)
        end
    end)

target("learnopengl")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs(src)
    add_packages("GL", "glfw", "imgui", "glad", "assimp", "glew", "glm", "spdlog", "nlohmann_json")
    add_rules("copy_config")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

