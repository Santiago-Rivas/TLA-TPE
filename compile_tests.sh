#!/bin/bash

./script/build.sh
# Create directories if they don't exist

rm -r test_pdf
rm -r test_tex

mkdir -p test_pdf
mkdir -p test_tex
# Iterate over all files in test/accept/
for file in test/accept/*; do
    # Check if it's a file
    if [ -f "$file" ]; then
        # Extract the file name without extension
        filename=$(basename -- "$file")
        filename_noext="${filename%.*}"

        # Run the provided command for each file
        ./script/start.sh "$file"
        mv output.tex test_tex/"$filename_noext".tex

        pdflatex "test_tex/$filename_noext.tex"

        # Move the generated PDF to test_pdf directory
        mv "${filename_noext}.pdf" "test_pdf/"

        # Clean up temporary files
        rm -f "${filename_noext}.aux" "${filename_noext}.log" "${filename_noext}.tex"
    fi
done

