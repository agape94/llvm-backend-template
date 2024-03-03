# LLVM Backend Template and Environment setup
This project aims to solve one of the biggest challenges when writing an LLVM Backend from scratch: **Initial setup**.

If you want to write an LLVM Backend, the general advice from the LLVM Developers is to copy one of the official backends and adapt it to fit your architecture. However, an LLVM backend has a lot of custom code that only works for that specific architecture, and one would need to invest a large amount of time and effort to adapt the copied backend to the new architecture. 

The template implements a minimal target called `Simple`, that only has a few instructions and a couple of registers, just to give the user a base to build a real architecture on. Development of this backend was done according to the general advice mentioned above. As a starting point, the `Lanai` backend was copied, all the occurences of `Lanai` were replaced with `Simple`, and all the functionality was stripped down to a minimum. However, the template produces a functional compiler that is able to generate assembly code. More details in section [Generate assembly code](#generate-assembly-code) of this document.

# Prerequisites
- Linux, MacOS or Windows
- Python 3.x
- Python invoke module
- ninja for building the environment

# How to use
This project makes use of the excelent [invoke](https://www.pyinvoke.org/) module from Python. It allows the creation of custom scripts, called `tasks`, that can be run from anywhere within the directory where `tasks.py` exists (subdirectories as well). Invoke can be installed via Pip (`pip install invoke`).

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
  setup-new-environment (se)   Setup a new LLVM backend development environment with the given target name.
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
  -b, --[no-]build                   Whether or not to start compiling the target after setting up the environment. Default
                                     value: True
  -e STRING, --endianess=STRING      Whether the new target is little-endian of big-endian. Default value: little. Possible
                                     values: 'little' or 'big'
  -l STRING, --llvm-version=STRING   Which LLVM release to use. Default version: 17.0.6. You could use a different version if
                                     you want to, however, the template was only tested with 17.0.6, therefore it is not
                                     guarateed to work properly with other LLVM versions.
  -p STRING, --path=STRING           Location on disk where the new environment will be placed
  -r INT, --parallel-link-jobs=INT   Number of parallel link jobs. By default it is set to 2. Note: A high number of link
                                     jobs may cause your system to run out of memory and crash.
  -t STRING, --target-name=STRING    The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM, Mips
```

Usage example:
```
$ inv se -p ~/temp -t MyTarget -a 32 -e "little" --no-build -l 17.0.2 -r 2
```

### Detailed explanation about the process
The process of setting up a new LLVM Backend development environment has a the following steps:

1. **Download the LLVM Release** - For this project, the `17.0.6` version is downloaded by default, which is the latest release at the time of writing. It is possible to provide any released version, however it is not guarantedd that the build will succeed or that the compiler will work. The reason why is because LLVM does not guarantee API stability in any way. That means that the template may not compile anymore with other LLVM versions.
2. **Initialize git repository and initial commit** - The LLVM release is unzipped to the given location and the git repository is initialized. The git repository is necessary in order to apply git patches at steps 4 and 5.
3. **Copy the `Simple` template folder and replace all occurences with the `target-name`** - The new target directory will be placed in the `[llvm-source-folder]/lib/Target/[target-name]`.
4. **Integrate new target into LLVM build system** - This is done by applying a git patch. Integrating a new backend into the LLVM build system requires various changes to the LLVM sources. These changes also depend on the bit-width of the target (8, 16, 32 or 64 bit architecture), which can be specified with a command-line argument.
5. **Integrate new target into CLANG to enable C/C++ support** - This is done by applying a git patch. Integrating a new backend into CLANG requires various changes to the CLANG sources.
6. **Build the new target** - The task will trigger the build automatically, unless `--no-build` is provided as an argument.

### Working with the new environment
The new LLVM environment will be placed at the specified path. The template will also provide a build task in the environment folder to make it easier to build whenever there are new changes to the code. Run `inv --help b[uild-target]` inside the environment directory to see detailed descriptions for all arguments.
```
Usage: inv[oke] [--core-opts] b [--options] [other tasks here ...]

Docstring:
  Build the given target

Options:
  -a INT, --parallel-link-jobs=INT       Number of parallel link jobs. By default it is set to 2. Note: A high number of link
                                         jobs may cause your system to run out of memory and crash.
  -b STRING, --build-type=STRING         Possible values: Debug, Release
  -c, --[no-]clang                       Whether to build clang or not. Default value: True
  -p STRING, --path=STRING               Path to llvm source root. By default the current directory is considered
  -t STRING, --target-name=STRING        The name of the Target. It should be capitalized and camelcase. Ex: TriCore, ARM,
                                         Mips
  -u STRING, --upstream-targets=STRING   List of upstream targets to build, Strings separated by ';' (semicolon). By default
                                         it is None. Example: Lanai;ARM;RISCV;etc
```

Usage example:

```
inv b -t MyTarget -p ~/temp/llvm-mytarget-backend -a 2 -b Release -u ARM;RISCV --no-clang
```

## Generate assembly code
This template target is just meant as a starting point for a new LLVM backend, and so the functionalities built into it are very limited. One can, however, build some very simple assembly code with it.

If the build is successful, a new directory will be created inside the environment directory. This directory will be called `build_<target-name>`, and will contain all the binaries generated, including `llc`, which is the actual compiler we are interested in.

As input, `llc` needs `LLVM IR` source code. Create a new file `main.ll` and place the following code inside.
```
; ModuleID = 'example'

define i32 @main() {
entry:
  ; Perform the addition operation
  %0 = add i32 2147483640, 3
  
  ; Return the result
  ret i32 %0
}
```

In the code above, a function `main` is defined that simply returns the result of an `add` instruction between two 32-bit integers.
In order to generate the assembly code for this program, `llc` can be run as follows:

```
$ ./build_<target_name>/bin/llc --mtriple=<target_name> /path/to/main.ll
```

Note: 
  - The `--mtriple` flag tells `llc` for which target the code will be compiled. In our case it is `<target-name>`.
  - The `.s` file containing the assembly code will be placed alongside the `.ll` file.

The generated assembly code for the `LLVM IR` code above is the following:
```
	.text
	.file	"main.ll"
	.globl	main                            # -- Begin function main
	.type	main,@function
main:                                   # @main
# %bb.0:                                # %entry
	ld %R0, 0x7fff0000
	ori %R0, %R0, 0xfffb
	ld %R1, 0x0
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
```
Notes:
  - As you can see, the `add` instruction was optimized by LLVM during the optimizing phases and it was replaced by a `ld` instruction that simply placed the result computed at compile time into the return register `R0`. 
  - The 32-bit result is splitted in two 16-bit values because the instructions are 32-bit wide, and the result would not fit into a single `ld` instruction. And so, the 16-bit `high` is loaded into the first 16-bits of the `R0` register and then a logical `or` with the `low` 16-bits ensures the whole 32-bit value is loaded into the `R0` register. 
  - `R1` is also a return register, but only for 64-bit values by combining `R1:R0` registers, with `R0` being `low` and `R1` being `high`.
  - The program then exits after the `ret` call.
