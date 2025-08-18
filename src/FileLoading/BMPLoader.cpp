//
// Created by Gabriel Salach on 17/08/2025.
//

#include "BMPLoader.h"

namespace GlitchArtist {

    void BMPLoader::LoadImage(const std::string &filename) {
        if (image == nullptr)
            image = new BMPImage();

        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
            textureID = 0;
        }
        image->data.clear();
        image->valid = false;

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            printf("ERREUR: Impossible d'ouvrir %s\n", filename.c_str());
            return;
        }

        // Headers
        BMPFileHeader file_header;
        BMPInfoHeader info_header;

        file.read(reinterpret_cast<char*>(&file_header), sizeof(BMPFileHeader));
        file.read(reinterpret_cast<char*>(&info_header), sizeof(BMPInfoHeader));

        // // DEBUG: Afficher toutes les informations
        // printf("=== DEBUG BMP ===\n");
        // printf("File type: 0x%04X (doit être 0x4D42)\n", file_header.file_type);
        // printf("File size: %u bytes\n", file_header.file_size);
        // printf("Data offset: %u\n", file_header.offset_data);
        // printf("Image: %dx%d\n", info_header.width, info_header.height);
        // printf("Bit count: %d\n", info_header.bit_count);
        // printf("Compression: %u\n", info_header.compression);
        // printf("=================\n");

        // Vérifications
        if (file_header.file_type != 0x4D42) {
            printf("ERREUR: Pas un BMP valide\n");
            return;
        }

        if (info_header.compression != 0) {
            printf("ERREUR: Compression non supportée\n");
            return;
        }

        if (info_header.bit_count != 24 && info_header.bit_count != 32) {
            printf("ERREUR: Format %d bits non supporté\n", info_header.bit_count);
            return;
        }

        // Configuration image
        image->width = info_header.width;
        image->height = abs(info_header.height);
        image->channels = info_header.bit_count / 8;

        // printf("Image configurée: %dx%d, %d canaux\n",
        //        image->width, image->height, image->channels);

        // CORRECTION CRITIQUE: Position correcte des données
        file.clear(); // Clear any error flags
        file.seekg(file_header.offset_data, std::ios::beg);

        // Vérifier la position
        auto pos = file.tellg();
        // printf("Position dans le fichier: %ld (offset attendu: %u)\n",
        //        (long)pos, file_header.offset_data);

        // Calcul du padding
        int bytes_per_row = image->width * image->channels;
        int padded_row_size = ((bytes_per_row + 3) / 4) * 4; // Alignement 4 bytes
        int padding = padded_row_size - bytes_per_row;

        // printf("Bytes par ligne: %d, avec padding: %d, padding: %d\n",
        //        bytes_per_row, padded_row_size, padding);

        // Allouer mémoire
        image->data.resize(image->width * image->height * image->channels);

        // MÉTHODE CORRIGÉE: Lecture ligne par ligne
        bool is_bottom_up = (info_header.height > 0);
        // printf("Bottom-up: %s\n", is_bottom_up ? "Oui" : "Non");

        std::vector<unsigned char> row_buffer(padded_row_size);

        for (int y = 0; y < image->height; y++) {
            // Lire une ligne complète (avec padding)
            file.read(reinterpret_cast<char*>(row_buffer.data()), padded_row_size);

            if (file.fail()) {
                printf("ERREUR: Échec lecture ligne %d\n", y);
                return;
            }

            // Déterminer la ligne de destination (flip vertical si nécessaire)
            int dest_y = is_bottom_up ? (image->height - 1 - y) : y;

            // Copier les pixels (sans le padding)
            for (int x = 0; x < image->width; x++) {
                int src_idx = x * image->channels;
                int dest_idx = (dest_y * image->width + x) * image->channels;

                if (image->channels == 3) {
                    // BGR -> RGB
                    image->data[dest_idx + 0] = row_buffer[src_idx + 2]; // R
                    image->data[dest_idx + 1] = row_buffer[src_idx + 1]; // G
                    image->data[dest_idx + 2] = row_buffer[src_idx + 0]; // B
                } else if (image->channels == 4) {
                    // BGRA -> RGBA
                    image->data[dest_idx + 0] = row_buffer[src_idx + 2]; // R
                    image->data[dest_idx + 1] = row_buffer[src_idx + 1]; // G
                    image->data[dest_idx + 2] = row_buffer[src_idx + 0]; // B
                    image->data[dest_idx + 3] = row_buffer[src_idx + 3]; // A
                }
            }
        }

        file.close();
        image->valid = true;
    }

    GLuint BMPLoader::GetTexture() {
        // Vérifier que l'image est valide
        if (!image || !image->valid) {
            printf("ERREUR: Image invalide ou non chargée\n");
            return 0;
        }

        // Génération/liaison texture
        if (textureID == 0) {
            glGenTextures(1, &textureID);
        }
        glBindTexture(GL_TEXTURE_2D, textureID);

        // CRITIQUE: Définir l'alignement correct
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Pas d'alignement forcé

        // Paramètres de texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Déterminer le format correct selon les canaux
        GLenum format, internal_format;

        if (image->channels == 3) {
            format = GL_RGB;
            internal_format = GL_RGB8;
        } else if (image->channels == 4) {
            format = GL_RGBA;
            internal_format = GL_RGBA8;
        } else {
            printf("ERREUR: Nombre de canaux invalide: %d\n", image->channels);
            return 0;
        }

        // Upload des données avec le bon format
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format,
                     image->width, image->height, 0,
                     format, GL_UNSIGNED_BYTE, image->data.data());

        // Vérifier les erreurs OpenGL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("ERREUR OpenGL lors de la création de texture: 0x%04X\n", error);
            return 0;
        }

        // Restaurer l'alignement par défaut
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        return textureID;
    }

    ImVec2 BMPLoader::GetSize() {
        return {static_cast<float>(image->width), static_cast<float>(image->height)};
    }

    std::vector<unsigned char> * BMPLoader::GetData() {
        if (image == nullptr || !image->valid) {
            return nullptr;
        }

        return &image->data;
    }

    BMPLoader::~BMPLoader() {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
            textureID = 0;
        }
        delete image;
    }
}
