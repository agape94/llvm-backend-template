import os
import platform
from enum import Enum

class SupportedLLVMVersions(Enum):
  # We are supporting the latest major releases of LLVM
  LLVM_17 = "17.0.6"
  LLVM_18 = "18.1.8"
  LLVM_19 = "19.1.7"

  def get_llvm_version(version_number):
    version_str = f"LLVM_{version_number}"
    try:
      return SupportedLLVMVersions[version_str]
    except KeyError:
      raise ValueError(f"Unsupported LLVM version: {version_number}")

ROOT_DIRECTORY = os.path.abspath(os.path.dirname(__file__))
CONFIG_ROOT = os.path.abspath(
    os.path.join(ROOT_DIRECTORY, "register_target", "configs"))

TEMPLATE_TARGET_NAME = "Skeleton"
TEMPLATE_BACKENDS_PATH = os.path.join(ROOT_DIRECTORY, "backends")
EXPERIMENTAL_TARGETS_FILE = "experimental_targets_to_build.ini"
UPSTREAM_TARGETS_FILE = "official_targets_to_build.ini"
LLVM_DEFAULT_VERSION = SupportedLLVMVersions.LLVM_19.value
