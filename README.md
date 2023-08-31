# Virtual File System (VFS) in C

## Features

### File and Directory Management
- **Create Files**: The `touch` command allows you to create new files in the virtual file system.
- **Create Directories**: Use the `mkdir` command to create new directories.
- **Remove Files**: The `rm` command lets you delete files.
- **Remove Directories**: The `rmdir` command is used for deleting directories.

### Navigation and Inspection
- **Change Directory**: The `cd` command allows you to navigate through different directories.
- **List Contents**: The `ls` command lists the contents of a directory.
- **Current Directory**: The `pwd` command prints the current working directory.

### File Content Manipulation
- **Append to File**: The `append` command allows you to append content to an existing file.
- **Read File**: The `cat` command lets you read and display the content of a file.

### Environment Variables
- The project initializes environment variables to manage the current state, including the root and current working directories.

### Storage
- The virtual file system uses an array-based storage mechanism for both files and directories.
- Actual files are created and deleted in the host system, making it a hybrid virtual file system.

### Command Parsing
- The project includes a robust command parser that can handle quoted strings and multiple arguments.

### Clean Exit
- The `exit` command ensures a clean exit by deleting all actual files related to the virtual file system.

## Getting Started

To get started with this project, you can clone the repository and compile the `vfs.c` file.

```bash
gcc vfs.c -o vfs
./vfs
```

## Contributing

Feel free to contribute to this project by submitting pull requests or issues.

## License

This project is open-source and available under the MIT License.

---

This should give a good overview of the features without diving into the code. Let me know if you need more details!
