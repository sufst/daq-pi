/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------
   adapted from previous code
   ------------------------------------------------------*/
/* include sd fat library */
#include "../libraries/SdFat/SdFat.h"

/*define base file for init*/
#define DEV__COMMS__SD_BASE_FILE_NAME "DATA"

/*defines sd obj */
typedef struct
{
    bool enabled;
    SdFat sd;
    SdFile file;
    uint16_t byteCnt;
    util__cir__buff_t txBuff;
} dev__comms__sd_obj_t

/*Init pins*/

dev__comms__state_t dev__comms__sd_init(uint8_t csPin)
{
    srv__daq__state_t ret = SRV__DAQ__ERROR_NONE;

    if (!dev_comms_sd_obj.sd.begin(csPin, SPI_FULL_SPEED))
    {
        ret = SRV__DAQ__ERROR;
        goto fail;
    }

    uint8_t sdBaseFileName;
    sdBaseFileName = sizeof(DEV__COMMS__SD_BASE_FILE_NAME) - 1;

    static char fileName[] = DEV__COMMS__SD_BASE_FILE_NAME "00.bin";

    /*
     * Loop until we find an available file name to use DATA##
     */
    while (dev_comms_sd_obj.sd.exists(fileName))
    {
        if (fileName[sdBaseFileName + 1] != '9')
        {
            fileName[sdBaseFileName + 1]++;
        }
        else if (fileName[sdBaseFileName] != '9')
        {
            fileName[sdBaseFileName + 1] = '0';
            fileName[sdBaseFileName]++;
        }
        else
        {

            ret = SRV__DAQ__ERROR;
            goto fail;
        }
    }

    if (!dev_comms_sd_obj.file.open(fileName, O_WRITE | O_CREAT))
    {

        ret = SRV__DAQ__ERROR;
        goto fail;
    }

    util__cir__buff__init(&dev_comms_sd_obj.txBuff, BlockBuff, SRV__DAQ__SD_TX_BLOCK_BUFFER_LEN,
                          sizeof(srv__daq__sd_block_t));

    dev_comms_sd_obj.enabled = true;

    return ret;

    fail:
    return ret;
}
