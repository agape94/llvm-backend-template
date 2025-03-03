import os
import platform

TEMPLATE_TARGET_NAME = "Skeleton"
EXPERIMENTAL_TARGETS_FILE = "experimental_targets_to_build.ini"
UPSTREAM_TARGETS_FILE = "official_targets_to_build.ini"
LLVM_DEFAULT_VERSION = "19.1.7"

ROOT_DIRECTORY = os.path.abspath(os.path.dirname(__file__))
CONTRIB_ROOT = os.path.abspath(os.path.join(ROOT_DIRECTORY, "contrib"))

LLVM_TOOLCHAIN_PATH = ""
if platform.system() == "Windows":
  LLVM_TOOLCHAIN_PATH = os.path.abspath(os.path.join(CONTRIB_ROOT, "Windows"))
elif platform.system() == "Linux":
  LLVM_TOOLCHAIN_PATH = os.path.abspath(os.path.join(CONTRIB_ROOT, "Linux"))
elif platform.system() == "Darwin":
  LLVM_TOOLCHAIN_PATH = os.path.abspath(os.path.join(CONTRIB_ROOT, "MacOS"))
else:
  raise RuntimeError(f"Unsupported operating system: {platform.system()}")

LLVM_TOOLCHAIN_VERSION = "19.1.7"
LLVM_TOOLCHAIN_BASE_VERSION = LLVM_TOOLCHAIN_VERSION.split(".")[0]

CONFIG_ROOT = os.path.abspath(
    os.path.join(ROOT_DIRECTORY, "register_target", "configs"))
