# create script that listens on txt file and graphs the data.
# The data is in the format:
# 13:59:35:170 -> Temperature: 50.04 degrees C
# 13:59:35:173 -> Humidity: 17.91% rH
# 13:59:36:222 -> Temperature: 50.03 degrees C
# 13:59:36:225 -> Humidity: 17.89% rH
# 13:59:37:274 -> Temperature: 50.05 degrees C
# 13:59:37:277 -> Humidity: 17.91% rH
# 13:59:38:326 -> Temperature: 50.08 degrees C
# 13:59:38:329 -> Humidity: 17.84% rH
# 13:59:39:379 -> Temperature: 50.15 degrees C
# create graph for Time (x axis), Temperature (y axis), Humidity (y axis)
# the graph should update every 5 seconds

import matplotlib.pyplot as plt
import pandas as pd
import time
import re


def parse_file():
    data = {"Time": [], "Temperature": [], "Humidity": []}
    with open(
        "/Users/michael/Documents/PlatformIO/Projects/FillamentDryer/logs/devttyusbserial02VB2BFD_2023_12_08.13.43.04.162.txt",
        "r",
    ) as file:
        temp = None
        humidity = None
        for line in file:
            try:
                if "Temperature" in line:
                    time, _, temp = line.partition(" -> ")
                    temp = re.findall(r"\d+\.\d+", temp)[0]
                elif "Humidity" in line:
                    _, _, humidity = line.partition(" -> ")
                    humidity = re.findall(r"\d+\.\d+", humidity)[0]
                    if type(float(temp)) == float and type(float(humidity)) == float:
                        data["Time"].append(time[0:8])
                        data["Temperature"].append(float(temp))
                        data["Humidity"].append(float(humidity))
                        temp = None
                        humidity = None
            except:
                pass
    return pd.DataFrame(data)


def update_parse_file():
    data = {"Time": [], "Temperature": [], "Humidity": []}
    with open(
        "/Users/michael/Documents/PlatformIO/Projects/FillamentDryer/logs/devttyusbserial02VB2BFD_2023_12_08.13.43.04.162.txt",
        "r",
    ) as file:
        lines = file.readlines()
        last_lines = lines[-10:]  # Get the last 10 lines
        temp = None
        humidity = None
        for line in last_lines:
            try:
                if "Temperature" in line:
                    time, _, temp = line.partition(" -> ")
                    temp = re.findall(r"\d+\.\d+", temp)[0]
                elif "Humidity" in line:
                    _, _, humidity = line.partition(" -> ")
                    humidity = re.findall(r"\d+\.\d+", humidity)[0]
                    if type(float(temp)) == float and type(float(humidity)) == float:
                        data["Time"].append(time[0:8])
                        data["Temperature"].append(float(temp))
                        data["Humidity"].append(float(humidity))
                        temp = None
                        humidity = None
            except:
                pass
    return pd.DataFrame(data)


# df = parse_file()
# print(df.tail())

# plt.ion()  # Turn on interactive mode
# while True:
#     print("Updating graph...")
#     df = df.merge(update_parse_file(), how="outer")

#     print(df.tail())
#     plt.clf()  # Clear the current figure

#     # Create a 3-row subplot for the three graphs
#     plt.subplot(3, 1, 1)  # The first plot
#     plt.plot(df["Time"], df["Temperature"], label="Temperature")
#     plt.plot(df["Time"], df["Humidity"], label="Humidity")
#     plt.legend()  # Add a legend

#     dt = df.tail(120)

#     plt.subplot(3, 1, 2)  # The second plot
#     plt.plot(dt["Time"], dt["Temperature"], label="Temperature", color="r")
#     plt.legend()  # Add a legend

#     plt.subplot(3, 1, 3)  # The third plot
#     plt.plot(dt["Time"], dt["Humidity"], label="Humidity", color="b")
#     plt.legend()  # Add a legend

#     plt.tight_layout()  # Adjust the layout
#     plt.draw()  # Redraw the figure
#     plt.pause(5)  # Pause for 5 seconds
df = parse_file()

plt.ion()  # Turn on interactive mode

# Create a 3-row subplot for the three graphs
fig, axs = plt.subplots(3, 1)
line1: plt.Line2D = None
line2: plt.Line2D = None
line3: plt.Line2D = None
line4: plt.Line2D = None
# Initial plots
(line1,) = axs[0].plot(df["Time"], df["Temperature"], label="Temperature")
(line2,) = axs[0].plot(df["Time"], df["Humidity"], label="Humidity")
(line3,) = axs[1].plot(df["Time"], df["Temperature"], label="Temperature", color="r")
(line4,) = axs[2].plot(df["Time"], df["Humidity"], label="Humidity", color="b")

# Add legends
axs[0].legend()
axs[1].legend()
axs[2].legend()

plt.tight_layout()  # Adjust the layout
t0 = time.time()
while True:
    print("Updating graph...")
    print(df.tail())
    df = df.merge(update_parse_file(), how="outer")

    if time.time() - t0 > 60:
        # Update the data of the plots
        line1.set_ydata(df["Temperature"])
        line1.set_xdata(df["Time"])
        line2.set_ydata(df["Humidity"])
        line2.set_xdata(df["Time"])
        t0 = time.time()
    ds = df.tail(120)  # Keep only the last 120 rows

    line3.set_ydata(ds["Temperature"])
    line3.set_xdata(ds["Time"])
    line4.set_ydata(ds["Humidity"])
    line4.set_xdata(ds["Time"])

    # Rescale the axes
    for ax in axs:
        ax.relim()
        ax.autoscale_view()
        ax.tick_params(axis="x", rotation=90)  # Rotate x-axis labels
    fig.canvas.draw()  # Redraw the figure
    plt.pause(5)  # Pause for 5 seconds
