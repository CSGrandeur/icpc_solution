#!/bin/bash
TEMP_FILE=./build/acm_algorithm_csgrandeur.md
TARGET_FILE=./build/acm_algorithm_csgrandeur_$(date "+%Y%m%d%H%M%S").pdf
mkdir -p ./build
echo "" > $TEMP_FILE
for file in ./*
do
    if [[ $file =~ \.md$ ]];
    then
        cat $file >> $TEMP_FILE
        echo "" >> $TEMP_FILE
        echo "" >> $TEMP_FILE
        echo "\newpage" >> $TEMP_FILE
        echo "" >> $TEMP_FILE
    fi
done