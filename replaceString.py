import os

# Function to search and replace a string in a file


def search_replace_in_file(file_path, search_string_lower, replace_string_lower,
                           search_string_capital, replace_string_capital,
                           search_string_upper, replace_string_upper):

  with open(file_path, 'r', encoding='utf-8') as src_file:
    lines = src_file.readlines()

  for i in range(0, len(lines)):
    lines[i] = lines[i].replace(search_string_lower, replace_string_lower)
    lines[i] = lines[i].replace(search_string_capital, replace_string_capital)
    lines[i] = lines[i].replace(search_string_upper, replace_string_upper)

  with open(file_path, 'w', encoding='utf-8') as src_file:
    src_file.writelines(lines)

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
        search_replace_in_file(file_path, search_string_lower, replace_string_lower,
                               search_string_capital, replace_string_capital,
                               search_string_upper, replace_string_upper)
