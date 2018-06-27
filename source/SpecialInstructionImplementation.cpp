//
// Created by a_mod on 26.06.2018.
//

#include "../header/SpecialInstructionImplementation.h"

vm2::ReadNthRegisterInstruction::ReadNthRegisterInstruction(int n) : n(n) {}

vm2::Instruction vm2::ReadNthRegisterInstruction::operator()(vm2::State* state){
    uint32_t storedValue = state->readRegister((unsigned)n);
    state->getStack().push(storedValue, (uint8_t)(0xa0 + n));
    return *this;
}

///////////////////////////////////////////////////////

vm2::SetNthRegisterInstruction::SetNthRegisterInstruction(int n) : n(n){}

vm2::Instruction vm2::SetNthRegisterInstruction::operator()(vm2::State *state) {
    StackObject obj = state->getStack().pop();
    if(!obj.isGood())
        throw std::runtime_error("Trying to write none good value to a register.");

    state->setRegister((uint32_t)n, obj.get());
    return *this;
}
