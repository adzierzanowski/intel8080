import sys

class Formatter:
  @staticmethod
  def section(title, end='\n'):
    print('\n\033[1m{}\033[0m:'.format(title), end=end)

  @staticmethod
  def warning(message):
    print('\033[1m\033[38;5;11m{}\033[0m: {}.'.format('Warning', message))

  @staticmethod
  def error(message):
    sys.stderr.write('\033[1m\033[38;5;9m{}\033[0m: {}.\n'.format('Error', message))
    sys.exit(1)
