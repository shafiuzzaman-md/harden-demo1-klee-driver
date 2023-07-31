import os
import re

def replace_copy_mem_with_memcpy(content):
    # Define the regular expression pattern to find CopyMem calls
    copy_mem_pattern = r'\bCopyMem\s*\('

    # Use regular expression to find and replace CopyMem with memcpy
    updated_content = re.sub(copy_mem_pattern, 'memcpy(', content)

    return updated_content

def replace_allocate_pool_with_malloc(content):
    # Define the regular expression pattern to find AllocatePool calls
    allocate_pool_pattern = r'AllocatePool\s*\('

    # Use regular expression to replace AllocatePool with malloc
    updated_content = re.sub(allocate_pool_pattern, r'malloc(', content)

    return updated_content

def replace_free_pool_with_free(content):
    # Define the regular expression pattern to find FreePool calls
    free_pool_pattern = r'FreePool\s*\('

    # Use regular expression to replace FreePool with free
    updated_content = re.sub(free_pool_pattern, r'free(', content)

    return updated_content

def replace_variables_with_null(content):
    # Define the regular expression pattern to match variables with &g prefix
    variable_pattern = r'&g(\w+)'

    # Use regular expression to find and replace the variables with NULL
    updated_content = re.sub(variable_pattern, r'NULL', content)

    return updated_content

def comment_out_gBS(source_file):

    # Define the regular expression pattern to find the statements containing gBS->
    gBS_pattern = r'(\w+\s*=\s*gBS->[^;]*;)'

    # Read the content of the source file in text mode with UTF-8 encoding
    with open(source_file, 'r', encoding='utf-8', errors='ignore') as file:
        content = file.read()

    # Use regular expression to find and comment out gBS
    updated_content = re.sub(gBS_pattern, r'/*\g<0>*/', content)

    # Write the updated content back to the source file
    with open(source_file, 'w', encoding='utf-8') as file:
        file.write(updated_content)

    # Define a separate regular expression pattern to capture gBS->CloseEvent (Event); and gBS->SignalEvent(AliceProtocol->Demo1_Ready_To_Run_Event);
    specific_patterns = r'(gBS->CloseEvent\s*\([^;]*\);|gBS->SignalEvent\s*\([^;]*\);)'

    # Read the content of the source file in text mode with UTF-8 encoding
    with open(source_file, 'r', encoding='utf-8', errors='ignore') as file:
        content = file.read()

    # Use regular expression to find and comment out gBS
    updated_content = re.sub(specific_patterns, r'/*\g<0>*/', content)

    # Write the updated content back to the source file
    with open(source_file, 'w', encoding='utf-8') as file:
        file.write(updated_content)

def comment_out_statement(source_file, target_pattern):
    # Read the content of the source file in text mode with UTF-8 encoding
    with open(source_file, 'r', encoding='utf-8', errors='ignore') as file:
        content = file.read()

    # Use regular expression to find the target pattern and comment it out
    updated_content = re.sub(target_pattern, r'//\g<0>', content)

    # Write the updated content back to the source file
    with open(source_file, 'w', encoding='utf-8') as file:
        file.write(updated_content)

def comment_include_lines(source_file):
    # Define the regular expression patterns to match <header.h> and DEBUG statements
    include_pattern = r'#\s*include\s*<.*?>'
    debug_pattern = r'\bDEBUG\b.*?\n?'

    # Define the pattern to match lines with __FUNCTION__
    function_pattern = r'.*?\b__FUNCTION__\b.*?\n?'

    # Read the content of the source file in text mode with UTF-8 encoding
    with open(source_file, 'r', encoding='utf-8', errors='ignore') as file:
        content = file.read()

    # Comment out <header.h> include statements
    updated_content = re.sub(include_pattern, r'//\g<0>', content)

    # Comment out DEBUG statements
    updated_content = re.sub(debug_pattern, r'//\g<0>', updated_content)

    # Comment out lines with __FUNCTION__
    updated_content = re.sub(function_pattern, r'//\g<0>', updated_content)

    # Replace variables with NULL
    updated_content = replace_variables_with_null(updated_content)

    # Write the updated content back to the source file
    with open(source_file, 'w', encoding='utf-8') as file:
        file.write(updated_content)

def process_directory(directory):
    # Get a list of all files in the specified directory
    source_files = os.listdir(directory)
   
    # Filter files to include both C and H source files (with extensions .c and .h)
    source_files = [file for file in source_files if file.endswith('.c') or file.endswith('.h')]

    # Process each source file and comment out the <header.h>, DEBUG statements, and lines with __FUNCTION__
    for file in source_files:
        source_file_path = os.path.join(directory, file)
        print("Processing:", source_file_path)
        comment_include_lines(source_file_path)
        comment_out_gBS(source_file_path)
        with open(source_file_path, 'r', encoding='utf-8', errors='ignore') as file:
            content = file.read()
            updated_content = replace_allocate_pool_with_malloc(content)
            updated_content = replace_free_pool_with_free(updated_content)
            updated_content = replace_copy_mem_with_memcpy(updated_content)
        with open(source_file_path, 'w', encoding='utf-8') as file:
            file.write(updated_content)
        

    # Recursively process subdirectories
    subdirectories = [d for d in os.listdir(directory) if os.path.isdir(os.path.join(directory, d))]
    for subdir in subdirectories:
        subdir_path = os.path.join(directory, subdir)
        process_directory(subdir_path)

def main():
    
    script_directory = os.path.dirname(os.path.abspath(__file__))
    root_directory = os.path.dirname(script_directory) + '/edk2/EmulatorPkg'
    print(root_directory)
    print("Running the script...")
    # Process the root directory and all its subdirectories
    process_directory(root_directory)

    print("Script execution completed.")

if __name__ == '__main__':
    main()
