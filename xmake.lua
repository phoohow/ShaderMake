set_languages("c++20")
add_rules("mode.debug", "mode.release")

target("ShaderBlob", function()
    set_kind("shared")
    add_includedirs("include", {public = true})
    add_files("src/ShaderBlob.cpp")
    
    if is_plat("windows") then
        add_defines("SHADERBLOB_EXPORT_DLL")
    end
end)

target("ShaderMake", function()
    set_kind("binary")
    add_files("src/argparse.c", "src/ShaderMake.cpp")
    add_deps("ShaderBlob")
    
    if is_plat("windows") then
        add_links("d3dcompiler", "dxcompiler", "delayimp")
    end
end)

target("ShaderMakeLib", function()
    set_kind("shared")
    add_includedirs("include", {public = true})
    add_files("src/argparse.c", "src/ShaderMakeLib.cpp")
    add_deps("ShaderBlob")
    
    if is_plat("windows") then
        add_defines("SHADERMAKE_EXPORT_DLL")
        add_links("d3dcompiler", "dxcompiler", "delayimp")
    end
end)
