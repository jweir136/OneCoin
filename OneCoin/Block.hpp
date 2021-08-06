#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>
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
 * All Blocks have four fields: the blocks, the hash, the size, and the nonce. The blocks field is just a list
 * of the serialized Transaction objects. The hash is the hash of all the Transaction's hashes appended
 * together. The size is just the number of Transactions in the Block. The nonce is the solution to the proof of work algorithm.
 * The Proof of Work Algorithm (PoW) is a mechinism to ensure that the blocks in the Blockchain aren't changed once they are added.
 */
class Block {
    private:
        json            blocks;
        std::size_t     hash;
        std::size_t     size;
        json            json_data;
        std::size_t     nonce;
        std::size_t     last_block;

    public:
        /**
         * @brief The constructor is used to make an empty Block object. An empty Block has a hash of
         * zero.
         */
        Block(std::size_t last_block) {
            this->blocks = {};
            this->hash = 0;
            this->size = 0;
            this->nonce = 0;
            this->last_block = last_block;

            this->json_data["nonce"] = this->nonce;
            this->json_data["blocks"] = this->blocks;
            this->json_data["size"] = this->size;
            this->json_data["hash"] = this->hash;
            this->json_data["last_block"] = this->last_block;
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
            this->nonce = this->json_data["nonce"];
            this->last_block = this->json_data["last_block"];
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
         * @brief Returns the value of the nonce. The nonce is the solution to the equation ```hash + nonce + last_hash % 100 = 0```.
         * @return Returns the value of the nonce.
         */
        std::size_t get_nonce() {
            return this->nonce;
        }

        /**
         * @brief Return the hash of the last Block. The last_block is the hash of the previous Block in the Blockchain which this
         * Block instance is stored. Note: every Block instance must have one and exactly one last_block.
         * @return Returns the hash of the last Block.
         */
        std::size_t get_last_block() {
            return this->last_block;
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

        /**
         * @brief Returns a given Transaction stored in the Block instance. The given Transaction is looked up by the given hash.
         * If the Transaction does not exist in the Block object, then an empty string is returned.
         * @param hash This is the hash of the Transaction object to look up.
         * @return Returns a serialized Transaction object in JSON format. If the Transaction does not exist in the Block, then
         * an empty string is returned.
         */
        std::string get(std::size_t hash) {
            for (int i = 0; i < this->size; i++)
                if (this->blocks[i]["hash"] == hash)
                    return this->blocks[i].dump();
            return "";
        } 

        /**
         * @brief Determines if all the Transaction objects currently stored in the Block object have a valid signature.
         * Whether or not they have a valid signature is determined by the Transaction.verify_transaction() method.
         * @return Returns whether or not every Transaction stored in the Block has a valid signature. Returns true
         * if and only if all signatures are valid.
         */
        bool all_transactions_have_valid_signatures() {
            for (int i = 0; i < this->size; i++) {
                Tx::Transaction trans = Tx::Transaction(this->blocks[i].dump());
                if (trans.verify_transaction() == false)
                    return false;
            }

            return true;
        }   

        /**
         * @brief Calculates and sets the Block's nonce value based on the Proof of Work (PoW) equation ```hash + last_block + nonce % 100 = 0```.
         * @param verbose Sets the verbose level for the method. When verbose is true, logging will be outputted. When false, there
         * will be no logging.
         */
        void calculate_nonce(bool verbose = false) {
            while (std::hash<std::string>()(std::to_string(this->hash) + std::to_string(this->last_block) + std::to_string(this->nonce)) % 100 != 0) {
                this->nonce += 1;
                
                if (verbose)
                    std::cout << "Nonce: " << this->nonce << " PoW: " << (std::hash<std::string>()(std::to_string(this->hash) + std::to_string(this->last_block) + std::to_string(this->nonce))) << std::endl;
            }
        }
};

#endif