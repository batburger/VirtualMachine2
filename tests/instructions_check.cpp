//
// Created by a_mod on 26.06.2018.
//

#include <iostream>
#include <sstream>
#include "unit_framework.h"

#include "../header/State.h"
#include "../header/InstructionSet.h"

using namespace vm2;

int unit_readRegisterN(){
    for(uint8_t i = 0; i < 10; i++){
        const uint8_t setOpcode = (uint8_t)0xb0 + i;
        const uint8_t readOpcode = (uint8_t)0xa0 + i;
        std::vector<uint8_t> code ({0xd0, 0x0, 0x0, 0x0, 0x03, setOpcode, readOpcode, 0x11});
        State* state = new State(code);
        InstructionSet instructionSet = InstructionSet();

        instructionSet.get(state->readIp())->call(state); // push
        ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
        ASSERT_EQUAL(state->getStack()->peek().getValue(), 3);

        instructionSet.get(state->readIp())->call(state); // set register i
        ASSERT_EQUAL(state->readRegister(i), 3);
        ASSERT_EQUAL(state->readIp(), readOpcode); // did set register i iterate the ip?

        instructionSet.get(state->readIp())->call(state); // read register i
        ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
        ASSERT_EQUAL(state->getStack()->peek().getValue(), 3);
        ASSERT_EQUAL(state->getStack()->peek().getOpcode(), readOpcode);

        ASSERT_EQUAL(state->readIp(), 0x11);
        delete state;
    }
    return 0;
}

int unit_setRegisterN(){
    for(uint8_t i = 0; i < 10; i++){
        const uint8_t setOpcode = (uint8_t)0xb0 + i;
        std::vector<uint8_t> code ({0xd0, 0x0, 0x0, 0x0, 0x03, setOpcode, 0x11});
        State* state = new State(code);
        InstructionSet instructionSet = InstructionSet();

        instructionSet.get(state->readIp())->call(state); // push
        ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
        ASSERT_EQUAL(state->getStack()->peek().getValue(), 3);
        ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xd0);

        instructionSet.get(state->readIp())->call(state); // set register i
        ASSERT_EQUAL(state->readRegister(i), 3);

        ASSERT_EQUAL(state->readIp(), 0x11);
        delete state;
    }

    return 0;
}

int unit_push(){
    std::vector<uint8_t> code ({0xd0, 0x80, 0x80, 0x80, 0x03, 0x11});
    State* state = new State(code);
    InstructionSet instructionSet = InstructionSet();

    instructionSet.get(code[0])->call(state);

    ASSERT_EQUAL(state->readIp(), 0x11);
    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 2155905027);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xd0);

    delete(state);
    return 0;
}

int unit_remove(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 3, 0xd0, 0, 0, 0, 4, 0xd1, 0x11});
    InstructionSet instructionSet = InstructionSet();

    State* state = new vm2::State(code);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x11);
    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->pop().getValue(), 3);

    delete(state);
    return 0;
}

int unit_move(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 3, 0xd0, 0, 0, 0, 0, 0xc1, 0x11});
    InstructionSet instructionSet = InstructionSet();

    State* state = new vm2::State(code);
    state->getLinearMemory()->setPageSize(10);
    state->getLinearMemory()->allocPage();

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getLinearMemory()->read(0), 3);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete state;
    return 0;
}

int unit_read(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 0, 0xc2, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    state->getLinearMemory()->setPageSize(10);
    state->getLinearMemory()->allocPage();
    state->getLinearMemory()->write(0, 3);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 3);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);
    return 0;
}

int unit_uadd(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 1, 0xd0, 0, 0, 0, 4, 0xe0, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 5);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);
    return 0;
}

int unit_sadd(){
    std::vector<uint8_t> code ({0xd0, 0x80, 0, 0, 1, 0xd0, 0, 0, 0, 4, 0xe1, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 3);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe1);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe1, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0x80000003);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe1);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe1, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 7);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe1);
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete(state);
    return 0;
}

