#include <gtest/gtest.h>
#include "core.hpp"

TEST(CoreTest, Initialization) {
    // Testuje, czy funkcja initCommunication nie powoduje awarii
    EXPECT_NO_THROW(initCommunication());
}

TEST(CoreTest, HelperFunction) {
    // Testuje, czy funkcja helperFunction nie powoduje awarii
    EXPECT_NO_THROW(helperFunction());
}
