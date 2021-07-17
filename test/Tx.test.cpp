#include "../include/catch2/catch.hpp"
#include <stdlib.h>
#include <ctime>
#include <string>
#include <OneCoin/Tx.hpp>

const std::string pub1 = "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEvo2EhDT60gPW+8BRq7iB0SkBn+UE\nSOcts5pJSiPb+7uL6jrVQYyyyyVvePXwbMvQBAkl7MuycLry0w9UTEwNDg==\n-----END PUBLIC KEY-----";
const std::string pub2 = "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEHbAzzNc3BXWI619W5W53bmEUlPP6\niO14u4O0GeliUj+U2ZHJ9atbkanpNhtgjoIALYvbM9pBQn2kC/Yb3hTh7Q==\n-----END PUBLIC KEY-----";
const std::string pub3 = "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEyQQr7BgHRtOB7SJTFbS7i06DX1lo\nQsKDuHIdOmL4z/frRzjdMUuowBH4FyNWDkH51HSxIhZKgxotYR35JAWPPw==\n-----END PUBLIC KEY-----";
const std::string pub_keys[3] = { pub1, pub2, pub3 };
const std::string priv_keys[3] = {
    "sample_private_keys/priv1.pem", "sample_private_keys/priv2.pem", "sample_private_keys/priv3.pem"
};

TEST_CASE("tx-serialization", "[]") {
    srand(time(NULL));

    std::size_t in_block;
    std::size_t in_tx;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            in_block = rand() % 100000;
            in_tx = rand() % 100000;

            Tx::Transaction trans = Tx::Transaction(pub_keys[i], in_block, in_tx, pub_keys[j]);
            Tx::Transaction trans2 = Tx::Transaction(trans.to_json());

            REQUIRE(trans.get_time() == trans2.get_time());
            REQUIRE(trans.get_author() == trans2.get_author());
            REQUIRE(trans.get_signature() == trans2.get_signature());
            REQUIRE(trans.get_input_block() == trans2.get_input_block());
            REQUIRE(trans.get_input_tx() == trans2.get_input_tx());
            REQUIRE(trans.get_input_hash() == trans2.get_input_hash());
            REQUIRE(trans.get_reciever() == trans2.get_reciever());
            REQUIRE(trans.get_output_hash() == trans2.get_output_hash());
        }
    }
};

