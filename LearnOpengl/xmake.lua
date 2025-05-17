add_rules("mode.debug", "mode.release")
add_languages("c++20")
set_optimize("fastest")
add_rules("plugin.compile_commands.autoupdate")
add_requires("glfw", "glad", "assimp", "glew", "glm", "spdlog", "nlohmann_json", "rttr")
add_requires("imgui", {configs = {glfw_opengl3 = true, opengl3 = true}})

-- 定义复制规则
rule("copy_config")
    on_run(function (target)
        local output_dir = target:targetdir()
        os.cp(is_plat("linux") and "config_linux.json" or "config_win.json", output_dir)
    end)

target("learnopengl")
    set_kind("binary")
    add_files(
        "src/*.cpp", 
        "src/application/*.cpp", 
        "src/core/src/*.cpp"
        )
    add_includedirs(
        "src/application",
        "src/core/include",
        "src/modules/keyboard/include"
        )
    add_packages("glfw", "imgui", "glad", "assimp", "glew", "glm", "spdlog", "nlohmann_json", "rttr")
    add_rules("copy_config", {always_run = true}) 