from invoke import task
import os
import multiprocessing

# ==================================================================================================================

def info (msg):
  print(f"INFO: {msg}")

def error (msg):
  print(f"ERROR: {msg}")

# ==================================================================================================================

@task(aliases = ['b'], 
      help={"target_name": "The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM, Mips",
             "build_type": "Possible values: Debug, Release",
             "clang": "If true, Clang will be built as well"})
def build_target(c, target_name, build_type="Debug", clang=True):
  """
    Build the given target
  """

  if not target_name[0].isupper():
    error(f"Please provide a valid target name (upper case and camel case when applicable)")
    return
  

  project_root = os.path.realpath(__file__).rsplit('/', 1)[0]
  current_directory = os.getcwd()

  build_dir = os.path.join(project_root, f'build_{target_name}')

  if not os.path.exists(f'{build_dir}'):
    os.mkdir(f'{build_dir}')

  # c.run(f'cd {project_root}')
  os.chdir(project_root)
  enable_clang = "-DLLVM_ENABLE_PROJECTS=\"clang\"" if clang else ""
  cmake_command = f'cmake -S llvm -B {build_dir} -G Ninja {enable_clang} -DCMAKE_INSTALL_PREFIX={build_dir} -DCMAKE_BUILD_TYPE={build_type} -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="{target_name}" -DLLVM_TARGETS_TO_BUILD="" -DLLVM_PARALLEL_LINK_JOBS=2 -DLLVM_USE_LINKER=gold'
  
  print()
  print(cmake_command)
  c.run(cmake_command)
  
  # c.run(f'cd {build_dir}')
  os.chdir(build_dir)

  cores = multiprocessing.cpu_count()
  build_command = f'ninja -j {cores - 2}' 
  print()
  print(build_command)
  c.run(build_command)

  # c.run(f'cd {current_directory}')
  os.chdir(current_directory)
  
  
# ==================================================================================================================
