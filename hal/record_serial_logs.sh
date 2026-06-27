#!/bin/bash

serial_input_file="/dev/ttyACM0"
output_file="serial_log.txt"

echo "" > "$output_file"

# Wait until the file exists
while [ ! -e "$serial_input_file" ]; do
    :
done

# Configure the serial port
stty -F "$serial_input_file" raw -echo

# Continuously read data from the file
while true; do
    if read -r line < "$serial_input_file"; then
        echo "$line" >> "$output_file"
    fi
done
