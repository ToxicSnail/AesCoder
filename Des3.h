// Simple wrapper around OpenSSL's 3DES (DES-EDE3) CBC routines.
#ifndef DES3_H
#define DES3_H

#include <string>

class Des3 {
public:
    static bool encryptFile(const std::string& inFile, const std::string& outFile,
                            const std::string& key);
    static bool decryptFile(const std::string& inFile, const std::string& outFile,
                            const std::string& key);
};

#endif // DES3_H
