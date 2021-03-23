#!/usr/bin/bash

# testing directory name
test_dir="testing-directory"

# if tree program is installed, use it to present changes in files in directories
if [ -n $(command -v tree) ]; then
    file_structure_command="tree $test_dir"
else
    file_structure_command="find $test_dir"
fi

# create a directory for tests
mkdir $test_dir && cd $test_dir

# sample directories and files generation
mkdir -p {test-dir1,\!test-dir2}/{test\*dir3,test\&dir4,"test dir5"}/-test-dir6
touch test1.txt
touch test-dir1/test2.txt
touch test-dir1/test\*dir3/te\*st3.txt
touch test-dir1/test\&dir4/te\&st4.txt
touch \!test-dir2/test\ dir5/te\ st5.txt
touch \!test-dir2/test\&dir4/-test-dir6/-test6.txt
touch \!test-dir2/test\*dir3/te\ st7.txt
touch test-dir1/test\ dir5/-test-dir6/test8.txt

cd ..

# print directories and files after generation
echo "File structure used for testing purpose:"
$file_structure_command
read -p "Press enter to continue" # wait for user's action
clear   # clear console

# test case 1 - print help message with -h argument
echo TEST CASE 1
./modify -h
read -p "Press enter to continue" # wait for user's action
clear   # clear console

# test case 2 - run command without any options or files
echo TEST CASE 2
./modify
read -p "Press enter to continue"
clear

# test case 3 - run command with a non-existing option
echo TEST CASE 3
./modify -c
read -p "Press enter to continue"
clear

# test case 4 - run command with -h option and other options with file
echo TEST CASE 4
./modify -h -l $test_dir/test1.txt
read -p "Press enter to continue"
clear

# test case 5 - run command with non-existing option
echo TEST CASE 5
./modify -f $test_dir/test1.txt
read -p "Press enter to continue"
clear

# test case 6 - run command with both -l and -u options
echo TEST CASE 6
./modify -lu $test_dir/test1.txt
read -p "Press enter to continue"
clear

rm -r testing-directory # tidy up testing directories