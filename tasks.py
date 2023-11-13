from invoke import task
import git
import os
import replaceString 
import wget
import zipfile


class Progress(git.remote.RemoteProgress):
    def update(self, op_code, cur_count, max_count=None, message=''):
        # print(f"update({op_code}, {cur_count}, {max_count}, {message})")
        print(self._cur_line)

LLVM_REPO='https://github.com/llvm/llvm-project.git'
LLVM_SUPPORTED_VERSION="17.0.2"

def download_and_extract_release(extract_path, target_name="Simple"):
  LLVM_RELEASE_LINK = f"https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-{LLVM_SUPPORTED_VERSION}.zip"
  print(f"Downloading LLVM {LLVM_SUPPORTED_VERSION} from https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-{LLVM_SUPPORTED_VERSION}.zip ...")
  file = wget.download(LLVM_RELEASE_LINK, out="/tmp", bar=None)
  if file is None:
      print(f"Encountered errors when downloading the LLVM release.");
      return False
  print(f"Succesfully downloaded {file}")
  
  print(f"Extracting {file} ...")
  with zipfile.ZipFile(file, 'r') as zip_ref:
      zip_ref.extractall(extract_path)
  print(f"Succesfully extracted {file} to {extract_path}")

  print("Cleaning up ...")
  # Try to delete the zip file.
  try:
      os.remove(file)
  except OSError as e:
    # If it fails, inform the user.
    print("Error: %s - %s." % (e.filename, e.strerror))
  
  print("Renaming folder")
  zip_file_name = os.path.basename(file)
  print(f"zip_file_name = {zip_file_name}")

  extract_folder_name = zip_file_name.rsplit('.')[0] 
  print(f"extract_folder_name = {extract_folder_name}")
  
  extract_folder_path = os.path.join(extract_path, extract_folder_name)
  print(f"extract_folder_path = {extract_folder_path}")

  env_folder_name = f"llvm_{target_name}"
  print(f"env_folder_name = {env_folder_name}")

  env_folder_path = os.path.join(extract_path, env_folder_name)
  print(f"env_folder_path = {env_folder_path}")

  input("Press Enter to rename folder...")

  os.rename(extract_folder_path, env_folder_path)
  return True

@task(aliases = ['se'])
def setup_llvm_environment(c, target_name="Simple", path="./"):
  download_and_extract_release(extract_path=path)
   

@task(aliases=['st'], help={"target_name": "The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM, Mips"})
def setup_target(c, target_name="Simple", clone_path="."):
  """
  Set up the environment in order to develop a new LLVM Backend for the given target name
  Clones the LLVM project repository to the given path and creates the template target inside the `llvm/lib/Target` folder
  """
  print(f"Setting up target... {target_name}")
  env_path = os.path.abspath(clone_path)
  print(f"Cloning LLVM project in {env_path}")

  # if os.path.isdir(env_path) and 

  with os.scandir(env_path) as enviroment:
    if any(enviroment):
        print(f'Repository folder not empty. Skipping to checking out LLVM-{LLVM_SUPPORTED_VERSION}')
        repo = git.Repo(os.path.join(env_path, "llvm-project")) 
        repo.git.checkout("main")
        repo.remote().pull(progress=Progress());
    else:
      repo = git.Repo.clone_from(LLVM_REPO, env_path, progress=Progress())
  print(f"Checked out llvm-project ({repo.remotes[0].url}) into {env_path}/llvm-project")

  
  for tag in repo.tags:
    if LLVM_SUPPORTED_VERSION in tag.name:
      llvm_version = tag.name
      break;

  repo.git.checkout(llvm_version)

  # print(f"Checked out LLVM version: {str()}")

