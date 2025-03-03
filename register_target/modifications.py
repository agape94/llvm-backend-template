from variables import CONFIG_ROOT
from enum import Enum
import yaml
import platform
from pathlib import PureWindowsPath
from utils import *


class PROJECT(Enum):
  LLVM = "llvm.yml"
  CLANG = "clang.yml"
  LLD = "lld.yml"


def applyModifications(config_file, environment_path, target_name, bit_width, endianness):
  message = f"Adding target \"{target_name}\" to"

  if config_file == PROJECT.LLVM:
    message += " LLVM ..."
  elif config_file == PROJECT.CLANG:
    message += " CLANG ..."
  elif config_file == PROJECT.LLD:
    message += " LLD ..."

  info(message)

  modifications = readModifications(
      os.path.join(CONFIG_ROOT, config_file.value))

  for mod in modifications:
    mod.apply(environment_path, target_name, bit_width, endianness)


def readModifications(file_path):
  with open(file_path, "r") as file:
    mods = yaml.safe_load(file)["modifications"]

  modifications = []

  for mod_dict in mods:
    modification = Modification(mod_dict)
    for change_dict in mod_dict.get("changes", []):
      change = Change(change_dict)
      modification.changes.append(change)
    modifications.append(modification)

  return modifications


class ModificationType(Enum):
  NEW_FILE = "new_file"
  STRING = "string"


class RelativeLocation(Enum):
  BEFORE = "before"
  AFTER = "after"
  UNKNOWN = "unknown"


class Modification:
  def __init__(self, mod):
    self.file_path = mod.get("file_path", None)
    assert self.file_path is not None
    self.file_path = str(self.file_path)
    self.changes = []

  def __str__(self):
    changes_str = [str(change) for change in self.changes]
    return f"file_path: {self.file_path}, changes: {changes_str}"

  def apply(self, environment_path, target_name, bit_width, endianness):
    self.replacePlaceHolders(target_name, bit_width, endianness)

    self.file_path = os.path.join(environment_path, self.file_path)
    if platform.system() == "Windows":
      self.file_path = str(PureWindowsPath(self.file_path))

    for change in self.changes:
      change.apply(self.file_path, target_name, bit_width, endianness)

  def replacePlaceHolders(self, target_name, bit_width, endianness):
    def replace(pattern, content):
      if pattern is not None and content is not None:
        self.file_path = self.file_path.replace(pattern, content)

    replace("<Target>", target_name)
    replace("<TARGET>", target_name.upper())
    replace("<target>", target_name.lower())
    replace("<endianness>", endianness)
    for change in self.changes:
      change.replacePlaceHolders(target_name, bit_width, endianness)


class Change:
  def __init__(self, change):
    assert change.get("type", None) is not None
    self.type = ModificationType(change.get("type", None))
    self.content = str(change.get("content", None)) if change.get(
        "content", None) else None
    self.pattern = str(change.get("pattern", None)) if change.get(
        "pattern", None) else None

    relative_location = str(change.get("relative_location", None)) if change.get(
        "relative_location", None) else None
    self.relative_location = RelativeLocation(
        relative_location) if relative_location else RelativeLocation.UNKNOWN

    self.min_bit_width = int(change.get("min_bit_width", None)) if change.get(
        "min_bit_width", None) else None

    self.bit_width = int(change.get("bit_width", None)) if change.get(
        "bit_width", None) else None

    self.offset = int(change.get("offset", None)) if change.get(
        "offset", None) else 0

    self.correct_bit_width_content = str(
        change.get("correct_bit_width_content", None)) if change.get("correct_bit_width_content", None) else None
    self.wrong_bit_width_content = str(
        change.get("wrong_bit_width_content", None)) if change.get("wrong_bit_width_content", None) else None

    self.endianness = str(change.get("endianness", None)) if change.get(
        "endianness", None) else None

  def __str__(self):
    change_str = f"type: \"{self.type}\", content: \"{self.content}\", pattern: \"{self.pattern}\", relative_location: \"{self.relative_location}\", min_bit_width: \"{self.min_bit_width}\", bit_width: \"{self.bit_width}\", offset: \"{self.offset}\", correct_bit_width_content: \"{self.correct_bit_width_content}\", wrong_bit_width_content: \"{self.wrong_bit_width_content}\", endianness: \"{self.endianness}\""
    return change_str

  def replacePlaceHolders(self, target_name, bit_width, endianness):
    def replace(pattern, content):
      if pattern is not None and content is not None:
        if self.content:
          self.content = self.content.replace(pattern, content)
        if self.pattern:
          self.pattern = self.pattern.replace(pattern, content)
        if self.correct_bit_width_content:
          self.correct_bit_width_content = self.correct_bit_width_content.replace(
              pattern, content)
        if self.wrong_bit_width_content:
          self.wrong_bit_width_content = self.wrong_bit_width_content.replace(
              pattern, content)

    replace("<Target>", target_name)
    replace("<TARGET>", target_name.upper())
    replace("<target>", target_name.lower())

    if self.min_bit_width and bit_width < self.min_bit_width:
      replace("<bit-width>", str(self.min_bit_width))
    else:
      replace("<bit-width>", str(bit_width))

    replace("<endianness>", endianness)

  def apply(self, file_path, target_name, bit_width, endianness):
    if self.type == ModificationType.NEW_FILE:
      self._addNewFile(
          file_path, target_name, bit_width, endianness)
    elif self.type == ModificationType.STRING:
      self._addString(
          file_path, target_name, bit_width, endianness)
    else:
      raise ValueError(f"Unsupported modification type: {self.type}")

  def _addNewFile(self, file_path: str, target_name: str, bit_width: int, endianness: str):
    assert self.content is not None

    with open(file_path, "w") as f:
      f.write(self.content)

  def _addString(self, file_path: str, target_name: str, bit_width: int, endianness: str):
    assert self.pattern is not None
    assert self.relative_location is not RelativeLocation.UNKNOWN
    assert self.content is not None

    with open(file_path, "r") as f:
      lines = f.readlines()

    if self.endianness and self.endianness != endianness:
      # if the endianness is not the same, return because there is nothing to do in this case
      return

    if self.correct_bit_width_content and self.wrong_bit_width_content and self.bit_width:
      if self.bit_width == 32 and bit_width <= self.bit_width or self.bit_width == 64 and bit_width == self.bit_width:
        self.content += self.correct_bit_width_content
      else:
        self.content += self.wrong_bit_width_content

    found = False
    for line in lines:
      if self.pattern in line:
        found = True
        index = lines.index(line)
        if self.relative_location == RelativeLocation.BEFORE:
          lines.insert(index - abs(self.offset), self.content)
        elif self.relative_location == RelativeLocation.AFTER:
          lines.insert(index + self.offset + 1, self.content)
        break

    if not found:
      raise ValueError(f"Pattern {self.pattern} not found in file {file_path}")

    with open(file_path, "w") as f:
      f.writelines(lines)
