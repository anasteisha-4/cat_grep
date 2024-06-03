#!/bin/bash

GREP=./s21_grep
FILE1=test.txt
FILE2=test2.txt
FILE3=test3.txt
REG1=regex.txt
REG2=regex2.txt

echo TEST no flags
diff <($GREP he $FILE1) <(grep he $FILE1)
diff <($GREP he $FILE2) <(grep he $FILE2)
diff <($GREP he $FILE3) <(grep he $FILE3)

echo TEST single flags without arguments
for fl in i v c l n h o s
    do
        diff <($GREP -$fl word $FILE1) <(grep -$fl word $FILE1)
    done

echo "TEST -e"
for fname in $FILE1 $FILE2 $FILE3
do
    diff <($GREP -e he $fname) <(grep -e he $fname)
    diff <($GREP $fname -e he) <(grep $fname -e he)
    diff <($GREP -e word $fname -e he) <(grep -e word $fname -e he)
    diff <($GREP -e word -e thing $fname -e he) <(grep -e word -e thing $fname -e he)
done

echo "TEST -f"
for fname in $FILE1 $FILE2 $FILE3
do
    diff <($GREP -f $REG1 $fname) <(grep -f $REG1 $fname)
    diff <($GREP $fname -f $REG1) <(grep $fname -f $REG1)

    diff <($GREP -f $REG2 $fname) <(grep -f $REG2 $fname)
    diff <($GREP $fname -f $REG2) <(grep $fname -f $REG2)

    diff <($GREP -f $REG1 -f $REG2 $fname) <(grep -f $REG1 -f $REG2 $fname)
    diff <($GREP -f $REG1 $fname -f $REG2) <(grep -f $REG1 $fname -f $REG2)
done

echo "TEST -s with wrong filenames (no errors expected)"
diff <($GREP -s he noexist.txt) <(grep -s he noexist.txt)
diff <($GREP -s he $FILE1 noexist.txt) <(grep -s he $FILE1 noexist.txt)
diff <($GREP -s he $FILE1 noexist.txt lalal) <(grep -s he $FILE1 noexist.txt lalal)

echo "TEST flags combinations"
for fl1 in i v c l n h s o
do
    for fl2 in i v c l n h s o
    do
        diff <($GREP -$fl1 -$fl2 word $FILE1) <(grep -$fl1 -$fl2 word $FILE1)
        diff <($GREP -$fl1 -$fl2 word *.txt) <(grep -$fl1 -$fl2 word *.txt)
        diff <($GREP -$fl1$fl2 word *.txt) <(grep -$fl1$fl2 word *.txt)

        diff <($GREP -$fl1 -$fl2 -e word $FILE1) <(grep -$fl1 -$fl2 -e word $FILE1)
        diff <($GREP -$fl1 -${fl2}e word *.txt) <(grep -$fl1 -${fl2}e word *.txt)
        diff <($GREP -$fl1${fl2}e word *.txt) <(grep -$fl1${fl2}e word *.txt)

        diff <($GREP -$fl1 -$fl2 -f $REG1 $FILE1) <(grep -$fl1 -$fl2 -f $REG1 $FILE1)
        diff <($GREP -$fl1 -${fl2}f $REG1 *.txt) <(grep -$fl1 -${fl2}f $REG1 *.txt)
        diff <($GREP -$fl1${fl2}f $REG1 *.txt) <(grep -$fl1${fl2}f $REG1 *.txt)
    done
done
