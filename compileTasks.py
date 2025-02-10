from invoke import task
import os
import multiprocessing
import subprocess
from datetime import datetime

# ==================================================================================================================

def info (msg):
  print(f"INFO: {msg}")

def error (msg):
  print(f"ERROR: {msg}")

# ==================================================================================================================

@task(aliases = ['b'], 
      help={"experimental_targets": "List of experimental targets to build, Strings separated by ';' (semicolon). Example: 'TriCore;RISCW;etc'. If not defined, the name of the target the environment was created for will be used.",
             "path": "Path to llvm source root. By default the current directory is considered",
             "build_type": "Possible values: Debug, Release. Default value: Release",
             "llc_only": "Whether to only build llc or not. Default value: False",
             "upstream_targets": "List of upstream targets to build, Strings separated by ';' (semicolon). By default it is None. Example: Lanai;ARM;RISCV;etc",
             "parallel_link_jobs": "Number of parallel link jobs. By default it is set to 2. Note: A high number of link jobs may cause your system to run out of memory and crash."})
def build_target(c, experimental_targets="", path=".", upstream_targets="", build_type="Release", llc_only=False, parallel_link_jobs=2):
  """
    Starts the build process for the target specified in the target_name parameter. If the target_name is not provided, the name of the target the environment was created for will be used.
    By default, the build type is set to Release. If you want to build the target in Debug mode, you can specify the build type as Debug.
  """
  # Check if there are any targets to build defined in the .*_targets_to_build files if the respective parameters are not provided
  # This is useful because the user may want to build multiple targets and it is easier to define them in a file than to provide them as a parameter
  if experimental_targets == "":
    with open(os.path.join(os.getcwd(), "experimental_targets_to_build.ini"), "r") as f:
      experimental_targets = ";".join([line.strip() for line in f if not line.strip().startswith("#")])
  
  if upstream_targets == "":
    with open(os.path.join(os.getcwd(), "upstream_targets_to_build.ini"), "r") as f:
      upstream_targets = ";".join([line.strip() for line in f if not line.strip().startswith("#")])

  if experimental_targets == "" and upstream_targets == "":
    raise ValueError("Please provide at least one target to build.")
  
  if debug == release:
    raise ValueError("Please provide only one of the build types: Debug or Release")
  
  if debug:
    release = False
  else:
    release = True

  build_type = "Debug" if debug else "Release"
  build_dir_name = f'build_{build_type.lower()}'
  build_dir = os.path.join(path, build_dir_name)

  if not os.path.exists(f'{build_dir}'):
    os.mkdir(f'{build_dir}')

  os.chdir(path)
  enable_clang_lld = "-DLLVM_ENABLE_PROJECTS=\"clang;lld\"" if not llc_only else ""
  enable_assertions = "-DLLVM_ENABLE_ASSERTIONS=ON" if build_type == "Debug" else "-DLLVM_ENABLE_ASSERTIONS=OFF"
  cmake_command = f'cmake -S llvm -B {build_dir} -G Ninja {enable_clang_lld} -DCMAKE_INSTALL_PREFIX={build_dir} {enable_assertions} -DCMAKE_BUILD_TYPE={build_type} -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="{experimental_targets}" -DLLVM_TARGETS_TO_BUILD="{upstream_targets}" -DLLVM_PARALLEL_LINK_JOBS={parallel_link_jobs} -DLLVM_USE_LINKER=lld'
  
  info(f"Configuring {experimental_targets} ... ")
  info(f"Cmake command: {cmake_command}")

  process = subprocess.Popen(cmake_command, shell=True)
  # Wait for the subprocess to complete
  process.wait()

  # Check if the command was successful (return code 0)
  if process.returncode == 0:
    info("Configured succesfully.")
  else:
    raise Exception("Configure step failed.")

  os.chdir(build_dir)

  cores = multiprocessing.cpu_count()
  build_command = f'ninja -j {cores - 1}'
    
  info(f"Build command: {build_command}")
  info(f"Building target {experimental_targets} ...")
  process = subprocess.Popen(build_command, shell=True)
  # Wait for the subprocess to complete
  process.wait()

  # Check if the command was successful (return code 0)
  if process.returncode == 0:
    info("Built succesfully!")
  else:
    raise Exception("The build has failed.")

  os.chdir(path)
