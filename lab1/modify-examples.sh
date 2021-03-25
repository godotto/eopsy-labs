#!/usr/bin/bash

# testing directory name
test_dir="testing-directory"

# if tree program is installed, use it to present changes in files in directories
if [ $(command -v tree) != '' ]; then
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
echo -e "File structure used for testing purpose:\n"
$file_structure_command
read -p "Press enter to continue" # wait for user's action
clear   # clear console

# test case 1
echo -e "TEST CASE 1 - print help message with -h argument\n"
./modify -h
read -p "Press enter to continue" # wait for user's action
clear   # clear console

# test case 2
echo -e "TEST CASE 2 - run command without any options or files\n"
./modify
read -p "Press enter to continue"
clear

# test case 3
echo -e "TEST CASE 3 - run command with a non-existing option\n"
./modify -c
read -p "Press enter to continue"
clear

# test case 4
echo -e "TEST CASE 4 - run command with -h option and other options with file\n"
./modify -h -l $test_dir/test1.txt
read -p "Press enter to continue"
clear

# test case 5
echo -e "TEST CASE 5 - run command with non-existing option\n"
./modify -f $test_dir/test1.txt
read -p "Press enter to continue"
clear

# test case 6
echo -e "TEST CASE 6 - run command with both -l and -u options\n"
./modify -lu $test_dir/test1.txt
read -p "Press enter to continue"
clear

# test case 7
echo -e "TEST CASE 7 - change letters to uppercase in one file\n"
find $test_dir > file_structure_before.txt
./modify -u $test_dir/test1.txt
find $test_dir > file_structure_after.txt
diff --color -y file_structure_before.txt file_structure_after.txt
read -p "Press enter to continue"
clear

# test case 8
echo -e "TEST CASE 8 - change letters to uppercase in two files with special characters in names\n"
find $test_dir > file_structure_before.txt
./modify -u $test_dir/test-dir1/test\*dir3/te\*st3.txt $test_dir/\!test-dir2/test\*dir3/te\ st7.txt $test_dir/test-dir1/test2.txt
find $test_dir > file_structure_after.txt
diff --color -y file_structure_before.txt file_structure_after.txt
read -p "Press enter to continue"
clear

# test case 9
echo -e "TEST CASE 9 - change letters to lowercase in one file\n"
find $test_dir > file_structure_before.txt
./modify -l $test_dir/test-dir1/TEST2.TXT
find $test_dir > file_structure_after.txt
diff --color -y file_structure_before.txt file_structure_after.txt
read -p "Press enter to continue"
clear

# test case 10
echo -e "TEST CASE 10 - change directory name with special characters according to sed pattern\n"
find $test_dir > file_structure_before.txt
./modify "s/-/=/g" $test_dir/\!test-dir2/test\ dir5/-test-dir6/
find $test_dir > file_structure_after.txt
diff --color -y file_structure_before.txt file_structure_after.txt
read -p "Press enter to continue"
clear

# test case 11
echo -e "TEST CASE 11 - run command with invalid sed pattern\n"
find $test_dir > file_structure_before.txt
./modify "s/-/=" $test_dir/test-dir1/test\ dir5/-test-dir6/test8.txt
find $test_dir > file_structure_after.txt
diff --color -y file_structure_before.txt file_structure_after.txt
read -p "Press enter to continue"
clear

# test case 12
echo -e "TEST CASE 12 - run command with non-existing file\n"
find $test_dir > file_structure_before.txt
./modify -l $test_dir/TEST9.TXT
find $test_dir > file_structure_after.txt
diff --color -y file_structure_before.txt file_structure_after.txt
read -p "Press enter to continue"
clear

# test case 13
echo -e "TEST CASE 13 - change letters into uppercase in a recursive way\n"
find $test_dir > file_structure_before.txt
./modify -ru $test_dir/\!test-dir2
find $test_dir > file_structure_after.txt
diff --color -y file_structure_before.txt file_structure_after.txt
read -p "Press enter to continue"
clear

# test case 14
echo -e "TEST CASE 14 - change letters into lowercase in a recursive way\n"
find $test_dir > file_structure_before.txt
./modify -rl $test_dir/\!TEST-DIR2/TEST\&DIR4
find $test_dir > file_structure_after.txt
diff --color -y file_structure_before.txt file_structure_after.txt
read -p "Press enter to continue"
clear

# test case 15
echo -e "TEST CASE 15 - change names according to sed pattern in every file and directory (in a recursive way)\n"
find $test_dir > file_structure_before.txt
./modify -r "s/test/passed-test/" $test_dir
test_dir=$(echo $test_dir | sed "s/test/passed-test/")
file_structure_command=$(echo $file_structure_command | sed "s/test/passed-test/")
find $test_dir > file_structure_after.txt
diff --color -y file_structure_before.txt file_structure_after.txt
read -p "Press enter to continue"
clear

rm -r $test_dir # tidy up testing directories
rm file_structure*