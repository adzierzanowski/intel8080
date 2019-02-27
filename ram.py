class RAM:
  def __init__(self, size):
    self.size = size
    self.data = [0x00 for n in range(self.size)]

  def print(self, a=0, b=0x80):
    for i, v in enumerate(self.data[a:b]):
      if i % 16 == 0: 
        print('\n{:02x}: '.format(i+a), end='')
      print('{:02x} '.format(v), end='')

    print()
