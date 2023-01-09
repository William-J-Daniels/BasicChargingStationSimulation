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
fig, ax = plt.subplots()

# set axis parameters
ax.set_xlim(-1.1, 1.1)
ax.set_ylim(0.0,  2.2)
ax.set_title(sys.argv[1])

# initialize objects to be plotted in animation
charge_station, = ax.plot([], [],
                          lw = 2, color = "black", aa = True)
length = float(sys.argv[2]) # use command line arg as length of robot
robot = Rectangle((0,0),
                  length, 0.618*length, # golden ratio for height (why not?)
                  facecolor = "red")
ax.add_patch(robot)
clock = ax.text(-0.75, 1.75,
                "", fontsize = "large")
gyro = ax.text(0.25, 1.75,
               "", fontsize = "large")

def frame_init():
    # called before each frame is drawn, resets the data in the objects
    charge_station.set_data([], [])

    robot.set_xy((0,0))
    robot.set_angle(0)

    clock.set_text("")
    gyro.set_text("")
    return charge_station, robot, clock, gyro

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
    return charge_station, robot, clock, gyro

# animation object
anim = FuncAnimation(fig, frame_animation, init_func=frame_init,
                     frames=1000, interval = 10, blit = True)

# comment out to avoid seeing the animation in real time
# plt.show()
# uncomment to save the sanimation
anim.save((sys.argv[1])[:-3] + "mp4")
