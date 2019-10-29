#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "api.h"

class Screen
{
  public:
    Screen();
    ~Screen();
    void setup_styles();
    pros::Task t_screen;
    static void update(void*)
    {
      while(1)
      {
        if(Screen_Battery_Percent_Label != NULL){
        std::string text1 = std::to_string((int)pros::battery::get_capacity()) + "%";
        lv_label_set_text(Screen_Battery_Percent_Label, text1.c_str());}
        pros::delay(20);
      }
    }

  private:
    /* Colors */
    static lv_color_t COLOR_ASU_MAROON;
    static lv_color_t COLOR_ASU_GOLD;

    /* Styles */
    static lv_style_t style_btn_main;
    static lv_style_t style_bg;

    /* Objects */
    static lv_obj_t *Screen_Title_Page_Main;
    static lv_obj_t *Screen_Title_PYRO;
    static lv_obj_t *Screen_Title_Btn_Continue;
    static lv_obj_t *Screen_Generic_Label;

    static lv_obj_t *Screen_Main_BG;
    static lv_obj_t *Screen_Battery_Percent_Label;


    static lv_obj_t *Screen_Mode_Select_Main;
    static lv_obj_t *Screen_Mode_Select_Comp;
    static lv_obj_t *Screen_Mode_Select_Comp_Label;
    static lv_obj_t *Screen_Mode_Select_Debug;
    static lv_obj_t *Screen_Mode_Select_Debug_Label;

    static lv_obj_t *Screen_CompSelect_Back;
    static lv_obj_t *Screen_CompSelect_Back_Label;



    lv_obj_t *Screen_End_Image;

    static lv_res_t button_title_continue_action(lv_obj_t * btn)
    {
      lv_obj_set_hidden(Screen_Title_PYRO,1);
      lv_obj_set_hidden(Screen_Title_Btn_Continue,1);
      lv_obj_set_hidden(Screen_Main_BG,0);
      lv_obj_set_hidden(Screen_Mode_Select_Main, 0);

      // lv_obj_del(Screen_Title_PYRO);
      // lv_obj_del(Screen_Title_Btn_Continue);

      return LV_RES_OK; /*Return OK if the button is not deleted*/
    }

    static lv_res_t button_competition_select(lv_obj_t * btn)
    {
      lv_obj_set_hidden(Screen_Mode_Select_Comp,1);
      lv_obj_set_hidden(Screen_Mode_Select_Debug,1);
      //lv_obj_set_hidden(Screen_Title_Btn_Continue,1);
      lv_obj_set_hidden(Screen_CompSelect_Back,0);

      // lv_obj_del(Screen_Title_PYRO);
      // lv_obj_del(Screen_Title_Btn_Continue);

      return LV_RES_OK; /*Return OK if the button is not deleted*/
    }

    static lv_res_t button_competition_back(lv_obj_t * btn)
    {
      lv_obj_set_hidden(Screen_CompSelect_Back,1);

      lv_obj_set_hidden(Screen_Mode_Select_Comp,0);
      lv_obj_set_hidden(Screen_Mode_Select_Debug,0);


      // lv_obj_del(Screen_Title_PYRO);
      // lv_obj_del(Screen_Title_Btn_Continue);

      return LV_RES_OK; /*Return OK if the button is not deleted*/
    }

};


#endif
