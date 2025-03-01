import clang.cindex
from clang.cindex import Index, Config, CursorKind
import platform
import os
from pathlib import PureWindowsPath
from enum import Enum
from .utils import *
from .modifications import *
from .parseCpp import *
from .addContent import *


def addTarget(target_name, bit_width, endianness, environment_path):
  environment_path = os.path.abspath(environment_path)

  if platform.system() == "Windows":
    environment_path = str(PureWindowsPath(environment_path))

  download_and_extract_llvm_toolchain()

  # applyModifications(PROJECT.LLVM, environment_path,
  #                    target_name, bit_width, endianness)
  applyModifications(PROJECT.CLANG, environment_path,
                     target_name, bit_width, endianness)
  # applyModifications(PROJECT.LLD, environment_path, target_name, bit_width, endianness)


# ==================================================================================================================
# Handle modifications
