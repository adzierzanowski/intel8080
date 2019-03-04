import os
import re
import json

from formatter import Formatter

INSTRUCTION_SET = os.path.dirname(os.path.realpath(__file__)) + '/instructions.json'

class Assembler:
  def __init__(self, data):
    self.data = data
    self.output = []
    self.instructions = []
    self.labels = {}
    self.defines = {}
    self.macros = {}
    self.offset = 0

    with open(INSTRUCTION_SET, 'r') as f:
      self.ins_set = json.load(f)

    self.origin = 0 # addresses offset

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

  def assemble(self):
    self.prepare()
    self.expand_macros()
    self.first_pass()
    self.second_pass()
    if len(self.output) > 0xffff:
      Formatter.error('Output exceeds memory limit by {} bytes'.format(len(self.output) - 0xffff))

  def prepare(self):
    self.data = self.data.replace('\t', ' ')
    self.data = self.data.replace('syscall', 'call 5')
    self.data = re.sub(r'[ ][ ]+', ' ', self.data).splitlines()
    self.data = [line for line in self.data if not line.startswith(';')]
    self.data = [''.join(line.split(';')[0].strip()) for line in self.data if line != '']

  def expand_macros(self):
    for line in self.data:
      if line.startswith('.def'):
        l = line.split(' ')
        self.macros[l[1]] = ' '.join(l[2:])

    self.data = '\n'.join(self.data)
    for macro, value in self.macros.items():
      self.data = self.data.replace(macro, value)
    self.data = self.data.splitlines()

  def parse_line(self, n):
    line = self.data[n]

    if self.is_directive(n):
      return
    if self.is_define(n):
      return
    if self.is_instruction(n):
      return
    if self.is_instruction_with_arg(n):
      return
    if self.is_label(n):
      return
    if line != '':
      Formatter.warning('Line `{}` is not parsable'.format(line))

  def first_pass(self):
    '''Find parsable things and segregate them'''
    for i, _ in enumerate(self.data):
      self.parse_line(i)

  def second_pass(self):
    for i in self.instructions:
      if 'define' in i:
        self.output += i['define']
        continue

      self.output.append(i['ins']['op'])
      if i['ins']['len'] > 1:
        if i['arg'] is None:
          req_arg = 'byte' if i['ins']['len'] == 2 else 'word'
          Formatter.error(
            'Instruction `{}` requires a {} as an argument'.format(i['ins']['mnem'], req_arg))

        # omfg, this is probably the ugliest piece of code I've ever written
        arg = i['arg']
        if arg in self.labels:
          arg = str(self.labels[arg]) # converting int to string to get int
        arg = Assembler.get_number(arg)
        if i['ins']['len'] == 3:
          arg = Assembler.make_word(arg)
        else:
          arg = [arg] # lol
        self.output += arg

  def is_directive(self, n):
    line = self.data[n]

    if line.startswith('.org'):
      l = line.split(' ')
      self.origin = Assembler.get_number(l[1])
      return True
    return False

  def is_define(self, n):
    line = self.data[n]

    if line.startswith('db '):
      define_bytes = []
      line = line[3:]
      line = [chunk for chunk in re.split(r'(\".*?\")', line) if chunk != '']
      for chunk in line:
        if chunk.startswith('"') or chunk.startswith(''):
          text = chunk[1:-1]
          for c in text:
            define_bytes.append(ord(c))
        else:
          num_literals = line.split(',')
          num_literals = [literal.strip() for literal in num_literals if literal.strip() != '']
          for literal in num_literals:
            define_bytes.append(Assembler.get_number(literal))

      self.instructions.append({'define': define_bytes})
      self.offset += len(define_bytes)
      return True
    return False

  def is_instruction(self, n):
    line = self.data[n].lower()
    line = re.sub(r'([abcdelhm]),([abcdelhm])', r'\1, \2', line)
    if line in self.ins_set:
      instruction = self.ins_set[line]
      self.instructions.append({'off': self.offset, 'ins': instruction, 'arg': None})
      self.offset += instruction['len']
      return True
    return False

  def is_instruction_with_arg(self, n):
    line = self.data[n].lower()
    space_split = line.split(' ')
    comma_split = line.split(',')
    space_instruction = space_split[0]
    comma_instruction = comma_split[0]

    ins = None
    if space_instruction in self.ins_set:
      ins = self.ins_set[space_instruction]
      line = space_split
    elif comma_instruction in self.ins_set:
      ins = self.ins_set[comma_instruction]
      line = comma_split

    if ins is not None:
      arg = None
      if ins['len'] > 1:
        arg = line[1].strip()
      self.instructions.append({'off': self.offset, 'ins': ins, 'arg': arg})
      self.offset += ins['len']
      return True
    return False

  def is_label(self, n):
    line = self.data[n].split(':')
    if len(line) < 2:
      return False

    label = line[0]
    if label in self.labels:
      Formatter.error('Two labels with the same name: `{}`'.format(label))

    self.labels[label] = self.origin + self.offset
    self.data[n] = ''.join(line[1:]).strip()
    self.parse_line(n)
    return True
