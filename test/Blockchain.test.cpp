#include "../include/catch2/catch.hpp"
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <string>
#include <OneCoin/Tx.hpp>
#include <OneCoin/Block.hpp>
#include <OneCoin/Blockchain.hpp>

const std::string pub1 = "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEvo2EhDT60gPW+8BRq7iB0SkBn+UE\nSOcts5pJSiPb+7uL6jrVQYyyyyVvePXwbMvQBAkl7MuycLry0w9UTEwNDg==\n-----END PUBLIC KEY-----";
const std::string pub2 = "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEHbAzzNc3BXWI619W5W53bmEUlPP6\niO14u4O0GeliUj+U2ZHJ9atbkanpNhtgjoIALYvbM9pBQn2kC/Yb3hTh7Q==\n-----END PUBLIC KEY-----";
const std::string pub3 = "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEyQQr7BgHRtOB7SJTFbS7i06DX1lo\nQsKDuHIdOmL4z/frRzjdMUuowBH4FyNWDkH51HSxIhZKgxotYR35JAWPPw==\n-----END PUBLIC KEY-----";
const std::string pub_keys[3] = { pub1, pub2, pub3 };
const std::string priv_keys[3] = {
    "test/sample_private_keys/priv1.pem",
    "test/sample_private_keys/priv2.pem", "test/sample_private_keys/priv3.pem"
};

TEST_CASE("blockchain-serialization-test", "[]") {
    srand(time(NULL));

    Block block;
    Blockchain blockchain;

    for (int k = 0; k < 10; k++) {
        std::size_t in_block;
        std::size_t in_tx;
        
        block = Block();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                in_block = rand() % 100000;
                in_tx = rand() % 100000;

                Tx::Transaction trans = Tx::Transaction(pub_keys[i], in_block, in_tx, pub_keys[j]);
                
                block.append(trans.to_json());
            }
        }
        blockchain.append(block.to_json());
    }

    Blockchain chain2 = Blockchain(blockchain.to_json());

    REQUIRE(blockchain.to_json() == chain2.to_json());
};

TEST_CASE("blockchain-get-test", "[]") {
    srand(time(NULL));

    Block block;
    Blockchain blockchain;

    std::size_t trans_hash;
    std::size_t block_hash;

    for (int k = 0; k < 10; k++) {
        std::size_t in_block;
        std::size_t in_tx;
        
        block = Block();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                in_block = rand() % 100000;
                in_tx = rand() % 100000;

                Tx::Transaction trans = Tx::Transaction(pub_keys[i], in_block, in_tx, pub_keys[j]);
                trans_hash = trans.get_hash();
                
                block.append(trans.to_json());
            }
        }
        block_hash = block.get_hash();

        blockchain.append(block.to_json());

        REQUIRE(blockchain.get(block_hash, trans_hash) != "");
        REQUIRE(blockchain.get(block.get_hash()) != "");
    }

    REQUIRE(blockchain.get(0) == "");
    REQUIRE(blockchain.get(0, 0) == "");
};

TEST_CASE("real-blockchain-test", "[]") {
    const std::string admin_keys[2] = {
        "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEvo2EhDT60gPW+8BRq7iB0SkBn+UE\nSOcts5pJSiPb+7uL6jrVQYyyyyVvePXwbMvQBAkl7MuycLry0w9UTEwNDg==\n-----END PUBLIC KEY-----",
        "/Users/jacobweir/projects/cpp/OneCoin/test/sample_private_keys/priv1.pem"
    };
    const std::string jacob_keys[2] = {
        "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEHbAzzNc3BXWI619W5W53bmEUlPP6\niO14u4O0GeliUj+U2ZHJ9atbkanpNhtgjoIALYvbM9pBQn2kC/Yb3hTh7Q==\n-----END PUBLIC KEY-----",
        "test/sample_private_keys/priv2.pem"
    };
    const std::string ryan_keys[2] = {
        "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEyQQr7BgHRtOB7SJTFbS7i06DX1lo\nQsKDuHIdOmL4z/frRzjdMUuowBH4FyNWDkH51HSxIhZKgxotYR35JAWPPw==\n-----END PUBLIC KEY-----",
        "test/sample_private_keys/priv3.pem"
    };

    Blockchain chain = Blockchain();

    // Transaction #1 Admin to Jacob
    Tx::Transaction admin_to_jacob = Tx::Transaction(admin_keys[0], NULL, NULL, jacob_keys[0]);
    admin_to_jacob.sign_transaction(admin_keys[1]);

    Block block1 = GenesisBlock();
    block1.append(admin_to_jacob.to_json());
    block1.calculate_nonce();

    chain.append(block1.to_json());   

    // Transaction #2 Jacob to Ryan
    Tx::Transaction jacob_to_ryan = Tx::Transaction(jacob_keys[0], block1.get_hash(), admin_to_jacob.get_hash(), ryan_keys[0]);
    jacob_to_ryan.sign_transaction(jacob_keys[1]);

    Block block2 = Block();
    block2.append(jacob_to_ryan.to_json());
    block2.calculate_nonce();

    chain.append(block2.to_json());

    REQUIRE(chain.all_transactions_have_matching_inputs_and_outputs(admin_keys[0]));
};