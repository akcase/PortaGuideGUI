#include "lv_port_linux/lvgl/lvgl.h"
#include "images/cloud_symbol.c"
#include "images/usb_symbol.c"

/***********
 * Defines *
 ***********/

#define COLOR_PALETTE LV_PALETTE_LIGHT_BLUE
#define BACK_BTN_HORZ 100
#define BACK_BTN_VERT 75

LV_IMAGE_DECLARE(cloud_symbol);
LV_IMAGE_DECLACE(usb_symbol);

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
static int32_t col_info_screen[] = {LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_info_screen[] = {LV_GRID_FR(1), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

/**
 * Demo Selection Screen
 *
 * This screen will be opened when "Demos" is selected from the Start
 * Screen. On the screen will be buttons, one for each demo program we
 * have made. Selecting a button will open a popup window (darken the
 * background) with a short description of the demo, an option to start
 * the program, and an option to go back.
 */

/* Screen */
static lv_obj_t *demo_screen;
static lv_style_t style_demo_screen;
/* Main Text */
static lv_obj_t *main_label_demo_screen;
static lv_style_t style_main_label_demo_screen;
/* Demo Button 1 */
static lv_obj_t *demo_btn_1_demo_screen;
static lv_obj_t *demo_label_1_demo_screen;
/* Demo Button 2 */
static lv_obj_t *demo_btn_2_demo_screen;
static lv_obj_t *demo_label_2_demo_screen;
/* Demo Button 3 */
static lv_obj_t *demo_btn_3_demo_screen;
static lv_obj_t *demo_label_3_demo_screen;
/* Demo Button 4 */
static lv_obj_t *demo_btn_4_demo_screen;
static lv_obj_t *demo_label_4_demo_screen;
/* Demo Button 5 */
static lv_obj_t *demo_btn_5_demo_screen;
static lv_obj_t *demo_label_5_demo_screen;
/* Demo Button 6 */
static lv_obj_t *demo_btn_6_demo_screen;
static lv_obj_t *demo_label_6_demo_screen;
/* Back Button */
static lv_obj_t *back_btn_demo_screen;
static lv_obj_t *back_label_demo_screen;
/* Styles */
static lv_style_t style_btn_demo_screen;
static lv_style_t style_label_demo_screen;
/* Layout */
static int32_t col_demo_screen[] = {LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_FR(8), LV_GRID_FR(1), LV_GRID_FR(8), LV_GRID_FR(1), LV_GRID_FR(8), LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_demo_screen[] = {LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(1), LV_GRID_FR(5), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};

/**
 * Demo Selection Popup
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
static lv_obj_t *demo_popup;
static lv_style_t style_demo_popup;
/* Main Text */
static lv_obj_t *main_text_demo_popup;
static lv_style_t style_main_text_demo_popup;
/* Subtext */
static lv_obj_t *sub_text_demo_popup;
static lv_style_t style_sub_text_demo_popup;
/* Back Button */
static lv_obj_t *back_btn_demo_popup;
static lv_style_t style_back_btn_demo_popup;
static lv_obj_t *back_label_demo_popup;
/* Start Button */
static lv_obj_t *start_btn_demo_popup;
static lv_style_t style_start_btn_demo_popup;
static lv_obj_t *start_label_demo_popup;
/* Layout */
static int32_t col_demo_popup[] = {LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_FR(9), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static int32_t row_demo_popup[] = {LV_GRID_FR(1), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(6), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

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

/************************
 * Function Declaration *
 ************************/

/***** Functions for all Screens *****/

/**
 * Initialize the styles for the window objects
 */
void style_init();

/**
 * Helper function used to darken a button when pressed
 */
static lv_color_t darken(const lv_color_filter_dsc_t *dsc, lv_color_t color, lv_opa_t opa);

/***** Start Screen Functions *****/

void open_start_screen();

void config_start_screen();

static void info_pressed_cb(lv_event_t *e);

static void new_proj_pressed_cb(lv_event_t *e);

static void demo_pressed_cb(lv_event_t *e);

static void demo_selected_cb(lv_event_t *e);

static void back_pressed_cb(lv_event_t *e);

static void back_demo_cb(lv_event_t *e);

static void usb_file_explorer_cb(lv_event_t *e);

static void cloud_file_explorer_cb(lv_event_t *e);

static void file_selected_cb(lv_event_t *e);

/***** Demo Screen Functions *****/

void open_demo_screen();

void config_demo_screen();

/***** Demo Popup Functions *****/

void open_demo_popup();

void config_demo_popup();

/***** Info Screen Functions *****/

void open_info_screen();

void config_info_screen();

/***** New Project Screen Functions *****/

void open_new_proj_screen();

void config_new_proj_screen();

/***** File Explorers *****/

void open_usb_explorer();

void open_cloud_explorer();

void config_usb_explorer();

void config_cloud_explorer();

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
    lv_style_set_text_color(&style_main_label_start_screen, lv_palette_lighten(COLOR_PALETTE, 5));

    /* Style for buttons */
    lv_style_init(&style_btn_start_screen);
    lv_style_set_bg_color(&style_btn_start_screen, lv_palette_lighten(COLOR_PALETTE, 5));
    lv_style_set_border_width(&style_btn_start_screen, 0);
    lv_style_set_radius(&style_btn_start_screen, 4);

    /* Style for labels */
    lv_style_init(&style_label_start_screen);
    lv_style_set_text_font(&style_label_start_screen, &lv_font_montserrat_34);
    lv_style_set_text_color(&style_label_start_screen, lv_palette_darken(COLOR_PALETTE, 5));
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
    lv_style_set_text_color(&style_main_label_info_screen, lv_palette_lighten(COLOR_PALETTE, 5));

    /* Style for subtext */
    lv_style_init(&style_sub_label_info_screen);
    lv_style_set_text_font(&style_sub_label_info_screen, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_sub_label_info_screen, lv_palette_lighten(COLOR_PALETTE, 5));

    /* Style for buttons */
    lv_style_init(&style_btn_info_screen);
    lv_style_set_bg_color(&style_btn_info_screen, lv_palette_lighten(COLOR_PALETTE, 5));
    lv_style_set_border_width(&style_btn_info_screen, 0);
    lv_style_set_radius(&style_btn_info_screen, 4);

    /* Style for labels */
    lv_style_init(&style_label_info_screen);
    lv_style_set_text_font(&style_label_info_screen, &lv_font_montserrat_34);
    lv_style_set_text_color(&style_label_info_screen, lv_palette_darken(COLOR_PALETTE, 5));
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
    lv_style_set_text_color(&style_main_label_demo_screen, lv_palette_lighten(COLOR_PALETTE, 5));

    /* Style for buttons */
    lv_style_init(&style_btn_demo_screen);
    lv_style_set_bg_color(&style_btn_demo_screen, lv_palette_lighten(COLOR_PALETTE, 5));
    lv_style_set_border_width(&style_btn_demo_screen, 0);
    lv_style_set_radius(&style_btn_demo_screen, 4);

    /* Style for labels */
    lv_style_init(&style_label_demo_screen);
    lv_style_set_text_font(&style_label_demo_screen, &lv_font_montserrat_34);
    lv_style_set_text_color(&style_label_demo_screen, lv_palette_darken(COLOR_PALETTE, 5));
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
    lv_style_set_text_color(&style_main_text_demo_popup, lv_palette_lighten(COLOR_PALETTE, 5));

    /* Style for subtext */
    lv_style_init(&style_sub_text_demo_popup);
    lv_style_set_text_font(&style_sub_text_demo_popup, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_sub_text_demo_popup, lv_palette_lighten(COLOR_PALETTE, 5));

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
    lv_style_set_text_color(&style_main_label_new_proj_screen, lv_palette_lighten(COLOR_PALETTE, 5));

    /* Style for file explorer button */
    lv_style_init(&style_file_btn_new_proj_screen);
    lv_style_set_bg_color(&style_file_btn_new_proj_screen, lv_palette_lighten(LV_PALETTE_GREY, 5));
    lv_style_set_border_width(&style_file_btn_new_proj_screen, 0);
    lv_style_set_radius(&style_file_btn_new_proj_screen, 4);

    /* Style for file explorer label */
    lv_style_init(&style_file_label_new_proj_screen);
    lv_style_set_text_font(&style_file_label_new_proj_screen, &lv_font_montserrat_30);
    lv_style_set_text_color(&style_file_label_new_proj_screen, lv_palette_darken(COLOR_PALETTE, 5));

    /* Style for buttons */
    lv_style_init(&style_btn_new_proj_screen);
    lv_style_set_bg_color(&style_btn_new_proj_screen, lv_palette_lighten(COLOR_PALETTE, 5));
    lv_style_set_border_width(&style_btn_new_proj_screen, 0);
    lv_style_set_radius(&style_btn_new_proj_screen, 4);

    /* Style for labels */
    lv_style_init(&style_label_new_proj_screen);
    lv_style_set_text_font(&style_label_new_proj_screen, &lv_font_montserrat_34);
    lv_style_set_text_color(&style_label_new_proj_screen, lv_palette_darken(COLOR_PALETTE, 5));
}

void style_init()
{
    start_screen_style_init();

    info_screen_style_init();

    demo_screen_style_init();

    demo_popup_style_init();

    new_proj_screen_style_init();

    /****************
     * Other Styles *
     ****************/

    /* Style for back button */
    lv_style_init(&style_back_label);
    lv_style_set_text_font(&style_back_label, &lv_font_montserrat_18);
    lv_style_set_text_color(&style_back_label, lv_palette_darken(COLOR_PALETTE, 5));

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
}

void open_info_screen()
{
    lv_screen_load(info_screen);
}

void open_demo_screen()
{
    lv_screen_load(demo_screen);
}

void open_demo_popup()
{
    lv_screen_load(demo_popup);
}

void open_new_proj_screen()
{
    lv_screen_load(new_proj_screen);
}

void open_usb_explorer()
{
    lv_screen_load(usb_file_explorer);
}

void open_cloud_explorer()
{
    lv_screen_load(cloud_file_explorer);
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
    lv_label_set_text(sub_label_info_screen, "This is a brief description of what this screen is. Here, you can find information about what PortaGuide is and how it was created, help guides to make using PortaGuide easier, as well as a tutorial to see the entire process from start to finish.");
    lv_label_set_long_mode(sub_label_info_screen, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(sub_label_info_screen, 750);
    lv_obj_set_grid_cell(sub_label_info_screen, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_START, 2, 1);
    /* Info Button */
    info_btn_info_screen = lv_obj_create(info_screen);
    lv_obj_add_style(info_btn_info_screen, &style_btn_info_screen, 0);
    lv_obj_add_style(info_btn_info_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(info_btn_info_screen, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    info_label_info_screen = lv_label_create(info_btn_info_screen);
    lv_obj_add_style(info_label_info_screen, &style_label_info_screen, 0);
    lv_label_set_text(info_label_info_screen, "Info");
    lv_obj_center(info_label_info_screen);
    /* Help Button */
    help_btn_info_screen = lv_obj_create(info_screen);
    lv_obj_add_style(help_btn_info_screen, &style_btn_info_screen, 0);
    lv_obj_add_style(help_btn_info_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(help_btn_info_screen, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    help_label_info_screen = lv_label_create(help_btn_info_screen);
    lv_obj_add_style(help_label_info_screen, &style_label_info_screen, 0);
    lv_label_set_text(help_label_info_screen, "Help");
    lv_obj_center(help_label_info_screen);
    /* Tutorial Button */
    tutorial_btn_info_screen = lv_obj_create(info_screen);
    lv_obj_add_style(tutorial_btn_info_screen, &style_btn_info_screen, 0);
    lv_obj_add_style(tutorial_btn_info_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(tutorial_btn_info_screen, LV_GRID_ALIGN_STRETCH, 5, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    tutorial_label_info_screen = lv_label_create(tutorial_btn_info_screen);
    lv_obj_add_style(tutorial_label_info_screen, &style_label_info_screen, 0);
    lv_label_set_text(tutorial_label_info_screen, "Tutorial");
    lv_obj_center(tutorial_label_info_screen);
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
    lv_obj_set_grid_cell(main_label_demo_screen, LV_GRID_ALIGN_CENTER, 2, 5, LV_GRID_ALIGN_CENTER, 1, 1);
    /* Demo Button 1 */
    demo_btn_1_demo_screen = lv_obj_create(demo_screen);
    lv_obj_add_style(demo_btn_1_demo_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_1_demo_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_1_demo_screen, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(demo_btn_1_demo_screen, demo_selected_cb, LV_EVENT_CLICKED, "Line");
    demo_label_1_demo_screen = lv_label_create(demo_btn_1_demo_screen);
    lv_obj_add_style(demo_label_1_demo_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_1_demo_screen, "Line");
    lv_obj_center(demo_label_1_demo_screen);
    /* Demo Button 2 */
    demo_btn_2_demo_screen = lv_obj_create(demo_screen);
    lv_obj_add_style(demo_btn_2_demo_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_2_demo_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_2_demo_screen, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(demo_btn_2_demo_screen, demo_selected_cb, LV_EVENT_CLICKED, "Square");
    demo_label_2_demo_screen = lv_label_create(demo_btn_2_demo_screen);
    lv_obj_add_style(demo_label_2_demo_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_2_demo_screen, "Square");
    lv_obj_center(demo_label_2_demo_screen);
    /* Demo Button 3 */
    demo_btn_3_demo_screen = lv_obj_create(demo_screen);
    lv_obj_add_style(demo_btn_3_demo_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_3_demo_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_3_demo_screen, LV_GRID_ALIGN_STRETCH, 6, 1, LV_GRID_ALIGN_STRETCH, 3, 1);
    lv_obj_add_event_cb(demo_btn_3_demo_screen, demo_selected_cb, LV_EVENT_CLICKED, "Circle");
    demo_label_3_demo_screen = lv_label_create(demo_btn_3_demo_screen);
    lv_obj_add_style(demo_label_3_demo_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_3_demo_screen, "Circle");
    lv_obj_center(demo_label_3_demo_screen);
    /* Demo Button 4 */
    demo_btn_4_demo_screen = lv_obj_create(demo_screen);
    lv_obj_add_style(demo_btn_4_demo_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_4_demo_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_4_demo_screen, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(demo_btn_4_demo_screen, demo_selected_cb, LV_EVENT_CLICKED, "Oval");
    demo_label_4_demo_screen = lv_label_create(demo_btn_4_demo_screen);
    lv_obj_add_style(demo_label_4_demo_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_4_demo_screen, "Oval");
    lv_obj_center(demo_label_4_demo_screen);
    /* Demo Button 5 */
    demo_btn_5_demo_screen = lv_obj_create(demo_screen);
    lv_obj_add_style(demo_btn_5_demo_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_5_demo_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_5_demo_screen, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(demo_btn_5_demo_screen, demo_selected_cb, LV_EVENT_CLICKED, "Signature");
    demo_label_5_demo_screen = lv_label_create(demo_btn_5_demo_screen);
    lv_obj_add_style(demo_label_5_demo_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_5_demo_screen, "Signature");
    lv_obj_center(demo_label_5_demo_screen);
    /* Demo Button 6 */
    demo_btn_6_demo_screen = lv_obj_create(demo_screen);
    lv_obj_add_style(demo_btn_6_demo_screen, &style_btn_demo_screen, 0);
    lv_obj_add_style(demo_btn_6_demo_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(demo_btn_6_demo_screen, LV_GRID_ALIGN_STRETCH, 6, 1, LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(demo_btn_6_demo_screen, demo_selected_cb, LV_EVENT_CLICKED, "Complex");
    demo_label_6_demo_screen = lv_label_create(demo_btn_6_demo_screen);
    lv_obj_add_style(demo_label_6_demo_screen, &style_label_demo_screen, 0);
    lv_label_set_text(demo_label_6_demo_screen, "Complex");
    lv_obj_center(demo_label_6_demo_screen);
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

void config_demo_popup()
{
    /* Configure Screen */
    demo_popup = lv_obj_create(NULL);
    lv_obj_set_size(demo_popup, 1024, 600);
    lv_obj_center(demo_popup);
    lv_obj_add_style(demo_popup, &style_demo_popup, 0);
    /* Set grid layout type */
    lv_obj_set_grid_dsc_array(demo_popup, col_demo_popup, row_demo_popup);
    /* Remove padding around all objects to fit them closer together */
    lv_obj_set_style_pad_all(demo_popup, 0, 0);
    lv_obj_set_style_pad_column(demo_popup, 0, 0);
    lv_obj_set_style_pad_row(demo_popup, 0, 0);
    /* Main Text */
    main_text_demo_popup = lv_label_create(demo_popup);
    lv_obj_add_style(main_text_demo_popup, &style_main_text_demo_popup, 0);
    lv_obj_set_grid_cell(main_text_demo_popup, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_CENTER, 1, 1);
    /* Subtext */
    sub_text_demo_popup = lv_label_create(demo_popup);
    lv_obj_add_style(sub_text_demo_popup, &style_sub_text_demo_popup, 0);
    lv_obj_set_width(sub_text_demo_popup, 700);
    lv_obj_set_grid_cell(sub_text_demo_popup, LV_GRID_ALIGN_CENTER, 1, 5, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_label_set_text(sub_text_demo_popup, "This is an example description");
    lv_label_set_long_mode(sub_label_info_screen, LV_LABEL_LONG_WRAP);
    /* Back Button */
    back_btn_demo_popup = lv_obj_create(demo_popup);
    lv_obj_add_style(back_btn_demo_popup, &style_back_btn_demo_popup, 0);
    lv_obj_add_style(back_btn_demo_popup, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(back_btn_demo_popup, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
    lv_obj_set_size(back_btn_demo_popup, BACK_BTN_HORZ, BACK_BTN_VERT);
    lv_obj_add_event_cb(back_btn_demo_popup, back_demo_cb, LV_EVENT_CLICKED, NULL);
    back_label_demo_popup = lv_label_create(back_btn_demo_popup);
    lv_obj_add_style(back_label_demo_popup, &style_back_label, 0);
    lv_label_set_text(back_label_demo_popup, "Back");
    lv_obj_center(back_label_demo_popup);
    /* Start Button */
    start_btn_demo_popup = lv_obj_create(demo_popup);
    lv_obj_add_style(start_btn_demo_popup, &style_start_btn_demo_popup, 0);
    lv_obj_add_style(start_btn_demo_popup, &style_start_btn_demo_popup, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(start_btn_demo_popup, LV_GRID_ALIGN_END, 5, 1, LV_GRID_ALIGN_END, 3, 1);
    lv_obj_set_size(start_btn_demo_popup, BACK_BTN_HORZ, BACK_BTN_VERT);
    start_label_demo_popup = lv_label_create(start_btn_demo_popup);
    lv_obj_add_style(start_label_demo_popup, &style_back_label, 0);
    lv_label_set_text(start_label_demo_popup, "Start");
    lv_obj_center(start_label_demo_popup);
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
    lv_obj_set_grid_cell(usb_file_btn_new_proj_screen, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(usb_file_btn_new_proj_screen, usb_file_explorer_cb, LV_EVENT_CLICKED, NULL);
    usb_file_image_new_proj_screen = lv_image_create(usb_file_btn_new_proj_screen);
    lv_image_set_src(usb_file_image_new_proj_screen, &usb_symbol);
    lv_obj_center(usb_file_image_new_proj_screen);
    usb_file_label_new_proj_screen = lv_label_create(usb_file_btn_new_proj_screen);
    lv_obj_add_style(usb_file_label_new_proj_screen, &style_file_label_new_proj_screen, 0);
    lv_obj_set_align(usb_file_label_new_proj_screen, LV_ALIGN_BOTTOM_MID);
    lv_label_set_text(usb_file_label_new_proj_screen, "Select from USB");
    /* Cloud File Explorer Button */
    cloud_file_btn_new_proj_screen = lv_obj_create(new_proj_screen);
    lv_obj_add_style(cloud_file_btn_new_proj_screen, &style_file_btn_new_proj_screen, 0);
    lv_obj_add_style(cloud_file_btn_new_proj_screen, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_grid_cell(cloud_file_btn_new_proj_screen, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(cloud_file_btn_new_proj_screen, cloud_file_explorer_cb, LV_EVENT_CLICKED, NULL);
    cloud_file_image_new_proj_screen = lv_image_create(cloud_file_btn_new_proj_screen);
    lv_image_set_src(cloud_file_image_new_proj_screen, &cloud_symbol);
    lv_obj_center(cloud_file_image_new_proj_screen);
    cloud_file_label_new_proj_screen = lv_label_create(cloud_file_btn_new_proj_screen);
    lv_obj_add_style(cloud_file_label_new_proj_screen, &style_file_label_new_proj_screen, 0);
    lv_obj_set_align(cloud_file_label_new_proj_screen, LV_ALIGN_BOTTOM_MID);
    lv_label_set_text(cloud_file_label_new_proj_screen, "Select from Cloud");
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

void config_usb_explorer()
{
    usb_file_explorer = lv_file_explorer_create(NULL);
    lv_file_explorer_set_sort(usb_file_explorer, LV_EXPLORER_SORT_KIND);
    lv_file_explorer_open_dir(usb_file_explorer, "aidancase/");
}

void config_cloud_explorer()
{
    cloud_file_explorer = lv_file_explorer_create(NULL);
}

/*************
 * Callbacks *
 *************/

static void info_pressed_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        open_info_screen();
    }
}

static void new_proj_pressed_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        open_new_proj_screen();
    }
}

static void demo_pressed_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        open_demo_screen();
    }
}

static void demo_selected_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        open_demo_popup();
        char *text = lv_event_get_user_data(e);
        lv_label_set_text(main_text_demo_popup, text);
        char *description = "Basic description filler";
        // Add code to change description text here
        if (text == "Line")
        {
            description = "This program draws a simple line. It demonstrates consistent speed and direction.";
            lv_label_set_text(sub_text_demo_popup, description);
        }
        else if (text == "Square")
        {
            description = "This programs draws a square. It demonstrates dimensional accuracy and the ability to stop and change direction in sharp angles.";
            lv_label_set_text(sub_text_demo_popup, description);
        }
        else if (text == "Circle")
        {
            description = "This program draws a circle. It demonstrates the ability to draw a consistent arc.";
            lv_label_set_text(sub_text_demo_popup, description);
        }
        else if (text == "Oval")
        {
            description = "This program draws an oval. It demonstrates the ability to transition smoothly between arcs with different radii.";
            lv_label_set_text(sub_text_demo_popup, description);
        }
        else if (text == "Signature")
        {
            description = "This program draws a basic signature. It demonstrates smooth lines with complex geometry.";
            lv_label_set_text(sub_text_demo_popup, description);
        }
        else // Complex
        {
            description = "This is a complex example that contains elements of all other demos, including curves, sharp changes in direction, and straight lines.";
            lv_label_set_text(sub_text_demo_popup, description);
        }
    }
}

static void back_pressed_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        open_start_screen();
    }
}

static void back_demo_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        lv_screen_load(demo_screen);
    }
}

static void usb_file_explorer_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        open_usb_explorer();
    }
}

static void cloud_file_explorer_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        open_cloud_explorer();
    }
}

static void file_selected_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
    }
}