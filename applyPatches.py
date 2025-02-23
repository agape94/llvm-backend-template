import clang.cindex
from clang.cindex import Index, Config, CursorKind
import platform
import os
from pathlib import PureWindowsPath
import wget
import json
import tarfile

LLVM_TOOLCHAIN_PATH = os.path.abspath(os.path.join(os.getcwd(), "llvm_toolchain"))
LLVM_TOOLCHAIN_VERSION = "19.1.7"
LLVM_TOOLCHAIN_BASE_VERSION = LLVM_TOOLCHAIN_VERSION.split(".")[0]

# ==================================================================================================================
# Utility functions
def download_and_extract_llvm_toolchain():
    """
    Downloads and extracts the LLVM toolchain.
    """
    if os.path.exists("llvm_toolchain") and os.path.isdir("llvm_toolchain") and os.listdir("llvm_toolchain"):
        return
        
    LLVM_TOOLCHAIN_ARCHIVE = ""
    LLVM_TOOLCHAIN_LINK = ""

    # Download the LLVM toolchain
    system = platform.system()
    if system == "Linux":
        LLVM_TOOLCHAIN_ARCHIVE = "clang+llvm-19.1.7-aarch64-linux-gnu.tar.xz"
        LLVM_TOOLCHAIN_LINK = f"https://github.com/llvm/llvm-project/releases/download/llvmorg-{LLVM_TOOLCHAIN_VERSION}/{LLVM_TOOLCHAIN_ARCHIVE}"
    elif system == "Darwin":  # macOS
        LLVM_TOOLCHAIN_ARCHIVE = "clang+llvm-19.1.7-armv7a-linux-gnueabihf.tar.gz "
        LLVM_TOOLCHAIN_LINK = f"https://github.com/llvm/llvm-project/releases/download/llvmorg-{LLVM_TOOLCHAIN_VERSION}/{LLVM_TOOLCHAIN_ARCHIVE}"
    elif system == "Windows":
        LLVM_TOOLCHAIN_ARCHIVE = "clang+llvm-19.1.7-x86_64-pc-windows-msvc.tar.xz"
        LLVM_TOOLCHAIN_LINK = f"https://github.com/llvm/llvm-project/releases/download/llvmorg-{LLVM_TOOLCHAIN_VERSION}/{LLVM_TOOLCHAIN_ARCHIVE}"
    else:
        raise RuntimeError(f"Unsupported operating system: {system}")
    

    if os.path.exists(LLVM_TOOLCHAIN_ARCHIVE):
        print(f"{temp_zip_path} already exists, skipping download")
    else:
        print(f"Downloading LLVM toolchain from {LLVM_TOOLCHAIN_LINK} ...")
        temp_zip_path = wget.download(LLVM_TOOLCHAIN_LINK, out=os.getcwd(), bar=None)
    
    # Extract the downloaded archive
    print(f"Extracting {LLVM_TOOLCHAIN_ARCHIVE} ...")
    if LLVM_TOOLCHAIN_ARCHIVE.endswith(".tar.gz") or LLVM_TOOLCHAIN_ARCHIVE.endswith(".tgz"):
        mode = "r:gz"
    elif LLVM_TOOLCHAIN_ARCHIVE.endswith(".tar.xz"):
        mode = "r:xz"
    else:
        mode = "r"

    with tarfile.open(LLVM_TOOLCHAIN_ARCHIVE, mode) as tar:
        tar.extractall(path=LLVM_TOOLCHAIN_PATH)
    print(f"Extraction complete.")

def set_libclang_path():
    system = platform.system()
    if system == "Linux":
        libclang_path = os.path.join(LLVM_TOOLCHAIN_PATH, "lib", f"libclang.so.{LLVM_TOOLCHAIN_VERSION}")
    elif system == "Darwin":  # macOS TODO CHECK AGAIN
        libclang_path = os.path.join(LLVM_TOOLCHAIN_PATH, "lib", f"libclang.so.{LLVM_TOOLCHAIN_VERSION}")
    elif system == "Windows": # TODO CHECK AGAIN
        libclang_path = "C:\\Program Files\\LLVM\\bin\\libclang.dll"
    else:
        raise RuntimeError(f"Unsupported operating system: {system}")

    if not os.path.exists(libclang_path):
        raise RuntimeError(f"libclang not found at {libclang_path}")

    Config.set_library_file(libclang_path)

def find_matching_pattern(lines, addition, node=None):
    """
    Finds the matching pattern in the file.        
    """
    pattern = addition["pattern"]
    location = addition["location"]
    addition_type = addition["type"]

    insertion_point = None
    last_match_content = ""
    enum_val = None
    indentation = None

    if addition_type == "general":
        for line in lines:
            if line.strip().startswith(pattern):
                insertion_point = lines.index(line)
                if location == "before_first_match" or location == "after_first_match":
                    break
    else:
        # Iterate through enum cases
        for child in node.get_children():
            if addition_type == "enum_value" and child.kind == CursorKind.ENUM_CONSTANT_DECL and child.spelling.startswith(pattern):
                insertion_point = child.extent.end.line
                enum_val = child.enum_value
                start_line = child.extent.start.line - 1
                end_line = child.extent.end.line
                last_match_content = "".join(lines[start_line:end_line])
                indentation = last_match_content[:len(last_match_content) - len(last_match_content.lstrip())]
                if location == "before_first_match" or location == "after_first_match":
                    break
    
    # Track the line number of the last matching case
    if location == "before_first_match" or location == "before_last_match":
        insertion_point = insertion_point - 1
    elif location == "after_last_match" or location == "after_first_match":
        pass

    return indentation, insertion_point, enum_val

