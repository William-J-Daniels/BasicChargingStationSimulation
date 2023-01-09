#ifndef WILL_ROBOT_H
#define WILL_ROBOT_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <forward_list>
#include <array>
#include <string>
#include <fstream>

namespace will
{

class Robot
{
    /*
     * This class describes the "robot" we will use to tune a PID loop for
     * charge station balancing.
     *
     * List of assumptions:
     *  - 2D robot
     *  - Uniform weight distribution
     *  - Infinite traction
     *  - Infinite jerk
     */

public:
    // no default constructor, must initialize mass, length, and num_wheels
    Robot() = delete;
    Robot(double init_mass, double init_length, int init_num_wheels);

    // allows configuration of robot capabilities
    void set_max_acceleration(double new_acceleraton);
    void set_max_velocity(double new_velocity);

    // accessors for data the PID loop knows
    double get_angle();

    // set the robot power between negative one and one, like WPIlib
    void set_power(double new_power);

    // the iterative simulation driver
    void advance_time(double time_step);

    // methods for getting simulation results
    void save_frame(double time); // adds current frame to linked list
    void export_results(std::string Filename); // saves results to a file

    // overload of << for nice debugging outputs
    friend std::ostream& operator<< (std::ostream& out, const Robot& Bot);

private:
    // SI base units assumed throughout, except angles which are in radians

    // robot and charging station parameters
    double mass, length;
    int num_wheels;
    double charging_station_length = 1.93; // as per game manual

    // limits on robot capabilities
    double max_acceleration = 3.0,  // applies to breaking and forward motion
           max_velocty      = 10.0,
           eppsilon         = 0.05; // acceptable relief to avoid jerking back
                                    // and forth

    std::forward_list<std::array<double, 3>> results; // linked list to store
    // results in before writing to disk. [time, position, angle]

    // variables describing the robots motion
    double power            = 0.0,   // simulates motor controller percentage
           angle            = 0.234, // angle left of charging station makes
           angular_velocity = 0.0,   // with respect to the floor
           tourque          = 0.0,
           position         = 0.0,   // measured from the left of the charging
           velocity         = 0.0,   // station to the leftmost wheel
           acceleration     = 0.0;

    // methods for updating accelerative quantities
    void update_tourque();
    void update_acceleration();
};

} // namespace will

#endif // WILL_ROBOT_H
