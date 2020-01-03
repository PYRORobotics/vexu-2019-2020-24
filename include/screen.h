#ifndef _SCREEN_HPP_
#define _SCREEN_HPP_

#include "api.h"
#include "autonomous.h"
#include "display/lv_objx/lv_btnm.h"
#include "arduino.h"


typedef  FILE * pc_file_t;


class Screen
{
private:
  enum modeType
  {
    Competition,
    DebugDriver,
    DebugAuton,
    Information,
    None
  } inline static currentMode = None;


  /* Colors */
  inline static lv_color_t COLOR_ASU_MAROON;
  inline static lv_color_t COLOR_ASU_GOLD;

  /* Styles */
  inline static lv_style_t style_btn_main;
  inline static lv_style_t style_btn_transp;
  inline static lv_style_t style_bg;

  /* Objects */
  inline static lv_obj_t *Screen_Title_Page_Main;
  inline static lv_obj_t *Screen_Bar;
  inline static lv_obj_t *Screen_Battery_Percent_Label;
  inline static lv_obj_t *Screen_Bar_Cont1;
  inline static lv_obj_t *Screen_Bar_Cont2;
  inline static lv_obj_t *Screen_Bar_Box;
  inline static lv_obj_t *Screen_Bar_Box_Reset_Btn;
  inline static lv_obj_t *Screen_Bar_Charging;
  inline static lv_obj_t *Screen_Coordinates_Label;

  inline static lv_obj_t *Screen_Title_PYRO;
  inline static lv_obj_t *Screen_Title_Btn_Continue;
  inline static lv_obj_t *Screen_Generic_Label;


  inline static lv_obj_t *Screen_Competition_Btn_Main;
  inline static lv_obj_t *Screen_Competition_Btn_Main_Label;
  inline static lv_obj_t *Screen_Driver_Btn_Main;
  inline static lv_obj_t *Screen_Driver_Btn_Main_Label;
  inline static lv_obj_t *Screen_Auton_Btn_Main;
  inline static lv_obj_t *Screen_Auton_Btn_Main_Label;
  inline static lv_obj_t *Screen_Info_Btn_Main;
  inline static lv_obj_t *Screen_Info_Btn_Main_Label;

  inline static lv_obj_t *Screen_Mode_Help_Label;


  inline static lv_obj_t *Screen_Mode_BG;

  inline static lv_obj_t *Screen_Field_Ready_Label;

  inline static lv_obj_t *Screen_Red_Auton_Btns;
  inline static lv_obj_t *Screen_Blue_Auton_Btns;

  inline static lv_obj_t *Screen_Mode_Title;
  inline static lv_obj_t *Screen_Mode_REDBLUE_Label;
  inline static lv_obj_t *Screen_Mode_Desc_Label;

  inline static lv_obj_t *Screen_Auton_Start_Btn_Label;
  inline static lv_obj_t *Screen_Driver_Start_Btn_Label;




  // inline static lv_obj_t *Screen_Main_BG;
  //
  //
  // inline static lv_obj_t *Screen_Mode_Select_Main;
  // inline static lv_obj_t *Screen_Mode_Select_Comp;
  // inline static lv_obj_t *Screen_Mode_Select_Comp_Label;
  // inline static lv_obj_t *Screen_Mode_Select_Debug;
  // inline static lv_obj_t *Screen_Mode_Select_Debug_Label;
  //
  // inline static lv_obj_t *Screen_CompSelect_Back;
  // inline static lv_obj_t *Screen_CompSelect_Back_Label;

  lv_obj_t *Screen_End_Image;

  enum { arrsize = 2 };
  inline static const char *  red_auton_btnm_map[] = {"1", "2", "Skills",""};
  inline static const char *  blue_auton_btnm_map[] = {"1", "2", "Skills",""};


  static lv_res_t button_title_continue_action(lv_obj_t * btn)
  {
    lv_obj_set_hidden(Screen_Title_PYRO,1);
    lv_obj_set_hidden(Screen_Title_Btn_Continue,1);

    lv_obj_set_hidden(Screen_Competition_Btn_Main,0);
    lv_obj_set_hidden(Screen_Driver_Btn_Main,0);
    lv_obj_set_hidden(Screen_Auton_Btn_Main,0);
    lv_obj_set_hidden(Screen_Info_Btn_Main,0);
    lv_obj_set_hidden(Screen_Mode_Help_Label,0);

    // lv_obj_del(Screen_Title_PYRO);
    // lv_obj_del(Screen_Title_Btn_Continue);

    return LV_RES_OK; /*Return OK if the button is not deleted*/
  }

