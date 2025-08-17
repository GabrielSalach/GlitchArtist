//
// Created by Gabriel Salach on 17/08/2025.
//

#ifndef GLITCHARTIST_IFORMATLOADER_H
#define GLITCHARTIST_IFORMATLOADER_H
#include <vector>

class IFormatLoader {
public:
    virtual ~IFormatLoader() = default;
    virtual void LoadImage(const std::string& filename) = 0;
    virtual std::vector<unsigned char>* GetData() = 0;
    virtual GLuint GetTexture() = 0;
};

#endif //GLITCHARTIST_IFORMATLOADER_H