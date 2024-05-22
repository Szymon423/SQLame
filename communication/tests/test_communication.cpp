#include <gtest/gtest.h>
#include "communication.hpp"

TEST(CommunicationTest, Initialization) {
    // Testuje, czy funkcja initCommunication nie powoduje awarii
    EXPECT_NO_THROW(initCommunication());
}

TEST(CommunicationTest, HelperFunction) {
    // Testuje, czy funkcja helperFunction nie powoduje awarii
    EXPECT_NO_THROW(helperFunction());
}
