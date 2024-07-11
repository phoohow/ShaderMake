#pragma once

#include <vector>
#include <string>
#include <cstdint>

#ifdef _WIN32
#    define EXPORT_DLL __declspec(dllexport)
#else
#    define EXPORT_DLL
#endif

namespace shaderMake
{

struct ShaderConstant
{
    const char* name;
    const char* value;
};

struct ShaderBlobEntry
{
    uint32_t permutationSize;
    uint32_t dataSize;
};

EXPORT_DLL bool FindPermutationInBlob(
    const void*           blob,
    size_t                blobSize,
    const ShaderConstant* constants,
    uint32_t              numConstants,
    const void**          pBinary,
    size_t*               pSize);

EXPORT_DLL void EnumeratePermutationsInBlob(
    const void*               blob,
    size_t                    blobSize,
    std::vector<std::string>& permutations);

EXPORT_DLL std::string FormatShaderNotFoundMessage(
    const void*           blob,
    size_t                blobSize,
    const ShaderConstant* constants,
    uint32_t              numConstants);

typedef bool (*WriteFileCallback)(const void* data, size_t size, void* context);

EXPORT_DLL bool WriteFileHeader(
    WriteFileCallback write,
    void*             context);

EXPORT_DLL bool WritePermutation(
    WriteFileCallback  write,
    void*              context,
    const std::string& permutationKey,
    const void*        binary,
    size_t             binarySize);

} // namespace shaderMake
