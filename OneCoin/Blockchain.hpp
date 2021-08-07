#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <include/catch2/json.hpp>
#include <string>
#include <iostream>
#include <OneCoin/Block.hpp>

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
         * @brief This is the accessor method for the size field.
         * @return Returns the value of the size field.
         */
        std::size_t get_size() {
            return this->size;
        }

        /**
         * @brief Return the serialized JSON string representing the current Blockchain object.
         * @return Returns a string containing a Blockchain object's JSON data.
         */
        std::string to_json() {
            return this->json_data.dump();
        }

        /**
         * @brief Add a new Block object to the new of the Blockchain. Upon being passed,
         * the method determines if the new Block should be a regular Block or a GenesisBlock.
         * If the Block is a regular Block, then the new last_block is added to the Block. The last step before
         * being added to the Blockchain is for the nonce to be computed.
         * @param block_json This is the Block object to add to the Blockchain. It is expected to be in the form of
         * a serialized JSON data. You can obtain the JSON data of another Block object by using Block.to_json() method.
         * @param verbose This is a paramter used to specify if the verbose mode for the Block.calculate_nonce() method should be
         * used.
         */
        void append(std::string block_json, bool verbose = false) {
            Block block;

            if (this->size == 0)
                block = GenesisBlock(block_json);
            else {
                block = Block(block_json);
                block.set_last_block(this->blocks[this->size - 1]["hash"]);
            }
            block.calculate_nonce(verbose);

            this->size++;
            this->blocks.push_back(json::parse(block.to_json()));
            this->json_data["size"] = this->size;
            this->json_data["blocks"] = this->blocks;
        }

        /**
         * @brief Return a Block object currently being stored in the Blockchain instance by its hash. If the hash does not belong
         * to any Block object in the Blockchain, then an empty string "" is returned.
         * @param hash The hash of the Block object you wish to retrieve.
         * @return Return the serialized JSON string of the chosen Block object.
         */
        std::string get(std::size_t hash) {
            for (int i = 0; i < this->size; i++)
                if (this->blocks[i]["hash"] == hash)
                    return this->blocks[i].dump();

            return "";
        }

    private:
        /**
         * @brief Determines whether or not every Block stored in the Blockchain instance has a valid signature.
         * @return Returns true if every single Block in the Blockchain instance is correctly signed. Returns false if
         * even one Block in the Blockchain has an invalid signature.
         */
        bool all_blocks_have_valid_signatures() {
            for (int i = 0; i < this->size; i++) {
                Block block = Block(this->blocks[i].dump());

                if (!block.all_transactions_have_valid_signatures())
                    return false;
            }

            return true;
        }

        /**
         * @brief Determines whether or not every Block stored in the Blockchain instance has a valid nonce.
         * @return Returns true if every single Block in the Blockchain instance has a correct nonce. Returns false if
         * even one Block in the Blockchain has an invalid nonce.
         */
        bool all_blocks_have_valid_nonce() {
            for (int i = 0; i < this->size; i++) {
                Block block = Block(this->blocks[i].dump());

                if (!block.is_nonce_valid())
                    return false;
            }

            return true;         
        }
};

#endif