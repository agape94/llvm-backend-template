import os
import shutil
import zipfile
import wget

# ==================================================================================================================


def info(msg):
  print(f"INFO: {msg}")


def error(msg):
  print(f"ERROR: {msg}")

# ==================================================================================================================


def search_replace_in_file(file_path, search_string, replace_string):
  with open(file_path, 'r', encoding='utf-8') as src_file:
    lines = src_file.readlines()

  for i in range(0, len(lines)):
    lines[i] = lines[i].replace(search_string, replace_string)

  with open(file_path, 'w', encoding='utf-8') as src_file:
    src_file.writelines(lines)

# ==================================================================================================================
# Function to recursively search and replace in a directory


def search_replace_in_directory(directory_path, replace_string, search_string="Skeleton"):
  search_string_lower = search_string.lower()
  replace_string_lower = replace_string.lower()

  search_string_capital = search_string
  replace_string_capital = replace_string

  search_string_upper = search_string.upper()
  replace_string_upper = replace_string.upper()

  for root, _, files in os.walk(directory_path):
    for file in files:
      file_path = os.path.join(root, file)
      file_path_new = file_path.replace(
          search_string_capital, replace_string_capital)
      os.rename(file_path, file_path_new)

      file_path = file_path_new

      if os.path.isfile(file_path):
        search_replace_in_file(
            file_path, search_string_lower, replace_string_lower)
        search_replace_in_file(
            file_path, search_string_capital, replace_string_capital)
        search_replace_in_file(
            file_path, search_string_upper, replace_string_upper)

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


def copy_file(src, dst):
  shutil.copyfile(src, dst)

# ==================================================================================================================


def write_file(path, content):
  with open(path, "a") as f:
    f.write("\n")
    f.write(content)
