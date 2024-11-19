#include "gui.h"

void pigpio_init()
{
    pi_num = pigpio_start(NULL, NULL);
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
        // callback(pi_num, GPIO_RUN_IN, RISING_EDGE, program_running_cb);
        // callback(pi_num, GPIO_RUN_IN, FALLING_EDGE, program_stopped_cb);
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
}

void config_screens()
{
    config_start_screen();
    config_info_screen();
    config_demo_screen();
    config_demo_popup();
    config_new_proj_screen();
    config_usb_explorer();
    config_cloud_explorer();
    config_program_running();
    config_program_done();
    config_writeup_screens();
}

void setup()
{
    pigpio_init();

    style_init();

    input_init();

    config_screens();

    open_start_screen();
}