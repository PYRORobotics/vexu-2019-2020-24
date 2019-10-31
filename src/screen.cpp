#include "screen.h"

//==================================START FILE==================================
//==============================================================================
// File:		screen.cpp
// Author:	Brandon Rice
// Created: 17 October 2019
// Last Modified: 20 October 2019
//
// Description:
// ------------
// This file contains the definitions of the Piston class.
//
// NOTE: TO EXPORT IMAGES TO SD CARD
//
// https://www.vexforum.com/t/lvgl-image-not-displaying/63612/15
//
// Update 9/27/2019
// For PROS kernel 3.2.0, which contains lvgl version 5.3, the new (or is that current) image converter should be used.
// Select “True color with Alpha” for the color format
// Select “Binary RGB888” for the output format
//------------------------------------------------------------------------------





/* Colors */
lv_color_t Screen::COLOR_ASU_MAROON;
lv_color_t Screen::COLOR_ASU_GOLD;

/* Styles */
lv_style_t Screen::style_btn_main;
lv_style_t Screen::style_bg;

/* Objects */
lv_obj_t *Screen::Screen_Title_Page_Main;
lv_obj_t *Screen::Screen_Title_PYRO;
lv_obj_t *Screen::Screen_Title_Btn_Continue;
lv_obj_t *Screen::Screen_Generic_Label;

lv_obj_t *Screen::Screen_Main_BG;

lv_obj_t *Screen::Screen_Mode_Select_Main;
lv_obj_t *Screen::Screen_Mode_Select_Comp;
lv_obj_t *Screen::Screen_Mode_Select_Comp_Label;
lv_obj_t *Screen::Screen_Mode_Select_Debug;
lv_obj_t *Screen::Screen_Mode_Select_Debug_Label;

lv_obj_t *Screen::Screen_CompSelect_Back;
lv_obj_t *Screen::Screen_CompSelect_Back_Label;
lv_obj_t *Screen::Screen_Battery_Percent_Label;



/* Button Action Functions */
//static lv_res_t button_title_continue_action(lv_obj_t * btn);



//Screen screen;




typedef  FILE * pc_file_t;

