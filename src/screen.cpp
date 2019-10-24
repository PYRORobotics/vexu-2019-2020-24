#include "screen.h"

/* TO EXPORT IMAGES TO SD CARD

https://www.vexforum.com/t/lvgl-image-not-displaying/63612/15

Update 9/27/2019
For PROS kernel 3.2.0, which contains lvgl version 5.3, the new (or is that current) image converter should be used.
Select “True color with Alpha” for the color format
Select “Binary RGB888” for the output format
*/


/* Colors */
lv_color_t Screen::COLOR_ASU_MAROON;
lv_color_t Screen::COLOR_ASU_GOLD;

/* Styles */
lv_style_t Screen::style_btn_main;

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

void Screen::setup_styles()
{

  COLOR_ASU_MAROON.red = 140;
  COLOR_ASU_MAROON.green = 15;
  COLOR_ASU_MAROON.blue = 32;

  COLOR_ASU_GOLD.red = 255;
  COLOR_ASU_GOLD.green = 198;
  COLOR_ASU_GOLD.blue = 39;


  lv_style_copy(&Screen::style_btn_main, &lv_style_btn_rel);
  style_btn_main.body.main_color = COLOR_ASU_GOLD;
  style_btn_main.body.grad_color = COLOR_ASU_GOLD;
  style_btn_main.text.color = LV_COLOR_BLACK;
  style_btn_main.line.color = COLOR_ASU_GOLD;
  style_btn_main.body.radius = 3;

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

  Screen_Title_Page_Main = lv_page_create(lv_scr_act(), NULL);
  lv_obj_set_size(Screen_Title_Page_Main, 500, 260);
  lv_obj_align(Screen_Title_Page_Main, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_page_set_style(Screen_Title_Page_Main, LV_PAGE_STYLE_BG, &lv_style_plain);

  Screen_Title_Image_Loading = lv_img_create(Screen_Title_Page_Main, NULL);
  lv_img_set_src(Screen_Title_Image_Loading, "D:/usd/img_title_loading_asu.bin");

  pros::delay(2000);

  lv_img_set_src(Screen_Title_Image_Loading, "D:/usd/img_title_loading_rrr.bin");

  pros::delay(1500);

  lv_obj_del(Screen_Title_Image_Loading);


  Screen_Title_Page_Loading = lv_page_create(lv_scr_act(), NULL);
  lv_obj_set_size(Screen_Title_Page_Loading, 500, 260);
  lv_obj_align(Screen_Title_Page_Loading, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_page_set_style(Screen_Title_Page_Loading, LV_PAGE_STYLE_BG, &lv_style_plain);

  Screen_Title_Image_Main = lv_img_create(Screen_Title_Page_Loading, NULL);
  lv_img_set_src(Screen_Title_Image_Main, "D:/usd/img_title_loading_main.bin");
  lv_obj_align(Screen_Title_Image_Main, NULL, LV_ALIGN_CENTER, 0, 0);


  Screen_Title_Btn_Continue = lv_btn_create(lv_scr_act(), NULL);
  //lv_obj_set_event_cb(btn1, event_handler);
  lv_obj_align(Screen_Title_Btn_Continue, Screen_Title_Page_Loading, LV_ALIGN_IN_BOTTOM_MID, 12, 10);
  lv_obj_set_size(Screen_Title_Btn_Continue, 100, 50);

  lv_obj_set_style(Screen_Title_Btn_Continue, &style_btn_main);
  Screen_Generic_Label = lv_label_create(Screen_Title_Btn_Continue, NULL);
  lv_label_set_text(Screen_Generic_Label, "CONTINUE");


}

Screen::~Screen()
{
  //lv_obj_del(Screen_Title_Page_Main);
}
