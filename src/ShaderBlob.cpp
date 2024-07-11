#include <shaderMake/ShaderBlob.h>

#include <sstream>
#include <cstring>

namespace shaderMake
{

static const char* g_BlobSignature     = "NVSP";
static size_t      g_BlobSignatureSize = 4;

bool FindPermutationInBlob(const void* blob, size_t blobSize, const ShaderConstant* constants, uint32_t numConstants, const void** pBinary, size_t* pSize)
{
    if (!blob || blobSize < g_BlobSignatureSize)
        return false;

    if (!pBinary || !pSize)
        return false;

    if (memcmp(blob, g_BlobSignature, g_BlobSignatureSize) != 0)
    {
        if (numConstants == 0)
        {
            *pBinary = blob;
            *pSize   = blobSize;

            return true; // this blob is not a permutation blob, and no permutation is requested
        }
        else
            return false; // this blob is not a permutation blob, but the caller requested a permutation
    }

    blob = static_cast<const char*>(blob) + g_BlobSignatureSize;
    blobSize -= g_BlobSignatureSize;

    std::stringstream ss;
    for (uint32_t n = 0; n < numConstants; n++)
    {
        const ShaderConstant& constant = constants[n];
        ss << constant.name << "=" << constant.value;
        if (n + 1 < numConstants)
            ss << " ";
    }
    std::string permutation = ss.str();

    while (blobSize > sizeof(ShaderBlobEntry))
    {
        const ShaderBlobEntry* header = static_cast<const ShaderBlobEntry*>(blob);

        if (header->dataSize == 0)
            return false; // last header in the blob is empty

        if (blobSize < sizeof(ShaderBlobEntry) + header->dataSize + header->permutationSize)
            return false; // insufficient bytes in the blob, cannot continue

        const char* entryPermutation = static_cast<const char*>(blob) + sizeof(ShaderBlobEntry);

        if ((header->permutationSize == permutation.size()) && ((permutation.size() == 0) || (strncmp(entryPermutation, permutation.data(), permutation.size()) == 0)))
        {
            const char* binary = static_cast<const char*>(blob) + sizeof(ShaderBlobEntry) + header->permutationSize;

            *pBinary = binary;
            *pSize   = header->dataSize;

            return true;
        }

        size_t offset = sizeof(ShaderBlobEntry) + header->dataSize + header->permutationSize;
        blob          = static_cast<const char*>(blob) + offset;
        blobSize -= offset;
    }

    return false; // went through the blob, permutation not found
}

void EnumeratePermutationsInBlob(const void* blob, size_t blobSize, std::vector<std::string>& permutations)
{
    if (!blob || blobSize < g_BlobSignatureSize)
        return;

    if (memcmp(blob, g_BlobSignature, g_BlobSignatureSize) != 0)
        return;

    blob = static_cast<const char*>(blob) + g_BlobSignatureSize;
    blobSize -= g_BlobSignatureSize;

    while (blobSize > sizeof(ShaderBlobEntry))
    {
        const ShaderBlobEntry* header = static_cast<const ShaderBlobEntry*>(blob);

        if (header->dataSize == 0)
            return;

        if (blobSize < sizeof(ShaderBlobEntry) + header->dataSize + header->permutationSize)
            return;

        if (header->permutationSize > 0)
        {
            std::string permutation;
            permutation.resize(header->permutationSize);

            memcpy(&permutation[0], static_cast<const char*>(blob) + sizeof(ShaderBlobEntry), header->permutationSize);

            permutations.push_back(permutation);
        }
        else
            permutations.push_back("<default>");

        size_t offset = sizeof(ShaderBlobEntry) + header->dataSize + header->permutationSize;
        blob          = static_cast<const char*>(blob) + offset;
        blobSize -= offset;
    }
}

std::string FormatShaderNotFoundMessage(const void* blob, size_t blobSize, const ShaderConstant* constants, uint32_t numConstants)
{
    std::stringstream ss;
    ss << "Couldn't find the required shader permutation in the blob, or the blob is corrupted." << std::endl;
    ss << "Required permutation key: " << std::endl;

    if (numConstants)
    {
        for (uint32_t n = 0; n < numConstants; n++)
        {
            const ShaderConstant& constant = constants[n];
            ss << constant.name << '=' << constant.value << ';';
        }
    }
    else
        ss << "<default>";

    ss << std::endl;

    std::vector<std::string> permutations;
    EnumeratePermutationsInBlob(blob, blobSize, permutations);

    if (!permutations.empty())
    {
        ss << "Permutations available in the blob:" << std::endl;
        for (const std::string& key : permutations)
            ss << key << std::endl;
    }
    else
        ss << "No permutations found in the blob.";

    return ss.str();
}

bool WriteFileHeader(
    WriteFileCallback write,
    void*             context)
{
    return write(g_BlobSignature, g_BlobSignatureSize, context);
}

bool WritePermutation(
    WriteFileCallback  write,
    void*              context,
    const std::string& permutationKey,
    const void*        binary,
    size_t             binarySize)
{
    ShaderBlobEntry binaryEntry{};
    binaryEntry.permutationSize = (uint32_t)permutationKey.size();
    binaryEntry.dataSize        = (uint32_t)binarySize;

    bool success;
    success = write(&binaryEntry, sizeof(binaryEntry), context);
    success &= write(permutationKey.data(), binaryEntry.permutationSize, context);
    success &= write(binary, binarySize, context);
    return success;
}

} // namespace shaderMake