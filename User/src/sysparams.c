#include "sysparams.h"

SysParams g_sys_params;
RunParams g_run_params;

// 8 - 900MHZ
// 3 - 1800MHZ
// 5 - 850MHZ
const char *g_band_list[BAND_CNT] = {"8,3,5", "3,8,5", "5,8,3"};

//void set_valve_state(u8 state)
//{
//    g_run_params.st = (g_run_params.st&~0x07) | state;
//}

u8 factory_check(void)
{
    if (g_sys_params.factory_flag == 0)
    {
        if (memcmp(g_sys_params.addr, "\x00\x00\x00\x00\x00\x00\x00", 7)!=0
            && memcmp(g_sys_params.addr, "\x00\x00\x00\x00\x00\x00\x01", 7)!=0)
        {
            g_sys_params.factory_flag = 1;
            eeprom_init();
            eeprom_write(0, (u8*)&g_sys_params, 1);
            eeprom_close();

            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

void sysparams_update_records_write_counter(u32 records_wr)
{
    eeprom_init();
    eeprom_write(E_ADDR_OFFSET_RECORDS_WRITE, (u8*)&records_wr, sizeof(u32));
    eeprom_close();
}

/*! \brief
*       更新系统参数
* \param addr[IN]           - 写入的地址
* \param dat[IN]            - 数据内容
* \param len[IN]            - 数据长度
*/
void sysparams_write(u8 addr, u8 *dat, u16 len)
{
    eeprom_init();
    eeprom_write((u32)addr, dat, len);
    eeprom_close();
}

/*! \brief
*       读取系统参数
* \param addr[IN]           - 写入的地址
* \param dat[OUT]           - 数据内容
* \param len[IN]            - 数据长度
*/
void sysparams_read(u8 addr, u8 *dat, u16 len)
{
    eeprom_read((u32)addr, dat, len);
}


