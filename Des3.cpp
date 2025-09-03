#include "Des3.h"
#include <openssl/des.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

bool Des3::encryptFile(const std::string& inFile, const std::string& outFile,
                       const std::string& keyStr)
{
    std::ifstream in(inFile, std::ios::binary);
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(in)), {});
    const int blockSize = 8; // DES block size in bytes
    int pad = blockSize - data.size() % blockSize;
    data.insert(data.end(), pad, pad);

    unsigned char key[24] = {0};
    std::memcpy(key, keyStr.data(), std::min(keyStr.size(), sizeof(key)));

    DES_cblock key1, key2, key3;
    std::memcpy(key1, key, 8);
    std::memcpy(key2, key + 8, 8);
    std::memcpy(key3, key + 16, 8);

    DES_key_schedule ks1, ks2, ks3;
    DES_set_key_unchecked(&key1, &ks1);
    DES_set_key_unchecked(&key2, &ks2);
    DES_set_key_unchecked(&key3, &ks3);

    DES_cblock iv = {0};
    std::vector<unsigned char> out(data.size());
    DES_ede3_cbc_encrypt(data.data(), out.data(), static_cast<long>(data.size()),
                         &ks1, &ks2, &ks3, &iv, DES_ENCRYPT);

    std::ofstream ofs(outFile, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(out.data()), out.size());
    return true;
}

bool Des3::decryptFile(const std::string& inFile, const std::string& outFile,
                       const std::string& keyStr)
{
    std::ifstream in(inFile, std::ios::binary);
    std::vector<unsigned char> enc((std::istreambuf_iterator<char>(in)), {});
    const int blockSize = 8;
    if (enc.size() % blockSize) return false;

    unsigned char key[24] = {0};
    std::memcpy(key, keyStr.data(), std::min(keyStr.size(), sizeof(key)));

    DES_cblock key1, key2, key3;
    std::memcpy(key1, key, 8);
    std::memcpy(key2, key + 8, 8);
    std::memcpy(key3, key + 16, 8);

    DES_key_schedule ks1, ks2, ks3;
    DES_set_key_unchecked(&key1, &ks1);
    DES_set_key_unchecked(&key2, &ks2);
    DES_set_key_unchecked(&key3, &ks3);

    DES_cblock iv = {0};
    std::vector<unsigned char> out(enc.size());
    DES_ede3_cbc_encrypt(enc.data(), out.data(), static_cast<long>(enc.size()),
                         &ks1, &ks2, &ks3, &iv, DES_DECRYPT);

    int pad = out.back();
    out.resize(out.size() - pad);

    std::ofstream ofs(outFile, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(out.data()), out.size());
    return true;
}
