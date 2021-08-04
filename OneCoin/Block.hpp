#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <include/catch2/json.hpp>
#include <OneCoin/Tx.hpp>

using namespace nlohmann;

class Block {
    private:
        json            blocks;
        std::size_t     hash;
        std::size_t     size;
        json            json_data;

    public:

        Block() {
            this->blocks = {};
            this->hash = 0;
            this->size = 0;

            this->json_data["blocks"] = this->blocks;
            this->json_data["size"] = this->size;
            this->json_data["hash"] = this->hash;
        }

        Block(std::string json_data) {
            this->json_data = json::parse(json_data);
            
            this->blocks = this->json_data["blocks"];
            this->hash = this->json_data["hash"];
            this->size = this->json_data["size"];
        }

};

#endif