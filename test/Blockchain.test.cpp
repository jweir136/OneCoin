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
    "/Users/jacobweir/projects/cpp/OneCoin/test/sample_private_keys/priv1.pem",
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

        REQUIRE(blockchain.get(block.get_hash()) != "");
    }

    REQUIRE(blockchain.get(0) == "");
};