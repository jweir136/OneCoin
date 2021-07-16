#define CATCH_CONFIG_MAIN
#include "../include/catch2/catch.hpp"

TEST_CASE( "DEFAULT TEST CASE", "[default]" ) {
    REQUIRE(1 == 1);
}