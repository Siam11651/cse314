#! /bin/bash

# using argument $1 as submissions folder name
# using argument $2 as target folder name
# using argument $3 as test folder name
# using argument $4 as answer folder name

# start making target directories
mkdir -p "$2/C"
mkdir -p "$2/Java"
mkdir -p "$2/Python"
# end making target directories

rm -rf "$2"
rm -rf "temp"   # delete temp directory if already exists
mkdir "temp"    # make temp directory

PlaceC(){
    # using argument $1 as root directory of search
    # using argument $2 as roll
    # using argument $3 as target

    for file in "$1/"*
    do
        if [ -f "$file" ]
        then
            if [ "${file: -2:2}" = ".c" ]
            then
                rm -rf "$3/C/$2"
                mkdir -p "$3/C/$2"
                cp "$file" "$3/C/$2/main.c"

                return 0
            fi
        else
            PlaceC "$file" "$2" "$3"

            if [ $? -eq 0 ]
            then
                return 0
            fi
        fi
    done
}

PlaceJava(){
    # using argument $1 as root directory of search
    # using argument $2 as roll
    # using argument $3 as target

    for file in "$1/"*
    do
        if [ -f "$file" ]
        then
            if [ "${file: -5:5}" = ".java" ]
            then
                rm -rf "$3/Java/$2"
                mkdir -p "$3/Java/$2"
                cp "$file" "$3/Java/$2/Main.java"

                return 0
            fi
        else
            PlaceJava "$file" "$2" "$3"

            if [ $? -eq 0 ]
            then
                return 0
            fi
        fi
    done
}

PlacePython(){
    # using argument $1 as root directory of search
    # using argument $2 as roll
    # using argument $3 as target

    for file in "$1/"*
    do
        if [ -f "$file" ]
        then
            if [ "${file: -3:3}" = ".py" ]
            then
                rm -rf "$3/Python/$2"
                mkdir -p "$3/Python/$2"
                cp "$file" "$3/Python/$2/main.py"

                return 0
            fi
        else
            PlacePython "$file" "$2" "$3"

            if [ $? -eq 0 ]
            then
                return 0
            fi
        fi
    done
}

# start decompress and place files
for file in "$1/"*".zip"
do
    rm -rf "temp/"*
    unzip "$file" -d "temp"

    roll="${file: -11:-4}"

    PlaceC "temp" "$roll" "$2"          # place c files
    PlaceJava "temp" "$roll" "$2"       # place java files
    PlacePython "temp" "$roll" "$2"     # place python files
done
# end decompress and place files

# start compile c files
for folder in "$2/C/"*
do
    gcc "$folder/main.c" -o "$folder/main.out"
done
# # end compile c files

# start compile java files
for folder in "$2/Java/"*
do
    javac "$folder/Main.java"
done
# end compile java files

touch "$2/result.csv"
echo "student_id,type,matched,not_matched" > "$2/result.csv"

testFileCount=$(($(ls -l "$3" | wc -l) - 1))
testFolderName="$3"
testFolderNameLength=${#testFolderName}
start=$(($testFolderNameLength + 5))

# start run c
for folder in "$2/C/"*
do
    roll=${folder: -7}
    matchCount=0

    for file in "$3/"*
    do
        fileNumber=${file:$start:-4}
        outFileName="$folder/out_$fileNumber.txt"
        answerFileName="$4/ans$fileNumber.txt"

        "$folder/main.out" < "$file" > "$outFileName"

        unmatchedLineCount=$(diff -y --suppress-common-lines "$answerFileName" "$outFileName" | wc -l)

        if [ $unmatchedLineCount -eq 0 ]
        then
            matchCount=$(($mathcCount + 1))
        fi
    done

    unmatchedCount=$(($testFileCount - $matchCount))

    echo "$roll,C,$matchCount,$unmatchedCount" >> "$2/result.csv"
done
# end run c

# start run java
for folder in "$2/Java/"*
do
    roll=${folder: -7}
    matchCount=0

    for file in "$3/"*
    do
        fileNumber=${file:$start:-4}
        outFileName="$folder/out_$fileNumber.txt"
        answerFileName="$4/ans$fileNumber.txt"

        cat "$file" | java -cp "$folder" "Main" > "$outFileName"

        unmatchedLineCount=$(diff -y --suppress-common-lines "$answerFileName" "$outFileName" | wc -l)

        if [ $unmatchedLineCount -eq 0 ]
        then
            matchCount=$(($mathcCount + 1))
        fi
    done

    unmatchedCount=$(($testFileCount - $matchCount))

    echo "$roll,C,$matchCount,$unmatchedCount" >> "$2/result.csv"
done
# end run java

# start run python
for folder in "$2/Python/"*
do
    roll=${folder: -7}
    matchCount=0

    for file in "$3/"*
    do
        fileNumber=${file:$start:-4}
        outFileName="$folder/out_$fileNumber.txt"
        answerFileName="$4/ans$fileNumber.txt"

        cat "$file" | python3 "$folder/main.py" > "$outFileName"

        unmatchedLineCount=$(diff -y --suppress-common-lines "$answerFileName" "$outFileName" | wc -l)

        if [ $unmatchedLineCount -eq 0 ]
        then
            matchCount=$(($mathcCount + 1))
        fi
    done

    unmatchedCount=$(($testFileCount - $matchCount))

    echo "$roll,C,$matchCount,$unmatchedCount" >> "$2/result.csv"
done
# end run python

rm -rf "temp"