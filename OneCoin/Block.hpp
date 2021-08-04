#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <include/catch2/json.hpp>
#include <OneCoin/Tx.hpp>

using namespace nlohmann;

/**
 * @brief A Block object is the entire foundation of the most fundamental data structure in bitcoin,
 * the Blockchain. A Block object is simply used to group and store Transaction objects and store them
 * on the Blockchain.
 * 
 * All Blocks have three fields: the blocks, the hash, and the size. The blocks field is just a list
 * of the serialized Transaction objects. The hash is the hash of all the Transaction's hashes appended
 * together. The size is just the number of Transactions in the Block.
 */
class Block {
    private:
        json            blocks;
        std::size_t     hash;
        std::size_t     size;
        json            json_data;

    public:
        /**
         * @brief The constructor is used to make an empty Block object. An empty Block has a hash of
         * zero.
         */
        Block() {
            this->blocks = {};
            this->hash = 0;
            this->size = 0;

            this->json_data["blocks"] = this->blocks;
            this->json_data["size"] = this->size;
            this->json_data["hash"] = this->hash;
        }

        /**
         * @brief The constructor takes a serialized Block object and creates a new object. The serialized
         * string is expected to be in JSON format, and from another Block object.
         * @param json_data This is the JSON string another serialized Block.
         */
        Block(std::string json_data) {
            this->json_data = json::parse(json_data);
            
            this->blocks = this->json_data["blocks"];
            this->hash = this->json_data["hash"];
            this->size = this->json_data["size"];
        }

};

#endif