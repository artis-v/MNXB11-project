#!/bin/bash

# Usage function for the script
usage() {
    echo "Usage: $0 <datafile1> <datafile2> ..."
    exit 1
}

# Logging function for consistent log messages
log() {
    local LOG_MSG=$1
    local LOG_TIMESTAMP=$(date -Iseconds)
    echo "[${LOG_TIMESTAMP}]: $LOG_MSG"
}

# Function to clean data in each file
clean_data() {
    local INPUT_FILE=$1
    local CLEANED_FILE="Cleaned_${INPUT_FILE}"
    log "Cleaning data for file: $INPUT_FILE"
    
    # Identify the line number where data starts
    local STARTLINE=$(grep -n 'Datum' "$INPUT_FILE" | cut -d':' -f 1)
    if [[ -z "$STARTLINE" ]]; then
        log "No 'Datum' line found in $INPUT_FILE. Skipping."
        return 1
    fi

    # Remove header line
    STARTLINE=$((STARTLINE + 1))

    # Extract data starting from the line after 'Datum' and save to cleaned file
    # To include "Kvalitet", if needed, change the "cut" part to inlcude 1,2,3,4
    tail -n +$STARTLINE "$INPUT_FILE" | cut -d';' -f 1,2,3 | sed 's/;/,/g' > "$CLEANED_FILE"
    
    # Check if data was successfully cleaned
    if [[ $? -ne 0 ]]; then
        log "Error cleaning data for file $INPUT_FILE"
        return 1
    fi
    
    log "Data cleaned successfully. Output: $CLEANED_FILE"
    echo "$CLEANED_FILE"
}

# Ensure at least one data file is provided
if [[ $# -eq 0 ]]; then
    log "No data files provided."
    usage
fi

# Process each file passed as an argument
for DATAFILE in "$@"; do
    # Check if file exists
    if [[ ! -f "$DATAFILE" ]]; then
        log "File not found: $DATAFILE"
        exit 1
    fi

    log "Processing file: $DATAFILE"
    
    # Step 1: Clean the data
    CLEANED_FILE=$(clean_data "$DATAFILE")

    # Check if cleaning was successful
    if [[ $? -eq 0 ]]; then
        log "File $CLEANED_FILE ready for further analysis."
    else
        log "Skipping file $DATAFILE due to errors."
    fi
done

log "All files processed successfully."

exit 0

