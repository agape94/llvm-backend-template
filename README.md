# LLVM Backend Development Environment
This project addresses a significant challenge in writing an LLVM Backend from scratch: **Initial setup**.

The LLVM documentation on the subject recommends copying an existing official backend and adapting it to fit your architecture. However, this approach involves a lot of custom code specific to the original architecture, requiring substantial time and effort to modify for a new architecture.

This project provides a minimal LLVM backend called `Skeleton`, a suite of scripts implemented as Python `invoke` tasks. With a simple command, these scripts create an `LLVM Backend Development Environment` that has the custom target already registered and ready to be built. Using the `LDE` allows backend developers to focus on writing the important components of the backend and not waste time and energy on wriitng boilerplate code.

# Prerequisites
- Linux, MacOS or Windows
- Python 3.x
- Python invoke, wget, pyyaml modules
- C++ compiler (gcc, clang, etc.)
- ninja for building the environment

# How to use
This project makes use of the excelent [invoke](https://www.pyinvoke.org/) module from Python. It allows the creation of custom scripts, called `tasks`, that can be run from anywhere within the directory where `tasks.py` file exists. Invoke can be installed via Pip (`pip install invoke`).

To get an overview of the available tasks, open the directory in the command line and run the following command:
```
$ invoke --list
```
Note: `inv` works as well.
For more information about Python `invoke`, please refer to the [official documentation](https://docs.pyinvoke.org/en/stable/).

Here there are two tasks defined. Running `inv --list` will show the available tasks:
```
Available tasks:

  create-target-in-environment (ct)   Add a new target to an existing LLVM backend development environment.
  setup-new-environment (se)          Setup a new LLVM backend development environment for the given target name.
```

To find more information about a specific task, you can run the following command: `inv --help [task_name]`. This command will print the usage instructions of the task, the task description, followed by detailed descriptions of all command line arguments.

## Setup a new environment
In order to create a new environment, clone this repository and open the folder in a new shell. 

For this purpose, the task `setup-new-environment (se)` will be used. Below there is the output of the command `inv --help se`:
```
Usage: inv[oke] [--core-opts] se [--options] [other tasks here ...]

Docstring:
  Setup a new LLVM backend development environment for the given target name.

Options:
  -a INT, --arch=INT                 Bit width of the new target. Default value: 32. Possible values: 8, 16, 32 or 64
  -e STRING, --endianness=STRING     Whether the new target is little-endian of big-endian. Default value: little. Possible values: 'little' or 'big'
  -l STRING, --llvm-version=STRING   Which LLVM release to use. Default version: 19. Possible values: 17, 18, 19, 20
  -p STRING, --path=STRING           Location on disk where the new environment will be placed
  -t STRING, --target-name=STRING    The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM, Mips
```

Usage example:
```
$ invoke setup-new-environment --path=~/Temp --target-name=MyTarget --arch=32 --endianness=little --llvm-version=19

or the shorter version:

$ inv se -p ~/temp -t MyTarget -a 32 -e little -l 19
```

### Detailed explanation about the process
The process of setting up a new LLVM Backend development environment has a the following steps:

1. **Download the LLVM Release** - For this project, the `19.1.7` version is downloaded by default, which is the latest major release at the time of writing. Currently the only LLVM versions supported by the backend template are `17.0.6`, `18.1.8`, `19.1.7` and `20.1.0-rc3` (at the time of writing, `LLVM 20.1.0` was not released yet). This restriction comes from the fact that LLVM does not guarantee API stability in any way. That means that the `LDE` may not compile anymore with other LLVM versions. Support can be added for newer LLVM versions if necessary.
2. **Copy the `lib/backends/Skeleton-llvm-<version>` template folder and replace all occurences with the `<target-name>`** - The new target directory will be placed in the `[llvm-source-folder]/lib/Target/<target-name>`.
5. **Integrate new target into LLVM build system** - Integrating a new backend into the `LLVM`, `CLANG` and `LLD` build systems requires various changes to the LLVM sources. These changes also depend on the bit-width of the target (8, 16, 32 or 64 bit architecture), as well as endianness, which can be specified with command-line arguments. All the changes that need to be done are defined in `lib/register_target/configs/llvm<version>/[llvm/clang/lld].yml`. The script simply reads the LLVM source files line by line, while searching for patterns. If the desired pattern is detected, the changes are inserted above or below the pattern. If the pattern is not found, an exception is raised.

### Working with the LLVM Backend Development Environment
The new LLVM environment will be placed at the specified path. The template will also provide a build task in the environment folder to make it easier to build whenever there are new changes to the code. Run `inv --help b[uild-target]` inside the environment directory to see detailed descriptions for all arguments.
```
Usage: inv[oke] [--core-opts] b [--options] [other tasks here ...]

Docstring:
  Starts the build process for the compiler. By default, the build type is set to Release.

Options:
  -d, --debug                                Build Debug version of the target. Default value: False
  -e STRING, --experimental-targets=STRING   List of experimental targets to build, Strings separated by ';' (semicolon). Example: 'TriCore;RISCW;etc'. If not defined, the targets are read from the
                                             'experimental_targets_to_build.ini' file
  -l, --llc-only                             Whether to only build llc or not. Default value: False
  -o STRING, --official-targets=STRING       List of official targets to build, Strings separated by ';' (semicolon). By default it is None. Example: Lanai;ARM;RISCV;etc. If not defined, the targets are read
                                             from the 'official_targets_to_build.ini' file
  -p INT, --parallel-link-jobs=INT           Number of parallel link jobs. By default it is set to 2. Note: A high number of link jobs may cause your system to run out of memory and crash.

```

Usage example:
For a basic release build of the LLVM Backend Development Environment that will build your new custom backend in Release mode, simply run the following command from within the environment's folder:
```
inv b
```

# Define Your Backend
You now have everything you need to start developing your custom backend for your architecture. The following documents will guide you through the next steps:
- To define the register set for your target, follow the instructions in [Define Register Set](docs/DefineRegisterSet.md).
- To define the instruction set for your target, follow the instructions in [Define ISA](docs/DefineInstructionSet.md).
