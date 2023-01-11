#include <pidcontroller.h>

using namespace will;

PIDController::PIDController(
    double init_product, double init_integral, double init_derivative,
    double init_period
)
{
    /*
     * Constructor which initializes each gain weight and the period. Gain
     * weights are positive semidefinte and period is positive.
     *
     * Uses iostream, cstdlib
     */

    set_pid(init_product, init_integral, init_derivative);
    set_period(init_period);
}

double PIDController::calculate(double new_measurement)
{
    /*
     * Given the current state of the system, returns the next output of the
     * PID controll.
     */

    last_error = error; // before updating the error, update the last error
    error = new_measurement - setpoint; // update the error
    cumulative_error = cumulative_error + error * period; // simple reimann sum
    if (cumulative_error > max_integral)
    {
        cumulative_error = max_integral;
    }
    if (cumulative_error < min_integral)
    {
        cumulative_error = min_integral;
    }

    return(
        product_gain * ( // proportional term
            error
        ) +
        integral_gain * ( // integral term
            cumulative_error
        ) +
        derivative_gain * ( // derivative term
            (error - last_error) / period
        )
    );
}

void PIDController::reset()
{
    /*
     * Resets the hisotrical elements (cumulative_error, last_error). Useful for
     * managing loops involving integral terms.
     */

    cumulative_error = 0.0;
    last_error       = 0.0;
}

void PIDController::set_p(double new_product)
{
    /*
     * Mutator for the product weight. Must be positive semidefinite.
     *
     * Uses iostream, cstdlib
     */

    if (new_product < 0.0)
    {
        std::cerr << "The weight for the product correction must be positive "
                  << "semidefinte. You provided " << new_product << ". Exiting "
                  << "program." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    product_gain = new_product;
}

void PIDController::set_i(double new_integral)
{
    /*
     * Mutator for the integral weight. Must be postive semidefinite.
     *
     * Uses iostream, cstdlib
     */

    if (new_integral < 0.0)
    {
        std::cerr << "The weight for the integral correction must be positive "
                  << "semidefinte. You provided " << new_integral
                  << ". Exiting program." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    integral_gain = new_integral;
}

void PIDController::set_d(double new_derivative)
{
    /*
     * Mutator for the derivative weight, Must be positive semidefinite.
     *
     * Uses iostream, cstdlib
     */

    if (new_derivative < 0.0)
    {
        std::cerr << "The weight for the derivative correction must be "
                  << "positive semidefinte. You provided " << new_derivative
                  << ". Exiting program." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    derivative_gain = new_derivative;
}

void PIDController::set_pid(
    double new_product, double new_integral, double new_derivative
)
{
    /*
     * Mutator for each weight. Must be positive semidefinite.
     *
     * Uses iostream, cstdlib
     */

    set_p(new_product);
    set_i(new_integral);
    set_d(new_derivative);
}

void PIDController::set_period(double new_period)
{
    /*
     * Mutator for the period. Must be positive.
     */

    if (new_period <= 0.0)
    {
        std::cerr << "The period must be positive. You provided" << new_period
                  << ". Exiting program." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    period = new_period;
}

void PIDController::set_setpoint(double new_setpoint)
{
    /*
     * Trivial mutator for the setpoint.
     */

    setpoint = new_setpoint;
}

void PIDController::set_relief(double new_relief)
{
    /*
     * Mutator for the distance the measurement can be from the setpoint and
     * still be considered at the setpoint. Must be positive.
     */

    if (new_relief <= 0.0)
    {
        std::cerr << "The relief must be positive. You provided " << new_relief
                  << ". Exiting program." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    relief = new_relief;
}

void PIDController::set_integral_limits(double new_upper, double new_lower)
{
    /*
     * Mutator fo rthe upper and lower bounds of the error integral. This exists
     * to help mitigate the effects of integrator windup. Checks that the upper
     * limit is greater than the lower limit.
     *
     * Uses iostream, cstdlib
     */

    if (new_upper <= new_lower)
    {
        std::cerr << "The upper limit must be greater than the lower limit. "
                  << "You provided " << new_upper << " for the upper limit and "
                  << new_lower << " for the lower limit. Exiting program."
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }
    max_integral = new_upper;
    min_integral = new_lower;
}
