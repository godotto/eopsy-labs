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

# test case 7 - change letters to uppercase in one file
echo TEST CASE 7
./modify -u $test_dir/test1.txt
$file_structure_command # print file structure
read -p "Press enter to continue"
clear

# test case 8 - change letters to uppercase in two files with special characters in names
echo TEST CASE 8
./modify -u $test_dir/test-dir1/test\*dir3/te\*st3.txt $test_dir/\!test-dir2/test\*dir3/te\ st7.txt $test_dir/test-dir1/test2.txt
$file_structure_command # print file structure
read -p "Press enter to continue"
clear

# test case 9 - change letters to lowercase in one file
echo TEST CASE 9
./modify -l $test_dir/test-dir1/TEST2.TXT
$file_structure_command # print file structure
read -p "Press enter to continue"
clear

# test case 10 - change directory name with special characters according to sed pattern
echo TEST CASE 10
./modify "s/-/=/g" $test_dir/\!test-dir2/test\ dir5/-test-dir6/
$file_structure_command # print file structure
read -p "Press enter to continue"
clear

# test case 11 - run command with invalid sed pattern
echo TEST CASE 11
./modify "s/-/=" $test_dir/test-dir1/test\ dir5/-test-dir6/test8.txt
$file_structure_command # print file structure
read -p "Press enter to continue"
clear

# test case 12 - run command with non-existing file
echo TEST CASE 12
./modify -l $test_dir/TEST9.TXT
$file_structure_command # print file structure
read -p "Press enter to continue"
clear

# test case 13 - change letters into uppercase in a recursive way
echo TEST CASE 13
./modify -ru $test_dir/\!test-dir2
$file_structure_command # print file structure
read -p "Press enter to continue"
clear

# test case 14 - change letters into lowercase in a recursive way
echo TEST CASE 14
./modify -rl $test_dir/\!TEST-DIR2/TEST\&DIR4
$file_structure_command # print file structure
read -p "Press enter to continue"
clear

# test case 15 - change names according to sed pattern in every file and directory (in a recursive way)
echo TEST CASE 15
./modify -r "s/test/passed-test/" $test_dir
test_dir=$(echo $test_dir | sed "s/test/passed-test/")
file_structure_command=$(echo $file_structure_command | sed "s/test/passed-test/")
$file_structure_command # print file structure
read -p "Press enter to continue"
clear

# rm -r $test_dir # tidy up testing directories