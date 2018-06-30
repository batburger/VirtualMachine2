//
// Created by a_mod on 22.06.2018.
//

#include <iostream>
#include "../header/InstructionSet.h"
#include "../header/SpecialInstructionImplementation.h"
#include "../header/maths.h"


vm2::InstructionSet::InstructionSet() {
    // Register readRegister0...readRegister9 instruction
    for(uint8_t i = 0; i < 10; i++){
        instructionMap.operator[]((uint8_t)0xa0 + i) = new vm2::ReadNthRegisterInstruction(i);
    }

    // Register setRegister0...setRegister9 instruction
    for(uint8_t i = 0; i < 10; i++){
        instructionMap.operator[]((uint8_t)0xb0 + i) = new vm2::SetNthRegisterInstruction(i);
    }

    instructionMap.insert(std::pair<uint8_t, vm2::Instruction*>(0xc0, new Instruction(op_move)));
    instructionMap.insert(std::pair<uint8_t, vm2::Instruction*>(0xc1, new Instruction(op_read)));

    instructionMap.insert(std::pair<uint8_t, vm2::Instruction*>(0xd0, new Instruction(op_push)));
    instructionMap.insert(std::pair<uint8_t, vm2::Instruction*>(0xd1, new Instruction(op_remove)));

    instructionMap.insert(std::pair<uint8_t, vm2::Instruction*>(0xe0, new Instruction(op_uadd)));
    instructionMap.insert(std::pair<uint8_t, vm2::Instruction*>(0xe1, new Instruction(op_sadd)));
}


vm2::InstructionSet::~InstructionSet() {
    for(auto& it : this->instructionMap){
        delete it.second;
    }
}

vm2::IInstruction* vm2::InstructionSet::get(uint8_t opt) {
    if(instructionMap.find(opt) == instructionMap.cend())
        throw std::runtime_error("Unknown opt code");
    return instructionMap.at(opt);
}

/// Instructions

void vm2::InstructionSet::op_push(vm2::State *state) {
    uint32_t value = 0;
    value = value | (state->peekIp(1) << 24);
    value = value | (state->peekIp(2) << 16);
    value = value | (state->peekIp(3) << 8);
    value = value | (state->peekIp(4));

    state->getStack().push(value, 0xd0);
    state->iterateIp(5);
}

void vm2::InstructionSet::op_remove(vm2::State *state) {
    state->getStack().pop();
    state->iterateIp();
}

void vm2::InstructionSet::op_move(vm2::State *state) {
    StackObject addressArgument = state->getStack().pop();
    StackObject valueArgument = state->getStack().pop();
    if(!valueArgument.isGood() || !addressArgument.isGood())
        throw std::runtime_error("Move has received an none good argument.");

    state->writeMemory(addressArgument.getValue(), valueArgument.getValue());
    state->iterateIp();
}

void vm2::InstructionSet::op_read(vm2::State *state) {
    StackObject addressArgument = state->getStack().pop();
    if(!addressArgument.isGood())
        throw std::runtime_error("Read has received an none good argument.");
    uint32_t readValue = state->readMemory(addressArgument.getValue());

    state->getStack().push(readValue, 0xc1);
    state->iterateIp();
}

void vm2::InstructionSet::op_uadd(vm2::State *state) {
    StackObject a = state->getStack().pop();
    StackObject b = state->getStack().pop();

    if(!a.isGood() || !b.isGood())
        throw std::runtime_error("uadd has received an none good argument.");

    uint32_t value = a.getValue() + b.getValue();
    state->getStack().push(value, 0xe0);

    state->iterateIp();
}

void vm2::InstructionSet::op_sadd(vm2::State *state) {
    StackObject b = state->getStack().pop();
    StackObject a = state->getStack().pop();

    if(!a.isGood() || !b.isGood())
        throw std::runtime_error("sadd has received an none good argument.");

    uint32_t value = maths::manualSignedAdding(a.getValue(), b.getValue());
    state->getStack().push(value, 0xe1);
    state->iterateIp();
}


///