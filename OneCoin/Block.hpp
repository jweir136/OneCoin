#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>
#include <functional>
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

        /**
         * @brief This method returns the object's hash. If no Transactions have been added to the Block, then the result will be zero.
         * @return Returns the hash of the object. The hash will be zero if there are no Transactions in the Block yet.
         */
        std::size_t get_hash() {
            return this->hash;
        }

        /**
         * @brief This method returns the number of Transactions in the Block.
         * @return Returns the number of Transactions stored.
         */
        std::size_t get_size() {
            return this->size;
        }

        /**
         * @brief Serialize the Block object and returns the serialized data as a JSON string. The serialized string can then be easily
         * stored and transferred for use everywhere. Block(std::string json_data) can be used to convert the data returned from this method
         * to a Block object.
         * @return Returns a JSON string for serialization purposes.
         */
        std::string to_json() {
            return this->json_data.dump();
        }  

        /**
         * @brief Add a Transaction object to the Block. Upon being added to the Block instance, a new hash is computed using
         * the old hash and the hash of the Transaction, and the size is incremented.
         * @param transaction_json This argument is the Transaction object to be added to the Block. The Transaction is expected
         * to be in a JSON serialized format. Call Transaction::to_json() to get a Transaction's JSON data.
         */
        void append(std::string transaction_json) {
            this->size++;
            this->blocks.push_back(json::parse(transaction_json));
            this->hash = std::hash<std::string>()(
                std::to_string(this->hash) + json::parse(transaction_json)["hash"].dump()
            );

            this->json_data["size"] = this->size;
            this->json_data["blocks"] = this->blocks;
            this->json_data["hash"] = this->hash;
        }      
};

#endif