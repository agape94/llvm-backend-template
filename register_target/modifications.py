import clang
from clang.cindex import Index, Config, CursorKind
from .parseCpp import *
from variables import CONFIG_ROOT
from enum import Enum
import yaml
import platform


class PROJECT(Enum):
  LLVM = "llvm.yml"
  CLANG = "clang.yml"
  LLD = "lld.yml"


def visit_function_node(node, function_name, file_path, callback):
  if (node.kind == clang.cindex.CursorKind.CXX_METHOD or node.kind == clang.cindex.CursorKind.FUNCTION_DECL) and node.spelling == function_name:
    for child in node.get_children():
      callback(child)
    return
  else:
    for child in node.get_children():
      if str(child.extent.start.file) == file_path:
        visit_function_node(child, function_name, file_path, callback)


def applyModifications(config_file, environment_path, target_name, bit_width, endianness):
  message = f"\nAdding target {target_name} with bit width {bit_width} and endianness {endianness} to"

  if config_file == PROJECT.LLVM:
    message += " LLVM ..."
  elif config_file == PROJECT.CLANG:
    message += " CLANG ..."
  elif config_file == PROJECT.LLD:
    message += " LLD ..."
  print(message)

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
  SWITCH_CASE = "switch_case"
  SWITCH_CASE_NESTED = "switch_case_nested"
  SWITCH_CASE_BIT_WIDTH = "switch_case_bit_width"
  SWITCH_CASE_ENDIANNESS = "switch_case_endianness"
  UNNAMED_ENUM_VALUE = "unnamed_enum_value"
  NAMED_ENUM_VALUE = "named_enum_value"
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

    print(f"  - {self.file_path}")

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
      change.replacePlaceHolders(
          target_name, bit_width, endianness)


