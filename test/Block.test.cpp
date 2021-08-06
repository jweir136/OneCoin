#include "../include/catch2/catch.hpp"
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <string>
#include <OneCoin/Tx.hpp>
#include <OneCoin/Block.hpp>

const std::string pub1 = "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEvo2EhDT60gPW+8BRq7iB0SkBn+UE\nSOcts5pJSiPb+7uL6jrVQYyyyyVvePXwbMvQBAkl7MuycLry0w9UTEwNDg==\n-----END PUBLIC KEY-----";
const std::string pub2 = "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEHbAzzNc3BXWI619W5W53bmEUlPP6\niO14u4O0GeliUj+U2ZHJ9atbkanpNhtgjoIALYvbM9pBQn2kC/Yb3hTh7Q==\n-----END PUBLIC KEY-----";
const std::string pub3 = "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEyQQr7BgHRtOB7SJTFbS7i06DX1lo\nQsKDuHIdOmL4z/frRzjdMUuowBH4FyNWDkH51HSxIhZKgxotYR35JAWPPw==\n-----END PUBLIC KEY-----";
const std::string pub_keys[3] = { pub1, pub2, pub3 };
const std::string priv_keys[3] = {
    "/Users/jacobweir/projects/cpp/OneCoin/test/sample_private_keys/priv1.pem",
    "test/sample_private_keys/priv2.pem", "test/sample_private_keys/priv3.pem"
};

TEST_CASE("block-serialization", "[]") {
    srand(time(NULL));

    Block block = Block(0);

    for (int k = 0; k < 10; k++) {
        std::size_t in_block;
        std::size_t in_tx;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                in_block = rand() % 100000;
                in_tx = rand() % 100000;

                Tx::Transaction trans = Tx::Transaction(pub_keys[i], in_block, in_tx, pub_keys[j]);
                
                block.append(trans.to_json());
            }
        }

        Block block2 = Block(block.to_json());

        REQUIRE(block2.get_size() == block.get_size());
        REQUIRE(block2.get_hash() == block.get_hash());
        REQUIRE(block2.to_json() == block.to_json());
    }
};

TEST_CASE("block-get", "[]") {
    srand(time(NULL));

    Block block = Block(0);

    for (int k = 0; k < 10; k++) {
        std::size_t in_block;
        std::size_t in_tx;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                in_block = rand() % 100000;
                in_tx = rand() % 100000;

                Tx::Transaction trans = Tx::Transaction(pub_keys[i], in_block, in_tx, pub_keys[j]);
                
                block.append(trans.to_json());
                REQUIRE(block.get(trans.get_hash()) != "");
            }
        }
    }

    REQUIRE(block.get(0) == "");
};

TEST_CASE("block-signing-verify-true", "[]") {
    srand(time(NULL));

    Block block = Block(0);

    for (int k = 0; k < 10; k++) {
        std::size_t in_block;
        std::size_t in_tx;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                in_block = rand() % 100000;
                in_tx = rand() % 100000;

                Tx::Transaction trans = Tx::Transaction(pub_keys[i], in_block, in_tx, pub_keys[j]);
                
                block.append(trans.to_json());
            }
        }
    }

    REQUIRE(block.all_transactions_have_valid_signatures());
};

TEST_CASE("block-signing-verify-false", "[]") {
    srand(time(NULL));

    Block block = Block(0);

    for (int k = 0; k < 10; k++) {
        std::size_t in_block;
        std::size_t in_tx;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                in_block = rand() % 100000;
                in_tx = rand() % 100000;

                Tx::Transaction trans = Tx::Transaction(pub_keys[i], in_block, in_tx, pub_keys[0]);
                
                block.append(trans.to_json());
            }
        }
    }

    REQUIRE(block.all_transactions_have_valid_signatures());
};

TEST_CASE("block-nonce", "[]") {
    srand(time(NULL));

    Block block = Block(0);

    for (int k = 0; k < 10; k++) {
        std::size_t in_block;
        std::size_t in_tx;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                in_block = rand() % 100000;
                in_tx = rand() % 100000;

                Tx::Transaction trans = Tx::Transaction(pub_keys[i], in_block, in_tx, pub_keys[0]);
                
                block.append(trans.to_json());
            }
        }
    }

    block.calculate_nonce(true);
};