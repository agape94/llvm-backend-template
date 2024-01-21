from invoke import task
import git
import os
import wget
import zipfile
import shutil
from replaceString import *
from compileTasks import *


LLVM_SUPPORTED_VERSION="17.0.2"
LLVM_RELEASE_LINK = f"https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-{LLVM_SUPPORTED_VERSION}.zip"

# ==================================================================================================================

def info (msg):
  print(f"INFO: {msg}")

def error (msg):
  print(f"ERROR: {msg}")

# ==================================================================================================================

def download_and_extract_release(extract_path, repo_name, cleanup=False):
  
  extract_folder_name = f"llvm-project-llvmorg-{LLVM_SUPPORTED_VERSION}"
  zip_file_name = f"{extract_folder_name}.zip"
  temp_zip_path = os.path.join("/tmp", zip_file_name)
  extract_folder_path = os.path.join(extract_path, extract_folder_name)
  
  if os.path.exists(temp_zip_path):
    info(f"{temp_zip_path} already exists, skipping download")
  else:
    info(f"Downloading LLVM release from {LLVM_RELEASE_LINK} ...")
    temp_zip_path = wget.download(LLVM_RELEASE_LINK, out="/tmp", bar=None)
    if temp_zip_path is None:
        error(f"Encountered errors when downloading the LLVM release.");
        return False
    info(f"Succesfully downloaded {temp_zip_path}")
  
  info(f"Extracting {temp_zip_path} ...")
  with zipfile.ZipFile(temp_zip_path, 'r') as zip_ref:
      zip_ref.extractall(extract_path)
  info(f"Succesfully extracted {temp_zip_path} to {extract_path}")

  if cleanup:
    info(f"Cleaning up ...")
    # Try to delete the zip file.
    try:
        os.remove(temp_zip_path)
    except OSError as e:
      # If it fails, inform the user.
      error(f"{e.filename} - {e.strerror}")
  
  info(f"Renaming folder")
  
  env_folder_path = os.path.join(extract_path, repo_name)

  os.rename(extract_folder_path, env_folder_path)
  return True

# ==================================================================================================================

def copy_directory_recursively(src, dst):
  
  # Copy the content of
  # source to destination
  try:
    shutil.copytree(src, dst)
  except OSError as err:
    # print("Error: % s" % err)
    error(f"{err}")
    return False
  return True

# ==================================================================================================================

def copy_file(src, dst):
  shutil.copyfile(src, dst)

# ==================================================================================================================

def check_arguments(target_name, path, arch):
  if not target_name[0].isupper():
    error(f"Please provide a valid target name (upper case and camel case when applicable)")
    return False
  
  if not os.path.isdir(path) :
    error(f"Please provide a valid path for the new backend")
    return False

  if not arch in {8, 16, 32, 64} :
    error(f"Please provide a valid architecture bit width. Possible values are: 8, 16, 32, 64")
    return False
  
  repo_name = f"llvm-{target_name.lower()}-backend"
  new_backend_path = os.path.join(path, repo_name)
  
  if os.path.exists(new_backend_path) and len(os.listdir(new_backend_path)) != 0:
    error(f"Directory {new_backend_path} is not empty. ")
    return False
  
  return True

# ==================================================================================================================

@task(aliases = ['se'], 
      help={"target_name": "The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM, Mips", 
            "path": "Location on disk where the new environment will be placed",
            "arch": "32 by default. Bit width of the new target. Possible values: 8, 16, 32 or 64",
            "init_repo" : "True by default. A git repository will be initialized in the new folder and initial commits will be added",
            "build": "True by default. Whether or not to start compiling the target after setting up the environment"})
def setup_new_environment(c, target_name, path, arch=32, init_repo=True, build=True):
  """
  Set up the environment in order to develop a new LLVM Backend for the given target name. 
  Downloads the LLVM release and places the new backend into the `llvm/lib/Target` folder. 
  The script also sets up a git repository, unless `init_repo` is False. 
  After the repository has been initialized, the script will proceed to build the new target unless `build` argument is False
  """
  if not check_arguments(target_name, path, arch):
    return
  
  info(f"Setting up new LLVM backend development environment for the target `{target_name}`")
  
  repo_name = f"llvm-{target_name.lower()}-backend"
  environment_path = os.path.join(path, repo_name)

  # download LLVM release
  ok = download_and_extract_release(extract_path=path, repo_name=repo_name)
  if not ok:
    return
  
  if not os.path.exists(environment_path) or len(os.listdir(environment_path)) == 0:
    error(f"Something went wrong during archive extraction")
    return
  
  if init_repo:
    info(f"Initializing repository ... ")
    repo = git.Repo.init(environment_path)
    repo.index.add(f"{environment_path}/*")
    repo.index.add(f"{environment_path}/.*")
    repo.index.add(f"{environment_path}/llvm")
    repo.index.add(f"{environment_path}/lld")

    info(f"Adding initial commit ")
    repo.index.commit(f"Add LLVM-{LLVM_SUPPORTED_VERSION} release")
  
  # copy the template folder to the given location
  template_path = "./Simple"
  target_folder_path = os.path.join(environment_path, f"llvm/lib/Target/{target_name}")
  info(f"Copying the template folder {template_path} into the {target_folder_path} folder ... ")
  ok = copy_directory_recursively(src=template_path, dst=target_folder_path)
  if not ok:
    return
  
  # replace all occurences of "Simple" with the given target name
  info(f"Search and replace `Simple` target name with {target_name} in the backend folder ... ")
  if target_name != "Simple":
    search_replace_in_directory(target_folder_path, target_name)
  if init_repo:
    repo.index.add(target_folder_path)
    repo.index.commit(f"Add template folder for target {target_name}")
  
  info(f"Add new target to LLVM build system")
  project_root = os.path.realpath(__file__).rsplit('/', 1)[0]
  
  template_patch_file_path = f"{project_root}/patches/addTargetToLLVM.patch"
  new_patch_file_path = f"{project_root}/patches/add{target_name}ToLLVM.patch"
  copy_file(template_patch_file_path, new_patch_file_path)
  
  search_replace_in_file(new_patch_file_path, "simple", target_name.lower(), 
                                "Simple", target_name,
                                "SIMPLE", target_name.upper())
  
  repo.git.apply(['-3', new_patch_file_path])
  repo.index.add(environment_path)
  repo.index.commit(f"Add target {target_name} to LLVM build system")

  info(f"Building the '{target_name}' target ... ")

  if build:
    build_target(c, target_name)
  
  info(f"Successfully created new LLVM Backend Development environment for target {target_name} at {environment_path}")

# ==================================================================================================================

@task(aliases = ['ntf'], 
      help={"target_name": "The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM, Mips", 
            "path" : "Location on disk where the new environment will be placed"})
def create_new_target_folder(c, target_name, path):
  """
  Creates a Target folder based on the template with the given name and also replaces all occurences of "Simple" with
  the target name
  """
  
  if not check_arguments(target_name, path):
    return
  template_path="./Simple"
  target_folder_path = f"./{target_name}"
  ok = copy_directory_recursively(src=template_path, dst=target_folder_path)
  if not ok:
    return
  # replace all occurences of "Simple" with the given target name
  info(f"Search and replace `Simple` target name with {target_name} in the backend folder ... ")
  if target_name != "Simple":
    search_replace_in_directory(target_folder_path, target_name)
