#ifndef _LCD_H
#define _LCD_H

#ifdef HAVE_LCD_ST7565R

// 字符句柄索引
#define E_TYPE_IDX_CHAR	0
#define E_TYPE_IDX_WORD	1

typedef struct _FontAttr{
	u8		type;
	u8		width;
	u8		height;
	u8		size;
}FontAttr;

/*! \brief
*       加载LCD配置参数
* \param type[IN]		- 液晶类型
* \param cfg[IN]		- 配置句柄
*/
void lcd_load_config(void *cfg);

/*! \brief
*       LCD初始化
*/
void lcd_init(void);

/*! \brief
*       关闭LCD显示
* \param type[IN]		- 液晶类型
* \param cfg[IN]		- 配置句柄
*/
void lcd_close(void);

/*! \brief
*       设置LCD背光
* \param sts[IN]		- 背光
*		LCD_BACKLIGHT_OFF-关；
*		LCD_BACKLIGHT_ON-开
*/
void lcd_set_backlight(u8 sts);

/*! \brief
 *       设置显示字体
 * \param char_type[IN]		- ASCII码字体
 * \param word_type[IN]		- 中文字体
 * \notes
 * \	仅适用于点阵液晶
 */
void lcd_set_font(u8 char_type, u8 word_type);

/*! \brief
 *       设置显示模式
 * \param mode[IN]		- 设置显示模式
 * \notes
 * \	仅适用于点阵液晶
 */
void lcd_set_mode(u8 mode);

/*! \brief
 *       显示一个字符
 * \param x[IN]		- X坐标
 * \param y[IN]		- Y坐标
 * \param ch[IN]		- 大小
 * \notes
 * \	仅适用于点阵液晶
 */
void lcd_disp_char(unsigned int x, unsigned int y, unsigned int ch);

/*! \brief
 *		LCD显示字符串
 *
 * \param x[IN]			- 坐标X
 * \param y[IN]			- 坐标Y
 * \param str[IN]		- 显示字符串
 * \notes
 * \	仅适用于点阵液晶
 */
void lcd_draw_str(unsigned int x, unsigned int y, const unsigned char *str);

/*! \brief
 *		LCD清除指定方形区域
 *
 * \param line[IN]			- 行数：0~3
 * \param col[IN]			- 列数：0~20
 * \param width[IN]			- 宽度：0~20
 * \param height[IN]		- 高度：0~3
 * \notes
 * \	仅适用于点阵液晶
 */
void lcd_disp_line_clear(unsigned char line, unsigned char col, unsigned char width, unsigned char height);

/*! \brief 清屏 */
void lcd_disp_clrscr(void);

/*! \brief
 *		显示一行信息（居左）
 *
 * \param line[IN]		- 行号
 * \param col[IN]		- 列号
 * \param width[IN]		- 宽度
 * \param height[IN]	- 高度
 * \param pdata[IN]		- 字符串
 * \notes
 * \	仅适用于点阵液晶
 */
void lcd_disp_line(unsigned char line, unsigned char col, unsigned char width, unsigned char height, const char *pdata);

/*! \brief
 *		LCD显示字符串（居中）
 *
 * \param line[IN]		- 行号
 * \param col[IN]		- 列号
 * \param width[IN]		- 宽度
 * \param pdata[IN]		- 字符串
 * \notes
 * \	仅适用于点阵液晶
 */
void lcd_disp_line_center(unsigned char line, unsigned char col, unsigned char width, const char *pdata);


/*! \brief
 *		LCD显示一行数据（居右）
 *
 * \param line[IN]		- 行号
 * \param col[IN]		- 列号
 * \param width[IN]		- 宽度
 * \param pdata[IN]		- 字符串
 * \notes
 * \	仅适用于点阵液晶
 */
void lcd_disp_line_right(unsigned char line, unsigned char col, unsigned char width, const char *pdata);

/*! \brief
 *		LCD显示图片
 *
 * \param x[IN]				- X轴坐标
 * \param y[IN]				- Y轴坐标
 * \param width[IN]			- 宽度
 * \param height[IN]		- 高度
 * \param bitmap[IN]		- 位图
 * \notes
 * \	仅适用于点阵液晶
 */
void lcd_disp_bitmap(unsigned char x, unsigned char y, unsigned char width, unsigned char height, const unsigned char *bitmap);

#elif defined(HAVE_LCD) || defined(HAVE_LCD_EXT) || defined(HAVE_HT1621)

void lcd_load_config(void *cfg);

void lcd_power_on(void);

void lcd_power_off(void);

void lcd_init(void);

void lcd_close(void);

void lcd_write_seg(unsigned char *disp_buf, unsigned char seg_id, unsigned char mode);

void lcd_update_screen(unsigned char *disp_buf, unsigned int size);

#endif

#endif