  static lv_res_t button_competition_select(lv_obj_t * btn)
  {

    switch (currentMode) {
      case Competition:
        lv_obj_set_hidden(Screen_Mode_BG,1);
        lv_obj_set_hidden(Screen_Mode_Title,1);
        lv_obj_set_hidden(Screen_Field_Ready_Label,1);
        lv_obj_set_hidden(Screen_Red_Auton_Btns,1);
        lv_obj_set_hidden(Screen_Blue_Auton_Btns,1);
        lv_obj_set_hidden(Screen_Mode_REDBLUE_Label,1);
        lv_obj_set_hidden(Screen_Mode_Desc_Label,1);
        DriverEnabled = false;
        AutonEnabled = false;
        currentMode = None;
        break;
      default:
        lv_label_set_text(Screen_Mode_Title, "      Competition Mode");
        lv_obj_set_hidden(Screen_Mode_Title,0);
        lv_obj_set_hidden(Screen_Mode_BG,0);
        lv_obj_set_hidden(Screen_Field_Ready_Label,0);
        lv_obj_set_hidden(Screen_Red_Auton_Btns,0);
        lv_obj_set_hidden(Screen_Blue_Auton_Btns,0);
        lv_obj_set_hidden(Screen_Mode_REDBLUE_Label,0);
        lv_obj_set_hidden(Screen_Mode_Desc_Label,0);
        lv_obj_set_hidden(Screen_Auton_Start_Btn_Label,1);
        lv_obj_set_hidden(Screen_Driver_Start_Btn_Label,1);
        DriverEnabled = true;
        AutonEnabled = false;
        currentMode = Competition;
        break;
    }

    return LV_RES_OK; /*Return OK if the button is not deleted*/
  }

  static lv_res_t button_driver_select(lv_obj_t * btn)
  {

    switch (currentMode) {
      case DebugDriver:
        lv_obj_set_hidden(Screen_Mode_BG,1);
        lv_obj_set_hidden(Screen_Mode_Title,1);
        lv_obj_set_hidden(Screen_Driver_Start_Btn_Label,1);
        currentMode = None;
        DriverEnabled = false;
        AutonEnabled = false;
        break;
      default:
        lv_label_set_text(Screen_Mode_Title, "     Driver Debug Mode");
        lv_obj_set_hidden(Screen_Mode_Title,0);
        lv_obj_set_hidden(Screen_Mode_BG,0);
        lv_obj_set_hidden(Screen_Field_Ready_Label,1);
        lv_obj_set_hidden(Screen_Red_Auton_Btns,1);
        lv_obj_set_hidden(Screen_Blue_Auton_Btns,1);
        lv_obj_set_hidden(Screen_Mode_REDBLUE_Label,1);
        lv_obj_set_hidden(Screen_Mode_Desc_Label,1);
        lv_obj_set_hidden(Screen_Auton_Start_Btn_Label,1);
        lv_obj_set_hidden(Screen_Driver_Start_Btn_Label,0);
        currentMode = DebugDriver;
        DriverEnabled = true;
        AutonEnabled = false;
        break;
    }

    return LV_RES_OK; /*Return OK if the button is not deleted*/
  }

