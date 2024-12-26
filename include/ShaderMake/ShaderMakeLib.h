#pragma once
#include <vector>
#include <string>

#ifdef _WIN32
#    define EXPORT_DLL __declspec(dllexport)
#else
#    define EXPORT_DLL
#endif

EXPORT_DLL int  createShaderBinary(int argc, char** argv);
EXPORT_DLL bool createShaderBinary(std::vector<std::string> argv);
