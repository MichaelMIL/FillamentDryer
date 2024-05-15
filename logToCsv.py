# create csv file from log file
# example of the log file structure:
# 18:26:24:683 -> 39.04,16.19,37.65,17.84,38.30,31.60
# 18:26:25:683 -> 39.05,16.35,37.67,17.85,38.30,31.60
# 18:26:26:683 -> 39.03,16.35,37.67,17.85,38.30,31.60
# 18:26:27:683 -> 39.01,16.30,37.66,17.84,38.30,31.60
# 18:26:28:683 -> 39.02,16.29,37.66,17.85,38.30,31.60
# 18:26:29:683 -> 39.03,16.40,37.66,17.86,38.30,31.60
# 18:26:30:683 -> 39.04,16.19,37.67,17.91,38.40,31.60
# 18:26:31:683 -> 39.06,16.47,37.69,17.87,38.40,31.60
#  create CSV file with 7 columns: Time, Heater Temperature, Heater Humidity, Cooler Temperature, Cooler Humidity, Ambient Temperature, Ambient Humidity

import csv
import datetime


def logToCsv(logFile, csvFile):
    with open(logFile, "r") as log:
        with open(csvFile, "w") as csvFile:
            csvWriter = csv.writer(csvFile)
            csvWriter.writerow(
                [
                    "Time",
                    "Heater Temperature",
                    "Heater Humidity",
                    "Cooler Temperature",
                    "Cooler Humidity",
                    "Ambient Temperature",
                    "Ambient Humidity",
                ]
            )
            for line in log:
                line = line.strip()
                try:
                    if line:
                        line = line.split(" -> ")
                        time = line[0]
                        time = time.split(":")
                        time = datetime.time(
                            int(time[0]), int(time[1]), int(time[2]), int(time[3])
                        )
                        data = line[1]
                        data = data.split(",")
                        csvWriter.writerow(
                            [time, data[0], data[1], data[2], data[3], data[4], data[5]]
                        )
                except:
                    print("Error in line")
                    continue


if __name__ == "__main__":
    logToCsv(
        "/Users/michael/Desktop/logs/devttyusbmodem1201_2023_12_20.18.15.39.914.txt",
        "/Users/michael/Desktop/logs/devttyusbmodem1201_2023_12_20.18.15.39.914.csv",
    )
