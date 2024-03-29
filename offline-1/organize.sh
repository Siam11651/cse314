#! /bin/bash

# using argument $1 as submissions folder name
# using argument $2 as target folder name
# using argument $3 as test folder name
# using argument $4 as answer folder name

if [ $# -lt 4 ]
then
    echo "oraginze <submissions-directory> <target-directory> <test-directory> <answer-directory> [options]"
    echo "Options:"
    echo -e "-v\t\tEnable verbose mode"
    echo -e "-noexecute\tEnable noexecute mode"

    exit
fi

verboseSwitch="-v"
noexecuteSwitch="-noexecute"

verbose=0
noexecute=0

if [ -n "$5" ]
then
    if [ "$5" = "$verboseSwitch" ]
    then
        verbose=1
    elif [ "$5" = "$noexecuteSwitch" ]
    then
        noexecute=1
    fi
fi

if [ -n "$6" ]
then
    if [ "$6" = "$noexecuteSwitch" ]
    then
        noexecute=1
    fi
fi

rm -rf "$2"             # clean output first
rm -rf "temp"           # delete temp directory if already exists

# start making target directories
mkdir -p "$2/C"
mkdir -p "$2/Java"
mkdir -p "$2/Python"
# end making target directories

mkdir "temp"            # make temp directory
touch "$2/result.csv"   # touch csv
echo "student_id,type,matched,not_matched" > "$2/result.csv"    # csv prepare

testFileCount=$(($(ls -l "$3" | wc -l) - 1))
testFolderName="$3"
testFolderNameLength=${#testFolderName}
start=$(($testFolderNameLength + 5))

[[ $verbose -ne 0 ]] && echo "Found $testFileCount test files"

DetectAndPlaceFile()
{
    # using argument $1 as root directory of search
    # using argument $2 as roll
    # using argument $3 as target

    for file in "$1/"*
    do
        if [ -f "$file" ]
        then
            if [ "${file: -2:2}" = ".c" ]
            then
                mkdir -p "$3/C/$2"
                cp "$file" "$3/C/$2/main.c"

                return 1    # c
            elif [ "${file: -5:5}" = ".java" ]
            then
                mkdir -p "$3/Java/$2"
                cp "$file" "$3/Java/$2/Main.java"

                return 2    # java
            elif [ "${file: -3:3}" = ".py" ]
            then
                mkdir -p "$3/Python/$2"
                cp "$file" "$3/Python/$2/main.py"

                return 3    # python
            fi
        else
            DetectAndPlaceFile "$file" "$2" "$3"

            returnValue=$?

            if [ $returnValue -ne 0 ]  # return value 0 means no file detected
            then
                return $returnValue
            fi
        fi
    done

    return 0
}

for file in "$1/"*".zip"
do
    rm -rf "temp/"*
    unzip -qq "$file" -d "temp"

    roll="${file: -11:-4}"

    [[ $verbose -ne 0 ]] && echo "Organizing files of $roll"

    DetectAndPlaceFile "temp" $roll "$2"

    fileType=$?

    if [ $noexecute -eq 0 ]
    then
        [[ $verbose -ne 0 ]] && echo "Executing files of $roll"

        matchCount=0

        if [ $fileType -eq 1 ]   # return value 1 means c
        then
            codeType="C"

            gcc "$2/C/$roll/main.c" -o "$2/C/$roll/main.out"

            for testFile in "$3/"*
            do
                fileNumber=${testFile:$start:-4}
                outFileName="$2/C/$roll/out_$fileNumber.txt"
                answerFileName="$4/ans$fileNumber.txt"

                "$2/C/$roll/main.out" < "$testFile" > "$outFileName"
                diff -q "$answerFileName" "$outFileName" > "/dev/null"

                if [ $? -eq 0 ]
                then
                    matchCount=$(($matchCount + 1))
                fi
            done
        elif [ $fileType -eq 2 ] # return value 2 means java
        then
            codeType="Java"

            javac "$2/Java/$roll/Main.java"

            for testFile in "$3/"*
            do
                fileNumber=${testFile:$start:-4}
                outFileName="$2/Java/$roll/out_$fileNumber.txt"
                answerFileName="$4/ans$fileNumber.txt"

                cat "$testFile" | java -cp "$2/Java/$roll" "Main" > "$outFileName"
                diff -q "$answerFileName" "$outFileName" > "/dev/null"

                if [ $? -eq 0 ]
                then
                    matchCount=$(($matchCount + 1))
                fi
            done
        elif [ $fileType -eq 3 ] # return value 3 means python
        then
            codeType="Python"

            for testFile in "$3/"*
            do
                fileNumber=${testFile:$start:-4}
                outFileName="$2/Python/$roll/out_$fileNumber.txt"
                answerFileName="$4/ans$fileNumber.txt"

                cat "$testFile" | python3 "$2/Python/$roll/main.py" > "$outFileName"
                diff -q "$answerFileName" "$outFileName" > "/dev/null"

                if [ $? -eq 0 ]
                then
                    matchCount=$(($matchCount + 1))
                fi
            done
        fi

        unmatchedCount=$(($testFileCount - $matchCount))

        echo "$roll,$codeType,$matchCount,$unmatchedCount" >> "$2/result.csv"
    fi
done

rm -rf "temp"