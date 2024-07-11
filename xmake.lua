set_languages("c++20")

target("shaderBlob")
    set_kind("shared")
    if get_config("buildir")~=nil then 
        set_targetdir(get_config("buildir").."/bin")
    end
    add_includedirs("include", {
        public = true
    })
    add_files("src/shaderBlob.cpp")
target_end()

target("shaderMake")
    set_kind("binary")
    if get_config("buildir")~=nil then 
        set_targetdir(get_config("buildir").."/bin")
    end
    add_files("src/argparse.c", "src/shaderMake.cpp")
    add_deps("shaderBlob")
    if is_plat("windows") then
        add_links("d3dcompiler", "dxcompiler", "delayimp")
    end
target_end()

target("shaderMakeLib")
    set_kind("shared")
    if get_config("buildir")~=nil then 
        set_targetdir(get_config("buildir").."/bin")
    end
    add_includedirs("include", {
        public = true
    })
    add_files("src/argparse.c", "src/shaderMakeLib.cpp")
    add_deps("shaderBlob")
    if is_plat("windows") then
        add_links("d3dcompiler", "dxcompiler", "delayimp")
    end
target_end()
