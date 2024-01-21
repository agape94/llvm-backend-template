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
      help={"target_name": "The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM, Mips",
             "environment_dir": "Path to llvm source root. By default the current directory is considered",
             "build_type": "Possible values: Debug, Release",
             "clang": "If true, Clang will be built as well",
             "upstream_targets": "List of upstream targets to build, Strings separated by ',' (comma). By default it is None. Example: Lanai,ARM,RISCV",
             "parallel_link_jobs": "Number of parallel link jobs. By default it is set to 2. Note: A high number of link jobs may cause your system to run out of memory and crash.",
             "log_file": "Whether to place the build logs in the log file or not. By default True"})
def build_target(c, target_name, environment_dir=".", upstream_targets="", build_type="Debug", clang=True, parallel_link_jobs=2, log_file=True):
  """
    Build the given target
  """

  if not target_name[0].isupper():
    error(f"Please provide a valid target name (upper case and camel case when applicable)")
    return -1

  if not os.path.exists(environment_dir) or len(os.listdir(environment_dir)) == 0:
    error(f"{environment_dir} is empty or doesn't exist")
    return -1

  build_dir = os.path.join(environment_dir, f'build_{target_name}')

  if not os.path.exists(f'{build_dir}'):
    os.mkdir(f'{build_dir}')

  os.chdir(environment_dir)
  enable_clang = "-DLLVM_ENABLE_PROJECTS=\"clang\"" if clang else ""
  cmake_command = f'cmake -S llvm -B {build_dir} -G Ninja {enable_clang} -DCMAKE_INSTALL_PREFIX={build_dir} -DCMAKE_BUILD_TYPE={build_type} -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="{target_name}" -DLLVM_TARGETS_TO_BUILD="{upstream_targets}" -DLLVM_PARALLEL_LINK_JOBS={parallel_link_jobs}'
  
  info(f"Configuring {target_name} ... ")
  info(f"Cmake command: {cmake_command}")
  
  build_log_file = os.path.join(environment_dir, f"build-{target_name}-{build_type}-{datetime.now().strftime('%Y%m%d%H%M%S')}.log")

  # Open a file for writing the output
  with open(build_log_file, "w") as file:
    if log_file:
      process = subprocess.Popen(cmake_command, shell=True, stdout=file, stderr=file, text=True)
    else:
      process = subprocess.Popen(cmake_command, shell=True)
  
  # Wait for the subprocess to complete
  process.wait()

  # Check if the command was successful (return code 0)
  if process.returncode == 0:
    info("Configured succesfully.")
  else:
    error(f"Configure step failed. Logs are available here: {build_log_file}")

  os.chdir(build_dir)

  cores = multiprocessing.cpu_count()
  build_command = f'ninja -j {cores - 2}'
    
  info(f"Build command: {build_command}")
  info(f"Building target {target_name} ...")
  
  if log_file:
    info("Please be patient as this may take 60 minutes or more depending on your hardware ...")
    info(f"The build logs are available here: {build_log_file}")

  # Open a file for writing the output
  with open(build_log_file, "a") as file:
    if log_file:
      process = subprocess.Popen(build_command, shell=True, stdout=file, stderr=file, text=True)
    else:
      process = subprocess.Popen(build_command, shell=True)
      return -1

  # Wait for the subprocess to complete
  process.wait()

  # Check if the command was successful (return code 0)
  if process.returncode == 0:
    info("Built succesfully!")
  else:
    error(f"The build has failed. Please check the log file for more information.")
    return -1

  os.chdir(environment_dir)
  return 0
  
  
# ==================================================================================================================
