//
// Created by a_mod on 22.06.2018.
//

#ifndef VM2_0_INSTRUCTIONSET_H
#define VM2_0_INSTRUCTIONSET_H


#include <cstdint>
#include <array>

#include "State.h"
#include "Instruction.h"
#include "IInstruction.h"

namespace vm2{
    class InstructionSet {
    public:
        InstructionSet();
        ~InstructionSet();
    private:
        /* The size of the array needs be 0xf2, because instructions will be stored so that
         * instructions opcode == index in the array and
         * the highest opcode is 0xf1.
         * This makes sure that every opcode look up happens in O(1) time, but wastes quit a bit of memory.
         * (wastes ~6,5kib assuming 32bit pointer, and wastes ~13kib assuming 64bit ptr.)*/
        std::array<vm2::IInstruction*, 0xf2> instructionMapArray;

    private:
        /// Instructions
        static void op_setSize(State* state);
        static void op_move(State* state);
        static void op_read(State* state);
        static void op_alloc(State* state);

        static void op_push(State* state);
        static void op_remove(State* state);

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

        static void op_utof(State* state);
        static void op_stof(State* state);
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
        static void op_return(State* state);

        static void op_int(State* state);
        static void op_noop(State* state);
        static void op_unsignedOut(State* state);
        static void op_signedOut(State* state);
        static void op_floatOut(State* state);
        static void op_charOut(State* state);
        ///
    public:
        vm2::IInstruction* get(uint8_t opt);
    };
}



#endif //VM2_0_INSTRUCTIONSET_H
