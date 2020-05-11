#include "adf.h"
#include "log_data.h"
#include "sysparams.h"

LogData   g_last_log_data;

void log_write(LogData *log, u32 records_wr)
{
    eeprom_init();
    eeprom_write(LOG_BASE_ADDR + records_wr*sizeof(LogData), (u8*)log, sizeof(LogData));
    eeprom_close();
}

void log_read(LogData *log, u32 records_rd)
{
    //printf("addr:%lu\r\n", LOG_BASE_ADDR + records_rd*sizeof(LogData));
    eeprom_read(LOG_BASE_ADDR + records_rd*sizeof(LogData), (u8*)log, sizeof(LogData));
}

void log_clear_data(void)
{
    int i = 0;
    u8 array = 0;

    for(i=0; i<72*13; i++)
    {
        eeprom_init();
        eeprom_write(LOG_BASE_ADDR+i , &array, 1);
        eeprom_close();
    }
}


//void

