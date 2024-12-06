#include "gui.h"

int pi_num;

void pigpiod_init()
{
    pi_num = pigpio_start(NULL, NULL);
    if (pi_num < 0)
    {
        printf("GPIO initialization failed\n");
        pid_t pid = getpid();
        kill(pid, SIGINT);
    }

    /**
     * Start Signal
     * From: GUI Pi
     * To: LinuxCNC Pi
     * Triggers the start of the LinuxCNC program with
     * a pulse High.
     */
    if(set_mode(pi_num, GPIO_START_OUT, PI_OUTPUT) == 0) {
        set_pull_up_down(pi_num, GPIO_START_OUT, PI_PUD_DOWN);
        gpio_write(pi_num, GPIO_START_OUT, PI_LOW);
    } else {
        printf("Start signal not initialized\n");
    }
    /**
     * Running Signal
     * From: LinuxCNC Pi
     * To: GUI Pi
     * High when the LinuxCNC program is running, low
     * when the program is finished running. Used to
     * tell the GUI Pi when to display a loading screen.
     */
    if(set_mode(pi_num, GPIO_RUN_IN, PI_INPUT) == 0) {
        set_pull_up_down(pi_num, GPIO_RUN_IN, PI_PUD_DOWN);
        callback(pi_num, GPIO_RUN_IN, FALLING_EDGE, program_stopped_cb);
    } else {
        printf("Running signal not initialized\n");
    }
    /**
     * E-Stop
     * From: GUI Pi
     * To: LinuxCNC Pi
     * This will cause the LinuxCNC program to stop immediately
     * using the built-in E-Stop functionality.
     */
    if(set_mode(pi_num, GPIO_E_STOP, PI_OUTPUT) == 0) {
        set_pull_up_down(pi_num, GPIO_E_STOP, PI_PUD_DOWN);
        gpio_write(pi_num, GPIO_E_STOP, PI_LOW);
    } else {
        printf("E-Stop signal not initialized\n");
    }
    /**
     * Homing Signal
     * From: GUI Pi
     * To: LinuxCNC Pi
     * This will signal to the Python script that the arm
     * is being manually homed.
     */
    if(set_mode(pi_num, GPIO_HOMING, PI_OUTPUT) == 0) {
        set_pull_up_down(pi_num, GPIO_HOMING, PI_PUD_DOWN);
        gpio_write(pi_num, GPIO_HOMING, PI_LOW);
    } else {
        printf("Homing signal not initialized\n");
    }
}

void config_screens()
{
    config_start_screen();
    config_info_screen();
    config_demo_screen();
    config_demo_pen_screen();
    config_demo_laser_screen();
    config_demo_pen_popup();
    config_demo_laser_popup();
    config_new_proj_screen();
    config_file_confirm_screen();
    config_program_running();
    config_program_done();
    config_homing_screen();
    config_writeup_screens();
}

void setup()
{
    pigpiod_init();

    style_init();

    input_init();

    config_screens();

    open_start_screen();
}