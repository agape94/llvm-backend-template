from invoke import task
import os
import wget
import zipfile
import shutil
from lib.register_target.addTarget import *
from lib.variables import *
from lib.utils import *

# ==================================================================================================================


def check_arguments(target_name, path, arch, endianess, llvm_version):
  if not target_name[0].isupper():
    raise ValueError(
        f"Please provide a valid target name (upper case and camel case when applicable)")

  if not os.path.isdir(path):
    os.mkdir(path)

  if not arch in {8, 16, 32, 64}:
    raise ValueError(
        f"Please provide a valid architecture bit width. Possible values are: 8, 16, 32, 64")

  if not endianess in {"little", "big"}:
    raise ValueError(
        f"Please provide a valid endianess. Possible values are: 'little' or 'big'")

  if not int(llvm_version) in {17, 18, 19, 20}:
    raise ValueError(
        "Please provide a valid major LLVM version. Supported versions are: 17, 18, 19, 20")

  repo_name = f"llvm-{target_name.lower()}-backend"
  new_backend_path = os.path.join(path, repo_name)

  if os.path.exists(new_backend_path) and len(os.listdir(new_backend_path)) != 0:
    raise OSError(f"Directory {new_backend_path} is not empty. ")

# ==================================================================================================================


@task(aliases=['se'],
      help={"target_name": "The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM, Mips",
            "path": "Location on disk where the new environment will be placed",
            "arch": "Bit width of the new target. Default value: 32. Possible values: 8, 16, 32 or 64",
            "endianness": "Whether the new target is little-endian of big-endian. Default value: little. Possible values: 'little' or 'big'",
            "llvm_version": f"Which LLVM release to use. Default version: {LLVM_DEFAULT_VERSION_SHORT}. Possible values: 17, 18, 19, 20"})
def setup_new_environment(c, target_name, path, arch=32, endianness="little", llvm_version=LLVM_DEFAULT_VERSION_SHORT):
  """
    Setup a new LLVM backend development environment for the given target name.
  """
  check_arguments(target_name, path, arch, endianness, llvm_version)

  info(
      f"Setting up new LLVM backend development environment for the target `{target_name}`")

  LLVM_VERSION_ENUM = SupportedLLVMVersions.get_llvm_version(llvm_version)

  repo_name = f"llvm-{target_name.lower()}-backend"
  environment_path = os.path.abspath(os.path.join(path, repo_name))

  # download LLVM release
  download_and_extract_release(
      extract_path=path, repo_name=repo_name, llvm_version=LLVM_VERSION_ENUM.value)

  if not os.path.exists(environment_path) or len(os.listdir(environment_path)) == 0:
    raise OSError(f"Something went wrong during archive extraction")

  copy_file(EXPERIMENTAL_TARGETS_FILE_PATH, os.path.join(
      environment_path, EXPERIMENTAL_TARGETS_FILE_NAME))

  create_target_in_environment(
      c, target_name, environment_path, arch, endianness, new_environment=True, llvm_version=llvm_version)

  copy_file(os.path.join(LIB_ROOT, "tasks", "environmentTasks.py.in"),
            os.path.join(environment_path, "tasks.py"))

  search_replace_in_file(os.path.join(environment_path, "tasks.py"),
                         "<EXPERIMENTAL_TARGETS_FILE_NAME>", EXPERIMENTAL_TARGETS_FILE_NAME)
  search_replace_in_file(os.path.join(environment_path, "tasks.py"),
                         "<OFFICIAL_TARGETS_FILE_NAME>", OFFICIAL_TARGETS_FILE_NAME)
  
  copy_file(os.path.join(LIB_ROOT, "vscode-workspace", "llvm-target-backend.code-workspace"),
            os.path.join(environment_path, f"llvm-{target_name.lower()}-backend.code-workspace"))
  
  search_replace_in_file(os.path.join(environment_path, f"llvm-{target_name.lower()}-backend.code-workspace"),
                         "<target>", target_name.lower())
  search_replace_in_file(os.path.join(environment_path, f"llvm-{target_name.lower()}-backend.code-workspace"),
                         "<Target>", target_name)
  search_replace_in_file(os.path.join(environment_path, f"llvm-{target_name.lower()}-backend.code-workspace"),
                         "<TARGET>", target_name.upper())

  copy_file(OFFICIAL_TARGETS_FILE_PATH, os.path.join(environment_path, OFFICIAL_TARGETS_FILE_NAME))
  
  copy_directory_recursively(src=os.path.join(LIB_ROOT, "tests"), dst=os.path.join(environment_path, "tests"))

  info(f"Successfully created new LLVM Backend Development environment for target {target_name} at {environment_path}")


# ==================================================================================================================


@task(aliases=['ct'],
      help={"target_name": "The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM, Mips",
            "environment_path": "Location on disk where the existing environment is placed",
            "arch": "Bit width of the new target. Default value: 32. Possible values: 8, 16, 32 or 64",
            "endianness": "Whether the new target is little-endian of big-endian. Default value: little. Possible values: 'little' or 'big'"})
def create_target_in_environment(c, target_name, environment_path, arch=32, endianness="little", new_environment=False, llvm_version=LLVM_DEFAULT_VERSION_SHORT):
  """
    Add a new target to an existing LLVM backend development environment.
  """
  check_arguments(target_name, environment_path,
                  arch, endianness, llvm_version)
  write_file(os.path.join(environment_path,
             EXPERIMENTAL_TARGETS_FILE_NAME), target_name)

  TEMPLATE_TARGET_FULL_NAME = TEMPLATE_TARGET_NAME + "-llvm-" + llvm_version

  # copy the template folder to the given location
  template_path = os.path.join(
      TEMPLATE_BACKENDS_PATH, TEMPLATE_TARGET_FULL_NAME)
  target_folder_path = os.path.join(
      environment_path, "llvm", "lib", "Target", target_name)
  info(
      f"Copying the template folder {template_path} into the {target_folder_path} folder ... ")
  copy_directory_recursively(src=template_path, dst=target_folder_path)

  # replace all occurences of `template_target_name` with the given target name
  info(
      f"Search and replace `{TEMPLATE_TARGET_NAME}` target name with {target_name} in the backend folder ... ")
  if target_name != TEMPLATE_TARGET_NAME:
    search_replace_in_directory(
        target_folder_path, target_name, search_string=TEMPLATE_TARGET_NAME)

  addTarget(target_name, arch, endianness, environment_path, llvm_version)

  if not new_environment:
    info(
        f"Successfully added new target {target_name} to existing environment at {environment_path}")
    