from invoke import task
import subprocess
import os
import wget
import zipfile
import shutil
import re
from replaceString import *
from compileTasks import *
from register_target.addTarget import *

# ==================================================================================================================


def info(msg):
  print(f"INFO: {msg}")


def error(msg):
  print(f"ERROR: {msg}")

# ==================================================================================================================


def download_and_extract_release(extract_path, repo_name, llvm_version, cleanup=False):

  extract_folder_name = f"llvm-project-llvmorg-{llvm_version}"
  zip_file_name = f"{extract_folder_name}.zip"
  temp_zip_path = os.path.join(os.getcwd(), zip_file_name)
  extract_folder_path = os.path.join(extract_path, extract_folder_name)

  LLVM_RELEASE_LINK = f"https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-{llvm_version}.zip"

  if os.path.exists(temp_zip_path):
    info(f"{temp_zip_path} already exists, skipping download")
  else:
    info(f"Downloading LLVM release from {LLVM_RELEASE_LINK} ...")
    temp_zip_path = wget.download(LLVM_RELEASE_LINK, out=os.getcwd(), bar=None)
    if temp_zip_path is None:
      raise RuntimeError(
          f"Encountered errors when downloading the LLVM release.")
    info(f"Succesfully downloaded {temp_zip_path}")

  info(f"Extracting {temp_zip_path} ...")
  with zipfile.ZipFile(temp_zip_path, 'r') as zip_ref:
    zip_ref.extractall(extract_path)
  info(f"Succesfully extracted {temp_zip_path} to {extract_path}")

  env_folder_path = os.path.join(extract_path, repo_name)
  info(f"Renaming folder to {repo_name} ...")

  os.rename(extract_folder_path, env_folder_path)

# ==================================================================================================================


def copy_directory_recursively(src, dst):
    # this call will raise an exception if something goes wrong
  shutil.copytree(src, dst)

# ==================================================================================================================


def copy_file(src, dst):
  shutil.copyfile(src, dst)


def write_file(path, content):
  with open(path, "a") as f:
    f.write(content)

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

  if not re.match(r"\d{1,2}\.\d{1,2}(\.\d{1,2})?", llvm_version):
    raise ValueError(
        "Please provide a valid LLVM version. Supported LLVM versions are 17.x.x")

  repo_name = f"llvm-{target_name.lower()}-backend"
  new_backend_path = os.path.join(path, repo_name)

  if os.path.exists(new_backend_path) and len(os.listdir(new_backend_path)) != 0:
    raise OSError(f"Directory {new_backend_path} is not empty. ")

# ==================================================================================================================


@task(aliases=['se'],
      help={"target_name": "The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM, Mips",
            "path": "Location on disk where the new environment will be placed",
            "arch": "Bit width of the new target. Default value: 32. Possible values: 8, 16, 32 or 64",
            "endianess": "Whether the new target is little-endian of big-endian. Default value: little. Possible values: 'little' or 'big'",
            "llvm_version": f"Which LLVM release to use. Default version: {LLVM_DEFAULT_VERSION}. You could use a different version if you want to, however, the template was only tested with 17.0.6, therefore it is not guarateed to work properly with other LLVM versions."})
def setup_new_environment(c, target_name, path, arch=32, endianess="little", llvm_version=LLVM_DEFAULT_VERSION):
  """
    Setup a new LLVM backend development environment for the given target name.
  """
  template_target_name = "Skeleton"

  check_arguments(target_name, path, arch, endianess, llvm_version)

  info(
      f"Setting up new LLVM backend development environment for the target `{target_name}`")

  repo_name = f"llvm-{target_name.lower()}-backend"
  environment_path = os.path.abspath(os.path.join(path, repo_name))
  project_root = os.getcwd()

  # download LLVM release
  download_and_extract_release(
      extract_path=path, repo_name=repo_name, llvm_version=llvm_version)

  if not os.path.exists(environment_path) or len(os.listdir(environment_path)) == 0:
    raise OSError(f"Something went wrong during archive extraction")

  write_file(os.path.join(environment_path,
             ".experimental_targets_to_build"), target_name)

  # copy the template folder to the given location
  template_path = os.path.join(project_root, template_target_name)
  target_folder_path = os.path.join(
      environment_path, "llvm", "lib", "Target", target_name)
  info(
      f"Copying the template folder {template_path} into the {target_folder_path} folder ... ")
  copy_directory_recursively(src=template_path, dst=target_folder_path)

  # replace all occurences of `template_target_name` with the given target name
  info(
      f"Search and replace `{template_target_name}` target name with {target_name} in the backend folder ... ")
  if target_name != template_target_name:
    search_replace_in_directory(
        target_folder_path, target_name, search_string=template_target_name)

  addTarget(target_name, arch, endianess, environment_path)

  copy_file(os.path.join(project_root, "compileTasks.py"),
            os.path.join(environment_path, "tasks.py"))
  os.chdir(project_root)

  info(
      f"Successfully created new LLVM Backend Development environment for target {target_name} at {environment_path}")
