/*
* Copyright (c) 2014-2021, NVIDIA CORPORATION. All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <vector>
#include <string>
#include <cstdint>

#ifdef _WIN32
#    ifdef SHADERBLOB_EXPORT_DLL
#        define SHADERBLOB_API __declspec(dllexport)
#    else
#        define SHADERBLOB_API __declspec(dllimport)
#    endif
#else
#    define SHADERBLOB_API
#endif

namespace ShaderMake
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

SHADERBLOB_API
bool FindPermutationInBlob(
    const void* blob,
    size_t blobSize,
    const ShaderConstant* constants,
    uint32_t numConstants,
    const void** pBinary,
    size_t* pSize
);

SHADERBLOB_API
void EnumeratePermutationsInBlob(
    const void* blob,
    size_t blobSize,
    std::vector<std::string>& permutations
);

SHADERBLOB_API
std::string FormatShaderNotFoundMessage(
    const void* blob,
    size_t blobSize,
    const ShaderConstant* constants,
    uint32_t numConstants
);

typedef bool (*WriteFileCallback)(const void* data, size_t size, void* context);

SHADERBLOB_API
bool WriteFileHeader(
	WriteFileCallback write,
    void* context
);

SHADERBLOB_API
bool WritePermutation(
	WriteFileCallback write,
    void* context,
	const std::string& permutationKey,
	const void* binary,
	size_t binarySize
);

} // namespace ShaderMake