int unit_fadd(){
    // 0x40600000 // 3.5
    // 0xbfc00000 // -1.5
    std::vector<uint8_t> code ({0xd0, 0x40, 0x60, 0x00, 0x00, 0xd0, 0xbf, 0xc0, 0x00, 0x00, 0xe2, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe2);
    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    // 0x40000000 // 2
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0x40000000);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);
    return 0;
}

int unit_usub() {
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe3, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 3);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe3);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);
    return 0;
}

int unit_ssub() {
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe4, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 3);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe4);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe4, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0x80000007);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe4);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 5, 0xd0, 0x80, 0, 0, 2, 0xe4, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0x80000003);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe4);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    return 0;
}

int unit_fsub(){
    // 0x40600000 // 3.5
    // 0xbfc00000 // -1.5
    std::vector<uint8_t> code ({0xd0, 0x40, 0x60, 0x00, 0x00, 0xd0, 0xbf, 0xc0, 0x00, 0x00, 0xe5, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe5)
    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    // 0x40a00000 // 5
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0x40a00000);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    return 0;
}

int unit_umult(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe6, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 10);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe6);
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete(state);
    return 0;
}

int unit_smult(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 5, 0xd0, 0x80, 0, 0, 2, 0xe7, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);


    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0x8000000A);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe7);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 5, 0xd0, 0x80, 0, 0, 2, 0xe7, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 10);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe7);
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe7, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 10);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe7);
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete(state);
    return 0;
}

int unit_fmult(){
    // 0x40600000 // 3.5
    // 0xbfc00000 // -1.5
    std::vector<uint8_t> code ({0xd0, 0x40, 0x60, 0x00, 0x00, 0xd0, 0xbf, 0xc0, 0x00, 0x00, 0xe8, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe8)
    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    // 0xc0a80000 // -5.25
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0xc0a80000);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    return 0;
}

int unit_udiv(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 4, 0xd0, 0, 0, 0, 2, 0xe9, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xe9);
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete(state);
    return 0;
}

int unit_sdiv(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 4, 0xd0, 0, 0, 0, 2, 0xea, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xea);
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 4, 0xd0, 0, 0, 0, 2, 0xea, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0x80000002);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xea);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 4, 0xd0, 0x80, 0, 0, 2, 0xea, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xea);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    return 0;
}

int unit_fdiv(){
    // 0x3fc00000 // 1.5
    // 0xbfc00000 // -1.5
    std::vector<uint8_t> code ({0xd0, 0x3f, 0xc0, 0x00, 0x00, 0xd0, 0xbf, 0xc0, 0x00, 0x00, 0xeb, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xeb)
    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    // 0xbf800000 // -1
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0xbf800000);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    return 0;
}

int unit_utof(){
    // 0x80000000 // 2147483648
    std::vector<uint8_t> code ({0xd0,  0x80, 0x00, 0x00, 0x00,     0xec, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xec)
    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    // 0x4f000000 // 2147483648
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0x4f000000);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    return 0;
}

int unit_stof(){
    // 0x 80 00 00 1e //-30
    std::vector<uint8_t> code ({0xd0,  0x80, 0x00, 0x00, 0x1e,     0xed, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xed)
    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    // 0xc1f00000 // -30
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0xc1f00000);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    return 0;
}

int unit_abs(){
    std::vector<uint8_t> code ({0xd0, 0x80, 0, 0, 5, 0xee, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 5);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xee);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete state;

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 5, 0xee, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 5);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xee);
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete(state);
    return 0;
}

int unit_ucmp(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 2, 0xd0, 0, 0, 0, 2, 0xef, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xef);
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 2, 0xd0, 0, 0, 0, 3, 0xef, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 1);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xef);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 3, 0xd0, 0, 0, 0, 2, 0xef, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xef);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    return 0;
}

int unit_scmp(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 2, 0xd0, 0, 0, 0, 2, 0xf0, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xf0);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 2, 0xd0, 0x80, 0, 0, 2, 0xf0, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xf0);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 1, 0xd0, 0, 0, 0, 1, 0xf0, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 1);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xf0);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 1, 0xd0, 0x80, 0, 0, 2, 0xf0, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xf0);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xf0, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xf0);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);


    return 0;
}

