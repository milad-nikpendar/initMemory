#include "initMemory.h"
#include <SD.h>
#include <SPIFFS.h>
#include <LittleFS.h>

#include "ff.h"
#include "vfs_fat_internal.h"

// Function to format the SD card
bool memoryAccess_t::format(fs::FS *memory)
{
  if (memory == &SD) // Check if the file system is SD
  {
    char drv[3] = {'0', ':', 0};
    const size_t workbuf_size = 4096;
    void *workbuf = NULL;
    esp_err_t err = ESP_OK;
    ESP_LOGW("sdcard", "Formatting the SD card");

    size_t allocation_unit_size = 16 * 1024;
    int sector_size_default = 512;

    workbuf = ff_memalloc(workbuf_size);
    if (workbuf == NULL)
    {
      return ESP_ERR_NO_MEM;
    }

    size_t alloc_unit_size = esp_vfs_fat_get_allocation_unit_size(
        sector_size_default,
        allocation_unit_size);

#if (ESP_IDF_VERSION_MAJOR < 5)
    FRESULT res = f_mkfs(drv, FM_ANY, alloc_unit_size, workbuf, workbuf_size);
#else
    const MKFS_PARM opt = {(BYTE)FM_ANY, 0, 0, 0, alloc_unit_size};
    FRESULT res = f_mkfs(drv, &opt, workbuf, workbuf_size);
#endif /* ESP_IDF_VERSION_MAJOR */
    if (res != FR_OK)
    {
      err = ESP_FAIL;
      ESP_LOGE("sdcard", "f_mkfs failed (%d)", res);
    }
    free(workbuf);
    ESP_LOGI("sdcard", "Successfully formatted the SD card");
    return err == ESP_OK;
  }
  else if (memory == &SPIFFS) // Check if the file system is SPIFFS
  {
    // Format SPIFFS
    if (!SPIFFS.format())
    {
      ESP_LOGE("spiffs", "Failed to format SPIFFS");
      return false;
    }
    ESP_LOGI("spiffs", "Successfully formatted SPIFFS");
    return true;
  }
  else if (memory == &LittleFS) // Check if the file system is LittleFS
  {
    // Format LittleFS
    if (!LittleFS.format())
    {
      ESP_LOGE("littlefs", "Failed to format LittleFS");
      return false;
    }
    ESP_LOGI("littlefs", "Successfully formatted LittleFS");
    return true;
  }
  else
  {
    ESP_LOGE("memoryAccess", "Unsupported file system for formatting");
    return false;
  }
}
