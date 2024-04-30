//
// Created by sebsn on 30-04-2024.
//

#include "Files.h"

namespace Kraken {
    std::string Files::ReadFile(const std::string &filepath) {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII

        if(in) {
            in.seekg(0, std::ios::end);
            if (const size_t size = in.tellg(); size != -1) {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            } else {
			    KRC_ERROR("Could not open file '{0}'", filepath);
            }
        } else {
			KRC_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }
} // Kraken