int unit_fcmp(){
    // 0x3fc00000 // 1.5
    std::vector<uint8_t> code ({0xd0, 0x3f, 0xc0, 0x00, 0x00, 0xd0, 0x3f, 0xc0, 0x00, 0x00, 0xf1, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xf1);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    // 0xbfc00000 // -1.5
    code = std::vector<uint8_t> ({0xd0, 0xbf, 0xc0, 0x00, 0x00, 0xd0, 0xbf, 0xc0, 0x00, 0x00, 0xf1, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 0);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xf1);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    // 0xbfc00000 // -1.5
    // 0x3fc00000 // 1.5
    code = std::vector<uint8_t> ({0xd0, 0xbf, 0xc0, 0x00, 0x00, 0xd0, 0x3f, 0xc0, 0x00, 0x00, 0xf1, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 1);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xf1);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    // 0xbfc00000 // -1.5
    // 0xc0200000 // -2.5
    code = std::vector<uint8_t> ({0xd0, 0xbf, 0xc0, 0, 0, 0xd0, 0xc0, 0x20, 0, 0, 0xf1, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xf1);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    // 0x40e80000 // 7.25
    // 0x3fc00000 // 1.5
    code = std::vector<uint8_t> ({0xd0, 0x40, 0xe8, 0, 0, 0xd0, 0x3f, 0xc0, 0, 0, 0xf1, 0x11});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0xf1);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);


    return 0;
}

int unit_jmp(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 7, 0x01, 0x11, 'x'});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 'x');

    delete(state);
    return 0;
}

int unit_jless(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 7, 0x02, 0x11, 'x'});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    state->getStack()->push(0, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 7, 0x02, 0x11, 'x'});
    state = new vm2::State(code);

    state->getStack()->push(2, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 7, 0x02, 0x11, 'x'});
    state = new vm2::State(code);

    state->getStack()->push(1, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 'x');
    delete(state);

    return 0;
}

int unit_jgreater(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 7, 0x03, 0x11, 'x'});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    state->getStack()->push(0, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 7, 0x03, 0x11, 'x'});
    state = new vm2::State(code);

    state->getStack()->push(2, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 'x');
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 7, 0x03, 0x11, 'x'});
    state = new vm2::State(code);

    state->getStack()->push(1, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    return 0;
}

int unit_jequal(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 7, 0x04, 0x11, 'x'});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    state->getStack()->push(0, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 'x');
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 7, 0x04, 0x11, 'x'});
    state = new vm2::State(code);

    state->getStack()->push(2, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 7, 0x04, 0x11, 'x'});
    state = new vm2::State(code);

    state->getStack()->push(1, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    return 0;
}

int unit_jNequal(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 7, 0x05, 0x11, 'x'});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    state->getStack()->push(0, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x11);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 7, 0x05, 0x11, 'x'});
    state = new vm2::State(code);

    state->getStack()->push(2, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 'x');
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 7, 0x05, 0x11, 'x'});
    state = new vm2::State(code);

    state->getStack()->push(1, 0x00);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 'x');
    delete(state);

    return 0;
}

int unit_call(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 7, 0x06, 0x11, 'x'});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack()->peek().isGood(), true);
    ASSERT_EQUAL(state->getStack()->peek().getValue(), 5);
    ASSERT_EQUAL(state->getStack()->peek().getOpcode(), 0x06);
    ASSERT_EQUAL(state->readIp(), 'x');

    delete(state);
    return 0;
}

int unit_return(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 7, 0x06, 0x11, 0x07});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x06);

    delete(state);
    return 0;
}

int unit_int(){

    return 0;
}

int unit_noop(){
    std::vector<uint8_t> code ({0x12, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x11);
    delete state;
    return 0;
}

int unit_setSize(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 5, 0xc0, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getLinearMemory()->getPageSize(), 5);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete state;
    return 0;
}

