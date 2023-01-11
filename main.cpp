#include <iostream>
#include <robot.h>
#include <pidcontroller.h>

int main() {
    /*
     * In this function, we simulate the robot we defined in robot.h and
     * robot.cpp.
     */

    // simulation parameters
    int current_frame       = 0;    // used to reduce floating point error
    double current_time     = 0.0;  // seconds
    const double TIME_STEP  = 0.01; // seconds
    const double SIM_LENGTH = 60.0; // seconds

    will::Robot MyBot = will::Robot( // make a robot that...
        45.0, // weights 45.0 kg,
        0.5,  // is 0.5 meters long,
        9     // and has 3 wheels
    );
    MyBot.set_max_acceleration(15.0);

    will::PIDController MyController = will::PIDController ( // make a PID with
        0.05,      // product weight 1.0,
        0.005,      // integral weight 1.0,
        0.125,    // derivative weight 1.0,
        TIME_STEP // and a period equal to the simulation time step (it must be)
    );
    MyController.set_integral_limits(0.25, -0.25);

    while (current_time <= SIM_LENGTH) //until we simulate SIM_LENGTH seconds...
    {
        // have the PID controller set a power for the robot
        MyBot.set_power(MyController.calculate(MyBot.get_angle()));

        // clear the historical variables of MyController every second
        if (current_frame % 100 == 0)
        {
            MyController.reset();
        }

        // update the simulation of the robot
        MyBot.advance_time(TIME_STEP);
        MyBot.save_frame(current_time);

        // update current_frame and current_time
        current_frame = current_frame + 1;
        current_time  = TIME_STEP * current_frame;
    }

    MyBot.export_results("../data/test.csv"); // export finished sim to file
    // csv file can be studied with visualization code-- the animate.py script
    // in tools/

    return 0;
}
