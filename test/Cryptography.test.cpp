#include "../include/catch2/catch.hpp"
#include <stdlib.h>
#include <ctime>
#include <string>

#include <OneCoin/Cryptography.hpp>

TEST_CASE("signing", "[]") {
    std::string test = std::to_string(123);

    ECDSA::generate_keys("pub.pem", "priv.pem");
    std::string sig = ECDSA::sign("priv.pem", test);

    REQUIRE(ECDSA::verify("pub.pem", test, sig));
};