int unit_alloc(){
    std::vector<uint8_t> code ({0xc3, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    bool errorTriggered = false;

    try {
        state->getLinearMemory()->write(10, 5);
    }catch (...){
        errorTriggered = true;
    }
    ASSERT_EQUAL(errorTriggered, true);

    state->getLinearMemory()->setPageSize(10);
    instructionSet.get(state->readIp())->call(state);

    errorTriggered = false;
    try {
        state->getLinearMemory()->write(5, 5);
    }catch (std::runtime_error& err){
        errorTriggered = true;
    }

    ASSERT_EQUAL(errorTriggered, false);
    ASSERT_EQUAL(state->readIp(), 0x11);
    delete state;
    return 0;
}

int unit_unsignedOut(){
    std::vector<uint8_t> code ({0x13, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    state->getStack()->push(0x80000001, 0xd0);

    // Redirect everything from std::cout to debugBuffer so we can inspect it.
    std::stringstream buffer;
    std::streambuf * old = std::cout.rdbuf(buffer.rdbuf());

    instructionSet.get(state->readIp())->call(state);
    std::cout.rdbuf(old);     // Restore normal std::cout behaviour

    ASSERT_EQUAL(buffer.str(), "2147483649");
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete state;
    return 0;
}

int unit_signedOut(){
    std::vector<uint8_t> code ({0x14, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    state->getStack()->push(0x80000001, 0xd0);

    // Redirect everything from std::cout to debugBuffer so we can inspect it.
    std::stringstream buffer;
    std::streambuf * old = std::cout.rdbuf(buffer.rdbuf());

    instructionSet.get(state->readIp())->call(state);
    std::cout.rdbuf(old);     // Restore normal std::cout behaviour

    ASSERT_EQUAL(buffer.str(), "-1");
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete state;
    return 0;
}

int unit_float(){
    std::vector<uint8_t> code ({0x15, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    state->getStack()->push(0xbf000000, 0xd0);

    // Redirect everything from std::cout to debugBuffer so we can inspect it.
    std::stringstream buffer;
    std::streambuf * old = std::cout.rdbuf(buffer.rdbuf());

    instructionSet.get(state->readIp())->call(state);
    std::cout.rdbuf(old);     // Restore normal std::cout behaviour

    ASSERT_EQUAL(buffer.str(), "-0.5");
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete state;
    return 0;
}

int unit_charOut(){
    std::vector<uint8_t> code ({0x16, 0x11});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    state->getStack()->push(0x00000041, 0xd0);

    // Redirect everything from std::cout to debugBuffer so we can inspect it.
    std::stringstream buffer;
    std::streambuf * old = std::cout.rdbuf(buffer.rdbuf());

    instructionSet.get(state->readIp())->call(state);
    std::cout.rdbuf(old);     // Restore normal std::cout behaviour

    ASSERT_EQUAL(buffer.str(), "A");
    ASSERT_EQUAL(state->readIp(), 0x11);

    delete state;
    return 0;
}

int main(){
    register_test(unit_readRegisterN);
    register_test(unit_setRegisterN);
    register_test(unit_push);
    register_test(unit_remove);
    register_test(unit_move);
    register_test(unit_read);
    register_test(unit_uadd);
    register_test(unit_sadd);
    register_test(unit_fadd);
    register_test(unit_usub);
    register_test(unit_ssub);
    register_test(unit_fsub);
    register_test(unit_umult);
    register_test(unit_smult);
    register_test(unit_fmult);
    register_test(unit_udiv);
    register_test(unit_sdiv);
    register_test(unit_fdiv);
    register_test(unit_stof);
    register_test(unit_utof);
    register_test(unit_abs);
    register_test(unit_ucmp);
    register_test(unit_scmp);
    register_test(unit_fcmp);
    register_test(unit_jmp);
    register_test(unit_jless);
    register_test(unit_jgreater);
    register_test(unit_jequal);
    register_test(unit_jNequal);
    register_test(unit_call);
    register_test(unit_return);
    register_test(unit_int);
    register_test(unit_noop);
    register_test(unit_setSize);
    register_test(unit_alloc);
    register_test(unit_unsignedOut);
    register_test(unit_signedOut);
    register_test(unit_float);
    register_test(unit_charOut);

    start_unit_test();
}