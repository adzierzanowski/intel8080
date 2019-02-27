class Register:
  def __init__(self, mnemonic, size, reference=None):
    self.value = 0x00
    self.size = size
    self.mnemonic = mnemonic
    self.reference = reference

  def __repr__(self):
    if self.mnemonic == 'fl':
      return '{}: {:08b}'.format(self.mnemonic, self.value)
    return '{}: {}'.format(self.mnemonic, hex(self.value))

class CPU:
  def __init__(self, ram):
    self.ram = ram
    self.registers = [
      # General purpose registers
      Register('a', 1, reference=0b111),
      Register('b', 1, reference=0b000),
      Register('c', 1, reference=0b001),
      Register('d', 1, reference=0b010),
      Register('e', 1, reference=0b011),
      Register('h', 1, reference=0b100),
      Register('l', 1, reference=0b101),

      Register('sp', 2), # Stack Pointer
      Register('pc', 2), # Program Counter

      # Flag register:
      #   bit  meaning
      #     0  Carry
      #     1
      #     2  Parity
      #     3
      #     4  (AC)
      #     5
      #     6  Zero
      #     7  Sign
      Register('fl', 1)
    ]


  def getRegisterByMnemonic(self, mnemonic):
    return list(filter(lambda m: m.mnemonic == mnemonic, self.registers))[0]

  def getRegisterByReference(self, reference):
    try:
      return list(filter(lambda m: m.reference == reference, self.registers))[0]
    except IndexError:
      return Register('memref', 0, reference=0b110)

  def execute(self, address):
    pc = self.getRegisterByMnemonic('pc')
    pc.value = address
    self.executeInstruction(self.ram.data[pc.value])

  def executeInstruction(self, instruction):
    sdFields = [0b110] # memref
    sdFields += [reg.reference for reg in self.registers if reg.reference is not None]
    pc = self.getRegisterByMnemonic('pc')

    if instruction in [0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38]:
      print(hex(instruction), 'nop')
      pc.value += 1
      self.nop()

    # jmp (unconditional)
    elif instruction == 0xc3:
      pc.value = self.ram.data[pc.value+1] | (self.ram.data[pc.value+2] << 8)

    # hlt
    elif instruction == 0x76:
      print(hex(instruction), 'hlt')

    # mov, hlt excluded earlier
    elif all([
          instruction >> 3 & 0x7 in sdFields,
          instruction & 0x7 in sdFields,
          ~(instruction & 0x80),
          instruction & 0x40
        ]):

      print(
        hex(instruction),
        'mov',
        self.getRegisterByReference(instruction >> 3 & 7).mnemonic,
        self.getRegisterByReference(instruction & 7).mnemonic
      )
    elif instruction in [0x06, 0x0e, 0x16, 0x1e, 0x26, 0x2e, 0x36, 0x3e]:
      print(
        hex(instruction),
        'mvi',
        self.getRegisterByReference(instruction >> 3 & 7).mnemonic,
        'imm'
      )

    else:
      print(hex(instruction), 'unknown instruction')

    self.execute(pc.value)

  def mov(self, source, dest):
    pass

  def nop(self):
    pass
