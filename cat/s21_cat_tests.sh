#!/bin/bash

CAT=./s21_cat
FILE1=sometxt.txt
FILE2=somebin

echo TEST no flags
diff <($CAT $FILE1) <(cat $FILE1)
diff <($CAT $FILE2) <(cat $FILE2)
diff <($CAT $FILE1 $FILE2) <(cat $FILE1 $FILE2)

# echo "TEST stdin (to check enter same symbols twice with newline and ctrl+D)"
# diff <($CAT -) <(cat -)

echo TEST single flag 1 file
for fl in -b --number-nonblank -n --number -s --squeeze-blank -e -t -v -E -T
do
    diff <($CAT $fl $FILE1) <(cat $fl $FILE1)
    diff <($CAT $FILE1 $fl) <(cat $FILE1 $fl)
    diff <($CAT $fl $FILE2) <(cat $fl $FILE2)
    diff <($CAT $FILE2 $fl) <(cat $FILE2 $fl)
done

echo TEST single flag 2 files
for fl in -b --number-nonblank -n --number -s --squeeze-blank -e -t -v -E -T
do
    diff <($CAT $fl $FILE1 $FILE2) <(cat $fl $FILE1 $FILE2)
    diff <($CAT $FILE1 $FILE2 $fl) <(cat $FILE1 $FILE2 $fl)
    diff <($CAT $FILE1 $fl $FILE2) <(cat $FILE1 $fl $FILE2)
done

echo TEST equivalent flags
diff <($CAT -b $FILE1) <(cat --number-nonblank $FILE1)
diff <($CAT --number-nonblank $FILE1) <(cat -b $FILE1)
diff <($CAT -n $FILE1) <(cat --number $FILE1)
diff <($CAT --number $FILE1) <(cat -n $FILE1)
diff <($CAT -s $FILE1) <(cat --squeeze-blank $FILE1)
diff <($CAT --squeeze-blank $FILE1) <(cat -s $FILE1)
diff <($CAT -t $FILE1) <(cat -Tv $FILE1)
diff <($CAT -Tv $FILE1) <(cat -t $FILE1)
diff <($CAT -e $FILE1) <(cat -Ev $FILE1)
diff <($CAT -Ev $FILE1) <(cat -e $FILE1)
diff <($CAT -b $FILE1) <(cat -nb $FILE1)
diff <($CAT -bn $FILE1) <(cat -b $FILE1)

diff <($CAT -b $FILE2) <(cat --number-nonblank $FILE2)
diff <($CAT --number-nonblank $FILE2) <(cat -b $FILE2)
diff <($CAT -n $FILE2) <(cat --number $FILE2)
diff <($CAT --number $FILE2) <(cat -n $FILE2)
diff <($CAT -s $FILE2) <(cat --squeeze-blank $FILE2)
diff <($CAT --squeeze-blank $FILE2) <(cat -s $FILE2)
diff <($CAT -t $FILE2) <(cat -Tv $FILE2)
diff <($CAT -Tv $FILE2) <(cat -t $FILE2)
diff <($CAT -e $FILE2) <(cat -Ev $FILE2)
diff <($CAT -Ev $FILE2) <(cat -e $FILE2)
diff <($CAT -b $FILE2) <(cat -nb $FILE2)
diff <($CAT -bn $FILE2) <(cat -b $FILE2)

echo TEST flags combinations
diff <($CAT -benstvET $FILE1) <(cat -benstvET $FILE1)
for fl1 in b e n s t v E T
do
    for fl2 in b e n s t v E T
    do
        for fl3 in b e n s t v E T
        do
            diff <($CAT -$fl1$fl2$fl3 $FILE1) <(cat -$fl1$fl2$fl3 $FILE1)
            diff <($CAT -$fl1 -$fl2 -$fl3 $FILE1) <(cat -$fl1 -$fl2 -$fl3 $FILE1)
            diff <($CAT -$fl1$fl2$fl3 $FILE2) <(cat -$fl1$fl2$fl3 $FILE2)
            diff <($CAT -$fl1 -$fl2 -$fl3 $FILE2) <(cat -$fl1 -$fl2 -$fl3 $FILE2)
        done
    done
done
