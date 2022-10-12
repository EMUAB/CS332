CS332 HW3
==================================

Prints the given directory's files and subdirectories' files. See _Flags_ for more options.

Compile
------------------

"make"

Usage
------------------

"./search \<pathname> \<flags>"
To show all files from the current directory, run "./search". To specify a directory to search, include the path e.g. "./search ~/Desktop".

Flags
------------------

1. -S | Shows the size, permissions, and last access time next to the filename
2. -s \<size in bytes> | Only lists files with sizes less than or equal to the specified size
3. -f "\<string pattern> \<depth>" | Only lists files whose name contains the string pattern, and traverses only as far as depth (0 is the starting directory and -1 sets no depth limit). MUST be surrounded by double quotation marks "".
4. -t \<option> | Use with 'f' as the option to only list regular files, or 'd' to only list directories.
5. -e "\<unix-command with args>" | For each file that matches the search criteria, the given command is executed with that file as its last argument. (Only one "e"-type flag may be used)
6. -E "\<unix-command with args> | Every file that matches the search criteria is appended to the given command as an argument, then executed. (Only one "e"-type flag may be used)
