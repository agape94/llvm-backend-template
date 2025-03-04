import os
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


LIB_ROOT = os.path.abspath(os.path.dirname(__file__))
CONFIG_ROOT = os.path.abspath(
    os.path.join(LIB_ROOT, "register_target", "configs"))
BUILD_CONFIG_ROOT = os.path.join(LIB_ROOT, "build_config")

TEMPLATE_TARGET_NAME = "Skeleton"
TEMPLATE_BACKENDS_PATH = os.path.join(LIB_ROOT, "backends")

EXPERIMENTAL_TARGETS_FILE_NAME = "experimental_targets_to_build.ini"
EXPERIMENTAL_TARGETS_FILE_PATH = os.path.join(
    BUILD_CONFIG_ROOT, EXPERIMENTAL_TARGETS_FILE_NAME)

OFFICIAL_TARGETS_FILE_NAME = "official_targets_to_build.ini"
OFFICIAL_TARGETS_FILE_PATH = os.path.join(
    BUILD_CONFIG_ROOT, OFFICIAL_TARGETS_FILE_NAME)

LLVM_DEFAULT_VERSION = SupportedLLVMVersions.LLVM_19.value
LLVM_DEFAULT_VERSION_SHORT = LLVM_DEFAULT_VERSION.split(".")[0]