class Change:
  def __init__(self, change):
    assert change.get("type", None) is not None
    self.type = ModificationType(change.get("type", None))
    self.location = str(change.get("location", None)) if change.get(
        "location", None) else None
    self.content = str(change.get("content", None)) if change.get(
        "content", None) else None
    self.pattern = str(change.get("pattern", None)) if change.get(
        "pattern", None) else None

    relative_location = str(change.get("relative_location", None)) if change.get(
        "relative_location", None) else None
    self.relative_location = RelativeLocation(
        relative_location) if relative_location else RelativeLocation.UNKNOWN

    self.switch_condition = str(change.get("switch_condition", None)) if change.get(
        "switch_condition", None) else None

    self.min_bit_width = int(change.get("min_bit_width", None)) if change.get(
        "min_bit_width", None) else None

    self.bit_width = int(change.get("bit_width", None)) if change.get(
        "bit_width", None) else None

    # specific to nested switch case modification
    self.parent_switch_condition = str(change.get("parent_switch_condition", None)) if change.get(
        "parent_switch_condition", None) else None
    self.parent_case_name = str(change.get("parent_case_name", None)) if change.get(
        "parent_case_name", None) else None

    self.value = bool(change.get("value", None)) if change.get(
        "value", None) else False
    self.comment = str(change.get("comment", None)) if change.get(
        "comment", None) else None
    self.offset = int(change.get("offset", None)) if change.get(
        "offset", None) else 0

    self.correct_bit_width_content = str(
        change.get("correct_bit_width_content", None)) if change.get("correct_bit_width_content", None) else None
    self.wrong_bit_width_content = str(
        change.get("wrong_bit_width_content", None)) if change.get("wrong_bit_width_content", None) else None

    self.endianness = str(change.get("endianness", None)) if change.get(
        "endianness", None) else None

  def __str__(self):
    change_str = f"type: \"{self.type}\", location: \"{self.location}\", content: \"{self.content}\", pattern: \"{self.pattern}\", relative_location: \"{self.relative_location}\", switch_condition: \"{self.switch_condition}\", parent_switch_condition: \"{self.parent_switch_condition}\", parent_case_name: \"{self.parent_case_name}\""
    return change_str

  def replacePlaceHolders(self, target_name, bit_width, endianness):
    def replace(pattern, content):
      if pattern is not None and content is not None:
        if self.location:
          self.location = self.location.replace(pattern, content)
        if self.content:
          self.content = self.content.replace(pattern, content)
        if self.pattern:
          self.pattern = self.pattern.replace(pattern, content)
        if self.switch_condition:
          self.switch_condition = self.switch_condition.replace(
              pattern, content)
        if self.parent_switch_condition:
          self.parent_switch_condition = self.parent_switch_condition.replace(
              pattern, content)
        if self.parent_case_name:
          self.parent_case_name = self.parent_case_name.replace(
              pattern, content)
        if self.comment:
          self.comment = self.comment.replace(pattern, content)

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
    if self.type == ModificationType.SWITCH_CASE:
      self._addSwitchCase(file_path, target_name, bit_width, endianness)
    elif self.type == ModificationType.SWITCH_CASE_NESTED:
      self._addSwitchCaseNested(file_path, target_name, bit_width, endianness)
    elif self.type == ModificationType.SWITCH_CASE_BIT_WIDTH:
      self._addSwitchCaseBitWidth(
          file_path, target_name, bit_width, endianness)
    elif self.type == ModificationType.SWITCH_CASE_ENDIANNESS:
      self._addSwitchCaseEndianness(
          file_path, target_name, bit_width, endianness)
    elif self.type == ModificationType.UNNAMED_ENUM_VALUE:
      self._addEnumValue(
          file_path, target_name, bit_width, endianness, named_enum=False)
    elif self.type == ModificationType.NAMED_ENUM_VALUE:
      self._addEnumValue(
          file_path, target_name, bit_width, endianness, named_enum=True)
    elif self.type == ModificationType.NEW_FILE:
      self._addNewFile(
          file_path, target_name, bit_width, endianness)
    elif self.type == ModificationType.STRING:
      self._addString(
          file_path, target_name, bit_width, endianness)
    else:
      raise ValueError(f"Unsupported modification type: {self.type}")

  def _addSwitchCase(self, file_path: str, target_name: str, bit_width: int, endianness: str):
    assert self.type is not None
    assert self.location is not None
    assert self.content is not None
    assert self.switch_condition is not None

    AST = parseFile(file_path)

    with open(file_path, "r") as f:
      lines = f.readlines()

    def visit_switch_node(node):
      if node.kind == clang.cindex.CursorKind.SWITCH_STMT:
        switch_stmt = node
        if self.switch_condition in lines[switch_stmt.extent.start.line - 1]:
          for switch_child in switch_stmt.get_children():
            if switch_child.kind == clang.cindex.CursorKind.COMPOUND_STMT:
              if self.correct_bit_width_content and self.wrong_bit_width_content and self.bit_width:
                if self.bit_width == 32 and bit_width <= self.bit_width or self.bit_width == 64 and bit_width == self.bit_width:
                  self.content += self.correct_bit_width_content
                else:
                  self.content += self.wrong_bit_width_content
              if self.endianness and self.endianness != endianness:
                return
              lines.insert(switch_child.extent.start.line, self.content)
              return
      else:
        for child in node.get_children():
          if str(child.extent.start.file) == file_path:
            visit_switch_node(child)

    visit_function_node(AST.cursor, self.location,
                        file_path, visit_switch_node)

    with open(file_path, "w") as f:
      f.writelines(lines)

  def _addSwitchCaseNested(self, file_path: str, target_name: str, bit_width: int, endianness: str):
    assert self.type is not None
    assert self.location is not None
    assert self.content is not None
    assert self.parent_switch_condition is not None
    assert self.parent_case_name is not None
    assert self.switch_condition is not None

    AST = parseFile(file_path)

    with open(file_path, "r") as f:
      lines = f.readlines()

    def visit_nested_switch_node(node):
      if node.kind == clang.cindex.CursorKind.SWITCH_STMT and str(node.extent.start.file):
        switch_stmt = node
        if self.parent_switch_condition in lines[switch_stmt.extent.start.line - 1]:
          parent_switch_stmt = switch_stmt
          # Visit the children of the parent switch statement to find the nested switch statement
          for child in parent_switch_stmt.get_children():
            visit_nested_switch_node(child)
      elif node.kind == clang.cindex.CursorKind.CASE_STMT and str(node.extent.start.file):
        case_stmt = node
        if self.parent_case_name in lines[case_stmt.extent.start.line - 1]:
          # Visit the children of the case statement to find the nested switch statement
          for child in node.get_children():
            if child.kind == clang.cindex.CursorKind.SWITCH_STMT:
              nested_switch_stmt = child
              for nested_switch_child in nested_switch_stmt.get_children():
                if nested_switch_child.kind == clang.cindex.CursorKind.COMPOUND_STMT:
                  lines.insert(
                      nested_switch_child.extent.start.line, self.content)
                  return
      else:
        for child in node.get_children():
          if str(child.extent.start.file) == file_path:
            visit_nested_switch_node(child)

    visit_function_node(AST.cursor, self.location,
                        file_path, visit_nested_switch_node)

    with open(file_path, "w") as f:
      f.writelines(lines)

  def _addEnumValue(self, file_path: str, target_name: str, bit_width: int, endianness: str, named_enum: bool):
    assert self.type is not None
    assert self.content is not None
    assert self.pattern is not None

    AST = parseFile(file_path)

    with open(file_path, "r") as f:
      lines = f.readlines()

    max_value = 0
    line_number = 0

    def visit_enum_node(node):
      nonlocal max_value
      nonlocal line_number
      if named_enum:
        named_condition = named_enum and node.spelling == self.location
      else:
        named_condition = not named_enum
      if node.kind == clang.cindex.CursorKind.ENUM_DECL and str(node.extent.start.file) == file_path and named_condition:
        for child in node.get_children():
          visit_enum_node(child)

      elif node.kind == clang.cindex.CursorKind.ENUM_CONSTANT_DECL and str(node.extent.start.file) == file_path and node.spelling.startswith(self.pattern):
        line_number = node.extent.start.line
        if self.value:
          for token in node.get_tokens():
            if token.kind == clang.cindex.TokenKind.LITERAL:
              if max_value < int(token.spelling):
                max_value = int(token.spelling)
      else:
        for child in node.get_children():
          visit_enum_node(child)
    visit_enum_node(AST.cursor)

    content = f"{self.content.rstrip()}"

    if self.value:
      content = f"{content} = {max_value + 1},"
    else:
      content = f"{content},"
    content = f"{content}\t{self.comment}" if self.comment is not None else content

    if self.relative_location is RelativeLocation.AFTER:
      line_number += 1 + self.offset
    elif self.relative_location is RelativeLocation.BEFORE:
      line_number -= 1 - abs(self.offset)

    lines.insert(line_number, content)

    with open(file_path, "w") as f:
      f.writelines(lines)

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

    for line in lines:
      if self.pattern in line:
        index = lines.index(line)
        if self.relative_location == RelativeLocation.BEFORE:
          lines.insert(index - abs(self.offset), self.content)
        elif self.relative_location == RelativeLocation.AFTER:
          lines.insert(index + self.offset + 1, self.content)
        break

    with open(file_path, "w") as f:
      f.writelines(lines)
