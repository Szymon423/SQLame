#include <gtest/gtest.h>
#include "transaction.hpp"

TEST(CompressionEncryptionTest, Initialization) {
    // Testuje, czy funkcja initCommunication nie powoduje awarii
    EXPECT_NO_THROW(initCommunication());
}

TEST(CompressionEncryptionTest, HelperFunction) {
    // Testuje, czy funkcja helperFunction nie powoduje awarii
    EXPECT_NO_THROW(helperFunction());
}
