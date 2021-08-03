#include "../include/catch2/catch.hpp"
#include <stdlib.h>
#include <ctime>
#include <string>
#include <iostream>

#include <OneCoin/Cryptography.hpp>

TEST_CASE("signing", "[]") {
    std::string test = std::to_string(123);

    ECDSA::generate_keys("pub.pem", "priv.pem");
    std::string sig = ECDSA::sign("priv.pem", test);

    REQUIRE(ECDSA::verify("pub.pem", test, sig));
};

TEST_CASE("signing-with-string", "[]") {
    std::string test = "abc";
    std::string reciever = "-----BEGIN PUBLIC KEY-----\nMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEuhPwLwTe2MEZwYFIlBCLsxaZOlER\nNICe4W4NyCzSIVvwPyQVl+7LnpA/pjoZxFozrrDu1eIgKRTBm3cXhUtKpA==\n-----END PUBLIC KEY-----";

    std::string sig = ECDSA::sign("priv.pem", test);
    bool passed = ECDSA::verify_from_string(reciever, test, sig);
    REQUIRE(passed);
};