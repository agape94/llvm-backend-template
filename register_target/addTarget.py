import platform
import os
from pathlib import PureWindowsPath
from enum import Enum
from .modifications import *
from variables import *


def addTarget(target_name, bit_width, endianness, environment_path, llvm_version=LLVM_DEFAULT_VERSION_SHORT):
  environment_path = os.path.abspath(environment_path)

  if platform.system() == "Windows":
    environment_path = str(PureWindowsPath(environment_path))

  applyModifications(PROJECT.LLVM, environment_path,
                     target_name, bit_width, endianness, llvm_version)
  applyModifications(PROJECT.CLANG, environment_path,
                     target_name, bit_width, endianness, llvm_version)
  applyModifications(PROJECT.LLD, environment_path,
                     target_name, bit_width, endianness, llvm_version)
