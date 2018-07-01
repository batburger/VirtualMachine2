//
// Created by a_mod on 26.06.2018.
//

#include "unit_framework.h"

#include "../header/State.h"
#include "../header/InstructionSet.h"

using namespace vm2;

int unit_readRegisterN(){
    for(uint8_t i = 0; i < 10; i++){
        const uint8_t setOpcode = (uint8_t)0xb0 + i;
        const uint8_t readOpcode = (uint8_t)0xa0 + i;
        std::vector<uint8_t> code ({0xd0, 0x0, 0x0, 0x0, 0x03, setOpcode, readOpcode, 0x01});
        State* state = new State(code);
        InstructionSet instructionSet = InstructionSet();

        instructionSet.get(state->readIp())->call(state); // push
        ASSERT_EQUAL(state->getStack().peek().isGood(), true);
        ASSERT_EQUAL(state->getStack().peek().getValue(), 3);

        instructionSet.get(state->readIp())->call(state); // set register i
        ASSERT_EQUAL(state->readRegister(i), 3);
        ASSERT_EQUAL(state->readIp(), readOpcode); // did set register i iterate the ip?

        instructionSet.get(state->readIp())->call(state); // read register i
        ASSERT_EQUAL(state->getStack().peek().isGood(), true);
        ASSERT_EQUAL(state->getStack().peek().getValue(), 3);
        ASSERT_EQUAL(state->getStack().peek().getOpcode(), readOpcode);

        ASSERT_EQUAL(state->readIp(), 0x01);
        delete state;
    }
    return 0;
}

int unit_setRegisterN(){
    for(uint8_t i = 0; i < 10; i++){
        const uint8_t setOpcode = (uint8_t)0xb0 + i;
        std::vector<uint8_t> code ({0xd0, 0x0, 0x0, 0x0, 0x03, setOpcode, 0x01});
        State* state = new State(code);
        InstructionSet instructionSet = InstructionSet();

        instructionSet.get(state->readIp())->call(state); // push
        ASSERT_EQUAL(state->getStack().peek().isGood(), true);
        ASSERT_EQUAL(state->getStack().peek().getValue(), 3);
        ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xd0);

        instructionSet.get(state->readIp())->call(state); // set register i
        ASSERT_EQUAL(state->readRegister(i), 3);

        ASSERT_EQUAL(state->readIp(), 0x01);
        delete state;
    }

    return 0;
}

int unit_push(){
    std::vector<uint8_t> code ({0xd0, 0x80, 0x80, 0x80, 0x03, 0x01});
    State* state = new State(code);
    InstructionSet instructionSet = InstructionSet();

    instructionSet.get(code[0])->call(state);

    ASSERT_EQUAL(state->readIp(), 0x01);
    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 2155905027);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xd0);

    delete(state);
    return 0;
}

int unit_remove(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 3, 0xd0, 0, 0, 0, 4, 0xd1, 0x01});
    InstructionSet instructionSet = InstructionSet();

    State* state = new vm2::State(code);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readIp(), 0x01);
    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().pop().getValue(), 3);

    delete(state);
    return 0;
}

int unit_move(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 3, 0xd0, 0, 0, 0, 0, 0xc0, 0x01});
    InstructionSet instructionSet = InstructionSet();

    State* state = new vm2::State(code);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->readMemory(0), 3);
    ASSERT_EQUAL(state->readIp(), 0x01);
    return 0;
}

int unit_read(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 0, 0xc1, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    state->writeMemory(0, 3);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 3);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);
    return 0;
}

int unit_uadd(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 1, 0xd0, 0, 0, 0, 4, 0xe0, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 5);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);
    return 0;
}

int unit_sadd(){
    std::vector<uint8_t> code ({0xd0, 0x80, 0, 0, 1, 0xd0, 0, 0, 0, 4, 0xe1, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 3);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe1);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe1, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 0x80000003);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe1);
    ASSERT_EQUAL(state->readIp(), 0x01);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe1, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 7);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe1);
    ASSERT_EQUAL(state->readIp(), 0x01);

    delete(state);
    return 0;
}

int unit_fadd(){
    return 0;
}

int unit_usub() {
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe3, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 3);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe3);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);
    return 0;
}

int unit_ssub() {
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe4, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 3);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe4);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe4, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 0x80000007);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe4);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 5, 0xd0, 0x80, 0, 0, 2, 0xe4, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 0x80000003);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe4);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);

    return 0;
}

int unit_fsub(){

    return 0;
}

int unit_umult(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe6, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 10);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe6);
    ASSERT_EQUAL(state->readIp(), 0x01);

    delete(state);
    return 0;
}

int unit_smult(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 5, 0xd0, 0x80, 0, 0, 2, 0xe7, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);


    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 0x8000000A);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe7);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 5, 0xd0, 0x80, 0, 0, 2, 0xe7, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 10);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe7);
    ASSERT_EQUAL(state->readIp(), 0x01);

    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 5, 0xd0, 0, 0, 0, 2, 0xe7, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 10);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe7);
    ASSERT_EQUAL(state->readIp(), 0x01);

    delete(state);
    return 0;
}

int unit_fmult(){

    return 0;
}


int unit_udiv(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 4, 0xd0, 0, 0, 0, 2, 0xe9, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xe9);
    ASSERT_EQUAL(state->readIp(), 0x01);

    delete(state);
    return 0;
}

int unit_sdiv(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 4, 0xd0, 0, 0, 0, 2, 0xea, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xea);
    ASSERT_EQUAL(state->readIp(), 0x01);

    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 4, 0xd0, 0, 0, 0, 2, 0xea, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 0x80000002);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xea);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0x80, 0, 0, 4, 0xd0, 0x80, 0, 0, 2, 0xea, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xea);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);

    return 0;
}

int unit_fdiv(){

    return 0;
}

int unit_tof(){

    return 0;
}

int unit_abs(){
    std::vector<uint8_t> code ({0xd0, 0x80, 0, 0, 5, 0xed, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 5);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xed);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete state;

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 5, 0xed, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 5);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xed);
    ASSERT_EQUAL(state->readIp(), 0x01);

    delete(state);
    return 0;
}

int unit_ucmp(){
    std::vector<uint8_t> code ({0xd0, 0, 0, 0, 2, 0xd0, 0, 0, 0, 2, 0xee, 0x01});
    InstructionSet instructionSet = InstructionSet();
    State* state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 0);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xee);
    ASSERT_EQUAL(state->readIp(), 0x01);

    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 2, 0xd0, 0, 0, 0, 3, 0xee, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 1);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xee);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);

    code = std::vector<uint8_t> ({0xd0, 0, 0, 0, 3, 0xd0, 0, 0, 0, 2, 0xee, 0x01});
    state = new vm2::State(code);

    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);
    instructionSet.get(state->readIp())->call(state);

    ASSERT_EQUAL(state->getStack().peek().isGood(), true);
    ASSERT_EQUAL(state->getStack().peek().getValue(), 2);
    ASSERT_EQUAL(state->getStack().peek().getOpcode(), 0xee);
    ASSERT_EQUAL(state->readIp(), 0x01);
    delete(state);

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
    register_test(unit_tof);
    register_test(unit_abs);
    register_test(unit_ucmp);

    start_unit_test();
    end_unit_test();
}