#include <robot.h>

using namespace will;

Robot::Robot(double init_mass, double init_length, int init_num_wheels)
{
    /*
     * Constructor which sets the mass in pounds, length in feet, and number of
     * wheels.
     */

    mass = init_mass;
    length = init_length;
    num_wheels = init_num_wheels;
}

void Robot::set_max_acceleration(double new_acceleraton)
{
    /*
     * Trivial mutator for max_acceleration.
     */

    max_acceleration = new_acceleraton;
}

void::Robot::set_max_velocity(double new_velocity)
{
    /*
     * Trivial mutator for max_velocity.
     */

    max_velocty = new_velocity;
}

double Robot::get_angle()
{
    /*
     * Trivial accessor for angle.
     */

    return angle;
}

void Robot::set_power(double new_power)
{
    /*
     * Sets a simulated motor controller power. Range of [-1, 1] is enforced.
     *
     * Uses cstdlib, iostream
     */

    if (power >= -1.0 && power <= 1.0)
    {
        power = new_power;
    }
    else
    {
        std::cerr << "Power must be between -1.0 and 1.0. Exiting program."
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Robot::advance_time(double time_step)
{
    /*
     * Applies Euler's method to simulate the motion due to computed forces over
     * time_step seconds.
     *
     * Uses cmath
     */

    // first we'll advance the angular quantities
    update_tourque();
    // now we can use the angular kinematic equations to update the angular
    // velocity and angle.
    angular_velocity = angular_velocity + tourque * time_step;
    angle = angle +
          angular_velocity * time_step + 0.5 * tourque * std::pow(time_step, 2);
    // if the angle is at its resting position, the angular velocity must be
    // zero. let's enforece that
    // the charging station motion is bounded to not exceed an angle of
    // 0.234 rad. Let's make sure we don't exceed those bounds.
    if (std::abs(angle) > 0.234)
    {
        angular_velocity = 0.0;
        if (angle > 0) { angle =  0.234; }
        if (angle < 0) { angle = -0.234; }
    }

    // now we update the linear quantities
    update_acceleration();
    // now we can use the kinematic equations to update velocity and position
    velocity = velocity + acceleration * time_step;
    position = position +
             velocity * time_step + 0.5 * acceleration * std::pow(time_step, 2);
}

void Robot::save_frame(double time) {
    /*
     * Adds to the forwardly linked list of std::array<double, 3> the partial
     * state [time, position, angle] when called.
     */

    results.push_front(std::array<double, 3> {time, position, angle});
}

void Robot::export_results(std::string Filename) {
    /*
     * Writes the contents of the forwardly linked list results to a file
     * provided as the argument Filename. File is formated as a csv file with
     * header. Note that Filename is a path with respect to the executable.
     *
     * Uses string, fstream, cstdlib
     */

    std::ofstream Outfile(Filename);
    if (Outfile.fail())
    { // should we fail to open the file for any reason
        // give and error, exit program
        std::cerr << "Failed to open " << Filename << ". Check that you have "
                  << "the necessary permisions and that the path exists."
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }
    // first write a header to the file
    Outfile << "time,position,angle" << std::endl;
    for (std::forward_list<std::array<double, 3>>::iterator p = results.begin();
         p != results.end(); p++)
    { // iterating over the entire list
        Outfile << (*p)[0] << "," << (*p)[1] << "," << (*p)[2] << std::endl;
    }
    Outfile.close(); // close the file when we're done
}

namespace will
{
    std::ostream& operator<< (std::ostream& out, const Robot& Bot)
    {
        /*
         * Overload of << for nice debugging outputs.
         *
         * Uses iostream
         */

        out << "Power:            " << Bot.power            << std::endl
            << "Angle:            " << Bot.angle            << std::endl
            << "Angular velocity: " << Bot.angular_velocity << std::endl
            << "Tourque:          " << Bot.tourque          << std::endl
            << "Position:         " << Bot.position         << std::endl
            << "Velocity:         " << Bot.velocity         << std::endl
            << "Acceleration:     " << Bot.acceleration     << std::endl;
        return out;
    }
}


void Robot::update_tourque()
{
    /*
     * Computes the tourque that the robot exerts on the charge station assuming
     * a uniform weight distribution on the robot. The gravitation constant is
     * taken to be 9.81 meters per second squared. The other component of the
     * force in not computed beacuse the robot is taken to have infinite
     * traction.
     *
     * Uses cmath
     */

    // There will be a contribution to the tourque from each wheel. We start
    // with the contribution from the leftmost wheel because it does not fit
    // into the for loop we will write next
    tourque = charging_station_length/2 - position;

    // now we do the rest of the wheels, starting fromt the second leftmost
    for (int i = num_wheels; i > 0; i--)
    { // for each other wheel...
        tourque = tourque + (charging_station_length/2 - position - length/i);
        // add the contribution from that wheel
    }

    // as a last step, we must multiply tourque by the force that a wheel
    // exerts on the charging station. This isn't dont until now so that we only
    // have to multiply once instead of num_wheels times, making the code faster
    tourque = tourque * (mass * 9.81 * std::cos(angle) / num_wheels);
}

void Robot::update_acceleration() {
    /*
     * Computes a simulated acceleration given a power setting between negative
     * one and one. Simple linear relationship between power and velocity is
     * used.
     */

    if (velocity < power*max_velocty - eppsilon)
    { // if we're slower than we want
        acceleration =  max_acceleration; // accelerate positively
    }
    else if (velocity > power*max_velocty + eppsilon)
    { // if we're faster than we want
        acceleration = -max_acceleration; // accelerate negatively
    }
    else
    { // if we're at the speed we want
        acceleration = 0.0; // dont accelerate at all
    }
}
