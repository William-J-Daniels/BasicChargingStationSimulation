#ifndef WILL_PIDCONTROLLER_H
#define WILL_PIDCONTROLLER_H

#include <iostream>
#include <cstdlib>
#include <limits>

namespace will
{

class PIDController
{ // somewhat imitates the class by the same name from WPIlib
public:
    // can construct defaultly, or by setting the gains and period
    PIDController() = default;
    PIDController(
        double init_product, double init_integral, double init_derivative,
        double init_period
    );

    // returns next output given current state
    double calculate(double new_measurement);
    // clears history
    void reset();

    // mutators
    void set_p(double new_product);
    void set_i(double new_integral);
    void set_d(double new_derivative);
    void set_pid(
        double new_product, double new_integral, double new_derivative
    );
    void set_period(double new_period);
    void set_setpoint(double new_setpoit);
    void set_relief(double new_relief);
    void set_integral_limits(double new_upper, double new_lower);

    // accessors
    // will write later

private:
    // gain weights for each type of correction, and the period
    double product_gain = 0, integral_gain = 0, derivative_gain = 0, // unitless
           period = 0.01;                                            // seconds
    // set point variables and relief
    double setpoint = 0.0, relief = 0.05, measurement = 0.0;
    // error vars-- error at this step, error at last step, error of all steps
    double error = 0.0, last_error = 0.0, cumulative_error = 0.0;
    // to help comba integrator windup, the cumulative_error can be capped at
    // some value. Of course if you wish to use them you must set them. I have
    // not set them to a limiting defualt because I have no knowledge of units
    double max_integral =  std::numeric_limits<double>::infinity(),
           min_integral = -std::numeric_limits<double>::infinity();
};

} // namespace will

#endif // WILL_PIDCONTROLLER_H
