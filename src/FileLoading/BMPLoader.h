#ifndef GLITCHARTIST_BMPLOADER_H
#define GLITCHARTIST_BMPLOADER_H

#include <fstream>
#include <imgui.h>
#include <vector>
#include <glad/glad.h>

#include "IFormatLoader.h"

namespace GlitchArtist {

#pragma pack(push, 1) // Assure un alignement sur 1 byte pour les structures
    struct BMPFileHeader {
        uint16_t file_type;      // 0x4D42 ('BM')
        uint32_t file_size;      // Taille du fichier
        uint16_t reserved1;      // Réservé (0)
        uint16_t reserved2;      // Réservé (0)
        uint32_t offset_data;    // Offset vers les données de pixels
    };

    struct BMPInfoHeader {
        uint32_t size;              // Taille de cette structure (40 bytes)
        int32_t width;              // Largeur de l'image
        int32_t height;             // Hauteur de l'image
        uint16_t planes;            // Nombre de plans (1)
        uint16_t bit_count;         // Bits par pixel (24 ou 32)
        uint32_t compression;       // Type de compression (0 = non compressé)
        uint32_t size_image;        // Taille de l'image en bytes
        int32_t x_pixels_per_meter; // Résolution horizontale
        int32_t y_pixels_per_meter; // Résolution verticale
        uint32_t colors_used;       // Nombre de couleurs utilisées
        uint32_t colors_important;  // Nombre de couleurs importantes
    };
#pragma pack(pop)

    struct BMPImage {
        int width;
        int height;
        int channels;
        std::vector<unsigned char> data;
        bool valid;
    };


    class BMPLoader : public IFormatLoader {
    public:
        std::vector<unsigned char>* GetData() override;

        ~BMPLoader();

        void LoadImage(const std::string& filename) override;
        GLuint GetTexture() override;
        ImVec2 GetSize() override;
    private:
        BMPImage* image = nullptr;
        GLuint textureID = 0;
    };
} // GlitchArtist

#endif //GLITCHARTIST_BMPLOADER_H