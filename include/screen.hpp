#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "api.h"

class Screen
{
  public:
    Screen();
    ~Screen();
    void setup_styles();

  private:
    /* Colors */
    static lv_color_t COLOR_ASU_MAROON;
    static lv_color_t COLOR_ASU_GOLD;

    /* Styles */
    static lv_style_t style_btn_main;

    /* Objects */
    lv_obj_t * Screen_Base;

    lv_obj_t * Screen_Title_Page_Loading;
    lv_obj_t * Screen_Title_Page_Main;
    lv_obj_t * Screen_Title_Image_Loading;
    lv_obj_t * Screen_Title_Image_Main;
    lv_obj_t * Screen_Title_Btn_Continue;

    lv_obj_t * Screen_Generic_Label;

};


#endif
