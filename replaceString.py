import os

# Function to search and replace a string in a file
def search_replace_in_file(file_path, search_string_small, replace_string_small, 
                                search_string_capital, replace_string_capital,
                                search_string_small_big, replace_string_small_big):
    
    with open(file_path, 'r', encoding='utf-8') as src_file:
        lines = src_file.readlines()

    for i in range(0, len(lines)):
        lines[i] = lines[i].replace(search_string_small, replace_string_small)
        lines[i] = lines[i].replace(search_string_capital, replace_string_capital)
        lines[i] = lines[i].replace(search_string_small_big, replace_string_small_big)

    with open(file_path, 'w', encoding='utf-8') as src_file:
        src_file.writelines(lines)

# Function to recursively search and replace in a directory
def search_replace_in_directory(directory_path, search_string_small, replace_string_small, 
                                search_string_capital, replace_string_capital,
                                search_string_small_big, replace_string_small_big):
    for root, _, files in os.walk(directory_path):
        for file in files:
            file_path = os.path.join(root, file)
            file_path_new = file_path.replace(search_string_capital, replace_string_capital)
            os.rename(file_path, file_path_new)

            file_path = file_path_new

            if os.path.isfile(file_path):
                search_replace_in_file(file_path, search_string_small, replace_string_small, 
                                search_string_capital, replace_string_capital,
                                search_string_small_big, replace_string_small_big)

if __name__ == "__main__":
    # Specify the directory path where you want to search and replace
    directory_path = '/home/alex/tugraz/thesis/llvm-backend-template/llvm/lib/Target/Simple'
    
    # Specify the string to search for and the replacement string
    search_string_small = 'lanai'
    replace_string_small = 'simple'

    search_string_capital = 'Lanai'
    replace_string_capital = 'Simple'

    search_string_small_big = 'LANAI'
    replace_string_small_big = 'SIMPLE'
    
    search_replace_in_directory(directory_path, 
                                search_string_small, replace_string_small, 
                                search_string_capital, replace_string_capital,
                                search_string_small_big, replace_string_small_big)
