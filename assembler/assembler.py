#!/usr/bin/env python3

import re
import sys
import json
import argparse

class Assembler:
  def __init__(self, data):
    self.data = data
    self.data.replace('\t', ' ')
    self.output = []
    self.instructions = []
    self.labels = {}
    self.defines = {}

    with open('opcodes.json', 'r') as o:
      self.opcodes = json.load(o)

    self.origin = 0

  def prepare(self):
    self.data = re.sub(r'[ ][ ]+', ' ', self.data).splitlines()
    self.data = [line for line in self.data if not line.startswith(';')]
    self.data = [''.join(line.split(';')[0].strip()) for line in self.data if line != '']

  @staticmethod
  def get_number(n):
    if n.startswith('0x'):
      return int(n[2:], 16)
    if n.endswith('h'):
      return int(n[:-1], 16)
    return int(n)
  
  @staticmethod
  def make_word(n):
    hb = n >> 8
    lb = n & 0x00ff
    return [lb, hb]

  def first_pass(self):
    offset = 0
    for i, line in enumerate(self.data):
      if line.startswith('.'):
        l = line[1:].split(' ')
        if l[0] == 'org':
          self.origin = Assembler.get_number(l[1])
          if self.origin > 0xffff:
            sys.stderr.write('Error: origin greater than 0xffff.\n')
            sys.exit(1)
        continue

      # I forgot what it does the minute I wrote it
      if line.startswith('db'):
        define = []
        l = line[3:]
        l = [ll for ll in re.split(r'(\".*?\")', l) if ll != '']
        for ll in l:
          if ll.startswith('"'):
            lll = ll[1:-1]
            for llll in lll:
              define.append(ord(llll))
          else:
            lll = ll.split(',')
            llll = [lllll.strip() for lllll in lll if lllll.strip() != '']
            for lllll in llll:
              define.append(Assembler.get_number(lllll))

        self.instructions.append({'define': define})
        continue

      line = line.lower()
      if line in self.opcodes:
        instruction = self.opcodes[line]
        self.instructions.append({'offset': offset, 'instruction': instruction, 'argument': None})
        offset += instruction['length']
      else:
        ls = line.split(',')
        l = ls[0]
        if l in self.opcodes:
          instruction = self.opcodes[l]
          arg = None
          if instruction['length'] > 1:
            arg = ls[1].strip()
          self.instructions.append({'offset': offset, 'instruction': instruction, 'argument': arg})
          offset += instruction['length']
          continue

        ls = line.split(' ')
        l = ls[0]
        if l in self.opcodes.keys():
          instruction = self.opcodes[l]
          arg = None
          if instruction['length'] > 1:
            arg = ls[1].strip()
          self.instructions.append({'offset': offset, 'instruction': instruction, 'argument': arg})
          offset += instruction['length']
        elif l.endswith(':'):
          l = l[:-1]
          if l in self.labels:
            sys.stderr.write('Error: Two labels with the same name: {}.\n'.format(l))
            sys.exit(1)
          self.labels[l] = self.origin + offset

  def second_pass(self):
    for i in self.instructions:
      if 'define' in i:
        self.output += i['define']
        continue

      self.output.append(i['instruction']['opcode'])
      if i['instruction']['length'] > 1:
        if i['argument'] is None:
          req_arg = 'byte' if i['instruction']['length'] == 2 else 'word'
          err = 'Error: Instruction `{}` requires a {} as an argument.\n'.format(
            i['instruction']['mnemonic'],
            req_arg
          )
          sys.stderr.write(err)
          sys.exit(1)

        # omfg, this is probably the ugliest piece of code I've ever written
        arg = i['argument']
        if arg in self.labels:
          arg = str(self.labels[arg]) # converting int to string to get int
        arg = Assembler.get_number(arg)
        if i['instruction']['length'] == 3:
          arg = Assembler.make_word(arg)
        else:
          arg = [arg] # lol
        self.output += arg

  def assemble(self):
    self.prepare()
    self.first_pass()
    self.second_pass()

parser = argparse.ArgumentParser(description='Intel8080 assembler')
parser.add_argument('file')
parser.add_argument('-o', '--output', required=True)
args = parser.parse_args()

with open(args.file, 'r') as f:
  asm = Assembler(f.read())
  f.close()

asm.assemble()

with open(args.output, 'wb') as f:
  f.write(bytes(asm.output))
