#include <gtest/gtest.h> // Include the Google Test header

extern "C"
{
#include "buffer.h" // Include your C header file
}

// Test case for back_text_size function
TEST_F(BufferTest, BackTextSizeTest)
{
}

// Test case for used_text_size function
TEST_F(BufferTest, UsedTextSizeTest)
{
}

int main(int argc, char **argv)
{
    // Initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