def replace_all(content, target_name, arch, endianness):
    if content is None:
        return ""
    
    content = content.replace("bit_width", f"{arch}")

    content = content.replace("Target", target_name.capitalize())
    content = content.replace("target", target_name.lower())
    content = content.replace("TARGET", target_name.upper())
    
    content = content.replace("Endianness", endianness.capitalize())
    content = content.replace("endianness", endianness.lower())
    content = content.replace("ENDIANNESS", endianness.upper())
    
    return content

# ==================================================================================================================

def apply_patches(environment_path, target_name, arch, endianess):
    with open("patches.json", "r") as f:
        config = json.load(f)
  
    for file_config in config["files"]:
        file_relative_path = file_config["path"]
        file_additions = file_config["additions"]
        file_absolute_path = os.path.join(environment_path, file_relative_path)
        
        if platform.system() == "Windows":
            file_absolute_path = str(PureWindowsPath(file_absolute_path))
        
        for addition in file_additions:
            # handle addition types
            if addition["type"] == "new_file":
                file_absolute_path = replace_all(file_absolute_path, target_name, arch, endianess)
                add_new_file(file_absolute_path, addition, target_name, arch, endianess)
            else:
                modify_file(file_absolute_path, addition, target_name, arch, endianess)

def modify_file(file_absolute_path, addition, target_name, arch, endianness):
    
    if addition["type"] == "general":
        # this does not require parsing the file therefore we can directly add the content
        # at the right location based on the pattern
        add_general_content(file_absolute_path, addition, target_name, arch, endianness)
        return

    download_and_extract_llvm_toolchain()
    # parse the file
    LLVM_TOOLCHAIN_INCLUDE_PATH = os.path.join(LLVM_TOOLCHAIN_PATH, "include")
    LLVM_INCLUDE_PATH = os.path.join(LLVM_TOOLCHAIN_INCLUDE_PATH, "llvm")
    CLANG_INCLUDE_PATH = os.path.join(LLVM_TOOLCHAIN_INCLUDE_PATH, "clang")
    CLANG_RESOURCE_PATH = os.path.join(LLVM_TOOLCHAIN_INCLUDE_PATH, "lib", "clang", LLVM_TOOLCHAIN_BASE_VERSION, "include")
    CPP_INCLUDE_PATH = os.path.join(LLVM_TOOLCHAIN_INCLUDE_PATH, "c++", "v1")
    CPP_STANDARD = "c++17"

    index = Index.create()
    
    args = [
        f"-std={CPP_STANDARD}",
        f"-I{LLVM_TOOLCHAIN_INCLUDE_PATH}",
        f"-I{CLANG_RESOURCE_PATH}",
        f"-I{CPP_INCLUDE_PATH}",
        f"-I{LLVM_INCLUDE_PATH}",
        f"-I{CLANG_INCLUDE_PATH}",
        "-x", "c++",
    ]  # Adjust based on your C++ standard

    ast = index.parse(file_absolute_path, args=args)

    if addition["type"] == "enum_value":
        add_new_enum_case(file_absolute_path, addition, ast, target_name, arch, endianness)

def add_new_enum_case(file_path, addition, ast, target_name, arch, endianness):
    with open(file_path, "r") as f:
        lines = f.readlines()
    
    content = replace_all(addition["content"], target_name, arch, endianness)
    
    # Traverse the AST
    for node in ast.cursor.walk_preorder():
        indentation = None
        insertion_point = None
        value = None
        if node.kind == CursorKind.ENUM_DECL:
            # Find the right place to insert the new case
            indentation, insertion_point, value = find_matching_pattern(lines, addition, node)
            if insertion_point and value and indentation:
                comment = addition["comment"]
                if comment != "":
                    comment = " // " + replace_all(addition["comment"], target_name, arch, endianness)
                # value = value if addition["options"].get("with_value", False) else ""
                new_case = f"{indentation}{content} = {value + 1}, {comment}"
                # Insert the new case
                lines.insert(insertion_point, f"{new_case}\n")

                with open(file_path, "w") as f:
                    f.writelines(lines)

def add_general_content(file_path, addition, target_name, arch, endianness):
    with open(file_path, "r") as f:
        lines = f.readlines()
    
    content = replace_all(addition["content"], target_name, arch, endianness)
    
    # Find the right place to insert the new enum definition
    _, insertion_point, _ = find_matching_pattern(lines, addition)
    if insertion_point:
        # Insert the new case
        lines.insert(insertion_point, f"\n{content}\n")

        with open(file_path, "w") as f:
            f.writelines(lines)

def add_new_file(file_path, addition, target_name, arch, endianess):
    content = replace_all(addition["content"], target_name, arch, endianess)

    with open(file_path, "w") as f:
        f.write(content)
