#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <include/catch2/json.hpp>
#include <string>

using namespace nlohmann;

/**
 * @brief A Blockchain object is the main data structure in OneCoin. A Blockchain object is simply
 * a linked list used to store all the Block data types. Each Block is held together by a nonce.
 * The nonce is just a way of ensuring that the Blocks won't be altered once they are added to the Blockchain.
 * 
 * Each Blockchain has only 3 fields: size, blocks, and json_data. The size is just the number of Blocks currently being
 * held in the Blockchain. blocks is just the list of the Block objects. The Block objects are represented as a serialized
 * JSON string. The json_data field is just the serialized string of the Blockchain instance, represented in JSON format.
 */
class Blockchain {
    private:
        std::size_t     size;
        json            blocks;
        json            json_data;

    public:
        /**
         * @brief The constructor creates an empty Blockchain instance with the size set to zero.
         */
        Blockchain() {
            this->size = 0;
            this->blocks = {};

            this->json_data["size"] = this->size;
            this->json_data["blocks"] = this->blocks;
        }

        /**
         * @brief This constructor creates a new Blockchain instance from the serialized JSON data of another Blockchain.
         * Call the to_json() method to get a Blockchain object's JSON string.
         * @param json_data This is the JSON data to use to create the new Blockchain object.
         */
        Blockchain(std::string json_data) {
            this->json_data = json::parse(json_data);

            this->size = this->json_data["size"];
            this->blocks = this->json_data["blocks"];
        }

        /**
         * @brief Return the serialized JSON string representing the current Blockchain object.
         * @return Returns a string containing a Blockchain object's JSON data.
         */
        std::string to_json() {
            return this->json_data.dump();
        }
};

#endif