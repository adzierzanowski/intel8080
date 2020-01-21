#ifndef EMU_H
#define EMU_H

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <iostream> // DEBUG
#include <boost/format.hpp> // DEBUG

#include <boost/algorithm/string.hpp>

#include "cpu.hh"
#include "opcode.hh"


class Emulator
{

  private:
    std::unique_ptr<CPU> cpu;
    std::vector<Opcode> opcodes;
    bool execute_flag;

    void load_opcodes(void);

    void nop(void);
    void lxi(Register x, Register y);
    void lxi(Register x);
    void inx(Register x, Register y);
    void inx(Register x);
    void inr(Register x);
    void dcr(Register x);
    void mvi(Register x);
    void stax(Register x, Register y);
    void rlc(void);
    void dad(Register dst_x, Register dst_y, Register src_x, Register src_y);
    void dad(Register dst_x, Register dst_y, Register src_x);
    void ldax(Register x, Register y);
    void dcx(Register x, Register y);
    void dcx(Register x);
    void rrc(void);
    void ral(void);
    void rar(void);
    void shld(void);
    void lhld(void);
    void cma(void);
    void sta(void);
    void stc(void);
    void lda(void);
    void cmc(void);
    void mov(Register dst, Register src);
    void add(Register src);
    void adc(Register src);
    void sub(Register src);
    void sbb(Register src);

  public:
    static const std::string opcodes_filename;
    Emulator(void);
    void load_program(std::vector<uint8_t> prog, uint16_t address=0);
    void execute(void);
    void execute_opcode(uint8_t opcode);
};

#endif
