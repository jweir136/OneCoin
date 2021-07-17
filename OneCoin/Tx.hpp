#ifndef TX_HPP
#define TX_HPP

#include <string>
#include <ctime>
#include <functional>
#include <iostream>
#include <cstdint>  
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdint>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/pem.h>

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
     *      - <b>hash</b> is the hash of the Transactions. The hash uses the all the data members.
     *      - <b>signature</b> is used to verify that a Transaction was created by the owner of the currency units being spent.
     *          The signature is the ECDSA signature of the hash when signed by the Transaction's author.
     */
    class Transaction {
        private:
            ulong           epoch;
            std::string     author;
            std::size_t     hash;
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
                this->signature = "";
                this->output_hash = std::hash<std::string>()(this->output_user_key);
                this->hash = get_hash();

                this->json_string["time"] = this->epoch;
                this->json_string["author"] = this->author;
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
                this->json_string = json::parse(json_data);

                this->epoch = this->json_string["time"];
                this->author = this->json_string["author"];
                this->input_block = this->json_string["input"]["block"];
                this->input_tx = this->json_string["input"]["tx"];
                this->input_hash = this->json_string["input"]["hash"];
                this->output_user_key = this->json_string["output"]["reciever"];
                this->signature = this->json_string["signature"];
                this->output_hash = this->json_string["output"]["hash"];
                this->hash = this->json_string["hash"];
            }

            /**
             * @brief This method returns the time that the Transaction was created.
             * @return  Returns the time the Transaction was created as a <i>size_t</i> type.
             */
            std::size_t get_time() {
                return this->epoch;
            }

            /**
             * @brief This method returns the Transaction's signing public key as a string. The string is in PEM format.
             * @return Returns a string holding the public key in PEM format.
             */
            std::string get_author() {
                return this->author;
            }

            /**
             * @brief This method returns the Transaction's signature as a hex string.
             * @return Returns a hex string containing the Transaction's signature.
             */
            std::string get_signature() {
                return this->signature;
            }

            /**
             * @brief This method returns the hash of the block of the Transaction's input.
             * @return Returns the hash of the input block.
             */
            std::size_t get_input_block() {
                return this->input_block;
            }

            /**
             * @brief This method returns the hash of the Transaction used in the Transaction's input.
             * @return Returns the hash of the inputed Transaction.
             */
            std::size_t get_input_tx() {
                return this->input_tx;
            }

            /**
             * @brief Returns the hash of the Transaction's input.
             * @return Returns the hash of the input as a <i>size_t</i> form.
             */
            std::size_t get_input_hash() {
                return this->input_hash;
            }

            /**
             * @brief Returns the public key of the user recieving the currency units as a string. The key is in PEM format.
             * @return Returns public key in PEM format.
             */
            std::string get_reciever() {
                return this->output_user_key;
            }

            /**
             * @brief Returns the hash of the Transaction's output. The output is mostly just the reciever's public key.
             * @return Returns the hash of the output in <i>size_t</i> form.
             */
            std::size_t get_output_hash() {
                return this->output_hash;
            }

            /**
             * @brief The to_json() method serializes the Transaction object and returns it as a JSON string.
             * @return  Returns a JSON string to be used to serialize and deserialize Transaction objects.
             */
            std::string to_json() {
                return this->json_string.dump();
            }

            /**
             * @brief This method is used to return the hash of the Transaction object. The hash is calculated using
             * all the Transactions's data members, except its own hash. Therefore, all the data members must be set before this method
             * can be called. If this method is called before all the data members can be initialized and assigned the correct values, then an error
             * may not be thrown.
             * @return This method returns the hash of the Transaction object.
             */
            std::size_t get_hash() {
                return std::hash<std::string>()(
                    std::to_string(this->epoch) +
                    this->author +
                    this->signature +
                    std::to_string(this->input_hash) +
                    std::to_string(this->output_hash)
                );
            }

            /**
             * @brief This method is used to sign and seal a Transaction. Signing a Transaction should only be done once all the correct data
             * has been added to it. This method must be provided with a filepath to a private key PEM file. The private key should
             * never be given as just a plain string for security reasons. Upon successfull completition, the <i>signature</i> field is set.
             * .
             * @param priv_key_filepath This is the filepath to the PEM file containing the private key. By default, the method looks for the
             * "~/onecoin/keys/priv.pem" file, but you are free to provide your own.
             */
            void sign(std::string priv_key_filepath = "~/onecoin/keys/priv.pem") {
                std::string signature;
                std::string input = std::to_string(this->hash);

                FILE* f = fopen(priv_key_filepath.c_str(), "r");

                if (!f)
                    throw std::runtime_error("Error: Failed to open pem");

                EC_KEY* ec_key = PEM_read_ECPrivateKey(f, NULL, NULL, NULL);

                assert(ec_key);

                fclose(f);

                if (!EC_KEY_check_key(ec_key))
                    throw std::runtime_error("Error: Private key failed check");

                EVP_PKEY* key = EVP_PKEY_new();

                if (!EVP_PKEY_assign_EC_KEY(key, ec_key))
                    throw std::runtime_error("Error: Key could not be assigned");

                EVP_PKEY_CTX* key_ctx = EVP_PKEY_CTX_new(key, NULL);

                EVP_PKEY_sign_init(key_ctx);
                EVP_PKEY_CTX_set_signature_md(key_ctx, EVP_sha256());

                std::size_t sig_len = 0;
                EVP_PKEY_sign(key_ctx, NULL, &sig_len, (unsigned char *)&input, input.length());
                signature.assign(sig_len, 0);
                EVP_PKEY_sign(key_ctx, (unsigned char *)&signature[0], &sig_len, (unsigned char *)&input, input.length());

                EVP_PKEY_CTX_free(key_ctx);
                EVP_PKEY_free(key);

                this->signature = signature;
            }

            /**
             * @brief This method is used to verify the validity of the Transaction's ECDSA system. Using the provided public key,
             * the hash of the Transaction, and the signature, the method returns whether or not the Transaction is considered valid.
             * In order for this method to work correctly, the Transaction must already be signed.
             * .
             * @return This method returns a boolean value to tell whether or not the Transaction's signature is valid.
             */
            bool verify() {
                std::string pem = this->author;
                std::string expected = std::to_string(this->hash);
                std::string signature = this->signature;

                EC_KEY* ec_key;
                BIO* bo = BIO_new(BIO_s_mem());
                BIO_write(bo, pem.c_str(),pem.length());
                PEM_read_bio_EC_PUBKEY(bo, &ec_key, NULL, NULL);

                EVP_PKEY* key = EVP_PKEY_new();

                EVP_PKEY_assign_EC_KEY(key, ec_key);

                EVP_PKEY_CTX* key_ctx = EVP_PKEY_CTX_new(key, NULL);
                EVP_PKEY_verify_init(key_ctx);
                
                EVP_PKEY_CTX_set_signature_md(key_ctx, EVP_sha256());
                std::size_t sig_len = 0;

                bool ret = EVP_PKEY_verify(key_ctx, (unsigned char *)&signature[0], signature.length(), (unsigned char *)&expected, expected.length());

                EVP_PKEY_CTX_free(key_ctx);
                EVP_PKEY_free(key);

                return ret;
            }

            /**
             * @brief This method is an overloaded << operator. Upon being called with an output stream (ostream) object, the Transaction will write itself in a
             * highly ledgible and easily understood format to the stream.
             * @param out This is the output stream to write the Transaction to.
             * @param trans This is the Transaction object to write from.
             * @return This method returns a reference to another ostream object. It will most likely be the same object to be passed
             * in the <i>out</i> argument.
             */
            friend std::ostream& operator<<(std::ostream &out, Transaction& trans) {
                out << "Transaction: { [ " << std::to_string(trans.get_input_block()) << " " << std::to_string(trans.get_input_tx()) << " " << std::to_string(trans.get_input_hash()) << "] ";
                out << std::to_string(trans.get_time()) << " " << std::hash<std::string>()(trans.get_author()) << " " << std::to_string(trans.get_hash()) << " " << trans.get_signature() << " [";
                out << std::hash<std::string>()(trans.get_reciever()) << " " << std::to_string(trans.get_output_hash()) << " ] }" << std::endl;
                return out;
            }

        private:
            /**
             * @brief convert a single, whether it be ascii or unicode to a string of hex characters.
             * @param in This is the input string to convert to hex characters.
             * @return Returns a regular string, but only containing hex characters.
             */
            std::string string_to_hex(const std::string& in) {
                std::stringstream ss;

                ss << std::hex << std::setfill('0');
                for (size_t i = 0; in.length() > i; ++i) {
                    ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(in[i]));
                }

                return ss.str(); 
            }

            /**
             * @brief Takes a string containing only hex characters and returns a the decoded string.
             * @param in The input hex string to decode.
             * @return Returns the regular string of decoded hex characters.
             */
            std::string hex_to_string(const std::string& in) {
                std::string output;

                if ((in.length() % 2) != 0) {
                    throw std::runtime_error("String is not valid length ...");
                }

                size_t cnt = in.length() / 2;

                for (size_t i = 0; cnt > i; ++i) {
                    uint32_t s = 0;
                    std::stringstream ss;
                    ss << std::hex << in.substr(i * 2, 2);
                    ss >> s;

                    output.push_back(static_cast<unsigned char>(s));
                }

                return output;
            }
    };
}

#endif