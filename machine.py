class Machine:
  def __init__(self, cpu, ram):
    self.cpu = cpu
    self.ram = ram

  def print(self):
    for reg in self.cpu.registers:
      print(reg)

    self.ram.print()

  def loadProgram(self, program, address):
    '''Load program into memory at a given address'''
    for i, b in enumerate(program):
      self.ram.data[address + i] = b

  def executeProgram(self, address):
    self.cpu.execute(address)
