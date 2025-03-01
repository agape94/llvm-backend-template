import platform
import os
from pathlib import PureWindowsPath
import wget
import tarfile
import yaml
from .modifications import *
from .variables import *


# ==================================================================================================================
# Utility functions


def download_and_extract_llvm_toolchain():
  """
  Downloads and extracts the LLVM toolchain.
  """
  if os.path.exists(LLVM_TOOLCHAIN_PATH) and os.path.isdir(LLVM_TOOLCHAIN_PATH) and os.listdir(LLVM_TOOLCHAIN_PATH):
    return

  LLVM_TOOLCHAIN_ARCHIVE = ""
  LLVM_TOOLCHAIN_LINK = ""

  # Download the LLVM toolchain
  system = platform.system()
  if system == "Linux":
    LLVM_TOOLCHAIN_ARCHIVE = "clang+llvm-19.1.7-aarch64-linux-gnu.tar.xz"
    LLVM_TOOLCHAIN_LINK = f"https://github.com/llvm/llvm-project/releases/download/llvmorg-{LLVM_TOOLCHAIN_VERSION}/{LLVM_TOOLCHAIN_ARCHIVE}"
  elif system == "Darwin":  # macOS
    LLVM_TOOLCHAIN_ARCHIVE = "clang+llvm-19.1.7-armv7a-linux-gnueabihf.tar.gz "
    LLVM_TOOLCHAIN_LINK = f"https://github.com/llvm/llvm-project/releases/download/llvmorg-{LLVM_TOOLCHAIN_VERSION}/{LLVM_TOOLCHAIN_ARCHIVE}"
  elif system == "Windows":
    LLVM_TOOLCHAIN_ARCHIVE = "clang+llvm-19.1.7-x86_64-pc-windows-msvc.tar.xz"
    LLVM_TOOLCHAIN_LINK = f"https://github.com/llvm/llvm-project/releases/download/llvmorg-{LLVM_TOOLCHAIN_VERSION}/{LLVM_TOOLCHAIN_ARCHIVE}"
  else:
    raise RuntimeError(f"Unsupported operating system: {system}")

  LLVM_TOOLCHAIN_ARCHIVE_PATH = os.path.join(
      os.path.dirname(__file__), LLVM_TOOLCHAIN_ARCHIVE)
  if os.path.exists(LLVM_TOOLCHAIN_ARCHIVE_PATH):
    print(f"{LLVM_TOOLCHAIN_ARCHIVE_PATH} already exists, skipping download")
  else:
    print(f"Downloading LLVM toolchain from {LLVM_TOOLCHAIN_LINK} ...")
    wget.download(LLVM_TOOLCHAIN_LINK, LLVM_TOOLCHAIN_ARCHIVE_PATH, bar=None)

  # Extract the downloaded archive
  print(f"Extracting {LLVM_TOOLCHAIN_ARCHIVE_PATH} ...")
  if LLVM_TOOLCHAIN_ARCHIVE_PATH.endswith(".tar.gz") or LLVM_TOOLCHAIN_ARCHIVE_PATH.endswith(".tgz"):
    mode = "r:gz"
  elif LLVM_TOOLCHAIN_ARCHIVE_PATH.endswith(".tar.xz"):
    mode = "r:xz"
  else:
    mode = "r"

  with tarfile.open(LLVM_TOOLCHAIN_ARCHIVE_PATH, mode) as tar:
    tar.extractall(path=LLVM_TOOLCHAIN_PATH)
  print(f"Extraction complete.")

# ==================================================================================================================
