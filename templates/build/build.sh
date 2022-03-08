#!/bin/bash
BASEDIR=$(dirname "$0")
TEMP_FILE=$BASEDIR/output/acm_algorithm_csgrandeur.md
TARGET_FILE=$BASEDIR/output/acm_algorithm_csgrandeur_$(date "+%Y%m%d%H%M%S").pdf
mkdir -p $BASEDIR/output
echo "" > $TEMP_FILE
for file in $(dirname $BASEDIR)/*
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


if [[ `uname -s` == "Darwin" ]]; then
    engine_prefix='--latex-engine'
    font='Hiragino Sans GB'
elif [[ `uname -s` == "Linux" ]]; then
    engine_prefix='--pdf-engine'
    # font='Microsoft YaHei'
    font='Droid Sans Fallback'
else
    engine_prefix='--pdf-engine'
    font='Source Han Sans CN'
fi

pandoc $TEMP_FILE \
$engine_prefix=xelatex \
-V CJKmainfont="$font" \
--default=meta.yaml \
-o $TARGET_FILE

