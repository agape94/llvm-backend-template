from clang.cindex import Index
import os
from pathlib import PureWindowsPath
from variables import LLVM_TOOLCHAIN_PATH, LLVM_TOOLCHAIN_BASE_VERSION


def parseFile(file_path):
  """
  Parses the given C++ header/source file and returns the AST.
  """
  # parse the file
  LLVM_TOOLCHAIN_INCLUDE_PATH = os.path.join(
      LLVM_TOOLCHAIN_PATH, "include")
  LLVM_INCLUDE_PATH = os.path.join(LLVM_TOOLCHAIN_INCLUDE_PATH, "llvm")
  CLANG_INCLUDE_PATH = os.path.join(LLVM_TOOLCHAIN_INCLUDE_PATH, "clang")
  CLANG_RESOURCE_PATH = os.path.join(
      LLVM_TOOLCHAIN_INCLUDE_PATH, "lib", "clang", LLVM_TOOLCHAIN_BASE_VERSION, "include")
  CPP_INCLUDE_PATH = os.path.join(LLVM_TOOLCHAIN_INCLUDE_PATH, "c++", "v1")
  CPP_STANDARD = "c++17"

  index = Index.create()

  args = [
      f"-std={CPP_STANDARD}",
      f"-I{CLANG_RESOURCE_PATH}",
      f"-I{CPP_INCLUDE_PATH}",
      f"-I{LLVM_INCLUDE_PATH}",
      f"-I{CLANG_INCLUDE_PATH}",
      "-x", "c++",
  ]  # Adjust based on your C++ standard

  return index.parse(file_path, args=args)
