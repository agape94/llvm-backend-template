# LLVM Backend Template and Environment setup
This project aims to solve one of the biggest challenges when writing an LLVM Backend from scratch: **Initial setup**.

If you want to write an LLVM Backend, the general advice from the LLVM Developers is to copy one of the official backends and adapt it to fit your architecture. However, an LLVM backend has a lot of custom code that only works for that specific architecture, and one would need to invest a large amount of time and effort to adapt the copied backend to the new architecture. 

The template implements a minimal target called `Simple`, that only has a few instructions and a couple of registers, just to give the user a base to build a real architecture on. Development of this backend was done according to the general advice mentioned above. As a starting point, the `Lanai` backend was copied, all the occurences of `Lanai` were replaced with `Simple`, and all the functionality was stripped down to a minimum.

# Prerequisites
- Linux, MacOS or Windows
- Python 3.x
- Python invoke module
- ninja for building the environment

# How to use
This project makes use of the excelent [invoke](https://www.pyinvoke.org/) module from Python. It allows the creation of custom scripts (called `tasks`) that can be run from anywhere within a directory (subdirectories as well). Invoke can be installed via Pip (`pip install invoke`).

To get an overview of the available tasks, open the directory in the command line and run the following command:
```
$ invoke --list
```
Note: `inv` works as well.
For more information about Python `invoke`, please refer to the [official documentation](https://docs.pyinvoke.org/en/stable/).

Here there are two tasks defined. Running `inv --list` will show the available tasks:
```
Available tasks:

  build-target (b)             Build the given target
  setup-new-environment (se)   Set up the environment in order to develop a new LLVM Backend for the given target name.
```

To find more information about a specific task, you can run the following command: `inv -help [task_name]`. This command will give the task description as well as possible command line arguments.

## Setup a new environment
In order to create a new environment, clone this repository and open the folder in a new shell. After that, run the following command:
```
inv se -p <path-to-place-new-environment> -t <target-name> [-a <8, 16, 32, or 64>] [-e <"little", "big">] [--[no-]build]
```
Notes:
- The `arch` argument is set to 32 bits by default
- The `build` argument is True by default, meaning that the task will trigger a build for the new target.

### Detailed explanation about the process
The process of setting up a new LLVM Backend development environment has a the following steps:

1. **Download the LLVM Release** - For this project, the release version has been fixed to `17.0.2`, because LLVM does not guarantee API stability in any way. That means that the template may not compile anymore with a newer LLVM version.
2. **Initialize git repository and initial commit** - The LLVM release is unzipped to the given location and the git repository is initialized. The git repository is necessary in order to apply git patches at steps 4 and 5.
3. **Copy the `Simple` template folder and replace all occurences with the `target-name`** - The new target directory will be placed in the `[llvm-source-folder]/lib/Target/[target-name]`
4. **Integrate new target into LLVM build system** - This is done by applying a git patch. Integrating a new backend into the LLVM build system requires various changes to the LLVM sources. These changes also depend on the bit-width of the target (8, 16, 32 or 64 bit architecture)
5. **Integrate new target into CLANG to enable C/C++ support** - This is done by applying a git patch. Integrating a new backend into CLANG requires various changes to the CLANG sources.
6. **Build the new target** - The task will trigger the build automatically, unless the `--no-build` is provided as an argument. 

### Working with the new environment
The new LLVM environment will be placed at the specified path. The template will also provide a build task in the environment folder to make it easier to build whenever there are new changes to the code. Run `inv --help b (or build-target)` to see detailed descriptions for all arguments

The build can be triggered by running the following command in the root folder of the environment:

```
inv b -t <My Target> [--path=./] [--parallel-link-jobs=2] --build-type=Debug [--upstream-targets=<ARM,RISCV,Lanai,etc>] [--[no-]clang]
```
The build logs will be placed into a log file in the environment directory to make it easier to see if there were any issues during the build. The task will also print out the configure and build commands should you want to run them separately.
