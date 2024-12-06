#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pigpiod_if2.h>
#include <pigpio.h>

#include "writeup.h"
#include "lv_port_linux/lvgl/lvgl.h"
#include "images/cloud_symbol.c"
#include "images/usb_symbol.c"

/***********
 * Defines *
 ***********/

#define COLOR_PALETTE LV_PALETTE_LIGHT_BLUE
#define BACK_BTN_HORZ 100
#define BACK_BTN_VERT 75

/********************************
 * PIGPIO Defines and Variables *
 ********************************/

/* GPIO4 -> Pin 7 */
#define GPIO_START_OUT 4
/* GPIO27 -> Pin 13 */
#define GPIO_RUN_IN 27
/* GPIO22 -> Pin 15 */
#define GPIO_E_STOP 22
/* GPIO23 -> Pin 18 */
#define GPIO_HOMING 23

int pi_num = 0;

LV_IMAGE_DECLARE(cloud_symbol);
LV_IMAGE_DECLARE(usb_symbol);

char *file_path;
char *file_name;
char file_path_and_name[128];

char demo_file_name[64];
char demo_file_path_and_name[128];

lv_display_t *display;

/**
 * Start Screen
 *
 * This is going to be the screen that is open on startup. The user
 * will be presented with the project name: PortaGuide, as well as a
 * few buttons below to choose different functions.
 *
 * Functions:
 * -> Demos: This will open another window to display a few pre-built
 *           demonstrations for demo day
 * -> New Project: This will open another window to start the process
 *                 of loading in a new file to create a project from
 *                 scratch
 * -> Info: This will open a window with helpful tips, a description
 *          of the project, and other useful information
 */