  static lv_res_t button_auton_select(lv_obj_t * btn)
  {

    switch (currentMode) {
      case DebugAuton:
        lv_obj_set_hidden(Screen_Mode_BG,1);
        lv_obj_set_hidden(Screen_Mode_Title,1);
        lv_obj_set_hidden(Screen_Red_Auton_Btns,1);
        lv_obj_set_hidden(Screen_Blue_Auton_Btns,1);
        lv_obj_set_hidden(Screen_Mode_REDBLUE_Label,1);
        lv_obj_set_hidden(Screen_Mode_Desc_Label,1);
        lv_obj_set_hidden(Screen_Auton_Start_Btn_Label,1);
        currentMode = None;
        DriverEnabled = false;
        AutonEnabled = false;
        break;
      default:
        lv_label_set_text(Screen_Mode_Title, "     Auton. Debug Mode");
        lv_obj_set_hidden(Screen_Mode_Title,0);
        lv_obj_set_hidden(Screen_Mode_BG,0);
        lv_obj_set_hidden(Screen_Field_Ready_Label,1);
        lv_obj_set_hidden(Screen_Red_Auton_Btns,0);
        lv_obj_set_hidden(Screen_Blue_Auton_Btns,0);
        lv_obj_set_hidden(Screen_Mode_REDBLUE_Label,0);
        lv_obj_set_hidden(Screen_Mode_Desc_Label,0);
        lv_obj_set_hidden(Screen_Auton_Start_Btn_Label,0);
        lv_obj_set_hidden(Screen_Driver_Start_Btn_Label,1);
        currentMode = DebugAuton;
        DriverEnabled = false;
        AutonEnabled = true;
        break;
    }

    return LV_RES_OK; /*Return OK if the button is not deleted*/
  }

  static lv_res_t button_info_select(lv_obj_t * btn)
  {

    switch (currentMode) {
      case Information:
        lv_obj_set_hidden(Screen_Mode_BG,1);
        lv_obj_set_hidden(Screen_Mode_Title,1);
        currentMode = None;
        DriverEnabled = false;
        AutonEnabled = false;
        break;
      default:
        lv_label_set_text(Screen_Mode_Title, "        PYRO GUI 2.0\nThis program was created\nwith PROS (Purdue Robotics\nOperating System) and lvgl\n(Little v Graphics Library).\nThanks to both dev. teams!\n\n   (c) 2020 Brandon Rice,\n       PYRO Robotics");
        lv_obj_set_hidden(Screen_Mode_Title,0);
        lv_obj_set_hidden(Screen_Mode_BG,0);
        lv_obj_set_hidden(Screen_Field_Ready_Label,1);
        lv_obj_set_hidden(Screen_Red_Auton_Btns,1);
        lv_obj_set_hidden(Screen_Blue_Auton_Btns,1);
        lv_obj_set_hidden(Screen_Mode_REDBLUE_Label,1);
        lv_obj_set_hidden(Screen_Mode_Desc_Label,1);
        lv_obj_set_hidden(Screen_Auton_Start_Btn_Label,1);
        lv_obj_set_hidden(Screen_Driver_Start_Btn_Label,1);
        currentMode = Information;
        DriverEnabled = false;
        AutonEnabled = false;
        break;
    }

    return LV_RES_OK; /*Return OK if the button is not deleted*/
  }

  static lv_res_t button_box_reset(lv_obj_t * btn)
  {

    okapi::PYRO_Arduino::reset();

    return LV_RES_OK; /*Return OK if the button is not deleted*/
  }



  static lv_res_t btnm_action_red(lv_obj_t * btnm, const char *txt) {

    int btnm_num = atoi(txt);

    std::string str = "    Red " + std::string(txt) + " Selected!";

    lv_label_set_text(Screen_Mode_Desc_Label, str.c_str());
    lv_label_set_text(Screen_Field_Ready_Label, "\n        Ready - Good Luck!");

    switch (btnm_num) {
    case 1:
      selectedAuton = red0;
      break;
    case 2:
      selectedAuton = red1;
      break;
    case 0:
      selectedAuton = redSkills0;
      break;
    default:
      selectedAuton = notSelected;
      break;
    }
  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}

static lv_res_t btnm_action_blue(lv_obj_t * btnm, const char *txt) {

  int btnm_num = atoi(txt);

  std::string str = "    Blue " + std::string(txt) + " Selected!";
  lv_label_set_text(Screen_Field_Ready_Label, "\n        Ready - Good Luck!");

  lv_label_set_text(Screen_Mode_Desc_Label, str.c_str());

  switch (btnm_num) {
  case 1:
    selectedAuton = blue0;
    break;
  case 2:
    selectedAuton = blue1;
    break;
  case 0:
    selectedAuton = blueSkills0;
    break;
  default:
    selectedAuton = notSelected;
    break;
  }
return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}


  static void setup_styles()
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

