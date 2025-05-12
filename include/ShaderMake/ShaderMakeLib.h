#pragma once
#include <vector>
#include <string>

#ifdef _WIN32
#    ifdef SHADERMAKE_EXPORT_DLL
#        define SHADERMAKE_API __declspec(dllexport)
#    else
#        define SHADERMAKE_API __declspec(dllimport)
#    endif
#else
#    define SHADERMAKE_API
#endif

SHADERMAKE_API int  createShaderBinary(int argc, char** argv);
SHADERMAKE_API bool createShaderBinary(std::vector<std::string> argv);
