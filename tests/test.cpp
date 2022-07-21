#include "gtest/gtest.h"
#include "chip8.h"

class InstructionsTests : public ::testing::Test {
protected:
    void SetUp() override {
        chip.set_vi(0x0, 0x42);
        chip.set_vi(0x4, 0x14);
        chip.set_vi(0x6, 0x14);
        chip.set_vi(0x3, 0x3);
        chip.set_vi(0xA, 0xFF);
    }

    Chip8 chip;
};

TEST_F(InstructionsTests, TestJump) {
    chip.run_instr(0x1222);
    ASSERT_EQ(chip.get_pc(), 0x0222);
}

TEST_F(InstructionsTests, TestSkippingXEqArg) {
    uint16_t starting_pc = chip.get_pc();
    chip.run_instr(0x3414);
    ASSERT_EQ(chip.get_pc(), starting_pc + 4);
    chip.run_instr(0x3413);
    ASSERT_EQ(chip.get_pc(), starting_pc + 6);
}

TEST_F(InstructionsTests, TestSkippingXNeArg) {
    uint16_t starting_pc = chip.get_pc();
    chip.run_instr(0x4414);
    ASSERT_EQ(chip.get_pc(), starting_pc + 2);
    chip.run_instr(0x4413);
    ASSERT_EQ(chip.get_pc(), starting_pc + 6);
}

TEST_F(InstructionsTests, TestSkippingXEqY) {
    uint16_t starting_pc = chip.get_pc();
    chip.run_instr(0x5460);
    ASSERT_EQ(chip.get_pc(), starting_pc + 4);
    chip.run_instr(0x5430);
    ASSERT_EQ(chip.get_pc(), starting_pc + 6);
}

TEST_F(InstructionsTests, TestUndefinedInstructions) {
    EXPECT_THROW(chip.run_instr(0x8009), UndefinedInstruction);
    EXPECT_THROW(chip.run_instr(0x5009), UndefinedInstruction);
    EXPECT_THROW(chip.run_instr(0x9003), UndefinedInstruction);
    EXPECT_THROW(chip.run_instr(0xF034), std::out_of_range);
}

TEST_F(InstructionsTests, TestStoring) {
    ASSERT_EQ(chip.get_v(0x1), 0x0);
    chip.run_instr(0x61AB);
    ASSERT_EQ(chip.get_v(0x1), 0xAB);
}

TEST_F(InstructionsTests, TestArgAddition) {
    chip.run_instr(0x7603);
    ASSERT_EQ(chip.get_v(0x6), 0x17);
    chip.run_instr(0x7602);
    ASSERT_NE(chip.get_v(0x6), 0x20);
}

TEST_F(InstructionsTests, TestStoringYInX) {
    chip.run_instr(0x8630);
    ASSERT_EQ(chip.get_v(0x6), chip.get_v(0x3));
}

TEST_F(InstructionsTests, TESTXOrY) {
    chip.run_instr(0x8631);
    ASSERT_EQ(chip.get_v(0x6), 0x17);
}

TEST_F(InstructionsTests, TestXAndY) {
    chip.run_instr(0x8632);
    ASSERT_EQ(chip.get_v(0x6), 0x0);
}
TEST_F(InstructionsTests, TestXXorY) {
    chip.run_instr(0x8643);
    ASSERT_EQ(chip.get_v(0x6), 0x0);
}

TEST_F(InstructionsTests, TestRegisterAddition) {
    chip.run_instr(0x8634);
    ASSERT_EQ(chip.get_v(0x6), 0x17);
    ASSERT_EQ(chip.get_vf(), 0x0);
    chip.run_instr(0x8A34);
    ASSERT_EQ(chip.get_v(0xA), 0x2);
    ASSERT_EQ(chip.get_vf(), 0x1);
}

TEST_F(InstructionsTests, TestSubtraction1) {
    chip.run_instr(0x8A35);
    ASSERT_EQ(chip.get_v(0xA), 0xFC);
    ASSERT_EQ(chip.get_vf(), 0x1);
    chip.run_instr(0x8365);
    ASSERT_EQ(chip.get_v(0x3), 0xEF);
    ASSERT_EQ(chip.get_vf(), 0x0);
}

TEST_F(InstructionsTests, TestRightShift) {
    chip.run_instr(0x8336);
    ASSERT_EQ(chip.get_v(0x3), 0x1);
    ASSERT_EQ(chip.get_vf(), 0x1);
}

TEST_F(InstructionsTests, TestSubtraction2) {
    chip.run_instr(0x8A37);
    ASSERT_EQ(chip.get_v(0xA), 0x04);
    ASSERT_EQ(chip.get_vf(), 0x0);
    chip.run_instr(0x8367);
    ASSERT_EQ(chip.get_v(0x3), 0x11);
    ASSERT_EQ(chip.get_vf(), 0x1);
}

TEST_F(InstructionsTests, TestLeftShift) {
    chip.run_instr(0x83AE);
    ASSERT_EQ(chip.get_v(0x3), 0xFE);
    ASSERT_EQ(chip.get_vf(), 0x1);
}

TEST_F(InstructionsTests, TestSkippingXNEY) {
    uint16_t pc = chip.get_pc();
    chip.run_instr(0x9640);
    ASSERT_EQ(chip.get_pc(), pc + 2);
    chip.run_instr(0x96A0);
    ASSERT_EQ(chip.get_pc(), pc + 6);
}

TEST_F(InstructionsTests, TestStoringMemAddr) {
    chip.run_instr(0xA111);
    ASSERT_EQ(chip.get_vi(), 0x111);
}

TEST_F(InstructionsTests, TestJumpWithV0) {
    chip.run_instr(0xB456);
    ASSERT_EQ(chip.get_pc(), 0x498);
}

// TODO: RNG?

TEST_F(InstructionsTests, TestAddXToI) {
    uint16_t vi = chip.get_vi();
    chip.run_instr(0xFA1E);
    ASSERT_EQ(chip.get_vi(), vi + chip.get_v(0xA));
}

TEST_F(InstructionsTests, TestBCD) {
    uint16_t vi = chip.get_vi();
    chip.run_instr(0xFA33);
    ASSERT_EQ(chip.get_mem().get(vi), 2);
    ASSERT_EQ(chip.get_mem().get(vi + 1), 5);
    ASSERT_EQ(chip.get_mem().get(vi + 2), 5);
}

TEST_F(InstructionsTests, TestSubroutines) {
    chip.run_instr(0x2AAA);
    ASSERT_EQ(chip.get_pc(), 0xAAA);
    chip.run_instr(0x2678);
    ASSERT_EQ(chip.get_pc(), 0x678);
    chip.run_instr(0x2EEE);
    chip.run_instr(0x00EE);
    ASSERT_EQ(chip.get_pc(), 0x678 + 2);
    chip.run_instr(0x00EE);
    ASSERT_EQ(chip.get_pc(), 0xAAA + 2);
}