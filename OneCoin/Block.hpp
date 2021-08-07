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
        Block() {
            this->blocks = {};
            this->hash = 0;
            this->size = 0;
            this->nonce = 0;
            this->last_block = 0;

            this->json_data["nonce"] = this->nonce;
            this->json_data["blocks"] = this->blocks;
            this->json_data["size"] = this->size;
            this->json_data["hash"] = this->hash;
            this->json_data["last_block"] = this->last_block;
        }

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
         * @brief Change the value of last_block to the given argument.
         * @param last_block The value of the hash to set last_block to.
         */
        void set_last_block(std::size_t last_block) {
            this->last_block = last_block;
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
         * @brief Returns whether of not an instance of a Block is a GenesisBlock or a Block.
         * @return Returns a boolean value to represent whether or not the Block is a GenesisBlock.
         */
        bool is_genesis_block() {
            return false;
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

        /**
         * @brief Determines whether or not the Block's nonce is valid. A nonce is valid if and only if the equation
         * ```hash(hash + last_block + nonce) % 100 = 0```.
         * @return Returns whether or not the Block's nonce is valid or not.
         */
        bool is_nonce_valid() {
            return std::hash<std::string>()(std::to_string(this->hash) + std::to_string(this->last_block) + std::to_string(this->nonce)) % 100 == 0;
        }

        /**
         * @brief This is an overloaded method of the << (output stream) operator. It is most commonly used to print details about the Block instance
         * for debugging or logging purposes. Please note that the output of this method is not a JSON serialized string, and can't be used as such.
         * Additonally note that no newline character will be automatically added.
         * @param out This is a reference to an object of type ```ostream```.
         * @param block This is a reference to an object of type Block or one of its subclasses. This is the object to output to the stream.
         * @return Returns a reference to the same ```ostream``` object that was passed in the <i>out</i> argument. The returned ```ostream``` object will contain
         * the object from the passed Block instance written to it.
         */
        friend std::ostream& operator<<(std::ostream &out, Block& block) {
            if (!block.is_genesis_block())
                out << "Block { size: " << std::to_string(block.get_size()) << ", hash: " << std::to_string(block.get_hash()) << ", last_block: " << std::to_string(block.get_last_block()) << ", nonce: " << std::to_string(block.get_nonce()) << " }";
            else
                out << "GenesisBlock { size: " << std::to_string(block.get_size()) << ", hash: " << std::to_string(block.get_hash()) << ", nonce: " << std::to_string(block.get_nonce()) << " }";
            return out;
        }
};

/**
 * @brief A GenesisBlock is a subclass of the Block class. A GenesisBlock holds all the same data and methods as a Block type.
 * The main difference between A GenesisBlock and a Block is that a GenesisBlock can only be first in a Blockchain. In fact,
 * the first Block in every Blockchain <b>must</b> be a GenesisBlock. Everything in a GenesisBlock is the same as a normal Block
 * except that the <i>last_block</i>, the hash of the last block in the Blockchain, is always zero.
 */
class GenesisBlock : public Block {
    public:
        /**
         * @brief The only constructor for this class simply creates a new instance using Block(std::size_t last_block). However,
         * it automatically sets the <i>last_block</i> to zero.
         */
        GenesisBlock() : Block(0) {
            ;
        }

        /**
         * 
         */
        GenesisBlock(std::string block_json) : Block(block_json) {
            ;
        }

        /**
         * @brief Returns whether of not an instance of a Block is a GenesisBlock or a Block.
         * @return Returns a boolean value to represent whether or not the Block is a GenesisBlock.
         */
        bool is_genesis_block() {
            return true;
        }
};

#endif