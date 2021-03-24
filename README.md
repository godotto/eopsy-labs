# eopsy-labs

## Lab 1

### Task
The task was to create a shell script (to be more specific - for bash) which will modify names of files and directories. It has to change the names in three following ways:
- all uppercase letters into lowercase letters
- all lowercase letters into uppercase letters
- with help of sed editor.
All of these may also be executed with recursion flag in order to change all names recursively (i.e. in a way find works and traverses file tree).

### Usage
The following usage manual may be displayed by script when it is executed with -h option.

Usage: modify [OPTION] [SED PATTERN] [[DIR]FILEs]

Modify name of the FILEs.

Options:

 -r     rename FILEs recursively
 
 -l     change all uppercase letters into lowercase letters
 
 -u     change all lowercase letters into uppercase letters
 
 -h     display this help

Alternatively, a SED PATTERN may be used instead of uppercase and lowercase options.

### Testing
The second task regarding the modify script was to create modify-examples.sh script which will lead several tests. To run tests just execute the script - everything including generation of sample file structure and cleaning it after execution is done automatically.
