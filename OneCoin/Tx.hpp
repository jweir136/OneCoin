#ifndef TX_HPP
#define TX_HPP

#include <string>
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
     *      - < b>epoch< /b> is the time that the Transaction was created in < i>unsigned long< /i> form.
     *      - < b>author< /b> is the public key of the user that created and Transaction. This is in PEM format.
     *      - < b>hash< b/> is the hash of the Transactions. The hash uses the all the data members, except the nonce.
     *      - < b>nonce< b/> is used to calculate and verify the < i>Proof of Work< /i>. A nonce is considered valid if and only if
     *         the hash of the nonce + the hash ends in at last four zeros. The nonce is calculated using a stochastic method.
     *      - < b>signature< /b> is used to verify that a Transaction was created by the owner of the currency units being spent.
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

    };
}

#endif