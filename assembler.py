#!/usr/bin/env python3

import argparse

class Assembler:
  def __init__(self, data):
    self.data = data.splitlines()
    self.first_pass_data = []
    self.output = []
    self.opcodes = {
      'nop': [1, {'none': 0x00}],
      'lxi': [3, {'b': 0x01, 'd': 0x11, 'h': 0x21, 'sp': 0x31}],
      'stax': [1, {'b': 0x02, 'd': 0x12}],
      'inx': [1, {'b': 0x03, 'd': 0x13, 'h': 0x23, 'sp': 0x33}],
      'inr': [1, {'b': 0x04, 'c': 0x0c, 'd': 0x14, 'e': 0x1c, 'h': 0x24, 'l': 0x2c, 'm': 0x34, 'a': 0x3c}],
      'dcr': [1, {'b': 0x05, 'c': 0x0d, 'd': 0x15, 'e': 0x1d, 'h': 0x25, 'l': 0x2d, 'm': 0x35, 'a': 0x3d}],
      'mvi': [2, {'b': 0x06, 'c': 0x0e, 'd': 0x16, 'e': 0x1e, 'h': 0x26, 'l': 0x2e, 'm': 0x36, 'a': 0x3e}],
      'rlc': [1, {'none': 0x07}],
      'dad': [1, {'b': 0x09, 'd': 0x19, 'h': 0x29, 'sp': 0x39}],
      'ldax': [1, {'b': 0x0a, 'd': 0x1a}],
      'dcx': [1, {'b': 0x0b, 'd': 0x1b, 'h': 0x2b, 'sp': 0x3b}],
      'rrc': [1, {'none': 0x0f}],
      'ral': [1, {'none': 0x17}],
      'rar': [1, {'none': 0x1f}],
      'shld': [3, {'none': 0x22}],
      'daa': [1, {'none': 0x27}],
      'lhld': [3, {'none': 0x2a}],
      'cma': [1, {'none': 0x2f}],
      'sta': [3, {'none': 0x32}],
      'stc': [1, {'none': 0x37}],
      'lda': [3, {'none': 0x32}],
      'cmc': [1, {'none': 0x3f}],

      'call': [3, {'none': 0xcd}],
    }
    '''
  0x40, 1, "mov", "Move B to B", &Intel8080::op_mov<Register::B, Register::B>));
  0x41, 1, "mov", "Move C to B", &Intel8080::op_mov<Register::B, Register::C>));
  0x42, 1, "mov", "Move D to B", &Intel8080::op_mov<Register::B, Register::D>));
  0x43, 1, "mov", "Move E to B", &Intel8080::op_mov<Register::B, Register::E>));
  0x44, 1, "mov", "Move H to B", &Intel8080::op_mov<Register::B, Register::H>));
  0x45, 1, "mov", "Move L to B", &Intel8080::op_mov<Register::B, Register::L>));
  0x46, 1, "mov", "Move M to B", &Intel8080::op_mov<Register::B, Register::M>));
  0x47, 1, "mov", "Move A to B", &Intel8080::op_mov<Register::B, Register::A>));
  0x48, 1, "mov", "Move B to C", &Intel8080::op_mov<Register::C, Register::B>));
  0x49, 1, "mov", "Move C to C", &Intel8080::op_mov<Register::C, Register::C>));
  0x4a, 1, "mov", "Move D to C", &Intel8080::op_mov<Register::C, Register::D>));
  0x4b, 1, "mov", "Move E to C", &Intel8080::op_mov<Register::C, Register::E>));
  0x4c, 1, "mov", "Move H to C", &Intel8080::op_mov<Register::C, Register::H>));
  0x4d, 1, "mov", "Move L to C", &Intel8080::op_mov<Register::C, Register::L>));
  0x4e, 1, "mov", "Move M to C", &Intel8080::op_mov<Register::C, Register::M>));
  0x4f, 1, "mov", "Move A to C", &Intel8080::op_mov<Register::C, Register::A>));
  0x50, 1, "mov", "Move B to D", &Intel8080::op_mov<Register::D, Register::B>));
  0x51, 1, "mov", "Move C to D", &Intel8080::op_mov<Register::D, Register::C>));
  0x52, 1, "mov", "Move D to D", &Intel8080::op_mov<Register::D, Register::D>));
  0x53, 1, "mov", "Move E to D", &Intel8080::op_mov<Register::D, Register::E>));
  0x54, 1, "mov", "Move H to D", &Intel8080::op_mov<Register::D, Register::H>));
  0x55, 1, "mov", "Move L to D", &Intel8080::op_mov<Register::D, Register::L>));
  0x56, 1, "mov", "Move M to D", &Intel8080::op_mov<Register::D, Register::M>));
  0x57, 1, "mov", "Move A to D", &Intel8080::op_mov<Register::D, Register::A>));
  0x58, 1, "mov", "Move B to E", &Intel8080::op_mov<Register::E, Register::B>));
  0x59, 1, "mov", "Move C to E", &Intel8080::op_mov<Register::E, Register::C>));
  0x5a, 1, "mov", "Move D to E", &Intel8080::op_mov<Register::E, Register::D>));
  0x5b, 1, "mov", "Move E to E", &Intel8080::op_mov<Register::E, Register::E>));
  0x5c, 1, "mov", "Move H to E", &Intel8080::op_mov<Register::E, Register::H>));
  0x5d, 1, "mov", "Move L to E", &Intel8080::op_mov<Register::E, Register::L>));
  0x5e, 1, "mov", "Move M to E", &Intel8080::op_mov<Register::E, Register::M>));
  0x5f, 1, "mov", "Move A to E", &Intel8080::op_mov<Register::E, Register::A>));
  0x60, 1, "mov", "Move B to H", &Intel8080::op_mov<Register::H, Register::B>));
  0x61, 1, "mov", "Move C to H", &Intel8080::op_mov<Register::H, Register::C>));
  0x62, 1, "mov", "Move D to H", &Intel8080::op_mov<Register::H, Register::D>));
  0x63, 1, "mov", "Move E to H", &Intel8080::op_mov<Register::H, Register::E>));
  0x64, 1, "mov", "Move H to H", &Intel8080::op_mov<Register::H, Register::H>));
  0x65, 1, "mov", "Move L to H", &Intel8080::op_mov<Register::H, Register::L>));
  0x66, 1, "mov", "Move M to H", &Intel8080::op_mov<Register::H, Register::M>));
  0x67, 1, "mov", "Move A to H", &Intel8080::op_mov<Register::H, Register::A>));
  0x68, 1, "mov", "Move B to L", &Intel8080::op_mov<Register::L, Register::B>));
  0x69, 1, "mov", "Move C to L", &Intel8080::op_mov<Register::L, Register::C>));
  0x6a, 1, "mov", "Move D to L", &Intel8080::op_mov<Register::L, Register::D>));
  0x6b, 1, "mov", "Move E to L", &Intel8080::op_mov<Register::L, Register::E>));
  0x6c, 1, "mov", "Move H to L", &Intel8080::op_mov<Register::L, Register::H>));
  0x6d, 1, "mov", "Move L to L", &Intel8080::op_mov<Register::L, Register::L>));
  0x6e, 1, "mov", "Move M to L", &Intel8080::op_mov<Register::L, Register::M>));
  0x6f, 1, "mov", "Move A to L", &Intel8080::op_mov<Register::L, Register::A>));
  0x70, 1, "mov", "Move B to M", &Intel8080::op_mov<Register::M, Register::B>));
  0x71, 1, "mov", "Move C to M", &Intel8080::op_mov<Register::M, Register::C>));
  0x72, 1, "mov", "Move D to M", &Intel8080::op_mov<Register::M, Register::D>));
  0x73, 1, "mov", "Move E to M", &Intel8080::op_mov<Register::M, Register::E>));
  0x74, 1, "mov", "Move H to M", &Intel8080::op_mov<Register::M, Register::H>));
  0x75, 1, "mov", "Move L to M", &Intel8080::op_mov<Register::M, Register::L>));
  0x76, 1, "hlt", "Halt processor", &Intel8080::op_hlt));
  0x77, 1, "mov", "Move A to M", &Intel8080::op_mov<Register::M, Register::A>));
  0x78, 1, "mov", "Move B to A", &Intel8080::op_mov<Register::A, Register::B>));
  0x79, 1, "mov", "Move C to A", &Intel8080::op_mov<Register::A, Register::C>));
  0x7a, 1, "mov", "Move D to A", &Intel8080::op_mov<Register::A, Register::D>));
  0x7b, 1, "mov", "Move E to A", &Intel8080::op_mov<Register::A, Register::E>));
  0x7c, 1, "mov", "Move H to A", &Intel8080::op_mov<Register::A, Register::H>));
  0x7d, 1, "mov", "Move L to A", &Intel8080::op_mov<Register::A, Register::L>));
  0x7e, 1, "mov", "Move M to A", &Intel8080::op_mov<Register::A, Register::M>));
  0x7f, 1, "mov", "Move A to A", &Intel8080::op_mov<Register::A, Register::A>));
  0x80, 1, "add", "Add B to A", &Intel8080::op_add<Register::B>));
  0x81, 1, "add", "Add C to A", &Intel8080::op_add<Register::C>));
  0x82, 1, "add", "Add D to A", &Intel8080::op_add<Register::D>));
  0x83, 1, "add", "Add E to A", &Intel8080::op_add<Register::E>));
  0x84, 1, "add", "Add H to A", &Intel8080::op_add<Register::H>));
  0x85, 1, "add", "Add L to A", &Intel8080::op_add<Register::L>));
  0x86, 1, "add", "Add M to A", &Intel8080::op_add<Register::M>));
  0x87, 1, "add", "Add A to A", &Intel8080::op_add<Register::A>));
  0x88, 1, "adc", "Add with carry B to A", &Intel8080::op_adc<Register::B>));
  0x89, 1, "adc", "Add with carry C to A", &Intel8080::op_adc<Register::C>));
  0x8a, 1, "adc", "Add with carry D to A", &Intel8080::op_adc<Register::D>));
  0x8b, 1, "adc", "Add with carry E to A", &Intel8080::op_adc<Register::E>));
  0x8c, 1, "adc", "Add with carry H to A", &Intel8080::op_adc<Register::H>));
  0x8d, 1, "adc", "Add with carry L to A", &Intel8080::op_adc<Register::L>));
  0x8e, 1, "adc", "Add with carry M to A", &Intel8080::op_adc<Register::M>));
  0x8f, 1, "adc", "Add with carry A to A", &Intel8080::op_adc<Register::A>));
  0x90, 1, "sub", "Substract B from A", &Intel8080::op_sub<Register::B>));
  0x91, 1, "sub", "Substract C from A", &Intel8080::op_sub<Register::C>));
  0x92, 1, "sub", "Substract D from A", &Intel8080::op_sub<Register::D>));
  0x93, 1, "sub", "Substract E from A", &Intel8080::op_sub<Register::E>));
  0x94, 1, "sub", "Substract H from A", &Intel8080::op_sub<Register::H>));
  0x95, 1, "sub", "Substract L from A", &Intel8080::op_sub<Register::L>));
  0x96, 1, "sub", "Substract M from A", &Intel8080::op_sub<Register::M>));
  0x97, 1, "sub", "Substract A from A", &Intel8080::op_sub<Register::A>));
  0x98, 1, "sbb", "Substract with borrow B from A", &Intel8080::op_sbb<Register::B>));
  0x99, 1, "sbb", "Substract with borrow C from A", &Intel8080::op_sbb<Register::C>));
  0x9a, 1, "sbb", "Substract with borrow D from A", &Intel8080::op_sbb<Register::D>));
  0x9b, 1, "sbb", "Substract with borrow E from A", &Intel8080::op_sbb<Register::E>));
  0x9c, 1, "sbb", "Substract with borrow H from A", &Intel8080::op_sbb<Register::H>));
  0x9d, 1, "sbb", "Substract with borrow L from A", &Intel8080::op_sbb<Register::L>));
  0x9e, 1, "sbb", "Substract with borrow M from A", &Intel8080::op_sbb<Register::M>));
  0x9f, 1, "sbb", "Substract with borrow A from A", &Intel8080::op_sbb<Register::A>));
  0xa0, 1, "ana", "AND B with A", &Intel8080::op_ana<Register::B>));
  0xa1, 1, "ana", "AND C with A", &Intel8080::op_ana<Register::C>));
  0xa2, 1, "ana", "AND D with A", &Intel8080::op_ana<Register::D>));
  0xa3, 1, "ana", "AND E with A", &Intel8080::op_ana<Register::E>));
  0xa4, 1, "ana", "AND H with A", &Intel8080::op_ana<Register::H>));
  0xa5, 1, "ana", "AND L with A", &Intel8080::op_ana<Register::L>));
  0xa6, 1, "ana", "AND M with A", &Intel8080::op_ana<Register::M>));
  0xa7, 1, "ana", "AND A with A", &Intel8080::op_ana<Register::A>));
  0xa8, 1, "xra", "XOR B with A", &Intel8080::op_xra<Register::B>));
  0xa9, 1, "xra", "XOR C with A", &Intel8080::op_xra<Register::C>));
  0xaa, 1, "xra", "XOR D with A", &Intel8080::op_xra<Register::D>));
  0xab, 1, "xra", "XOR E with A", &Intel8080::op_xra<Register::E>));
  0xac, 1, "xra", "XOR H with A", &Intel8080::op_xra<Register::H>));
  0xad, 1, "xra", "XOR L with A", &Intel8080::op_xra<Register::L>));
  0xae, 1, "xra", "XOR M with A", &Intel8080::op_xra<Register::M>));
  0xaf, 1, "xra", "XOR A with A", &Intel8080::op_xra<Register::A>));
  0xb0, 1, "ora", "OR B with A", &Intel8080::op_ora<Register::B>));
  0xb1, 1, "ora", "OR C with A", &Intel8080::op_ora<Register::C>));
  0xb2, 1, "ora", "OR D with A", &Intel8080::op_ora<Register::D>));
  0xb3, 1, "ora", "OR E with A", &Intel8080::op_ora<Register::E>));
  0xb4, 1, "ora", "OR H with A", &Intel8080::op_ora<Register::H>));
  0xb5, 1, "ora", "OR L with A", &Intel8080::op_ora<Register::L>));
  0xb6, 1, "ora", "OR M with A", &Intel8080::op_ora<Register::M>));
  0xb7, 1, "ora", "OR A with A", &Intel8080::op_ora<Register::A>));
  0xb8, 1, "cmp", "Compare B with A", &Intel8080::op_cmp<Register::B>));
  0xb9, 1, "cmp", "Compare C with A", &Intel8080::op_cmp<Register::C>));
  0xba, 1, "cmp", "Compare D with A", &Intel8080::op_cmp<Register::D>));
  0xbb, 1, "cmp", "Compare E with A", &Intel8080::op_cmp<Register::E>));
  0xbc, 1, "cmp", "Compare H with A", &Intel8080::op_cmp<Register::H>));
  0xbd, 1, "cmp", "Compare L with A", &Intel8080::op_cmp<Register::L>));
  0xbe, 1, "cmp", "Compare M with A", &Intel8080::op_cmp<Register::M>));
  0xbf, 1, "cmp", "Compare A with A", &Intel8080::op_cmp<Register::A>));
  0xc0, 1, "rnz", "Return if not zero", &Intel8080::op_r<Condition::ZERO_FLAG_NOT_SET>));
  0xc1, 1, "pop", "Pop to B:C off stack", &Intel8080::op_pop<RegisterPair::BC>));
  0xc2, 3, "jnz", "Jump if not zero", &Intel8080::op_j<Condition::ZERO_FLAG_NOT_SET>));
  0xc3, 3, "jmp", "Unconditional jump", &Intel8080::op_jmp));
  0xc4, 3, "cnz", "Call if not zero", &Intel8080::op_c<Condition::ZERO_FLAG_NOT_SET>));
  0xc5, 1, "push", "Push B:C onto stack", &Intel8080::op_push<RegisterPair::BC>));
  0xc6, 1, "adi", "Add immediate with A", &Intel8080::op_adi));
  0xc7, 1, "rst", "Reset 0", &Intel8080::op_rst<0>));
  0xc8, 1, "rz", "Return if zero", &Intel8080::op_r<Condition::ZERO_FLAG_SET>));
  0xc9, 1, "ret", "Return", &Intel8080::op_ret));
  0xca, 3, "jz", "Jump if zero", &Intel8080::op_j<Condition::ZERO_FLAG_SET>));
  0xcb, 3, "jmp", "Unconditional jump", &Intel8080::op_jmp));
  0xcc, 3, "cz", "Call if zero", &Intel8080::op_c<Condition::ZERO_FLAG_SET>));
  0xcd, 3, "call", "Unconditional call", &Intel8080::op_call));
  0xce, 2, "aci", "Add immediate to A with carry", &Intel8080::op_aci));
  0xcf, 1, "rst", "Reset 1", &Intel8080::op_rst<1>));
  0xd0, 1, "rnc", "Return if not carry", &Intel8080::op_r<Condition::CARRY_FLAG_NOT_SET>));
  0xd1, 1, "pop", "Pop to D:E off stack", &Intel8080::op_pop<RegisterPair::DE>));
  0xd2, 3, "jnc", "Jump if not carry", &Intel8080::op_j<Condition::CARRY_FLAG_NOT_SET>));
  0xd3, 2, "out", "Not implemented", nullptr));
  0xd4, 3, "cnc", "Call if not carry", &Intel8080::op_c<Condition::CARRY_FLAG_NOT_SET>));
  0xd5, 1, "push", "Push D:E onto stack", &Intel8080::op_push<RegisterPair::DE>));
  0xd6, 2, "sui", "Substract immediate from A", &Intel8080::op_sui));
  0xd7, 1, "rst", "Reset 2", &Intel8080::op_rst<2>));
  0xd8, 1, "rc", "Return if carry", &Intel8080::op_r<Condition::CARRY_FLAG_SET>));
  0xd9, 1, "ret", "Unconditional return", &Intel8080::op_ret));
  0xda, 3, "jc", "Jump if carry", &Intel8080::op_j<Condition::CARRY_FLAG_SET>));
  0xdb, 2, "in", "Not implemented", nullptr));
  0xdc, 3, "cc", "Call if carry", &Intel8080::op_c<Condition::CARRY_FLAG_SET>));
  0xdd, 3, "call", "Unconditional call", &Intel8080::op_call));
  0xde, 2, "sbi", "Substract immediate from A with borrow", &Intel8080::op_sbi));
  0xdf, 1, "rst", "Reset 3", &Intel8080::op_rst<3>));
  0xe0, 1, "rpo", "Return if parity odd", &Intel8080::op_r<Condition::PARITY_FLAG_NOT_SET>));
  0xe1, 1, "pop", "Pop to H:L off stack", &Intel8080::op_pop<RegisterPair::HL>));
  0xe2, 3, "jpo", "Jump if parity flag odd", &Intel8080::op_j<Condition::PARITY_FLAG_NOT_SET>));
  0xe3, 1, "xthl", "Exchange H:L with top word on the stack", &Intel8080::op_xthl));
  0xe4, 3, "cpo", "Call if parity flag odd", &Intel8080::op_c<Condition::PARITY_FLAG_NOT_SET>));
  0xe5, 1, "push", "Push H:L onto stack", &Intel8080::op_push<RegisterPair::HL>));
  0xe6, 2, "ani", "And immediate with A", &Intel8080::op_ani));
  0xe7, 1, "rst", "Reset 4", &Intel8080::op_rst<4>));
  0xe8, 1, "rpe", "Return if parity even", &Intel8080::op_r<Condition::PARITY_FLAG_SET>));
  0xe9, 1, "pchl", "Jump to [H:L]", &Intel8080::op_pchl));
  0xea, 3, "jpe", "Jump if parity flag even", &Intel8080::op_j<Condition::PARITY_FLAG_SET>));
  0xeb, 1, "xchg", "Exchange registers D:E and H:L", &Intel8080::op_xchg));
  0xec, 3, "cpe", "Call if parity flag even", &Intel8080::op_j<Condition::PARITY_FLAG_SET>));
  0xed, 3, "call", "Unconditional call", &Intel8080::op_call));
  0xee, 2, "xri", "XOR immediate with A", &Intel8080::op_xri));
  0xef, 1, "rst", "Reset 5", &Intel8080::op_rst<5>));
  0xf0, 1, "rp", "Return if positive", &Intel8080::op_r<Condition::SIGN_FLAG_NOT_SET>));
  0xf1, 1, "pop", "Pop to FLAGS:A off stack", &Intel8080::op_pop<RegisterPair::SP>));
  0xf2, 3, "jp", "Jump if positive", &Intel8080::op_j<Condition::SIGN_FLAG_NOT_SET>));
  0xf3, 1, "di", "Not implemented", nullptr));
  0xf4, 3, "cp", "Call if positive", &Intel8080::op_c<Condition::SIGN_FLAG_NOT_SET>));
  0xf5, 1, "push", "Push FLAGS:A onto stack", &Intel8080::op_push<RegisterPair::SP>));
  0xf6, 2, "ori", "OR immediate with A", &Intel8080::op_ori));
  0xf7, 1, "rst", "Reset 6", &Intel8080::op_rst<6>));
  0xf8, 1, "rm", "Return if negative", &Intel8080::op_r<Condition::SIGN_FLAG_SET>));
  0xf9, 1, "sphl", "Set SP to H:L", &Intel8080::op_sphl));
  0xfa, 3, "jm", "Jump if negative", &Intel8080::op_j<Condition::SIGN_FLAG_SET>));
  0xfb, 1, "ei", "Not implemented", nullptr));
  0xfc, 3, "cm", "Call if negative", &Intel8080::op_c<Condition::SIGN_FLAG_SET>));
  0xfd, 3, "call", "Unconditional call", &Intel8080::op_call));
  0xfe, 2, "cpi", "Compare immediate with A", &Intel8080::op_cpi));
  0xff, 1, "rst", "Reset 7", &Intel8080::op_rst<7>));
    ]
    '''

  def first_pass(self):
    for line in self.data:
      if not line.startswith(';'):
        self.first_pass_data.append(line)
    
    self.first_pass_data = [line.strip() for line in self.first_pass_data if line != '']
    d = []
    for line in self.first_pass_data:
      l = line.split(';')
      d.append(l[0])
    self.first_pass_data = [line.strip().replace('\t', ' ').lower() for line in d if line != '']
    for line in self.first_pass_data:
      print(line)
  
  def second_pass(self):

    for line in self.first_pass_data:
      l = line.split(' ')
      if l[0] == 'org':
        if l[1].endswith('h'):
          self.origin = int(l[1][:-1], 16)
        else:
          self.origin = int(l[1])
      else:
        if l[0] in self.opcodes:
          if 'none' in self.opcodes[l[0]][1]:
            self.output.append(self.opcodes[l[0]][1]['none'])


parser = argparse.ArgumentParser(description='Intel8080 assembler')
parser.add_argument('file')
args = parser.parse_args()

with open(args.file, 'r') as f:
  data = f.read()
  asm = Assembler(data)
  f.close()


asm.first_pass()
asm.second_pass()
print(asm.output)