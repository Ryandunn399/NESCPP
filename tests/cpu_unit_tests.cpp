#include <gtest/gtest.h>
#include "cpu/status_register.hpp"

class StatusRegisterTest : public ::testing::Test
{
protected:
    StatusRegister status;

    void SetUp() override
    {
        status.SetRegister(0x00);
    }
};

TEST_F(StatusRegisterTest, SetIndividualFlags) 
{
    status.SetCarry(true);
    EXPECT_TRUE(status.GetCarry());
    EXPECT_EQ(0x01, status.GetRegister());
    
    status.SetZero(true);
    EXPECT_TRUE(status.GetZero());
    EXPECT_EQ(0x03, status.GetRegister());
    
    // Test negative flag
    status.SetNegative(true);
    EXPECT_TRUE(status.GetNegative());
    EXPECT_EQ(0x83, status.GetRegister());
}

TEST_F(StatusRegisterTest, ClearIndividualFlags) 
{
    status.SetRegister(0xFF);

    status.SetCarry(false);
    EXPECT_FALSE(status.GetCarry());
    EXPECT_EQ(0xFE, status.GetRegister());
    
    // Clear zero flag
    status.SetZero(false);  
    EXPECT_FALSE(status.GetZero());
    EXPECT_EQ(0xFC, status.GetRegister());
    
    // Clear negative flag
    status.SetNegative(false);
    EXPECT_FALSE(status.GetNegative());
    EXPECT_EQ(0x7C, status.GetRegister());
}