static lv_fs_res_t pcfs_open( void * file_p, const char * fn, lv_fs_mode_t mode)
{
    errno = 0;
    const char * flags = "";
    if(mode == LV_FS_MODE_WR) flags = "wb";
    else if(mode == LV_FS_MODE_RD) flags = "rb";
    else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) flags = "a+";

    char buf[256];
    sprintf(buf, "/%s", fn);
    pc_file_t f = fopen(buf, flags);

    if(f == NULL)
      return LV_FS_RES_UNKNOWN;
    else {
      fseek(f, 0, SEEK_SET);
      pc_file_t * fp = (pc_file_t *)file_p;
      *fp = f;
    }

    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_close( void * file_p)
{
    pc_file_t * fp = (pc_file_t *)file_p;
    fclose(*fp);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_read( void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    pc_file_t * fp =  (pc_file_t *)file_p;
    *br = fread(buf, 1, btr, *fp);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_seek( void * file_p, uint32_t pos)
{
    pc_file_t * fp = (pc_file_t *)file_p;
    fseek(*fp, pos, SEEK_SET);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_tell( void * file_p, uint32_t * pos_p)
{
    pc_file_t * fp =  (pc_file_t *)file_p;
    *pos_p = ftell(*fp);
    return LV_FS_RES_OK;
}


// Class Defintions
// ----------------

//------------------------------------------------------------------------------
// Method: PYROChassis() :
// ----------------------------
// Description:
// 		Constructs a PYROChassis object containing a Position PID Controller and
//    other okapi chassis objects.
//
// Parameters:
//```
//		NONE
//```
// Objects to Initialize:
//```
//		PositionPIDController (PIDController),
//    left_motors (okapi::MotorGroup),
//    right_motors (okapi::MotorGroup),
//    driveController (okapi::ChassisControllerPID),
//    MotionController (okapi::AsyncMotionProfileController)
//    N/A: t_update_differential_pos (pros::Task)
//```
//------------------------------------------------------------------------------
void Screen::setup_styles()
{

  COLOR_ASU_MAROON.red = 140;
  COLOR_ASU_MAROON.green = 15;
  COLOR_ASU_MAROON.blue = 32;

  COLOR_ASU_GOLD.red = 255;
  COLOR_ASU_GOLD.green = 198;
  COLOR_ASU_GOLD.blue = 39;


  lv_style_copy(&Screen::style_btn_main, &lv_style_btn_rel);
  style_btn_main.body.main_color = LV_COLOR_BLACK;
  style_btn_main.body.grad_color = LV_COLOR_BLACK;
  style_btn_main.text.color = LV_COLOR_WHITE;
  style_btn_main.body.border.color = COLOR_ASU_GOLD;
  style_btn_main.body.border.width = 3;
  style_btn_main.body.padding.hor = 5;
  style_btn_main.body.padding.ver = 5;
  style_btn_main.body.radius = 3;

  lv_style_copy(&Screen::style_bg, &lv_style_plain_color);
  Screen::style_bg.body.main_color = LV_COLOR_BLACK;
  Screen::style_bg.body.grad_color = LV_COLOR_BLACK;

}

Screen::Screen() : t_screen (pros::Task(update, (void*)NULL, TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "My Task"))
{
  setup_styles();

  lv_fs_drv_t pcfs_drv;                         /*A driver descriptor*/
  memset(&pcfs_drv, 0, sizeof(lv_fs_drv_t));    /*Initialization*/

  pcfs_drv.file_size = sizeof(pc_file_t);       /*Set up fields...*/
  pcfs_drv.letter = 'D';
  pcfs_drv.open = pcfs_open;
  pcfs_drv.close = pcfs_close;
  pcfs_drv.read = pcfs_read;
  pcfs_drv.seek = pcfs_seek;
  pcfs_drv.tell = pcfs_tell;
  lv_fs_add_drv(&pcfs_drv);

  pros::delay(20);



  Screen_Title_Page_Main = lv_page_create(lv_scr_act(), NULL);
  lv_obj_set_size(Screen_Title_Page_Main, 480, 240);
  lv_obj_align(Screen_Title_Page_Main, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_page_set_style(Screen_Title_Page_Main, LV_PAGE_STYLE_BG, &Screen::style_bg);

  Screen_Title_PYRO = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(Screen_Title_PYRO, "D:/usd/img_pyro_large.bin");
  lv_obj_align(Screen_Title_PYRO, NULL, LV_ALIGN_CENTER, 0, 0);

  Screen_Title_Btn_Continue = lv_btn_create(Screen_Title_PYRO, NULL);
  //lv_obj_set_event_cb(btn1, event_handler);
  lv_obj_align(Screen_Title_Btn_Continue, Screen_Title_PYRO, LV_ALIGN_IN_BOTTOM_MID, 0, 15);
  lv_obj_set_size(Screen_Title_Btn_Continue, 110, 50);
  lv_obj_set_style(Screen_Title_Btn_Continue, &style_btn_main);
  lv_btn_set_action(Screen_Title_Btn_Continue, LV_BTN_ACTION_CLICK, button_title_continue_action);

  Screen_Generic_Label = lv_label_create(Screen_Title_Btn_Continue, NULL);
  lv_label_set_text(Screen_Generic_Label, "CONTINUE");




  Screen_Mode_Select_Main = lv_page_create(lv_scr_act(), NULL);
  lv_obj_set_size(Screen_Mode_Select_Main, 480, 260);
  lv_obj_align(Screen_Mode_Select_Main, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_page_set_style(Screen_Mode_Select_Main, LV_PAGE_STYLE_BG, &Screen::style_bg);

  Screen_Main_BG = lv_img_create(Screen_Mode_Select_Main, NULL);
  lv_img_set_src(Screen_Main_BG, "D:/usd/img_bg.bin");
  lv_obj_align(Screen_Main_BG, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_hidden(Screen_Main_BG, 1);

  lv_obj_t * Screen_Battery_Percent_Label = lv_label_create(Screen_Main_BG, NULL);
  lv_obj_align(Screen_Battery_Percent_Label, Screen_Main_BG, LV_ALIGN_IN_BOTTOM_RIGHT, -10, 2); /*Align to the top*/


  Screen_Mode_Select_Comp = lv_btn_create(Screen_Mode_Select_Main, NULL);
  //lv_obj_set_event_cb(btn1, event_handler);
  lv_obj_align(Screen_Mode_Select_Comp, Screen_Mode_Select_Main, LV_ALIGN_CENTER, -140, -10);
  lv_obj_set_size(Screen_Mode_Select_Comp, 180, 100);
  lv_obj_set_style(Screen_Mode_Select_Comp, &style_btn_main);
  lv_btn_set_action(Screen_Mode_Select_Comp, LV_BTN_ACTION_CLICK, button_competition_select);

  Screen_Mode_Select_Comp_Label = lv_label_create(Screen_Mode_Select_Comp, NULL);
  lv_label_set_text(Screen_Mode_Select_Comp_Label, "Competition");

  Screen_Mode_Select_Debug = lv_btn_create(Screen_Mode_Select_Main, NULL);
  //lv_obj_set_event_cb(btn1, event_handler);
  lv_obj_align(Screen_Mode_Select_Debug, Screen_Mode_Select_Comp, LV_ALIGN_CENTER, 230-27, -8);
  lv_obj_set_size(Screen_Mode_Select_Debug, 180, 100);
  lv_obj_set_style(Screen_Mode_Select_Debug, &style_btn_main);
  lv_btn_set_action(Screen_Mode_Select_Debug, LV_BTN_ACTION_CLICK, button_title_continue_action);

  Screen_Mode_Select_Debug_Label = lv_label_create(Screen_Mode_Select_Debug, NULL);
  lv_label_set_text(Screen_Mode_Select_Debug_Label, "Debugging");

  lv_obj_set_hidden(Screen_Mode_Select_Main, 1);




  Screen_CompSelect_Back = lv_btn_create(Screen_Mode_Select_Main, NULL);
  //lv_obj_set_event_cb(btn1, event_handler);
  lv_obj_align(Screen_CompSelect_Back, Screen_Mode_Select_Main, LV_ALIGN_IN_BOTTOM_LEFT, 10, -47);
  lv_obj_set_size(Screen_CompSelect_Back, 100, 30);
  lv_obj_set_style(Screen_CompSelect_Back, &style_btn_main);
  lv_btn_set_action(Screen_CompSelect_Back, LV_BTN_ACTION_CLICK, button_competition_back);

  Screen_CompSelect_Back_Label = lv_label_create(Screen_CompSelect_Back, NULL);
  lv_label_set_text(Screen_CompSelect_Back_Label, "Back");
  lv_obj_set_hidden(Screen_CompSelect_Back, 1);




  Screen_End_Image = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(Screen_End_Image, "D:/usd/img_innovate.bin");
  lv_obj_align(Screen_End_Image, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_hidden(Screen_End_Image, 1);
}

Screen::~Screen()
{
  //lv_obj_del(Screen_Title_Page_Main);
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
