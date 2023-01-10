import sys                                     # for command line arguments
import matplotlib.pyplot as plt                # for ploting
from matplotlib.animation import FuncAnimation # for animation
from matplotlib.patches import Rectangle       # for rudimentary robot drawing
import pandas as pd                            # for reading and storing data
import numpy as np                             # for trig functions

# the command line arguments must be, in this order:
# path_to_csv length

# read csv file from command-line argument into a pandas dataframe
data = pd.read_csv(sys.argv[1])
num_rows = len(data.index)-1

# initialize a figure and axis
fig, (ax1, ax2) = plt.subplots(2,1)

# set axis parameters
ax1.set_xlim(-1.1, 1.1)
ax1.set_ylim(0.0,  2.2)
ax1.set_title(sys.argv[1])

ax2.set_xlim(min(data["time"]), max(data["time"]))
ax2.set_ylim(min(data["angle"]), max(data["angle"]))
ax2.set_xlabel("Time [s]")
ax2.set_ylabel("Angle [rad]")

# initialize objects to be plotted in animation
charge_station, = ax1.plot([], [],
                          lw = 2, color = "black")
length = float(sys.argv[2]) # use command line arg as length of robot
robot = Rectangle((0,0),
                  length, 0.618*length, # golden ratio for height (why not?)
                  facecolor = "red")
ax1.add_patch(robot)
clock = ax1.text(-0.75, 1.75,
                "", fontsize = "large")
gyro = ax1.text(0.25, 1.75,
               "", fontsize = "large")
stream, = ax2.plot([], [],
                color = "red")
target, = ax2.plot([], [],
                  color = "black")

def frame_init():
    # called before each frame is drawn, resets the data in the objects
    charge_station.set_data([], [])

    robot.set_xy((0,0))
    robot.set_angle(0)

    clock.set_text("")
    gyro.set_text("")

    stream.set_data([], [])
    target.set_data([], [])

    return charge_station, robot, clock, gyro, stream, target

def frame_animation(i):
    # called iteratively to draw the animated frames; i is the frame number
    x = [0.0, 0.0,
         -0.965*np.cos(data["angle"][num_rows-i]),
         0.965*np.cos(data["angle"][num_rows-i])]
    y = [0.0, 0.23,
         0.23-0.965*np.sin(data["angle"][num_rows-i]),
         0.23+0.965*np.sin(data["angle"][num_rows-i])]
    charge_station.set_data(x, y)

    robot.set_x(-(0.965 - data["position"][num_rows - i]) *
                np.cos(data["angle"][num_rows - i]))
    robot.set_y(0.23 - (0.965 - data["position"][num_rows - i]) *
                np.sin(data["angle"][num_rows - i]))
    robot.set_angle(np.degrees(data["angle"][num_rows-i]))

    clock.set_text("Time [s]: %.2f" % data["time"][num_rows-i])
    gyro.set_text("Angle [deg]: %.2f" % np.degrees(data["angle"][num_rows-i]))

    stream.set_data(data["time"].tail(i), data["angle"].tail(i))
    target.set_data(data["time"].tail(i), np.zeros(i))

    return charge_station, robot, clock, gyro, stream, target

# animation object
anim = FuncAnimation(fig, frame_animation, init_func=frame_init,
                     frames=1000, interval = 10, blit = True)

# comment out to avoid seeing the animation in real time
plt.show()
# uncomment to save the sanimation
#anim.save((sys.argv[1])[:-3] + "mp4")