/* Screen */
static lv_obj_t *start_screen;
static lv_style_t style_start_screen;
/* Main Label */
static lv_obj_t *main_label_start_screen;
static lv_style_t style_main_label_start_screen;
/* Info Button */
static lv_obj_t *info_btn_start_screen;
static lv_obj_t *info_label_start_screen;
/* New Project Button */
static lv_obj_t *new_proj_btn_start_screen;
static lv_obj_t *new_proj_label_start_screen;
/* Demo Button */
static lv_obj_t *demo_btn_start_screen;
static lv_obj_t *demo_label_start_screen;
/* Quit Button */
static lv_obj_t *quit_btn_start_screen;
static lv_obj_t *quit_label_start_screen;
/* Styles */
static lv_style_t style_btn_start_screen;
static lv_style_t style_label_start_screen;
/* Layout */
static int32_t col_start_screen[] = {LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_start_screen[] = {LV_GRID_FR(1), LV_GRID_FR(12), LV_GRID_FR(6), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

/**
 * Info Screen
 *
 * This is where we can put information about the project, tips for
 * using the project, and maybe even a step-by-step process of how
 * use it.
 */

/* Screen */
static lv_obj_t *info_screen;
static lv_style_t style_info_screen;
/* Main Text */
static lv_obj_t *main_label_info_screen;
static lv_style_t style_main_label_info_screen;
/* Subtext */
static lv_obj_t *sub_label_info_screen;
static lv_style_t style_sub_label_info_screen;
/* Info Button */
static lv_obj_t *info_btn_info_screen;
static lv_obj_t *info_label_info_screen;
/* Help Button */
static lv_obj_t *help_btn_info_screen;
static lv_obj_t *help_label_info_screen;
/* Tutorial Button */
static lv_obj_t *tutorial_btn_info_screen;
static lv_obj_t *tutorial_label_info_screen;
/* Back Button */
static lv_obj_t *back_btn_info_screen;
static lv_obj_t *back_label_info_screen;
/* Styles */
static lv_style_t style_btn_info_screen;
static lv_style_t style_label_info_screen;
/* Layout */
static int32_t col_info_screen[] = {LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(5), LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_info_screen[] = {LV_GRID_FR(1), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

/*************************
 * Writeup Screen Styles *
 *************************/
/* Screen */
static lv_style_t style_writeup_screen;
/* Headers */
static lv_style_t style_header;
/* Main Text Blocks */
static lv_style_t style_text;

/**
 * Info Writeup
 *
 * This will hold information about the project.
 *
 * 1) The inspiration behind the project
 * 2) What it is capable of
 * 3) What we used to make the project
 * 4) Who made the project
 */

/* Screen */
static lv_obj_t *info_writeup;
static lv_style_t *style_info_writeup;
/* Headers */
static lv_obj_t *inspiration_header;
static lv_obj_t *capability_header;
static lv_obj_t *materials_header;
static lv_obj_t *people_header;
/* Main Text Blocks */
static lv_obj_t *inspiration_text;
static lv_obj_t *capability_text;
static lv_obj_t *materials_text;
static lv_obj_t *people_text;
/* Other */
static lv_obj_t *info_back_btn;
static lv_obj_t *info_back_label;

/**
 * Help Writeup
 *
 * This will be a sort-of FAQ section, helping to navigate to
 * the correct parts of the program based on what the user is
 * hoping to do.
 */

/* Screen */
static lv_obj_t *help_writeup;
/* FAQ Headers */
static lv_obj_t *FAQ_Q1;
static lv_obj_t *FAQ_Q2;
static lv_obj_t *FAQ_Q3;
/* FAQ Answers */
static lv_obj_t *FAQ_A1;
static lv_obj_t *FAQ_A2;
static lv_obj_t *FAQ_A3;
/* Other */
static lv_obj_t *help_back_btn;
static lv_obj_t *help_back_label;

/**
 * Tutorial
 *
 * This will just display a QR code that can be used to watch
 * a YouTube video showing how everything works.
 */

/* Screen */
static lv_obj_t *tutorial_screen;
/* Headers */
static lv_obj_t *tutorial_header;
/* QR Code */
static lv_obj_t *qr_code;
/* Other */
static lv_obj_t *tutorial_back_btn;
static lv_obj_t *tutorial_back_label;

/**
 * Demo Screen
 * 
 * This switches between Pen and Laser demos
 */

static lv_obj_t demo_screen;

static lv_obj_t *main_label_demo_screen;
static lv_style_t style_main_label_demo_screen;

static lv_obj_t *pen_demo_btn;
static lv_obj_t *pen_demo_label;

static lv_obj_t *laser_demo_btn;
static lv_obj_t *laser_demo_label;
/* Styles */
static lv_style_t style_btn_demo_screen;
static lv_style_t style_label_demo_screen;
/* Layout */
static int32_t col_demo_screen[] = {LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(5), LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_demo_screen[] = {LV_GRID_FR(1), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

/**
 * Pen Demo Selection Screen
 *
 * This screen will be opened when "Demos" is selected from the Start
 * Screen. On the screen will be buttons, one for each demo program we
 * have made. Selecting a button will open a popup window (darken the
 * background) with a short description of the demo, an option to start
 * the program, and an option to go back.
 */

/* Screen */
static lv_obj_t *demo_pen_screen;
static lv_style_t style_demo_pen_screen;
/* Main Text */
static lv_obj_t *main_label_demo_pen_screen;
/* Demo Button 1 */
static lv_obj_t *demo_btn_1_demo_pen_screen;
static lv_obj_t *demo_label_1_demo_pen_screen;
/* Demo Button 2 */
static lv_obj_t *demo_btn_2_demo_pen_screen;
static lv_obj_t *demo_label_2_demo_pen_screen;
/* Demo Button 3 */
static lv_obj_t *demo_btn_3_demo_pen_screen;
static lv_obj_t *demo_label_3_demo_pen_screen;
/* Demo Button 4 */
static lv_obj_t *demo_btn_4_demo_pen_screen;
static lv_obj_t *demo_label_4_demo_pen_screen;
/* Demo Button 5 */
static lv_obj_t *demo_btn_5_demo_pen_screen;
static lv_obj_t *demo_label_5_demo_pen_screen;
/* Demo Button 6 */
static lv_obj_t *demo_btn_6_demo_pen_screen;
static lv_obj_t *demo_label_6_demo_pen_screen;
/* Back Button */
static lv_obj_t *back_btn_demo_pen_screen;
static lv_obj_t *back_label_demo_pen_screen;
/* Layout */
static int32_t col_demo_pen_screen[] = {LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_FR(8), LV_GRID_FR(1), LV_GRID_FR(8), LV_GRID_FR(1), LV_GRID_FR(8), LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_demo_pen_screen[] = {LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};

/**
 * Laser Demo Selection Screen
 *
 * This screen will be opened when "Demos" is selected from the Start
 * Screen. On the screen will be buttons, one for each demo program we
 * have made. Selecting a button will open a popup window (darken the
 * background) with a short description of the demo, an option to start
 * the program, and an option to go back.
 */

/* Screen */
static lv_obj_t *demo_laser_screen;
static lv_style_t style_demo_laser_screen;
/* Main Text */
static lv_obj_t *main_label_demo_laser_screen;
/* Demo Button 1 */
static lv_obj_t *demo_btn_1_demo_laser_screen;
static lv_obj_t *demo_label_1_demo_laser_screen;
/* Demo Button 2 */
static lv_obj_t *demo_btn_2_demo_laser_screen;
static lv_obj_t *demo_label_2_demo_laser_screen;
/* Demo Button 3 */
static lv_obj_t *demo_btn_3_demo_laser_screen;
static lv_obj_t *demo_label_3_demo_laser_screen;
/* Demo Button 4 */
static lv_obj_t *demo_btn_4_demo_laser_screen;
static lv_obj_t *demo_label_4_demo_laser_screen;
/* Demo Button 5 */
static lv_obj_t *demo_btn_5_demo_laser_screen;
static lv_obj_t *demo_label_5_demo_laser_screen;
/* Demo Button 6 */
static lv_obj_t *demo_btn_6_demo_laser_screen;
static lv_obj_t *demo_label_6_demo_laser_screen;
/* Back Button */
static lv_obj_t *back_btn_demo_laser_screen;
static lv_obj_t *back_label_demo_laser_screen;
/* Styles */
static lv_style_t style_btn_demo_laser_screen;
static lv_style_t style_label_demo_laser_screen;
/* Layout */
static int32_t col_demo_laser_screen[] = {LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_FR(8), LV_GRID_FR(1), LV_GRID_FR(8), LV_GRID_FR(1), LV_GRID_FR(8), LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_demo_laser_screen[] = {LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};


/**
 * Demo Pen Selection Popup
 *
 * This is the popup that will show up when you press a button on
 * the Demo Selection screen. It will take up most of the screen,
 * but the rest of the screen will be behind the popup, but darkened.
 *
 * The popup will have a back button in the top left corner, similar
 * to the other screens, a start button in the bottom right corner
 * that will eventually start the selected program, and a description
 * of what the program does in the middle.
 */

/* Popup (Screen) */
static lv_obj_t *demo_pen_popup;
static lv_style_t style_demo_popup;
/* Main Text */
static lv_obj_t *main_text_demo_pen_popup;
static lv_style_t style_main_text_demo_popup;
/* Subtext */
static lv_obj_t *sub_text_demo_pen_popup;
static lv_style_t style_sub_text_demo_popup;
/* Back Button */
static lv_obj_t *back_btn_demo_pen_popup;
static lv_style_t style_back_btn_demo_popup;
static lv_obj_t *back_label_demo_pen_popup;
/* Start Button */
static lv_obj_t *start_btn_demo_pen_popup;
static lv_style_t style_start_btn_demo_popup;
static lv_obj_t *start_label_demo_pen_popup;
/* Layout */
static int32_t col_demo_pen_popup[] = {LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_demo_pen_popup[] = {LV_GRID_FR(1), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

/**
 * Demo Laser Selection Popup
 *
 * This is the popup that will show up when you press a button on
 * the Demo Selection screen. It will take up most of the screen,
 * but the rest of the screen will be behind the popup, but darkened.
 *
 * The popup will have a back button in the top left corner, similar
 * to the other screens, a start button in the bottom right corner
 * that will eventually start the selected program, and a description
 * of what the program does in the middle.
 */

/* Popup (Screen) */
static lv_obj_t *demo_laser_popup;
/* Main Text */
static lv_obj_t *main_text_demo_laser_popup;
/* Subtext */
static lv_obj_t *sub_text_demo_laser_popup;
/* Back Button */
static lv_obj_t *back_btn_demo_laser_popup;
static lv_obj_t *back_label_demo_laser_popup;
/* Start Button */
static lv_obj_t *start_btn_demo_laser_popup;
static lv_obj_t *start_label_demo_laser_popup;
/* Layout */
static int32_t col_demo_laser_popup[] = {LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_demo_laser_popup[] = {LV_GRID_FR(1), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

/**
 * New Project Screen
 *
 * This is the start of a step-by-step process that we can walk the
 * user through. Unsure what this will have on it currently, but it
 * will likely prompt the user to upload a file to PortaGuide, either
 * via USB, cloud, or other method (give options for each?). After
 * that, TBD
 */

/* Screen */
static lv_obj_t *new_proj_screen;
static lv_style_t style_new_proj_screen;
/* Main Text */
static lv_obj_t *main_label_new_proj_screen;
static lv_style_t style_main_label_new_proj_screen;
/* USB File Explorer Button */
static lv_obj_t *usb_file_btn_new_proj_screen;
static lv_obj_t *usb_file_image_new_proj_screen;
static lv_obj_t *usb_file_label_new_proj_screen;
/* Cloud File Explorer Button */
static lv_obj_t *cloud_file_btn_new_proj_screen;
static lv_obj_t *cloud_file_image_new_proj_screen;
static lv_obj_t *cloud_file_label_new_proj_screen;
/* File Explorer Styles */
static lv_style_t style_file_btn_new_proj_screen;
static lv_style_t style_file_label_new_proj_screen;
/* Back Button */
static lv_obj_t *back_btn_new_proj_screen;
static lv_obj_t *back_label_new_proj_screen;
/* Styles */
static lv_style_t style_btn_new_proj_screen;
static lv_style_t style_label_new_proj_screen;
/* Layout */
static int32_t col_new_proj_screen[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(13), LV_GRID_FR(2), LV_GRID_FR(13), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_new_proj_screen[] = {LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(13), LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

/**
 * Select File Screen
 *
 * This will open once a file has been selected in the file explorer.
 * Once it opens, there will be a button to go back, which will re-open
 * the file explorer, a text prompt asking whether the selected file
 * is the correct one, the file selected, and a start button to confirm
 * the file choice and start the program.
 */

/* Screen */
static lv_obj_t *file_confirm_screen;
static lv_style_t style_file_confirm_screen;
/* Main Text */
static lv_obj_t *main_label_file_confirm;
static lv_style_t style_main_label_file_confirm;
/* File Path */
static lv_obj_t *filepath_file_confirm;
static lv_style_t style_filepath_file_confirm;
/* Back Button */
static lv_obj_t *back_btn_file_confirm;
static lv_obj_t *back_label_file_confirm;
static lv_style_t style_back_btn_file_confirm;
/* Start Button */
static lv_obj_t *start_btn_file_confirm;
static lv_obj_t *start_label_file_confirm;
static lv_style_t style_start_btn_file_confirm;
/* Button Labels */
static lv_style_t style_btn_label_file_confirm;

/**
 * Program Running Screen
 *
 * This screen will be displayed while the program is running. It will
 * display this with text, and will also have a quit button that will
 * stop the program by triggering the E-Stop GPIO configured in LinuxCNC.
 */

static lv_obj_t *program_running_screen;
static lv_obj_t *program_running_label;
static lv_obj_t *style_program_running_label;
static lv_obj_t *program_running_quit_button;
static lv_obj_t *program_running_quit_label;

/**
 * Homing Screen
 * 
 * This screen will be opened if the E-Stop is triggered. It will set 
 * the Homing Pin high to signal to the Python script on the CNC Pi 
 * that the arm is being manually homed. It will have a message on the
 * screen to say that the program was stopped, and also a message to
 * prompt the user to home the arm.
 */
static lv_obj_t *homing_screen;
static lv_obj_t *homing_label;
static lv_obj_t *homing_msg_1;
static lv_obj_t *homing_msg_2;
static lv_obj_t *homing_done_btn;
static lv_obj_t *homing_done_label;


/**
 * Program Done Screen
 *
 * This screen will be opened once the program stops running. It will
 * have text saying that the program finished running and a button that,
 * when pressed, will return the user back to the main screen
 */

static lv_obj_t *program_done_screen;
static lv_obj_t *program_done_label;
static lv_obj_t *style_program_done_label;
static lv_obj_t *program_done_quit_button;
static lv_obj_t *program_done_quit_label;

/******************
 * File Explorers *
 ******************/

/* USB File Explorer */
static lv_obj_t *usb_file_explorer;
/* Cloud File Explorer */
static lv_obj_t *cloud_file_explorer;

/**************************
 * General Objects/Styles *
 **************************/

/* Back Button */
static lv_obj_t *back_btn;
static lv_obj_t *back_label;
static lv_style_t style_back_label;
/* Pressed Button */
static lv_style_t style_btn_pressed;

lv_indev_t *gpio_run_in;

/************************
 * Function Declaration *
 ************************/

/***** GPIO Callback Functions *****/

// void program_running_cb(int pi, unsigned int user_gpio, unsigned int level, uint32_t tick);

// void program_stopped_cb(int pi, unsigned int user_gpio, unsigned int level, uint32_t tick);

/***** Functions for all Screens *****/

/**
 * Initialize the styles for the window objects
 */
void style_init();

/**
 * Helper function used to darken a button when pressed
 */
static lv_color_t darken(const lv_color_filter_dsc_t *dsc, lv_color_t color, lv_opa_t opa);

/***** Callbacks *****/

static void info_pressed_cb(lv_event_t *e);

static void info_writeup_cb(lv_event_t *e);

static void help_writeup_cb(lv_event_t *e);

static void tutorial_cb(lv_event_t *e);

static void writeup_back_cb(lv_event_t *e);

static void new_proj_pressed_cb(lv_event_t *e);

static void demo_pressed_cb(lv_event_t *e);

static void demo_pen_selected_cb(lv_event_t *e);

static void demo_laser_selected_cb(lv_event_t *e);

static void back_pressed_cb(lv_event_t *e);

static void back_demo_pen_cb(lv_event_t *e);

static void back_demo_laser_cb(lv_event_t *e);

static void usb_file_explorer_cb(lv_event_t *e);

static void cloud_file_explorer_cb(lv_event_t *e);

static void file_selected_cb(lv_event_t *e);

static void quit_cb(lv_event_t *e);

static void sidebar_event_cb(lv_event_t *e);

static void estop_pressed_cb(lv_event_t *e);

static void start_demo_program_cb(lv_event_t *e);

static void start_program_cb(lv_event_t *e);

static void quit_program_cb(lv_event_t *e);

/***** Start Screen Functions *****/

void open_start_screen();

void config_start_screen();

/***** Demo Screen Functions *****/

void open_demo_screen();

void config_demo_screen();

/***** Demo Pen Screen *****/

void open_demo_pen_screen();

void config_demo_pen_screen();

/***** Demo Laser Screen *****/

void open_demo_laser_screen();

void config_demo_laser_screen();

/***** Demo Pen Popup Functions *****/

void open_demo_pen_popup();

void config_demo_pen_popup();

/***** Demo Laser Popup Functions *****/

void open_demo_laser_popup();

void config_demo_laser_popup();

/***** Info Screen Functions *****/

void open_info_screen();

void config_info_screen();

/***** Info Writeup Screen Functions *****/

void open_info_writeup();
void open_help_writeup();
void open_tutorial_writeup();

void config_writeup_screens();

/***** New Project Screen Functions *****/

void open_new_proj_screen();

void config_new_proj_screen();

/***** File Confirm Screen Functions *****/

void open_file_confirm_screen();

void config_file_confirm_screen();

/***** File Explorers *****/

void open_usb_explorer();

void open_cloud_explorer();

void config_usb_explorer();

void config_cloud_explorer();

/***** Program Running Screen *****/

void config_program_running();

void open_program_running();

/***** Homing Screen *****/

void config_homing_screen();

void open_homing_screen();

/***** Program Done Screen *****/

void config_program_done();

void open_program_done();

/**************************
 * Touch Screen and Mouse *
 **************************/

void input_init()
{
    lv_sdl_mouse_create();
}

void program_stopped_cb(int pi, unsigned user_gpio, unsigned level, uint32_t tick, void *userdata)
{
    if (level == PI_LOW)
    {
        open_program_done();
        gpio_write(pi_num, GPIO_START_OUT, PI_LOW);
    }
}

/***********************
 * Function Definition *
 ***********************/

static lv_color_t darken(const lv_color_filter_dsc_t *dsc, lv_color_t color, lv_opa_t opa)
{
    LV_UNUSED(dsc);
    return lv_color_darken(color, opa);
}

void start_screen_style_init()
{
    /****************
     * Start Screen *
     ****************/

    /* Style for screen */
    lv_style_init(&style_start_screen);
    lv_style_set_bg_color(&style_start_screen, lv_palette_darken(COLOR_PALETTE, 4));
    lv_style_set_border_width(&style_start_screen, 0);
    lv_style_set_radius(&style_start_screen, 0);

    /* Style for main text */
    lv_style_init(&style_main_label_start_screen);
    lv_style_set_text_font(&style_main_label_start_screen, &lv_font_montserrat_48);
    lv_style_set_text_color(&style_main_label_start_screen, lv_color_white());

    /* Style for buttons */
    lv_style_init(&style_btn_start_screen);
    lv_style_set_bg_color(&style_btn_start_screen, lv_color_white());
    lv_style_set_border_width(&style_btn_start_screen, 0);
    lv_style_set_radius(&style_btn_start_screen, 4);

    /* Style for labels */
    lv_style_init(&style_label_start_screen);
    lv_style_set_text_font(&style_label_start_screen, &lv_font_montserrat_34);
    lv_style_set_text_color(&style_label_start_screen, lv_color_black());
}

void info_screen_style_init()
{
    /***************
     * Info Screen *
     ***************/

    /* Style for screen */
    lv_style_init(&style_info_screen);
    lv_style_set_bg_color(&style_info_screen, lv_palette_darken(COLOR_PALETTE, 4));
    lv_style_set_border_width(&style_info_screen, 0);
    lv_style_set_radius(&style_info_screen, 0);

    /* Style for main text */
    lv_style_init(&style_main_label_info_screen);
    lv_style_set_text_font(&style_main_label_info_screen, &lv_font_montserrat_48);
    lv_style_set_text_color(&style_main_label_info_screen, lv_color_white());

    /* Style for subtext */
    lv_style_init(&style_sub_label_info_screen);
    lv_style_set_text_font(&style_sub_label_info_screen, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_sub_label_info_screen, lv_color_white());

    /* Style for buttons */
    lv_style_init(&style_btn_info_screen);
    lv_style_set_bg_color(&style_btn_info_screen, lv_color_white());
    lv_style_set_border_width(&style_btn_info_screen, 0);
    lv_style_set_radius(&style_btn_info_screen, 4);

    /* Style for labels */
    lv_style_init(&style_label_info_screen);
    lv_style_set_text_font(&style_label_info_screen, &lv_font_montserrat_34);
    lv_style_set_text_color(&style_label_info_screen, lv_color_black());
}

void demo_screen_style_init()
{
    /***************
     * Demo Screen *
     ***************/

    /* Style for screen */
    lv_style_init(&style_demo_screen);
    lv_style_set_bg_color(&style_demo_screen, lv_palette_darken(COLOR_PALETTE, 4));
    lv_style_set_border_width(&style_demo_screen, 0);
    lv_style_set_radius(&style_demo_screen, 0);

    /* Style for main text */
    lv_style_init(&style_main_label_demo_screen);
    lv_style_set_text_font(&style_main_label_demo_screen, &lv_font_montserrat_48);
    lv_style_set_text_color(&style_main_label_demo_screen, lv_color_white());

    /* Style for buttons */
    lv_style_init(&style_btn_demo_screen);
    lv_style_set_bg_color(&style_btn_demo_screen, lv_color_white());
    lv_style_set_border_width(&style_btn_demo_screen, 0);
    lv_style_set_radius(&style_btn_demo_screen, 4);

    /* Style for labels */
    lv_style_init(&style_label_demo_screen);
    lv_style_set_text_font(&style_label_demo_screen, &lv_font_montserrat_34);
    lv_style_set_text_color(&style_label_demo_screen, lv_color_black());
}

void demo_popup_style_init()
{
    /**************
     * Demo Popup *
     **************/

    /* Style for popup */
    lv_style_init(&style_demo_popup);
    lv_style_set_bg_color(&style_demo_popup, lv_palette_darken(COLOR_PALETTE, 4));
    lv_style_set_border_width(&style_demo_popup, 0);
    lv_style_set_radius(&style_demo_popup, 5);

    /* Style for main text */
    lv_style_init(&style_main_text_demo_popup);
    lv_style_set_text_font(&style_main_text_demo_popup, &lv_font_montserrat_48);
    lv_style_set_text_color(&style_main_text_demo_popup, lv_color_white());

    /* Style for subtext */
    lv_style_init(&style_sub_text_demo_popup);
    lv_style_set_text_font(&style_sub_text_demo_popup, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_sub_text_demo_popup, lv_color_white());

    /* Style for back button */
    lv_style_init(&style_back_btn_demo_popup);
    lv_style_set_bg_color(&style_back_btn_demo_popup, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_border_width(&style_back_btn_demo_popup, 0);
    lv_style_set_radius(&style_back_btn_demo_popup, 4);

    /* Style for start button */
    lv_style_init(&style_start_btn_demo_popup);
    lv_style_set_bg_color(&style_start_btn_demo_popup, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_border_width(&style_start_btn_demo_popup, 0);
    lv_style_set_radius(&style_start_btn_demo_popup, 4);
}

void new_proj_screen_style_init()
{
    /**********************
     * New Project Screen *
     **********************/

    /* Style for screen */
    lv_style_init(&style_new_proj_screen);
    lv_style_set_bg_color(&style_new_proj_screen, lv_palette_darken(COLOR_PALETTE, 4));
    lv_style_set_border_width(&style_new_proj_screen, 0);
    lv_style_set_radius(&style_new_proj_screen, 0);

    /* Style for main text */
    lv_style_init(&style_main_label_new_proj_screen);
    lv_style_set_text_font(&style_main_label_new_proj_screen, &lv_font_montserrat_48);
    lv_style_set_text_color(&style_main_label_new_proj_screen, lv_color_white());

    /* Style for file explorer button */
    lv_style_init(&style_file_btn_new_proj_screen);
    lv_style_set_bg_color(&style_file_btn_new_proj_screen, lv_palette_lighten(LV_PALETTE_GREY, 5));
    lv_style_set_border_width(&style_file_btn_new_proj_screen, 0);
    lv_style_set_radius(&style_file_btn_new_proj_screen, 4);

    /* Style for file explorer label */
    lv_style_init(&style_file_label_new_proj_screen);
    lv_style_set_text_font(&style_file_label_new_proj_screen, &lv_font_montserrat_30);
    lv_style_set_text_color(&style_file_label_new_proj_screen, lv_color_black());

    /* Style for buttons */
    lv_style_init(&style_btn_new_proj_screen);
    lv_style_set_bg_color(&style_btn_new_proj_screen, lv_color_white());
    lv_style_set_border_width(&style_btn_new_proj_screen, 0);
    lv_style_set_radius(&style_btn_new_proj_screen, 4);

    /* Style for labels */
    lv_style_init(&style_label_new_proj_screen);
    lv_style_set_text_font(&style_label_new_proj_screen, &lv_font_montserrat_34);
    lv_style_set_text_color(&style_label_new_proj_screen, lv_color_black());
}

void file_confirm_screen_style_init()
{
    /* Screen */
    lv_style_init(&style_file_confirm_screen);
    lv_style_set_bg_color(&style_file_confirm_screen, lv_palette_darken(COLOR_PALETTE, 4));
    lv_style_set_border_width(&style_file_confirm_screen, 0);
    lv_style_set_radius(&style_file_confirm_screen, 0);
    /* Main Text */
    lv_style_init(&style_main_label_file_confirm);
    lv_style_set_text_font(&style_main_label_file_confirm, &lv_font_montserrat_40);
    lv_style_set_text_color(&style_main_label_file_confirm, lv_color_white());
    /* File Path */
    lv_style_init(&style_filepath_file_confirm);
    lv_style_set_text_font(&style_filepath_file_confirm, &lv_font_montserrat_24);
    lv_style_set_text_color(&style_filepath_file_confirm, lv_color_white());
    /* Back Button */
    lv_style_init(&style_back_btn_file_confirm);
    lv_style_set_bg_color(&style_back_btn_file_confirm, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_border_width(&style_back_btn_file_confirm, 0);
    lv_style_set_radius(&style_back_btn_file_confirm, 4);
    /* Start Button */
    lv_style_init(&style_start_btn_file_confirm);
    lv_style_set_bg_color(&style_start_btn_file_confirm, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_border_width(&style_start_btn_file_confirm, 0);
    lv_style_set_radius(&style_start_btn_file_confirm, 4);
    /* Button Labels */
    lv_style_init(&style_btn_label_file_confirm);
    lv_style_set_text_font(&style_btn_label_file_confirm, &lv_font_montserrat_18);
    lv_style_set_text_color(&style_btn_label_file_confirm, lv_color_black());
}

void writeup_screen_style_init()
{
    lv_style_init(&style_writeup_screen);
    lv_style_set_bg_color(&style_writeup_screen, lv_palette_darken(COLOR_PALETTE, 4));
    lv_style_set_border_width(&style_writeup_screen, 0);
    lv_style_set_radius(&style_writeup_screen, 0);

    lv_style_init(&style_header);
    lv_style_set_text_font(&style_header, &lv_font_montserrat_48);
    lv_style_set_text_color(&style_header, lv_color_white());
    lv_style_set_pad_all(&style_header, 25);

    lv_style_init(&style_text);
    lv_style_set_text_font(&style_text, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_text, lv_color_white());
    lv_style_set_pad_top(&style_text, 25);
    lv_style_set_pad_bottom(&style_text, 50);
}

void style_init()
{
    start_screen_style_init();

    info_screen_style_init();

    demo_screen_style_init();

    demo_popup_style_init();

    new_proj_screen_style_init();

    file_confirm_screen_style_init();

    writeup_screen_style_init();

    /****************
     * Other Styles *
     ****************/

    /* Style for back button */
    lv_style_init(&style_back_label);
    lv_style_set_text_font(&style_back_label, &lv_font_montserrat_18);
    lv_style_set_text_color(&style_back_label, lv_color_black());

    /* Darken button when pressed */
    static lv_color_filter_dsc_t color_filter_btn;
    lv_color_filter_dsc_init(&color_filter_btn, darken);
    lv_style_init(&style_btn_pressed);
    lv_style_set_color_filter_dsc(&style_btn_pressed, &color_filter_btn);
    lv_style_set_color_filter_opa(&style_btn_pressed, LV_OPA_30);
}

/****************
 * Open Screens *
 ****************/

void open_start_screen()
{
    lv_screen_load(start_screen);
    gpio_write(pi_num, GPIO_START_OUT, PI_LOW);
    gpio_write(pi_num, GPIO_E_STOP, PI_LOW);
    gpio_write(pi_num, GPIO_HOMING, PI_LOW);
}

void open_info_screen()
{
    lv_screen_load(info_screen);
    gpio_write(pi_num, GPIO_E_STOP, PI_LOW);
}

void open_demo_screen()
{
    lv_screen_load(demo_screen);
    gpio_write(pi_num, GPIO_E_STOP, PI_LOW);
}

void open_demo_pen_screen()
{
    lv_screen_load(demo_pen_screen);
}

void open_demo_laser_screen()
{
    lv_screen_load(demo_laser_screen);
}

void open_demo_pen_popup()
{
    lv_screen_load(demo_pen_popup);
}

void open_demo_laser_popup()
{
    lv_screen_load(demo_laser_popup);
}

void open_new_proj_screen()
{
    lv_screen_load(new_proj_screen);
    gpio_write(pi_num, GPIO_E_STOP, PI_LOW);
}

void open_file_confirm_screen()
{
    lv_screen_load(file_confirm_screen);
}

void open_usb_explorer()
{
    usb_file_explorer = lv_file_explorer_create(NULL);
    lv_file_explorer_set_sort(usb_file_explorer, LV_EXPLORER_SORT_KIND);
    lv_file_explorer_open_dir(usb_file_explorer, "A:/media/PortaGuide");
    lv_obj_add_event_cb(usb_file_explorer, file_selected_cb, LV_EVENT_ALL, NULL);
    lv_obj_t *file_explorer_quick_access = lv_file_explorer_get_quick_access_area(usb_file_explorer);
    lv_obj_t *file_explorer_header = lv_file_explorer_get_header(usb_file_explorer);
    lv_obj_t *file_explorer_main = lv_file_explorer_get_file_table(usb_file_explorer);
    lv_obj_add_flag(file_explorer_quick_access, LV_OBJ_FLAG_HIDDEN);
    lv_obj_t *close_btn = lv_button_create(file_explorer_header);
    lv_obj_set_size(close_btn, 100, 50);
    lv_obj_set_style_radius(close_btn, 2, 0);
    lv_obj_set_style_pad_all(close_btn, 4, 0);
    lv_obj_align(close_btn, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_event_cb(close_btn, open_new_proj_screen, LV_EVENT_CLICKED, NULL);
    lv_obj_t *close_label = lv_label_create(close_btn);
    lv_label_set_text(close_label, "Close");
    lv_obj_center(close_label);
    
    lv_screen_load(usb_file_explorer);
}

void open_cloud_explorer()
{
    lv_screen_load(cloud_file_explorer);
}

void open_program_running()
{
    lv_screen_load(program_running_screen);
}

void open_program_done()
{
    lv_screen_load(program_done_screen);
}

void open_homing_screen()
{
    lv_screen_load(homing_screen);
}

void open_info_writeup()
{
    lv_screen_load(info_writeup);
}

void open_help_writeup()
{
    lv_screen_load(help_writeup);
}

void open_tutorial_writeup()
{
    lv_screen_load(tutorial_screen);
}

/*********************
 * Configure Screens *
 *********************/

void config_start_screen()
{
    /* Configure Screen */
    start_screen = lv_obj_create(NULL);
    lv_obj_add_style(start_screen, &style_start_screen, 0);
    lv_obj_set_size(start_screen, 1024, 600);
    lv_obj_center(start_screen);
    /* Set grid layout type */
    lv_obj_set_grid_dsc_array(start_screen, col_start_screen, row_start_screen);
    /* Remove padding around all objects to fit them closer together */
    lv_obj_set_style_pad_all(start_screen, 0, 0);
    lv_obj_set_style_pad_column(start_screen, 0, 0);
    lv_obj_set_style_pad_row(start_screen, 0, 0);
    /* Configure Main Label */
    main_label_start_screen = lv_label_create(start_screen);
    lv_obj_add_style(main_label_start_screen, &style_main_label_start_screen, 0);
    lv_label_set_text(main_label_start_screen, "PortaGuide");
    lv_obj_set_grid_cell(main_label_start_screen, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_CENTER, 1, 1);
    /* Info Button */
    info_btn_start_screen = lv_button_create(start_screen);
    lv_obj_add_style(info_btn_start_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(info_btn_start_screen, &style_btn_start_screen, 0);
    lv_obj_set_grid_cell(info_btn_start_screen, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(info_btn_start_screen, info_pressed_cb, LV_EVENT_CLICKED, NULL);
    info_label_start_screen = lv_label_create(info_btn_start_screen);
    lv_obj_add_style(info_label_start_screen, &style_label_start_screen, 0);
    lv_label_set_text(info_label_start_screen, "Info");
    lv_obj_center(info_label_start_screen);
    /* New Project Button */
    new_proj_btn_start_screen = lv_button_create(start_screen);
    lv_obj_add_style(new_proj_btn_start_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(new_proj_btn_start_screen, &style_btn_start_screen, 0);
    lv_obj_set_grid_cell(new_proj_btn_start_screen, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(new_proj_btn_start_screen, new_proj_pressed_cb, LV_EVENT_CLICKED, NULL);
    new_proj_label_start_screen = lv_label_create(new_proj_btn_start_screen);
    lv_obj_add_style(new_proj_label_start_screen, &style_label_start_screen, 0);
    lv_label_set_text(new_proj_label_start_screen, "New Project");
    lv_obj_center(new_proj_label_start_screen);
    /* Demo Button */
    demo_btn_start_screen = lv_button_create(start_screen);
    lv_obj_add_style(demo_btn_start_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(demo_btn_start_screen, &style_btn_start_screen, 0);
    lv_obj_set_grid_cell(demo_btn_start_screen, LV_GRID_ALIGN_STRETCH, 5, 1, LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(demo_btn_start_screen, demo_pressed_cb, LV_EVENT_CLICKED, NULL);
    demo_label_start_screen = lv_label_create(demo_btn_start_screen);
    lv_obj_add_style(demo_label_start_screen, &style_label_start_screen, 0);
    lv_label_set_text(demo_label_start_screen, "Demos");
    lv_obj_center(demo_label_start_screen);
    /* Quit Button */
    quit_btn_start_screen = lv_button_create(start_screen);
    lv_obj_add_style(quit_btn_start_screen, &style_back_btn_demo_popup, 0);
    lv_obj_add_style(quit_btn_start_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(quit_btn_start_screen, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
    lv_obj_add_event_cb(quit_btn_start_screen, quit_cb, LV_EVENT_CLICKED, NULL);
    quit_label_start_screen = lv_label_create(quit_btn_start_screen);
    lv_obj_add_style(quit_label_start_screen, &style_back_label, 0);
    lv_label_set_text(quit_label_start_screen, "Quit");
    lv_obj_center(quit_label_start_screen);
}

void config_info_screen()
{
    /* Configure Screen */
    info_screen = lv_obj_create(NULL);
    lv_obj_set_size(info_screen, 1024, 600);
    lv_obj_center(info_screen);
    lv_obj_add_style(info_screen, &style_info_screen, 0);
    /* Set grid layout type */
    lv_obj_set_grid_dsc_array(info_screen, col_info_screen, row_info_screen);
    /* Remove padding around all objects to fit them closer together */
    lv_obj_set_style_pad_all(info_screen, 0, 0);
    lv_obj_set_style_pad_column(info_screen, 0, 0);
    lv_obj_set_style_pad_row(info_screen, 0, 0);
    /* Main Text */
    main_label_info_screen = lv_label_create(info_screen);
    lv_obj_add_style(main_label_info_screen, &style_main_label_info_screen, 0);
    lv_label_set_text(main_label_info_screen, "Help & Information");
    lv_obj_set_grid_cell(main_label_info_screen, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_CENTER, 1, 1);
    /* Subtext */
    sub_label_info_screen = lv_label_create(info_screen);
    lv_obj_add_style(sub_label_info_screen, &style_sub_label_info_screen, 0);
    lv_label_set_text(sub_label_info_screen, info_text);
    lv_label_set_long_mode(sub_label_info_screen, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(sub_label_info_screen, 750);
    lv_obj_set_grid_cell(sub_label_info_screen, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_START, 2, 1);
    /* Info Button */
    info_btn_info_screen = lv_obj_create(info_screen);
    lv_obj_add_style(info_btn_info_screen, &style_btn_info_screen, 0);
    lv_obj_add_style(info_btn_info_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(info_btn_info_screen, LV_GRID_ALIGN_STRETCH, 1, 2, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(info_btn_info_screen, info_writeup_cb, LV_EVENT_CLICKED, NULL);
    info_label_info_screen = lv_label_create(info_btn_info_screen);
    lv_obj_add_style(info_label_info_screen, &style_label_info_screen, 0);
    lv_label_set_text(info_label_info_screen, "Info");
    lv_obj_center(info_label_info_screen);
    /* Help Button */
    help_btn_info_screen = lv_obj_create(info_screen);
    lv_obj_add_style(help_btn_info_screen, &style_btn_info_screen, 0);
    lv_obj_add_style(help_btn_info_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(help_btn_info_screen, LV_GRID_ALIGN_STRETCH, 4, 2, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(help_btn_info_screen, help_writeup_cb, LV_EVENT_CLICKED, NULL);
    help_label_info_screen = lv_label_create(help_btn_info_screen);
    lv_obj_add_style(help_label_info_screen, &style_label_info_screen, 0);
    lv_label_set_text(help_label_info_screen, "Help");
    lv_obj_center(help_label_info_screen);
    /* Tutorial Button */
    // tutorial_btn_info_screen = lv_obj_create(info_screen);
    // lv_obj_add_style(tutorial_btn_info_screen, &style_btn_info_screen, 0);
    // lv_obj_add_style(tutorial_btn_info_screen, &style_btn_pressed, LV_STATE_PRESSED);
    // lv_obj_set_grid_cell(tutorial_btn_info_screen, LV_GRID_ALIGN_STRETCH, 5, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    // lv_obj_add_event_cb(tutorial_btn_info_screen, tutorial_cb, LV_EVENT_CLICKED, NULL);
    // tutorial_label_info_screen = lv_label_create(tutorial_btn_info_screen);
    // lv_obj_add_style(tutorial_label_info_screen, &style_label_info_screen, 0);
    // lv_label_set_text(tutorial_label_info_screen, "Tutorial");
    // lv_obj_center(tutorial_label_info_screen);
    /* Back Button */
    back_btn_info_screen = lv_obj_create(info_screen);
    lv_obj_add_style(back_btn_info_screen, &style_btn_info_screen, 0);
    lv_obj_add_style(back_btn_info_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(back_btn_info_screen, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
    lv_obj_set_size(back_btn_info_screen, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_add_event_cb(back_btn_info_screen, back_pressed_cb, LV_EVENT_CLICKED, NULL);
    back_label_info_screen = lv_label_create(back_btn_info_screen);
    lv_obj_add_style(back_label_info_screen, &style_back_label, 0);
    lv_label_set_text(back_label_info_screen, "Back");
    lv_obj_center(back_label_info_screen);
}

void config_writeup_screens()
{
    int16_t back_btn_offset_x = 33;
    int16_t back_btn_offset_y = 30;

    /********
     * Info *
     ********/
    /***** Configure Screen *****/
    info_writeup = lv_obj_create(NULL);
    lv_obj_set_size(info_writeup, 1024, 600);
    lv_obj_center(info_writeup);
    lv_obj_add_style(info_writeup, &style_writeup_screen, 0);
    /* Remove padding from all objects */
    lv_obj_set_style_pad_all(info_writeup, 0, 0);
    /***** Inspiration Section *****/
    /* Header */
    inspiration_header = lv_label_create(info_writeup);
    lv_obj_add_style(inspiration_header, &style_header, 0);
    lv_label_set_text(inspiration_header, inspiration_header_text);
    lv_obj_align_to(inspiration_header, info_writeup, LV_ALIGN_TOP_MID, 0, 100);
    /* Text */
    inspiration_text = lv_label_create(info_writeup);
    lv_obj_add_style(inspiration_text, &style_text, 0);
    lv_obj_set_size(inspiration_text, 950, LV_SIZE_CONTENT);
    lv_label_set_long_mode(inspiration_text, LV_LABEL_LONG_WRAP);
    lv_label_set_text(inspiration_text, inspiration_text_text);
    lv_obj_align_to(inspiration_text, inspiration_header, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /***** Capability Section *****/
    /* Header */
    capability_header = lv_label_create(info_writeup);
    lv_obj_add_style(capability_header, &style_header, 0);
    lv_label_set_text(capability_header, capability_header_text);
    lv_obj_align_to(capability_header, inspiration_text, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /* Text */
    capability_text = lv_label_create(info_writeup);
    lv_obj_add_style(capability_text, &style_text, 0);
    lv_obj_set_size(capability_text, 950, LV_SIZE_CONTENT);
    lv_label_set_text(capability_text, capability_text_text);
    lv_label_set_long_mode(capability_text, LV_LABEL_LONG_WRAP);
    lv_obj_align_to(capability_text, capability_header, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /***** Materials Section *****/
    /* Header */
    materials_header = lv_label_create(info_writeup);
    lv_obj_add_style(materials_header, &style_header, 0);
    lv_label_set_text(materials_header, materials_header_text);
    lv_obj_align_to(materials_header, capability_text, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /* Text */
    materials_text = lv_label_create(info_writeup);
    lv_obj_add_style(materials_text, &style_text, 0);
    lv_obj_set_size(materials_text, 950, LV_SIZE_CONTENT);
    lv_label_set_text(materials_text, materials_text_text);
    lv_label_set_long_mode(materials_text, LV_LABEL_LONG_WRAP);
    lv_obj_align_to(materials_text, materials_header, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /***** People Section *****/
    /* Header */
    people_header = lv_label_create(info_writeup);
    lv_obj_add_style(people_header, &style_header, 0);
    lv_label_set_text(people_header, people_header_text);
    lv_obj_align_to(people_header, materials_text, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /* Text */
    people_text = lv_label_create(info_writeup);
    lv_obj_add_style(people_text, &style_text, 0);
    lv_obj_set_size(people_text, 950, LV_SIZE_CONTENT);
    lv_label_set_text(people_text, people_text_text);
    lv_label_set_long_mode(people_text, LV_LABEL_LONG_WRAP);
    lv_obj_align_to(people_text, people_header, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /***** Back Button *****/
    info_back_btn = lv_obj_create(info_writeup);
    lv_obj_add_style(info_back_btn, &style_btn_demo_screen, 0);
    lv_obj_add_style(info_back_btn, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_size(info_back_btn, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_align_to(info_back_btn, info_writeup, LV_ALIGN_TOP_LEFT, back_btn_offset_x, back_btn_offset_y);
    lv_obj_add_event_cb(info_back_btn, writeup_back_cb, LV_EVENT_CLICKED, NULL);
    info_back_label = lv_label_create(info_back_btn);
    lv_obj_add_style(info_back_label, &style_back_label, 0);
    lv_label_set_text(info_back_label, "Back");
    lv_obj_center(info_back_label);

    /********
     * Help *
     ********/
    /***** Configure Screen *****/
    help_writeup = lv_obj_create(NULL);
    lv_obj_set_size(help_writeup, 1024, 600);
    lv_obj_center(help_writeup);
    lv_obj_add_style(help_writeup, &style_writeup_screen, 0);
    /* Remove padding from all objects */
    lv_obj_set_style_pad_all(help_writeup, 0, 0);
    /***** FAQ 1 *****/
    /* Question */
    FAQ_Q1 = lv_label_create(help_writeup);
    lv_obj_add_style(FAQ_Q1, &style_header, 0);
    lv_label_set_text(FAQ_Q1, FAQ_Q1_text);
    lv_obj_align_to(FAQ_Q1, help_writeup, LV_ALIGN_TOP_MID, 0, 100);
    /* Answer */
    FAQ_A1 = lv_label_create(help_writeup);
    lv_obj_add_style(FAQ_A1, &style_text, 0);
    lv_obj_set_size(FAQ_A1, 950, LV_SIZE_CONTENT);
    lv_label_set_text(FAQ_A1, FAQ_A1_text);
    lv_label_set_long_mode(FAQ_A1, LV_LABEL_LONG_WRAP);
    lv_obj_align_to(FAQ_A1, FAQ_Q1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /***** FAQ 2 *****/
    /* Question */
    FAQ_Q2 = lv_label_create(help_writeup);
    lv_obj_add_style(FAQ_Q2, &style_header, 0);
    lv_label_set_text(FAQ_Q2, FAQ_Q2_text);
    lv_obj_align_to(FAQ_Q2, FAQ_A1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /* Answer */
    FAQ_A2 = lv_label_create(help_writeup);
    lv_obj_add_style(FAQ_A2, &style_text, 0);
    lv_obj_set_size(FAQ_A2, 950, LV_SIZE_CONTENT);
    lv_label_set_text(FAQ_A2, FAQ_A2_text);
    lv_label_set_long_mode(FAQ_A2, LV_LABEL_LONG_WRAP);
    lv_obj_align_to(FAQ_A2, FAQ_Q2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /***** FAQ 3 *****/
    /* Question */
    FAQ_Q3 = lv_label_create(help_writeup);
    lv_obj_add_style(FAQ_Q3, &style_header, 0);
    lv_label_set_text(FAQ_Q3, FAQ_Q3_text);
    lv_obj_align_to(FAQ_Q3, FAQ_A2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /* Answer */
    FAQ_A3 = lv_label_create(help_writeup);
    lv_obj_add_style(FAQ_A3, &style_text, 0);
    lv_obj_set_size(FAQ_A3, 950, LV_SIZE_CONTENT);
    lv_label_set_text(FAQ_A3, FAQ_A3_text);
    lv_label_set_long_mode(FAQ_A3, LV_LABEL_LONG_WRAP);
    lv_obj_align_to(FAQ_A3, FAQ_Q3, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /***** Back Button *****/
    help_back_btn = lv_obj_create(help_writeup);
    lv_obj_add_style(help_back_btn, &style_btn_demo_screen, 0);
    lv_obj_add_style(help_back_btn, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_size(help_back_btn, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_align_to(help_back_btn, help_writeup, LV_ALIGN_TOP_LEFT, back_btn_offset_x, back_btn_offset_y);
    lv_obj_add_event_cb(help_back_btn, writeup_back_cb, LV_EVENT_CLICKED, NULL);
    help_back_label = lv_label_create(help_back_btn);
    lv_obj_add_style(help_back_label, &style_back_label, 0);
    lv_label_set_text(help_back_label, "Back");
    lv_obj_center(help_back_label);

    /************
     * Tutorial *
     ************/
    /***** Configure Screen *****/
    tutorial_screen = lv_obj_create(NULL);
    lv_obj_set_size(tutorial_screen, 1024, 600);
    lv_obj_center(tutorial_screen);
    lv_obj_add_style(tutorial_screen, &style_writeup_screen, 0);
    /* Remove padding from all objects */
    lv_obj_set_style_pad_all(tutorial_screen, 0, 0);
    /***** Header *****/
    tutorial_header = lv_label_create(tutorial_screen);
    lv_obj_add_style(tutorial_header, &style_header, 0);
    lv_label_set_text(tutorial_header, tutorial_text);
    lv_obj_align_to(tutorial_header, tutorial_screen, LV_ALIGN_TOP_MID, 0, 100);
    /***** QR Code *****/
    qr_code = lv_image_create(tutorial_screen);
    lv_obj_set_style_pad_top(qr_code, 50, 0);
    lv_obj_align_to(qr_code, tutorial_screen, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /***** Back Button *****/
    tutorial_back_btn = lv_obj_create(tutorial_screen);
    lv_obj_add_style(tutorial_back_btn, &style_btn_demo_screen, 0);
    lv_obj_add_style(tutorial_back_btn, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_size(tutorial_back_btn, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_align_to(tutorial_back_btn, tutorial_screen, LV_ALIGN_TOP_LEFT, back_btn_offset_x, back_btn_offset_y);
    lv_obj_add_event_cb(tutorial_back_btn, writeup_back_cb, LV_EVENT_CLICKED, NULL);
    tutorial_back_label = lv_label_create(tutorial_back_btn);
    lv_obj_add_style(tutorial_back_label, &style_back_label, 0);
    lv_label_set_text(tutorial_back_label, "Back");
    lv_obj_center(tutorial_back_label);
}

void config_demo_screen()
{
    /* Configure Screen */
    demo_screen = lv_obj_create(NULL);
    lv_obj_set_size(demo_screen, 1024, 600);
    lv_obj_center(demo_screen);
    lv_obj_add_style(demo_screen, &style_demo_screen, 0);
    /* Set grid layout type */
    lv_obj_set_grid_dsc_array(demo_screen, col_demo_screen, row_demo_screen);
    /* Remove padding around all objects to fit them closer together */
    lv_obj_set_style_pad_all(demo_screen, 0, 0);
    lv_obj_set_style_pad_column(demo_screen, 0, 0);
    lv_obj_set_style_pad_row(demo_screen, 0, 0);
    /* Main Text */
    main_label_demo_screen = lv_label_create(demo_screen);
    lv_obj_add_style(main_label_demo_screen, &style_main_label_demo_screen, 0);
    lv_label_set_text(main_label_demo_screen, "Demos");
    lv_obj_set_grid_cell(main_label_demo_screen, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_CENTER, 1, 1);
    /* Pen Select Button */
    pen_demo_btn = lv_obj_create(demo_screen);
    lv_obj_add_style(pen_demo_btn, &style_btn_demo_screen, 0);
    lv_obj_add_style(pen_demo_btn, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(pen_demo_btn, LV_GRID_ALIGN_STRETCH, 1, 2, LV_GRID_ALIGN_STRETCH, 3, 1);
    pen_demo_label = lv_label_create(pen_demo_btn);
    lv_obj_add_style(pen_demo_label, &style_label_demo_screen, 0);
    lv_label_set_text(pen_demo_label, "Pen Demos");
    lv_obj_center(pen_demo_label);
    /* Laser Select Button */
    laser_demo_btn = lv_obj_create(demo_screen);
    lv_obj_add_style(laser_demo_btn, &style_btn_demo_screen, 0);
    lv_obj_add_style(laser_demo_btn, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(laser_demo_btn, LV_GRID_ALIGN_STRETCH, 4, 2, LV_GRID_ALIGN_STRETCH, 3, 1);
    laser_demo_label = lv_label_create(laser_demo_btn);
    lv_obj_add_style(laser_demo_label, &style_label_demo_screen, 0);
    lv_label_set_text(laser_demo_label, "Laser Demos");
    lv_obj_center(laser_demo_label);
    /* Back Button */
    back_btn_demo_screen = lv_obj_create(demo_screen);
    lv_obj_add_style(back_btn_demo_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(back_btn_demo_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(back_btn_demo_screen, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
    lv_obj_set_size(back_btn_demo_screen, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_add_event_cb(back_btn_demo_screen, back_pressed_cb, LV_EVENT_CLICKED, NULL);
    back_label_demo_screen = lv_label_create(back_btn_demo_screen);
    lv_obj_add_style(back_label_demo_screen, &style_back_label, 0);
    lv_label_set_text(back_label_demo_screen, "Back");
    lv_obj_center(back_label_demo_screen);
}

void config_demo_pen_screen()
{
    /* Configure Screen */
    demo_pen_screen = lv_obj_create(NULL);
    lv_obj_set_size(demo_pen_screen, 1024, 600);
    lv_obj_center(demo_pen_screen);
    lv_obj_add_style(demo_pen_screen, &style_demo_pen_screen, 0);
    /* Set grid layout type */
    lv_obj_set_grid_dsc_array(demo_pen_screen, col_demo_pen_screen, row_demo_pen_screen);
    /* Remove padding around all objects to fit them closer together */
    lv_obj_set_style_pad_all(demo_pen_screen, 0, 0);
    lv_obj_set_style_pad_column(demo_pen_screen, 0, 0);
    lv_obj_set_style_pad_row(demo_pen_screen, 0, 0);
    /* Main Text */
    main_label_demo_pen_screen = lv_label_create(demo_pen_screen);
    lv_obj_add_style(main_label_demo_pen_screen, &style_main_label_demo_screen, 0);
    lv_label_set_text(main_label_demo_pen_screen, "Demos");
    lv_obj_set_grid_cell(main_label_demo_pen_screen, LV_GRID_ALIGN_CENTER, 2, 5, LV_GRID_ALIGN_CENTER, 1, 1);
    /* Demo Button 1 */
    demo_btn_1_demo_pen_screen = lv_obj_create(demo_pen_screen);
    lv_obj_add_style(demo_btn_1_demo_pen_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_1_demo_pen_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_1_demo_pen_screen, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(demo_btn_1_demo_pen_screen, demo_pen_selected_cb, LV_EVENT_CLICKED, "Line");
    demo_label_1_demo_spen_creen = lv_label_create(demo_btn_1_demo_pen_screen);
    lv_obj_add_style(demo_label_1_demo_pen_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_1_demo_pen_screen, "Line");
    lv_obj_center(demo_label_1_demo_pen_screen);
    /* Demo Button 2 */
    demo_btn_2_demo_pen_screen = lv_obj_create(demo_pen_screen);
    lv_obj_add_style(demo_btn_2_demo_pen_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_2_demo_pen_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_2_demo_pen_screen, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(demo_btn_2_demo_pen_screen, demo_pen_selected_cb, LV_EVENT_CLICKED, "Square");
    demo_label_2_demo_pen_screen = lv_label_create(demo_btn_2_demo_pen_screen);
    lv_obj_add_style(demo_label_2_demo_pen_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_2_demo_pen_screen, "Square");
    lv_obj_center(demo_label_2_demo_pen_screen);
    /* Demo Button 3 */
    demo_btn_3_demo_pen_screen = lv_obj_create(demo_pen_screen);
    lv_obj_add_style(demo_btn_3_demo_pen_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_3_demo_pen_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_3_demo_pen_screen, LV_GRID_ALIGN_STRETCH, 6, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(demo_btn_3_demo_pen_screen, demo_pen_selected_cb, LV_EVENT_CLICKED, "Circle");
    demo_label_3_demo_pen_screen = lv_label_create(demo_btn_3_demo_pen_screen);
    lv_obj_add_style(demo_label_3_demo_pen_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_3_demo_pen_screen, "Circle");
    lv_obj_center(demo_label_3_demo_pen_screen);
    /* Demo Button 4 */
    demo_btn_4_demo_pen_screen = lv_obj_create(demo_pen_screen);
    lv_obj_add_style(demo_btn_4_demo_pen_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_4_demo_pen_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_4_demo_pen_screen, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(demo_btn_4_demo_pen_screen, demo_pen_selected_cb, LV_EVENT_CLICKED, "Oval");
    demo_label_4_demo_pen_screen = lv_label_create(demo_btn_4_demo_pen_screen);
    lv_obj_add_style(demo_label_4_demo_pen_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_4_demo_pen_screen, "Oval");
    lv_obj_center(demo_label_4_demo_pen_screen);
    /* Demo Button 5 */
    demo_btn_5_demo_pen_screen = lv_obj_create(demo_pen_screen);
    lv_obj_add_style(demo_btn_5_demo_pen_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_5_demo_pen_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_5_demo_pen_screen, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(demo_btn_5_demo_pen_screen, demo_pen_selected_cb, LV_EVENT_CLICKED, "Star");
    demo_label_5_demo_pen_screen = lv_label_create(demo_btn_5_demo_pen_screen);
    lv_obj_add_style(demo_label_5_demo_pen_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_5_demo_pen_screen, "Star");
    lv_obj_center(demo_label_5_demo_pen_screen);
    /* Demo Button 6 */
    demo_btn_6_demo_pen_screen = lv_obj_create(demo_pen_screen);
    lv_obj_add_style(demo_btn_6_demo_pen_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_6_demo_pen_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_6_demo_pen_screen, LV_GRID_ALIGN_STRETCH, 6, 1, LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(demo_btn_6_demo_pen_screen, demo_pen_selected_cb, LV_EVENT_CLICKED, "Complex");
    demo_label_6_demo_pen_screen = lv_label_create(demo_btn_6_demo_pen_screen);
    lv_obj_add_style(demo_label_6_demo_pen_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_6_demo_pen_screen, "Complex");
    lv_obj_center(demo_label_6_demo_pen_screen);
    /* Back Button */
    back_btn_demo_pen_screen = lv_obj_create(demo_pen_screen);
    lv_obj_add_style(back_btn_demo_pen_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(back_btn_demo_pen_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(back_btn_demo_pen_screen, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
    lv_obj_set_size(back_btn_demo_pen_screen, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_add_event_cb(back_btn_demo_pen_screen, back_pen_laser_pressed_cb, LV_EVENT_CLICKED, NULL);
    back_label_demo_pen_screen = lv_label_create(back_btn_demo_pen_screen);
    lv_obj_add_style(back_label_demo_pen_screen, &style_back_label, 0);
    lv_label_set_text(back_label_demo_pen_screen, "Back");
    lv_obj_center(back_label_demo_pen_screen);
}

void config_demo_laser_screen()
{
    /* Configure Screen */
    demo_laser_screen = lv_obj_create(NULL);
    lv_obj_set_size(demo_laser_screen, 1024, 600);
    lv_obj_center(demo_laser_screen);
    lv_obj_add_style(demo_laser_screen, &style_demo_screen, 0);
    /* Set grid layout type */
    lv_obj_set_grid_dsc_array(demo_laser_screen, col_demo_laser_screen, row_demo_laser_screen);
    /* Remove padding around all objects to fit them closer together */
    lv_obj_set_style_pad_all(demo_laser_screen, 0, 0);
    lv_obj_set_style_pad_column(demo_laser_screen, 0, 0);
    lv_obj_set_style_pad_row(demo_laser_screen, 0, 0);
    /* Main Text */
    main_label_demo_laser_screen = lv_label_create(demo_laser_screen);
    lv_obj_add_style(main_label_demo_laser_screen, &style_main_label_demo_screen, 0);
    lv_label_set_text(main_label_demo_laser_screen, "Demos");
    lv_obj_set_grid_cell(main_label_demo_laser_screen, LV_GRID_ALIGN_CENTER, 2, 5, LV_GRID_ALIGN_CENTER, 1, 1);
    /* Demo Button 1 */
    demo_btn_1_demo_laser_screen = lv_obj_create(demo_laser_screen);
    lv_obj_add_style(demo_btn_1_demo_laser_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_1_demo_laser_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_1_demo_laser_screen, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(demo_btn_1_demo_laser_screen, demo_laser_selected_cb, LV_EVENT_CLICKED, "Line");
    demo_label_1_demo_laser_screen = lv_label_create(demo_btn_1_demo_laser_screen);
    lv_obj_add_style(demo_label_1_demo_laser_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_1_demo_laser_screen, "Line");
    lv_obj_center(demo_label_1_demo_laser_screen);
    /* Demo Button 2 */
    demo_btn_2_demo_laser_screen = lv_obj_create(demo_laser_screen);
    lv_obj_add_style(demo_btn_2_demo_laser_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_2_demo_laser_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_2_demo_laser_screen, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(demo_btn_2_demo_laser_screen, demo_laser_selected_cb, LV_EVENT_CLICKED, "Square");
    demo_label_2_demo_laser_screen = lv_label_create(demo_btn_2_demo_laser_screen);
    lv_obj_add_style(demo_label_2_demo_laser_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_2_demo_laser_screen, "Square");
    lv_obj_center(demo_label_2_demo_laser_screen);
    /* Demo Button 3 */
    demo_btn_3_demo_laser_screen = lv_obj_create(demo_laser_screen);
    lv_obj_add_style(demo_btn_3_demo_laser_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_3_demo_laser_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_3_demo_laser_screen, LV_GRID_ALIGN_STRETCH, 6, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(demo_btn_3_demo_laser_screen, demo_laser_selected_cb, LV_EVENT_CLICKED, "Circle");
    demo_label_3_demo_laser_screen = lv_label_create(demo_btn_3_demo_laser_screen);
    lv_obj_add_style(demo_label_3_demo_laser_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_3_demo_laser_screen, "Circle");
    lv_obj_center(demo_label_3_demo_laser_screen);
    /* Demo Button 4 */
    demo_btn_4_demo_laser_screen = lv_obj_create(demo_laser_screen);
    lv_obj_add_style(demo_btn_4_demo_laser_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_4_demo_laser_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_4_demo_laser_screen, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(demo_btn_4_demo_laser_screen, demo_laser_selected_cb, LV_EVENT_CLICKED, "Oval");
    demo_label_4_demo_laser_screen = lv_label_create(demo_btn_4_demo_laser_screen);
    lv_obj_add_style(demo_label_4_demo_laser_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_4_demo_laser_screen, "Oval");
    lv_obj_center(demo_label_4_demo_laser_screen);
    /* Demo Button 5 */
    demo_btn_5_demo_laser_screen = lv_obj_create(demo_laser_screen);
    lv_obj_add_style(demo_btn_5_demo_laser_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_5_demo_laser_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_5_demo_laser_screen, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(demo_btn_5_demo_laser_screen, demo_laser_selected_cb, LV_EVENT_CLICKED, "Star");
    demo_label_5_demo_laser_screen = lv_label_create(demo_btn_5_demo_laser_screen);
    lv_obj_add_style(demo_label_5_demo_laser_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_5_demo_laser_screen, "Star");
    lv_obj_center(demo_label_5_demo_laser_screen);
    /* Demo Button 6 */
    demo_btn_6_demo_laser_screen = lv_obj_create(demo_laser_screen);
    lv_obj_add_style(demo_btn_6_demo_laser_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_6_demo_laser_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_6_demo_laser_screen, LV_GRID_ALIGN_STRETCH, 6, 1, LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(demo_btn_6_demo_laser_screen, demo_laser_selected_cb, LV_EVENT_CLICKED, "Complex");
    demo_label_6_demo_laser_screen = lv_label_create(demo_btn_6_demo_laser_screen);
    lv_obj_add_style(demo_label_6_demo_laser_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_6_demo_laser_screen, "Complex");
    lv_obj_center(demo_label_6_demo_laser_screen);
    /* Back Button */
    back_btn_demo_laser_screen = lv_obj_create(demo_laser_screen);
    lv_obj_add_style(back_btn_demo_laser_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(back_btn_demo_laser_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(back_btn_demo_laser_screen, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
    lv_obj_set_size(back_btn_demo_laser_screen, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_add_event_cb(back_btn_demo_laser_screen, back_pen_laser_pressed_cb, LV_EVENT_CLICKED, NULL);
    back_label_demo_laser_screen = lv_label_create(back_btn_demo_laser_screen);
    lv_obj_add_style(back_label_demo_laser_screen, &style_back_label, 0);
    lv_label_set_text(back_label_demo_laser_screen, "Back");
    lv_obj_center(back_label_demo_laser_screen);
}

void config_demo_laser_popup()
{
    /* Configure Screen */
    demo_laser_popup = lv_obj_create(NULL);
    lv_obj_set_size(demo_laser_popup, 1024, 600);
    lv_obj_center(demo_laser_popup);
    lv_obj_add_style(demo_laser_popup, &style_demo_popup, 0);
    /* Set grid layout type */
    lv_obj_set_grid_dsc_array(demo_laser_popup, col_demo_laser_popup, row_demo_laser_popup);
    /* Remove padding around all objects to fit them closer together */
    lv_obj_set_style_pad_all(demo_laser_popup, 0, 0);
    lv_obj_set_style_pad_column(demo_laser_popup, 0, 0);
    lv_obj_set_style_pad_row(demo_laser_popup, 0, 0);
    /* Main Text */
    main_text_demo_laser_popup = lv_label_create(demo_laser_popup);
    lv_obj_add_style(main_text_demo_laser_popup, &style_main_text_demo_popup, 0);
    lv_obj_set_grid_cell(main_text_demo_laser_popup, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_CENTER, 1, 1);
    /* Subtext */
    sub_text_demo_laser_popup = lv_label_create(demo_laser_popup);
    lv_obj_add_style(sub_text_demo_laser_popup, &style_sub_text_demo_popup, 0);
    lv_obj_set_width(sub_text_demo_laser_popup, 700);
    lv_obj_set_grid_cell(sub_text_demo_laser_popup, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_label_set_text(sub_text_demo_laser_popup, "This is an example description");
    lv_label_set_long_mode(sub_label_info_screen, LV_LABEL_LONG_WRAP);
    /* Back Button */
    back_btn_demo_laser_popup = lv_obj_create(demo_laser_popup);
    lv_obj_add_style(back_btn_demo_laser_popup, &style_back_btn_demo_popup, 0);
    lv_obj_add_style(back_btn_demo_laser_popup, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(back_btn_demo_laser_popup, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
    lv_obj_set_size(back_btn_demo_laser_popup, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_add_event_cb(back_btn_demo_laser_popup, back_demo_laser_cb, LV_EVENT_CLICKED, NULL);
    back_label_demo_laser_popup = lv_label_create(back_btn_demo_laser_popup);
    lv_obj_add_style(back_label_demo_laser_popup, &style_back_label, 0);
    lv_label_set_text(back_label_demo_laser_popup, "Back");
    lv_obj_center(back_label_demo_laser_popup);
    /* Start Button */
    start_btn_demo_laser_popup = lv_obj_create(demo_laser_popup);
    lv_obj_add_style(start_btn_demo_laser_popup, &style_start_btn_demo_popup, 0);
    lv_obj_add_style(start_btn_demo_laser_popup, &style_start_btn_demo_popup, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(start_btn_demo_laser_popup, LV_GRID_ALIGN_END, 5, 1, LV_GRID_ALIGN_END, 3, 1);
    lv_obj_set_size(start_btn_demo_laser_popup, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_add_event_cb(start_btn_demo_laser_popup, start_demo_program_cb, LV_EVENT_CLICKED, NULL);
    start_label_demo_laser_popup = lv_label_create(start_btn_demo_laser_popup);
    lv_obj_add_style(start_label_demo_laser_popup, &style_back_label, 0);
    lv_label_set_text(start_label_demo_laser_popup, "Start");
    lv_obj_center(start_label_demo_laser_popup);
}

void config_demo_pen_popup()
{
    /* Configure Screen */
    demo_pen_popup = lv_obj_create(NULL);
    lv_obj_set_size(demo_pen_popup, 1024, 600);
    lv_obj_center(demo_pen_popup);
    lv_obj_add_style(demo_pen_popup, &style_demo_popup, 0);
    /* Set grid layout type */
    lv_obj_set_grid_dsc_array(demo_pen_popup, col_demo_pen_popup, row_demo_pen_popup);
    /* Remove padding around all objects to fit them closer together */
    lv_obj_set_style_pad_all(demo_pen_popup, 0, 0);
    lv_obj_set_style_pad_column(demo_pen_popup, 0, 0);
    lv_obj_set_style_pad_row(demo_pen_popup, 0, 0);
    /* Main Text */
    main_text_demo_pen_popup = lv_label_create(demo_pen_popup);
    lv_obj_add_style(main_text_demo_pen_popup, &style_main_text_demo_popup, 0);
    lv_obj_set_grid_cell(main_text_demo_pen_popup, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_CENTER, 1, 1);
    /* Subtext */
    sub_text_demo_pen_popup = lv_label_create(demo_pen_popup);
    lv_obj_add_style(sub_text_demo_pen_popup, &style_sub_text_demo_popup, 0);
    lv_obj_set_width(sub_text_demo_pen_popup, 700);
    lv_obj_set_grid_cell(sub_text_demo_pen_popup, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_label_set_text(sub_text_demo_pen_popup, "This is an example description");
    lv_label_set_long_mode(sub_label_info_screen, LV_LABEL_LONG_WRAP);
    /* Back Button */
    back_btn_demo_pen_popup = lv_obj_create(demo_pen_popup);
    lv_obj_add_style(back_btn_demo_pen_popup, &style_back_btn_demo_popup, 0);
    lv_obj_add_style(back_btn_demo_pen_popup, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(back_btn_demo_pen_popup, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
    lv_obj_set_size(back_btn_demo_pen_popup, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_add_event_cb(back_btn_demo_pen_popup, back_demo_pen_cb, LV_EVENT_CLICKED, NULL);
    back_label_demo_pen_popup = lv_label_create(back_btn_demo_pen_popup);
    lv_obj_add_style(back_label_demo_pen_popup, &style_back_label, 0);
    lv_label_set_text(back_label_demo_pen_popup, "Back");
    lv_obj_center(back_label_demo_pen_popup);
    /* Start Button */
    start_btn_demo_pen_popup = lv_obj_create(demo_pen_popup);
    lv_obj_add_style(start_btn_demo_pen_popup, &style_start_btn_demo_popup, 0);
    lv_obj_add_style(start_btn_demo_pen_popup, &style_start_btn_demo_popup, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(start_btn_demo_pen_popup, LV_GRID_ALIGN_END, 5, 1, LV_GRID_ALIGN_END, 3, 1);
    lv_obj_set_size(start_btn_demo_pen_popup, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_add_event_cb(start_btn_demo_pen_popup, start_demo_program_cb, LV_EVENT_CLICKED, NULL);
    start_label_demo_pen_popup = lv_label_create(start_btn_demo_pen_popup);
    lv_obj_add_style(start_label_demo_pen_popup, &style_back_label, 0);
    lv_label_set_text(start_label_demo_pen_popup, "Start");
    lv_obj_center(start_label_demo_pen_popup);
}

void config_new_proj_screen()
{
    /* Configure Screen */
    new_proj_screen = lv_obj_create(NULL);
    lv_obj_set_size(new_proj_screen, 1024, 600);
    lv_obj_center(new_proj_screen);
    lv_obj_add_style(new_proj_screen, &style_new_proj_screen, 0);
    /* Set grid layout type */
    lv_obj_set_grid_dsc_array(new_proj_screen, col_new_proj_screen, row_new_proj_screen);
    /* Remove padding around all objects to fit them closer together */
    lv_obj_set_style_pad_all(new_proj_screen, 0, 0);
    lv_obj_set_style_pad_column(new_proj_screen, 0, 0);
    lv_obj_set_style_pad_row(new_proj_screen, 0, 0);
    /* USB File Explorer Button */
    usb_file_btn_new_proj_screen = lv_obj_create(new_proj_screen);
    lv_obj_add_style(usb_file_btn_new_proj_screen, &style_file_btn_new_proj_screen, 0);
    lv_obj_add_style(usb_file_btn_new_proj_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(usb_file_btn_new_proj_screen, LV_GRID_ALIGN_STRETCH, 2, 3, LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(usb_file_btn_new_proj_screen, usb_file_explorer_cb, LV_EVENT_CLICKED, NULL);
    usb_file_image_new_proj_screen = lv_image_create(usb_file_btn_new_proj_screen);
    lv_image_set_src(usb_file_image_new_proj_screen, &usb_symbol);
    lv_obj_center(usb_file_image_new_proj_screen);
    usb_file_label_new_proj_screen = lv_label_create(usb_file_btn_new_proj_screen);
    lv_obj_add_style(usb_file_label_new_proj_screen, &style_file_label_new_proj_screen, 0);
    lv_obj_set_align(usb_file_label_new_proj_screen, LV_ALIGN_BOTTOM_MID);
    lv_label_set_text(usb_file_label_new_proj_screen, "Select from USB");
    /* Back Button */
    back_btn_new_proj_screen = lv_obj_create(new_proj_screen);
    lv_obj_add_style(back_btn_new_proj_screen, &style_btn_new_proj_screen, 0);
    lv_obj_add_style(back_btn_new_proj_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(back_btn_new_proj_screen, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
    lv_obj_set_size(back_btn_new_proj_screen, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_add_event_cb(back_btn_new_proj_screen, back_pressed_cb, LV_EVENT_CLICKED, NULL);
    back_label_new_proj_screen = lv_label_create(back_btn_new_proj_screen);
    lv_obj_add_style(back_label_new_proj_screen, &style_back_label, 0);
    lv_label_set_text(back_label_new_proj_screen, "Back");
    lv_obj_center(back_label_new_proj_screen);
}

void config_file_confirm_screen()
{
    /***** Configure Screen *****/
    file_confirm_screen = lv_obj_create(NULL);
    lv_obj_set_size(file_confirm_screen, 1024, 600);
    lv_obj_center(file_confirm_screen);
    lv_obj_add_style(file_confirm_screen, &style_file_confirm_screen, 0);
    /* Remove padding from all objects */
    lv_obj_set_style_pad_all(file_confirm_screen, 0, 0);
    /***** Main Text *****/
    main_label_file_confirm = lv_label_create(file_confirm_screen);
    lv_obj_add_style(main_label_file_confirm, &style_main_label_file_confirm, 0);
    lv_obj_align(main_label_file_confirm, LV_ALIGN_TOP_MID, 0, 50);
    lv_label_set_text(main_label_file_confirm, "Is the file below correct?");
    /***** File Path *****/
    filepath_file_confirm = lv_label_create(file_confirm_screen);
    lv_obj_add_style(filepath_file_confirm, &style_filepath_file_confirm, 0);
    lv_obj_align_to(filepath_file_confirm, main_label_file_confirm, LV_ALIGN_OUT_BOTTOM_MID, 0, 100);
    lv_label_set_text(filepath_file_confirm, "empty filepath"); // Empty filepath, will set once a file is selected
    /***** Back Button *****/
    back_btn_file_confirm = lv_obj_create(file_confirm_screen);
    lv_obj_add_style(back_btn_file_confirm, &style_back_btn_file_confirm, 0);
    lv_obj_set_size(back_btn_file_confirm, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_set_pos(back_btn_file_confirm, 33, 30);
    lv_obj_add_event_cb(back_btn_file_confirm, usb_file_explorer_cb, LV_EVENT_CLICKED, NULL);
    back_label_file_confirm = lv_label_create(back_btn_file_confirm);
    lv_obj_add_style(back_label_file_confirm, &style_btn_label_file_confirm, 0);
    lv_label_set_text(back_label_file_confirm, "Back");
    lv_obj_center(back_label_file_confirm);
    /***** Start Button *****/
    start_btn_file_confirm = lv_obj_create(file_confirm_screen);
    lv_obj_add_style(start_btn_file_confirm, &style_start_btn_file_confirm, 0);
    lv_obj_set_size(start_btn_file_confirm, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_set_pos(start_btn_file_confirm, 1024 - BACK_BTN_HORZ - 33, 600 - BACK_BTN_VERT - 30);
    lv_obj_add_event(start_btn_file_confirm, start_program_cb, LV_EVENT_CLICKED, NULL);
    start_label_file_confirm = lv_label_create(start_btn_file_confirm);
    lv_obj_add_style(start_label_file_confirm, &style_btn_label_file_confirm, 0);
    lv_label_set_text(start_label_file_confirm, "Start");
    lv_obj_center(start_label_file_confirm);
}

void config_program_running()
{
    program_running_screen = lv_obj_create(NULL);
    lv_obj_set_size(program_running_screen, 1024, 600);
    lv_obj_center(program_running_screen);
    lv_obj_add_style(program_running_screen, &style_start_screen, 0);

    program_running_quit_button = lv_obj_create(program_running_screen);
    lv_obj_set_size(program_running_quit_button, 100, 75);
    lv_obj_add_style(program_running_quit_button, &style_back_btn_demo_popup, 0);
    lv_obj_add_style(program_running_quit_button, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_pos(program_running_quit_button, 1024 - BACK_BTN_HORZ - 33, 600 - BACK_BTN_VERT - 30);
    lv_obj_add_event_cb(program_running_quit_button, quit_program_cb, LV_EVENT_CLICKED, NULL);
    program_running_quit_label = lv_label_create(program_running_quit_button);
    lv_obj_center(program_running_quit_label);
    lv_obj_add_style(program_running_quit_label, &style_back_label, 0);
    lv_label_set_text(program_running_quit_label, "Quit");

    program_running_label = lv_label_create(program_running_screen);
    lv_obj_center(program_running_label);
    lv_obj_add_style(program_running_label, &style_main_text_demo_popup, 0);
    lv_label_set_text(program_running_label, "Program is Running...");
}

void config_program_done()
{
    program_done_screen = lv_obj_create(NULL);
    lv_obj_set_size(program_done_screen, 1024, 600);
    lv_obj_center(program_done_screen);
    lv_obj_add_style(program_done_screen, &style_start_screen, 0);

    program_done_quit_button = lv_obj_create(program_done_screen);
    lv_obj_set_size(program_done_quit_button, 100, 75);
    lv_obj_add_style(program_done_quit_button, &style_start_btn_demo_popup, 0);
    lv_obj_add_style(program_done_quit_button, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_align(program_done_quit_button, LV_ALIGN_BOTTOM_RIGHT, -50, -50);
    lv_obj_add_event_cb(program_done_quit_button, back_pressed_cb, LV_EVENT_CLICKED, NULL);
    program_done_quit_label = lv_label_create(program_done_quit_button);
    lv_obj_center(program_done_quit_label);
    lv_obj_add_style(program_done_quit_label, &style_back_label, 0);
    lv_label_set_text(program_done_quit_label, "Finish");

    program_done_label = lv_label_create(program_done_screen);
    lv_obj_center(program_done_label);
    lv_obj_add_style(program_done_label, &style_main_text_demo_popup, 0);
    lv_label_set_text(program_done_label, "Program is Done Running");
}

void config_homing_screen()
{
    homing_screen = lv_obj_create(NULL);
    lv_obj_set_size(homing_screen, 1024, 600);
    lv_obj_center(homing_screen);
    lv_obj_add_style(homing_screen, &style_start_screen, 0);

    homing_label = lv_label_create(homing_screen);
    lv_obj_align(homing_label, LV_ALIGN_TOP_MID, 0, 100);
    lv_obj_add_style(homing_label, &style_main_text_demo_popup, 0);
    lv_label_set_text(homing_label, "Program was Stopped");

    homing_msg_1 = lv_label_create(homing_screen);
    lv_label_set_text(homing_msg_1, "Please move the robot back to it's home position");
    lv_obj_add_style(homing_msg_1, &style_filepath_file_confirm, 0);
    lv_obj_align_to(homing_msg_1, homing_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 100);

    homing_msg_2 = lv_label_create(homing_screen);
    lv_label_set_text(homing_msg_2, "When done, press the \"Complete\" button");
    lv_obj_add_style(homing_msg_2, &style_filepath_file_confirm, 0);
    lv_obj_align_to(homing_msg_2, homing_msg_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);

    homing_done_btn = lv_obj_create(homing_screen);
    lv_obj_set_size(homing_done_btn, 125, 75);
    lv_obj_add_style(homing_done_btn, &style_start_btn_demo_popup, 0);
    lv_obj_add_style(homing_done_btn, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_pos(homing_done_btn, 1024 - 125 - 33, 600 - BACK_BTN_VERT - 30);
    lv_obj_add_event_cb(homing_done_btn, back_pressed_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_remove_flag(homing_done_btn, LV_OBJ_FLAG_SCROLLABLE);
    homing_done_label = lv_label_create(homing_done_btn);
    lv_obj_center(homing_done_label);
    lv_obj_add_style(homing_done_label, &style_back_label, 0);
    lv_label_set_text(homing_done_label, "Complete");
}

/*************
 * Callbacks *
 *************/

static void info_pressed_cb(lv_event_t *e)
{
    open_info_screen();
}

static void info_writeup_cb(lv_event_t *e)
{
    open_info_writeup();
}

static void help_writeup_cb(lv_event_t *e)
{
    open_help_writeup();
}

static void tutorial_cb(lv_event_t *e)
{
    open_tutorial_writeup();
}

static void writeup_back_cb(lv_event_t *e)
{
    open_info_screen();
}

static void new_proj_pressed_cb(lv_event_t *e)
{
    open_new_proj_screen();
}

static void demo_pressed_cb(lv_event_t *e)
{
    open_demo_screen();
}

static void demo_pen_selected_cb(lv_event_t *e)
{
    open_demo_pen_popup();
    char *text = lv_event_get_user_data(e);
    lv_label_set_text(main_text_demo_popup, text);
    char *description = "Basic description filler";
    // Add code to change description text here
    if (text == "Line")
    {
        description = "This program draws a simple line. It demonstrates consistent speed and direction.";
        memset(&demo_file_path_and_name[0], 0, sizeof(demo_file_path_and_name));
        memset(&demo_file_name[0], 0, sizeof(demo_file_name));
        snprintf(demo_file_path_and_name, sizeof(demo_file_path_and_name), "~/PG_Demos/LINE.ngc");
        snprintf(demo_file_name, sizeof(demo_file_name), "LINE.ngc");
        lv_label_set_text(sub_text_demo_popup, description);
    }
    else if (text == "Square")
    {
        description = "This programs draws a square. It demonstrates dimensional accuracy and the ability to stop and change direction in sharp angles.";
        memset(&demo_file_path_and_name[0], 0, sizeof(demo_file_path_and_name));
        memset(&demo_file_name[0], 0, sizeof(demo_file_name));
        snprintf(demo_file_path_and_name, sizeof(demo_file_path_and_name), "~/PG_Demos/square.ngc");
        snprintf(demo_file_name, sizeof(demo_file_name), "square.ngc");
        lv_label_set_text(sub_text_demo_popup, description);
    }
    else if (text == "Circle")
    {
        description = "This program draws a circle. It demonstrates the ability to draw a consistent arc.";
        memset(&demo_file_path_and_name[0], 0, sizeof(demo_file_path_and_name));
        memset(&demo_file_name[0], 0, sizeof(demo_file_name));
        snprintf(demo_file_path_and_name, sizeof(demo_file_path_and_name), "~/PG_Demos/CIRCLE.ngc");
        snprintf(demo_file_name, sizeof(demo_file_name), "CIRCLE.ngc");
        lv_label_set_text(sub_text_demo_popup, description);
    }
    else if (text == "Oval")
    {
        description = "This program draws an oval. It demonstrates the ability to transition smoothly between arcs with different radii.";
        memset(&demo_file_path_and_name[0], 0, sizeof(demo_file_path_and_name));
        memset(&demo_file_name[0], 0, sizeof(demo_file_name));
        snprintf(demo_file_path_and_name, sizeof(demo_file_path_and_name), "~/PG_Demos/OVAL.ngc");
        snprintf(demo_file_name, sizeof(demo_file_name), "OVAL.ngc");
        lv_label_set_text(sub_text_demo_popup, description);
    }
    else if (text == "Star")
    {
        description = "This program draws star. It demonstrates many straight lines and quick direction changes.";
        memset(&demo_file_path_and_name[0], 0, sizeof(demo_file_path_and_name));
        memset(&demo_file_name[0], 0, sizeof(demo_file_name));
        snprintf(demo_file_path_and_name, sizeof(demo_file_path_and_name), "~/PG_Demos/STAR.ngc");
        snprintf(demo_file_name, sizeof(demo_file_name), "STAR.ngc");
        lv_label_set_text(sub_text_demo_popup, description);
    }
    else // Complex
    {
        description = "This is a complex example that contains elements of all other demos, including curves, sharp changes in direction, and straight lines.";
        lv_label_set_text(sub_text_demo_popup, description);
    }
}

static void demo_laser_selected_cb(lv_event_t *e)
{
    open_demo_laser_popup();
    char *text = lv_event_get_user_data(e);
    lv_label_set_text(main_text_demo_popup, text);
    char *description = "Basic description filler";
    // Add code to change description text here
    if (text == "Line")
    {
        description = "This program draws a simple line. It demonstrates consistent speed and direction.";
        memset(&demo_file_path_and_name[0], 0, sizeof(demo_file_path_and_name));
        memset(&demo_file_name[0], 0, sizeof(demo_file_name));
        snprintf(demo_file_path_and_name, sizeof(demo_file_path_and_name), "~/PG_Demos/LINE.ngc");
        snprintf(demo_file_name, sizeof(demo_file_name), "LINE.ngc");
        lv_label_set_text(sub_text_demo_popup, description);
    }
    else if (text == "Square")
    {
        description = "This programs draws a square. It demonstrates dimensional accuracy and the ability to stop and change direction in sharp angles.";
        memset(&demo_file_path_and_name[0], 0, sizeof(demo_file_path_and_name));
        memset(&demo_file_name[0], 0, sizeof(demo_file_name));
        snprintf(demo_file_path_and_name, sizeof(demo_file_path_and_name), "~/PG_Demos/square.ngc");
        snprintf(demo_file_name, sizeof(demo_file_name), "square.ngc");
        lv_label_set_text(sub_text_demo_popup, description);
    }
    else if (text == "Circle")
    {
        description = "This program draws a circle. It demonstrates the ability to draw a consistent arc.";
        memset(&demo_file_path_and_name[0], 0, sizeof(demo_file_path_and_name));
        memset(&demo_file_name[0], 0, sizeof(demo_file_name));
        snprintf(demo_file_path_and_name, sizeof(demo_file_path_and_name), "~/PG_Demos/CIRCLE.ngc");
        snprintf(demo_file_name, sizeof(demo_file_name), "CIRCLE.ngc");
        lv_label_set_text(sub_text_demo_popup, description);
    }
    else if (text == "Oval")
    {
        description = "This program draws an oval. It demonstrates the ability to transition smoothly between arcs with different radii.";
        memset(&demo_file_path_and_name[0], 0, sizeof(demo_file_path_and_name));
        memset(&demo_file_name[0], 0, sizeof(demo_file_name));
        snprintf(demo_file_path_and_name, sizeof(demo_file_path_and_name), "~/PG_Demos/OVAL.ngc");
        snprintf(demo_file_name, sizeof(demo_file_name), "OVAL.ngc");
        lv_label_set_text(sub_text_demo_popup, description);
    }
    else if (text == "Star")
    {
        description = "This program draws star. It demonstrates many straight lines and quick direction changes.";
        memset(&demo_file_path_and_name[0], 0, sizeof(demo_file_path_and_name));
        memset(&demo_file_name[0], 0, sizeof(demo_file_name));
        snprintf(demo_file_path_and_name, sizeof(demo_file_path_and_name), "~/PG_Demos/STAR.ngc");
        snprintf(demo_file_name, sizeof(demo_file_name), "STAR.ngc");
        lv_label_set_text(sub_text_demo_popup, description);
    }
    else // Complex
    {
        description = "This is a complex example that contains elements of all other demos, including curves, sharp changes in direction, and straight lines.";
        lv_label_set_text(sub_text_demo_popup, description);
    }
}

static void back_pressed_cb(lv_event_t *e)
{
    open_start_screen();
}

static void back_pen_laser_pressed_cb(lv_event_t *e)
{
    open_demo_screen();
}

static void back_demo_pen_cb(lv_event_t *e)
{
    open_demo_pen_screen();
}

static void back_demo_laser_cb(lv_event_t *e)
{
    open_demo_laser_screen();
}

static void usb_file_explorer_cb(lv_event_t *e)
{
    open_usb_explorer();
}

static void cloud_file_explorer_cb(lv_event_t *e)
{
    open_cloud_explorer();
}

static void file_selected_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if ((lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) && (lv_event_get_code(e) != LV_EVENT_READY))
    {
        file_path = lv_file_explorer_get_current_path(obj);
        file_name = lv_file_explorer_get_selected_file_name(obj);
        memset(&file_path_and_name[0], 0, sizeof(file_path_and_name));
        strcat(file_path_and_name, file_path);
        strcat(file_path_and_name, file_name);
        open_file_confirm_screen();
        lv_label_set_text(filepath_file_confirm, file_path_and_name);
        lv_obj_align_to(filepath_file_confirm, main_label_file_confirm, LV_ALIGN_OUT_BOTTOM_MID, 0, 100);
    }
}

static void quit_cb(lv_event_t *e)
{
    lv_sdl_quit();
    pid_t pid = getpid();
    kill(pid, SIGINT);
}

static void sidebar_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *file_explorer = lv_event_get_user_data(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        if (lv_obj_has_state(btn, LV_STATE_CHECKED))
        {
            lv_obj_add_flag(file_explorer, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_remove_flag(file_explorer, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void start_demo_program_cb(lv_event_t *e)
{
    open_program_running();
    char cmd_1[255];
    snprintf(cmd_1, sizeof(cmd_1), "cp %s /home/PortaGuide/", demo_file_path_and_name);
    system(cmd_1);
    char cmd_2[255];
    snprintf(cmd_2, sizeof(cmd_2), "cp %s output_file.ngc", demo_file_name);
    system(cmd_2);
    char cmd_3[255];
    snprintf(cmd_3, sizeof(cmd_3), "rm %s", demo_file_name);
    system(cmd_3);
    system("scp output_file.ngc cnc@10.0.0.20:/home/cnc/Desktop/PortaGuide/");
    lv_delay_ms(200);
    gpio_write(pi_num, GPIO_START_OUT, PI_HIGH); // Tell other Pi that program is starting
}

static void start_program_cb(lv_event_t *e)
{
    open_program_running();
    char cmd_1[255];
    char *new_file_path_and_name = &file_path_and_name[2];
    snprintf(cmd_1, sizeof(cmd_1), "cp %s /home/PortaGuide/", new_file_path_and_name);
    system(cmd_1);
    char cmd_2[255];
    snprintf(cmd_2, sizeof(cmd_2), "cp %s output_file.ngc", file_name);
    system(cmd_2);
    char cmd_3[255];
    snprintf(cmd_3, sizeof(cmd_3), "rm %s", file_name);
    system(cmd_3);
    system("scp output_file.ngc cnc@10.0.0.20:/home/cnc/Desktop/PortaGuide/");
    lv_delay_ms(200);
    gpio_write(pi_num, GPIO_START_OUT, PI_HIGH); // Tell other Pi that program is starting
}

static void quit_program_cb(lv_event_t *e)
{
    gpio_write(pi_num, GPIO_E_STOP, PI_HIGH); // Activate E-Stop
    gpio_write(pi_num, GPIO_START_OUT, PI_LOW);
    gpio_write(pi_num, GPIO_HOMING, PI_HIGH);
    open_homing_screen();
}