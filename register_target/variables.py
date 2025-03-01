import os

LLVM_TOOLCHAIN_PATH = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "llvm_toolchain"))
LLVM_TOOLCHAIN_VERSION = "19.1.7"
LLVM_TOOLCHAIN_BASE_VERSION = LLVM_TOOLCHAIN_VERSION.split(".")[0]

CONFIG_ROOT = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "configs"))
