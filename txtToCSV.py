# create script to convert txt files to csv files, example of the txt file structure:
# 13:59:35:170 -> Temperature: 50.04 degrees C
# 13:59:35:173 -> Humidity: 17.91% rH
# 13:59:36:222 -> Temperature: 50.03 degrees C
# 13:59:36:225 -> Humidity: 17.89% rH
# 13:59:37:274 -> Temperature: 50.05 degrees C
# 13:59:37:277 -> Humidity: 17.91% rH
# 13:59:38:326 -> Temperature: 50.08 degrees C
# 13:59:38:329 -> Humidity: 17.84% rH
# 13:59:39:379 -> Temperature: 50.15 degrees C
# create CSV file with 3 columns: Time, Temperature, Humidity

import csv
import re


# Open the text file and the CSV file
with open(
    "/Users/michael/Documents/PlatformIO/Projects/FillamentDryer/logs/devttyusbserial02VB2BFD_2023_12_08.13.43.04.162.txt",
    "r",
) as txt_file, open(
    "/Users/michael/Documents/PlatformIO/Projects/FillamentDryer/logs/devttyusbserial02VB2BFD_2023_12_08.13.43.04.162.csv",
    "w",
    newline="",
) as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(["Time", "Temperature", "Humidity"])  # Write the header

    temp = None
    for line in txt_file:
        try:
            if "Temperature" in line:
                time, _, temp = line.partition(" -> ")
                temp = re.findall(r"\d+\.\d+", temp)[0]
            elif "Humidity" in line:
                _, _, humidity = line.partition(" -> ")
                humidity = re.findall(r"\d+\.\d+", humidity)[0]
            if type(float(temp)) == float and type(float(humidity)) == float:
                writer.writerow([time, temp, humidity])  # Write the data
        except:
            pass
