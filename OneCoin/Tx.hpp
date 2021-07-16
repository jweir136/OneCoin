#ifndef TX_HPP
#define TX_HPP

#include <string>
#include <ctime>
#include <functional>

#include <include/catch2/json.hpp>

using namespace nlohmann;

typedef unsigned long ulong;

namespace Tx {

    /**
     *  @brief A Transaction object is used to represent the transfer of digital currency.
     *  The role of this object is to keep track of where currency is being transferred to.
     * 
     *  All Transactions must have exactly one input and output. An input is comprised of the hashes
     *  of the block and Transaction where the input is located, and the hash of the inputs. 
     *  An output is comprised of the public key of the user the currency is being sent to and
     *  the hash of the output.
     * 
     *  Transactions have five additional data members:
     *      - <b>epoch</b> is the time that the Transaction was created in <i>unsigned long</i> form.
     *      - <b>author</b> is the public key of the user that created and Transaction. This is in PEM format.
     *      - <b>hash<b/> is the hash of the Transactions. The hash uses the all the data members, except the nonce.
     *      - <b>nonce<b/> is used to calculate and verify the <i>Proof of Work</i>. A nonce is considered valid if and only if
     *         the hash of the nonce + the hash ends in at last four zeros. The nonce is calculated using a stochastic method.
     *      - <b>signature</b> is used to verify that a Transaction was created by the owner of the currency units being spent.
     *          The signature is the ECDSA signature of the hash when signed by the Transaction's author.
     */
    class Transaction {
        private:
            ulong           epoch;
            std::string     author;
            std::size_t     hash;
            std::size_t     nonce;
            std::string     signature;
            json            json_string;

            /*  Transaction Inputs */
            std::size_t     input_block;
            std::size_t     input_tx;
            std::size_t     input_hash;

            /* Transaction Outputs */
            std::string     output_user_key;
            std::size_t     output_hash;

        public:

            /**
             * @brief The Transaction constructor creates a new Transaction object using the data provided by the user.
             * This provided data includes the public key, the user to send the currency unit to, and the input info.
             * .
             * @param public_key   This is a string containing the PEM file data
             * of the <i>Public Key</i> of the Transaction's author. The key being invalid may result in a <i>runtime_error</i> being thrown.
             * If this is the case, then be sure that the public key is valid and that newline tokens (\\n) are being included wherever a linebreak
             * occurs in the PEM file.
             * @param input_block   This is the hash of the block where the input token is located. The input token
             * is the input to the Transaction, it directs the <i>Blockchain</i> where to find the currency units being spent.
             * Warning: The validity of input_block is not checked when the constructor is being called, however an incorrect
             * input_block will cause the Transaction to be null and void.
             * @param input_tx  This is the hash of the Transaction where the input token is located. This directs the <i>Blockchain</i>
             * where to look for the inputed currency unit.
             * Warning: The validity of input_block is not checked when the constructor is being called, however an incorrect
             * input_block will cause the Transaction to be null and void.
             * @param reciever  This is the <i>public key</i> of the user to which the currency units are being sent to. The key
             * should be a basic string, in PEM format. Be sure not to omit newline (\\n) characters when newlines are present in the PEM
             * file.
             */
            Transaction(std::string public_key, std::size_t input_block, std::size_t input_tx, std::string reciever) {
                this->epoch = (ulong)time(NULL);
                this->author = public_key;
                this->input_block = input_block;
                this->input_tx = input_tx;
                this->input_hash = std::hash<std::string>()(std::to_string(this->input_block) + std::to_string(this->input_tx));
                this->output_user_key = reciever;
                this->nonce = 0;                    
                this->signature = "";
                this->output_hash = std::hash<std::string>()(this->output_user_key);
                this->hash = get_hash();

                this->json_string["time"] = this->epoch;
                this->json_string["author"] = this->author;
                this->json_string["nonce"] = this->nonce;
                this->json_string["signature"] = this->signature;
                this->json_string["hash"] = this->hash;
                this->json_string["input"]["block"] = this->input_block;
                this->json_string["input"]["tx"] = this->input_tx;
                this->json_string["input"]["hash"] = this->input_hash;
                this->json_string["output"]["reciever"] = this->output_user_key;
                this->json_string["output"]["hash"] = this->output_hash;
            }

            /**
             * @brief The Transaction constructor creates a new Transaction using the serialized JSON data from another Transaction object.
             * The given JSON string should be valid and from another Transaction object.
             * .
             * @param json_data This is the JSON string to use to create the Transaction object. This input should be a valid JSON string
             * for the Transaction class.
             */
            Transaction(std::string json_data) {

            }

            /**
             * @brief This method is used to return the hash of the Transaction object. The hash is calculated using
             * all the Transactions's data members, except its own hash. Therefore, all the data members must be set before this method
             * can be called. If this method is called before all the data members can be initialized and assigned the correct values, then an error
             * may not be thrown.
             */
            std::size_t get_hash() {
                return std::hash<std::string>()(
                    std::to_string(this->epoch) +
                    this->author +
                    std::to_string(this->nonce) +
                    this->signature +
                    std::to_string(this->input_hash) +
                    std::to_string(this->output_hash)
                );
            }

    };
}

#endif