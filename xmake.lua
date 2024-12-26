set_languages("c++20")

target("ShaderBlob")
    set_kind("shared")
    if get_config("buildir")~=nil then 
        set_targetdir(get_config("buildir").."/bin")
    end
    add_includedirs("include", {
        public = true
    })
    add_files("src/ShaderBlob.cpp")
target_end()

target("ShaderMake")
    set_kind("binary")
    if get_config("buildir")~=nil then 
        set_targetdir(get_config("buildir").."/bin")
    end
    add_files("src/argparse.c", "src/ShaderMake.cpp")
    add_deps("ShaderBlob")
    if is_plat("windows") then
        add_links("d3dcompiler", "dxcompiler", "delayimp")
    end
target_end()

target("ShaderMakeLib")
    set_kind("shared")
    if get_config("buildir")~=nil then 
        set_targetdir(get_config("buildir").."/bin")
    end
    add_includedirs("include", {
        public = true
    })
    add_files("src/argparse.c", "src/ShaderMakeLib.cpp")
    add_deps("ShaderBlob")
    if is_plat("windows") then
        add_links("d3dcompiler", "dxcompiler", "delayimp")
    end
target_end()
