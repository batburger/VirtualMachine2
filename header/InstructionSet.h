//
// Created by a_mod on 22.06.2018.
//

#ifndef VM2_0_INSTRUCTIONSET_H
#define VM2_0_INSTRUCTIONSET_H


#include <cstdint>
#include <unordered_map>
#include <functional>

#include "State.h"
#include "Instruction.h"
#include "IInstruction.h"

namespace vm2{
    class InstructionSet {
    public:
        InstructionSet();
        ~InstructionSet();
    private:
        std::unordered_map<uint8_t, vm2::IInstruction*> instructionMap;

    private:
        /// Instructions
        static void op_move(State* state);
        static void op_read(State* state);
        static void op_push (State* state);
        static void op_remove (State* state);

        static void op_uadd(State* state);
        static void op_sadd(State* state);
        static void op_fadd(State* state);

        static void op_usub(State* state);
        static void op_ssub(State* state);
        static void op_fsub(State* state);

        static void op_umult(State* state);
        static void op_smult(State* state);
        static void op_fmult(State* state);

        static void op_udiv(State* state);
        static void op_sdiv(State* state);
        static void op_fdiv(State* state);

        static void op_tof(State* state);
        static void op_abs(State* state);

        static void op_ucmp(State* state);
        static void op_scmp(State* state);
        static void op_fcmp(State* state);

        static void op_jmp(State* state);
        static void op_jless(State* state);
        static void op_jgreater(State* state);
        static void op_jequal(State* state);
        static void op_jNequal(State* state);

        static void op_call(State* state);
        ///
    public:
        vm2::IInstruction* get(uint8_t opt);
    };
}



#endif //VM2_0_INSTRUCTIONSET_H
