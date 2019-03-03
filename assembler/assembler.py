#!/usr/bin/env python3

import re
import json
import argparse

class Assembler:
  def __init__(self, data):
    self.data = data
    self.data.replace('\t', ' ')
    self.output = []

    with open('opcodes.json', 'r') as o:
      self.opcodes = json.load(o)

    self.origin = 0

  def prepare(self):
    self.data = re.sub(r'[ ][ ]+', ' ', self.data).lower().splitlines()
    self.first_pass_data = []
    self.data = [line for line in self.data if not line.startswith(';')]
    self.data = [''.join(line.split(';')[0].strip()) for line in self.data if line != '']
    for line in self.data:
      print(line)
    
  def first_pass(self):
    pass

  def second_pass(self):
    pass

parser = argparse.ArgumentParser(description='Intel8080 assembler')
parser.add_argument('file')
args = parser.parse_args()

with open(args.file, 'r') as f:
  asm = Assembler(f.read())
  f.close()

asm.prepare()
