import clang.cindex
from clang.cindex import Index, Config, CursorKind
import platform
import os
from pathlib import PureWindowsPath
from enum import Enum
from .modifications import *
from .parseCpp import *


def addTarget(target_name, bit_width, endianness, environment_path):
  environment_path = os.path.abspath(environment_path)

  if platform.system() == "Windows":
    environment_path = str(PureWindowsPath(environment_path))

  applyModifications(PROJECT.LLVM, environment_path,
                     target_name, bit_width, endianness)
  applyModifications(PROJECT.CLANG, environment_path,
                     target_name, bit_width, endianness)
  applyModifications(PROJECT.LLD, environment_path,
                     target_name, bit_width, endianness)