    lv_style_copy(&Screen::style_btn_transp, &lv_style_btn_rel);
    style_btn_transp.body.main_color = LV_COLOR_TRANSP;
    style_btn_transp.body.grad_color = LV_COLOR_TRANSP;
    style_btn_transp.text.color = LV_COLOR_TRANSP;
    style_btn_transp.body.border.color = LV_COLOR_TRANSP;
    style_btn_transp.body.opa = 0;
    style_btn_transp.body.border.width = 0;
    style_btn_transp.body.padding.hor = 5;
    style_btn_transp.body.padding.ver = 5;
    style_btn_transp.body.radius = 0;


    lv_style_copy(&Screen::style_bg, &lv_style_plain_color);
    Screen::style_bg.body.main_color = LV_COLOR_BLACK;
    Screen::style_bg.body.grad_color = LV_COLOR_BLACK;
  }

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

  static void initialize()
  {
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



      Screen_Bar = lv_img_create(lv_scr_act(), NULL);
      lv_img_set_src(Screen_Bar, "D:/usd/img_bar.bin");
      lv_obj_align(Screen_Bar, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

      Screen_Battery_Percent_Label = lv_label_create(Screen_Bar, NULL);
      lv_label_set_text(Screen_Battery_Percent_Label, "---%");
      lv_obj_align(Screen_Battery_Percent_Label, Screen_Bar, LV_ALIGN_CENTER, 210, 0);

      Screen_Bar_Charging = lv_img_create(Screen_Bar, NULL);
      lv_img_set_src(Screen_Bar_Charging, "D:/usd/img_bolt.bin");
      lv_obj_align(Screen_Bar_Charging, Screen_Bar, LV_ALIGN_CENTER, 155, 0);
      lv_obj_set_hidden(Screen_Bar_Charging,1);


      Screen_Bar_Cont1 = lv_img_create(Screen_Bar, NULL);
      lv_img_set_src(Screen_Bar_Cont1, "D:/usd/img_cont1w.bin");
      lv_obj_align(Screen_Bar_Cont1, Screen_Bar, LV_ALIGN_CENTER, -189, 0);


      Screen_Bar_Cont2 = lv_img_create(Screen_Bar, NULL);
      lv_img_set_src(Screen_Bar_Cont2, "D:/usd/img_cont2w.bin");
      lv_obj_align(Screen_Bar_Cont2, Screen_Bar, LV_ALIGN_CENTER, -147, 0);

      Screen_Bar_Box = lv_img_create(Screen_Bar, NULL);
      lv_img_set_src(Screen_Bar_Box, "D:/usd/img_boxw.bin");
      lv_obj_align(Screen_Bar_Box, Screen_Bar, LV_ALIGN_CENTER, -104, 0);

      Screen_Bar_Box_Reset_Btn = lv_btn_create(Screen_Bar, NULL);
      //lv_obj_set_event_cb(btn1, event_handler);
      lv_obj_align(Screen_Bar_Box_Reset_Btn, Screen_Bar, LV_ALIGN_CENTER, -56, 25);
      lv_obj_set_size(Screen_Bar_Box_Reset_Btn, 32, 32);
      lv_obj_set_style(Screen_Bar_Box_Reset_Btn, &style_btn_transp);
      lv_btn_set_action(Screen_Bar_Box_Reset_Btn, LV_BTN_ACTION_CLICK, button_box_reset);

      Screen_Coordinates_Label = lv_label_create(Screen_Bar, NULL);
      lv_label_set_text(Screen_Coordinates_Label, "(x,y,h)");
      lv_obj_align(Screen_Coordinates_Label, Screen_Bar, LV_ALIGN_CENTER, 85, 0);

      lv_obj_set_hidden(Screen_Bar_Cont1,1);
      lv_obj_set_hidden(Screen_Bar_Cont2,1);
      lv_obj_set_hidden(Screen_Bar_Box,1);



      Screen_Title_PYRO = lv_img_create(lv_scr_act(), NULL);
      lv_img_set_src(Screen_Title_PYRO, "D:/usd/img_pyro_large.bin");
      lv_obj_align(Screen_Title_PYRO, NULL, LV_ALIGN_CENTER, 0, -20);

      Screen_Title_Btn_Continue = lv_btn_create(Screen_Title_PYRO, NULL);
      //lv_obj_set_event_cb(btn1, event_handler);
      lv_obj_align(Screen_Title_Btn_Continue, Screen_Title_PYRO, LV_ALIGN_IN_BOTTOM_MID, 6, 10);
      lv_obj_set_size(Screen_Title_Btn_Continue, 110, 50);
      lv_obj_set_style(Screen_Title_Btn_Continue, &style_btn_main);
      lv_btn_set_action(Screen_Title_Btn_Continue, LV_BTN_ACTION_CLICK, button_title_continue_action);

      Screen_Generic_Label = lv_label_create(Screen_Title_Btn_Continue, NULL);
      lv_label_set_text(Screen_Generic_Label, "CONTINUE");




///
      Screen_Competition_Btn_Main = lv_btn_create(lv_scr_act(), NULL);
      //lv_obj_set_event_cb(btn1, event_handler);
      lv_obj_align(Screen_Competition_Btn_Main, lv_scr_act(), LV_ALIGN_CENTER, -160, -70);
      lv_obj_set_size(Screen_Competition_Btn_Main, 150, 40);
      lv_obj_set_style(Screen_Competition_Btn_Main, &style_btn_main);
      lv_btn_set_action(Screen_Competition_Btn_Main, LV_BTN_ACTION_CLICK, button_competition_select);
      lv_obj_set_hidden(Screen_Competition_Btn_Main,1);
      Screen_Competition_Btn_Main_Label = lv_label_create(Screen_Competition_Btn_Main, NULL);
      lv_label_set_text(Screen_Competition_Btn_Main_Label, "Competition");

      Screen_Driver_Btn_Main = lv_btn_create(lv_scr_act(), NULL);
      //lv_obj_set_event_cb(btn1, event_handler);
      lv_obj_align(Screen_Driver_Btn_Main, lv_scr_act(), LV_ALIGN_CENTER, -160, -20);
      lv_obj_set_size(Screen_Driver_Btn_Main, 150, 40);
      lv_obj_set_style(Screen_Driver_Btn_Main, &style_btn_main);
      lv_btn_set_action(Screen_Driver_Btn_Main, LV_BTN_ACTION_CLICK, button_driver_select);
      lv_obj_set_hidden(Screen_Driver_Btn_Main,1);
      Screen_Driver_Btn_Main_Label = lv_label_create(Screen_Driver_Btn_Main, NULL);
      lv_label_set_text(Screen_Driver_Btn_Main_Label, "Debug - Driver");

      Screen_Auton_Btn_Main = lv_btn_create(lv_scr_act(), NULL);
      //lv_obj_set_event_cb(btn1, event_handler);
      lv_obj_align(Screen_Auton_Btn_Main, lv_scr_act(), LV_ALIGN_CENTER, -160, 30);
      lv_obj_set_size(Screen_Auton_Btn_Main, 150, 40);
      lv_obj_set_style(Screen_Auton_Btn_Main, &style_btn_main);
      lv_btn_set_action(Screen_Auton_Btn_Main, LV_BTN_ACTION_CLICK, button_auton_select);
      lv_obj_set_hidden(Screen_Auton_Btn_Main,1);
      Screen_Auton_Btn_Main_Label = lv_label_create(Screen_Auton_Btn_Main, NULL);
      lv_label_set_text(Screen_Auton_Btn_Main_Label, "Debug - Auton.");

      Screen_Info_Btn_Main = lv_btn_create(lv_scr_act(), NULL);
      //lv_obj_set_event_cb(btn1, event_handler);
      lv_obj_align(Screen_Info_Btn_Main, lv_scr_act(), LV_ALIGN_CENTER, -160, 80);
      lv_obj_set_size(Screen_Info_Btn_Main, 150, 40);
      lv_obj_set_style(Screen_Info_Btn_Main, &style_btn_main);
      lv_btn_set_action(Screen_Info_Btn_Main, LV_BTN_ACTION_CLICK, button_info_select);
      lv_obj_set_hidden(Screen_Info_Btn_Main,1);
      Screen_Info_Btn_Main_Label = lv_label_create(Screen_Info_Btn_Main, NULL);
      lv_label_set_text(Screen_Info_Btn_Main_Label, "Information");

      Screen_Mode_Help_Label = lv_label_create(lv_scr_act(), NULL);
      lv_label_set_text(Screen_Mode_Help_Label, "Please Select a Mode\nto the Left");
      lv_obj_align(Screen_Mode_Help_Label, lv_scr_act(), LV_ALIGN_CENTER, 90, -16);
      lv_obj_set_hidden(Screen_Mode_Help_Label,1);

      Screen_Mode_BG = lv_img_create(lv_scr_act(), NULL);
      lv_img_set_src(Screen_Mode_BG, "D:/usd/img_modebg.bin");
      lv_obj_align(Screen_Mode_BG, NULL, LV_ALIGN_CENTER, 90, -16);
      lv_obj_set_hidden(Screen_Mode_BG,1);

      Screen_Mode_Title = lv_label_create(Screen_Mode_BG, NULL);
      lv_label_set_text(Screen_Mode_Title, "");
      lv_obj_align(Screen_Mode_Title, Screen_Mode_BG, LV_ALIGN_CENTER, -112, -85);
      lv_obj_set_hidden(Screen_Mode_Title,1);

      Screen_Mode_Desc_Label = lv_label_create(Screen_Mode_BG, NULL);
      lv_label_set_text(Screen_Mode_Desc_Label, "Choose an Autonomous:");
      lv_obj_align(Screen_Mode_Desc_Label, Screen_Mode_BG, LV_ALIGN_CENTER, 0, -65);
      lv_obj_set_hidden(Screen_Mode_Desc_Label,1);

      Screen_Mode_REDBLUE_Label = lv_label_create(Screen_Mode_BG, NULL);
      lv_label_set_text(Screen_Mode_REDBLUE_Label, "RED\n\nBLUE");
      lv_obj_align(Screen_Mode_REDBLUE_Label, Screen_Mode_BG, LV_ALIGN_CENTER, -115, -2);
      lv_obj_set_hidden(Screen_Mode_REDBLUE_Label,1);

      Screen_Field_Ready_Label = lv_label_create(Screen_Mode_BG, NULL);
      lv_obj_align(Screen_Field_Ready_Label, Screen_Mode_BG, LV_ALIGN_CENTER, -115, 50);
      lv_label_set_text(Screen_Field_Ready_Label, "\n       Choose an Autonomous!");
      lv_obj_set_hidden(Screen_Field_Ready_Label,1);

      Screen_Red_Auton_Btns = lv_btnm_create(Screen_Mode_BG, NULL);
      lv_btnm_set_map(Screen_Red_Auton_Btns, red_auton_btnm_map);
      lv_obj_align(Screen_Red_Auton_Btns, Screen_Mode_BG, LV_ALIGN_CENTER, 32, -10);
      lv_obj_set_size(Screen_Red_Auton_Btns, 220, 35);
      lv_obj_set_style(Screen_Red_Auton_Btns, &style_btn_main);
      lv_btnm_set_action(Screen_Red_Auton_Btns, btnm_action_red);
      lv_obj_set_hidden(Screen_Red_Auton_Btns,1);

      Screen_Blue_Auton_Btns = lv_btnm_create(Screen_Mode_BG, NULL);
      lv_btnm_set_map(Screen_Blue_Auton_Btns, blue_auton_btnm_map);
      lv_obj_align(Screen_Blue_Auton_Btns, Screen_Mode_BG, LV_ALIGN_CENTER, 32, 28);
      lv_obj_set_size(Screen_Blue_Auton_Btns, 220, 35);
      lv_obj_set_style(Screen_Blue_Auton_Btns, &style_btn_main);
      lv_btnm_set_action(Screen_Blue_Auton_Btns, btnm_action_blue);
      lv_obj_set_hidden(Screen_Blue_Auton_Btns,1);


      Screen_Auton_Start_Btn_Label = lv_label_create(Screen_Mode_BG, NULL);
      lv_label_set_text(Screen_Auton_Start_Btn_Label, "Press (A) on the Cont. 1\n  to start the routine.");
      lv_obj_align(Screen_Auton_Start_Btn_Label, Screen_Mode_BG, LV_ALIGN_CENTER, 0, 60);
      lv_obj_set_hidden(Screen_Auton_Start_Btn_Label,1);

      Screen_Driver_Start_Btn_Label = lv_label_create(Screen_Mode_BG, NULL);
      lv_obj_align(Screen_Driver_Start_Btn_Label, Screen_Mode_BG, LV_ALIGN_CENTER, -115, 50);
      lv_label_set_text(Screen_Driver_Start_Btn_Label, "       You are now enabled!\n    Have fun and safety first!");
      lv_obj_set_hidden(Screen_Driver_Start_Btn_Label,1);


      Screen_Plate_R = lv_img_create(lv_scr_act(), NULL);
      lv_img_set_src(Screen_Plate_R, "D:/usd/img_licencePlateR.bin");
      lv_obj_align(Screen_Plate_R, NULL, LV_ALIGN_CENTER, 0, -20);
      lv_obj_set_hidden(Screen_Plate_R,1);

      Screen_Plate_B = lv_img_create(lv_scr_act(), NULL);
      lv_img_set_src(Screen_Plate_B, "D:/usd/img_licencePlateB.bin");
      lv_obj_align(Screen_Plate_B, NULL, LV_ALIGN_CENTER, 0, -20);
      lv_obj_set_hidden(Screen_Plate_B,1);

      Screen_Sponsors = lv_img_create(lv_scr_act(), NULL);
      lv_img_set_src(Screen_Sponsors, "D:/usd/img_sponsors.bin");
      lv_obj_align(Screen_Sponsors, NULL, LV_ALIGN_CENTER, 0, -20);
      lv_obj_set_hidden(Screen_Sponsors,1);


      Screen_Innovate = lv_img_create(lv_scr_act(), NULL);
      lv_img_set_src(Screen_Innovate, "D:/usd/img_innovate.bin");
      lv_obj_align(Screen_Innovate, NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_hidden(Screen_Innovate,1);

  }

  public:
    Screen() : t_screen (pros::Task(update, (void*)NULL, TASK_PRIORITY_DEFAULT,
                        TASK_STACK_DEPTH_DEFAULT, "My Task"))
    {
      setup_styles();
      Screen::initialize();
    }
    ~Screen(){}

    inline static lv_obj_t *Screen_Plate_R;
    inline static lv_obj_t *Screen_Plate_B;
    inline static lv_obj_t *Screen_Sponsors;
    inline static lv_obj_t *Screen_Innovate;

    inline static autonNames selectedAuton;
    inline static bool DriverEnabled = false;
    inline static bool AutonEnabled = false;

    pros::Task t_screen;
    static void update(void*)
    {
      pros::delay(1000);
      pros::Controller master(pros::E_CONTROLLER_MASTER);
      pros::Controller partner(pros::E_CONTROLLER_PARTNER);
      while(1)
      {
        if(Screen_Battery_Percent_Label != NULL){
          std::string text1 = std::to_string((int)pros::battery::get_capacity()) + "%";
          lv_label_set_text(Screen_Battery_Percent_Label, text1.c_str());
        }

        bool gotMutex = false;
          try
          {
            gotMutex = OrientationData::mutex.take(3000);
            std::string text1 = "(" + std::to_string((int)OrientationData::getPosition(x)) + ", " + std::to_string((int)OrientationData::getPosition(y)) + ", " + std::to_string((int)OrientationData::getHeading()) + ")                ";
            lv_label_set_text(Screen_Coordinates_Label, text1.c_str());
          } catch(...){}

          if(gotMutex)
          OrientationData::mutex.give();



        if(master.is_connected())
        {
          lv_obj_set_hidden(Screen_Bar_Cont1,0);
        }
        else
        {
          lv_obj_set_hidden(Screen_Bar_Cont1,1);
        }

        if(partner.is_connected())
        {
          lv_obj_set_hidden(Screen_Bar_Cont2,0);
        }
        else
        {
          lv_obj_set_hidden(Screen_Bar_Cont2,1);
        }

        if(okapi::PYRO_Arduino::isCommumnicating)
        {
          lv_obj_set_hidden(Screen_Bar_Box,0);
        }
        else
        {
          lv_obj_set_hidden(Screen_Bar_Box,1);
        }

        if(Screen_Field_Ready_Label != NULL)
        {
          if(!pros::competition::is_connected())
            lv_label_set_text(Screen_Field_Ready_Label, "NOT CONNECTED TO THE FIELD!");
          else
            lv_label_set_text(Screen_Field_Ready_Label, "\n       Choose an Autonomous!");

        }

        if(pros::battery::get_current() < 0)
        {
          lv_obj_set_hidden(Screen_Bar_Charging,0);
        }
        else
        {
          lv_obj_set_hidden(Screen_Bar_Charging,1);
        }

        pros::delay(20);
      }
    }
};


#endif
