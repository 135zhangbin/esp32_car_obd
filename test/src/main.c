/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <stddef.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "OBD_simulator.h"
#include "example_ble_sec_gatts_demo.h"
#include "esp_log.h"


#define RX_TASK_PRIO            8
#define TX_TASK_PRIO            9
#define MSG_ID 0x7DF// 11 bit standard format ID

#define TX_GPIO_NUM GPIO_NUM_4
#define RX_GPIO_NUM GPIO_NUM_5


//  ISO15765_11bit_500K; //当前协议



void twai_receive_task(void *arg)
{
    while (1) {
      twai_receive_data();
   }
}
void twai_transmit_task(void *arg)
{
       while (1) {
          twai_transmit_data();
          vTaskDelay(pdMS_TO_TICKS(2000)); 
       }
}


void app_main(void)
{
   ESP_LOGI("MAIN","开始运行");

   OBD_twai_init_500();
   init_sendcommond();
   blue_tooth_start();


   //创建任务
   xTaskCreatePinnedToCore(twai_transmit_task, "TWAI_tx", 4096, NULL, TX_TASK_PRIO, NULL, tskNO_AFFINITY);

   vTaskDelay(pdMS_TO_TICKS(1000));
   xTaskCreatePinnedToCore(twai_receive_task, "TWAI_rx", 4096, NULL, RX_TASK_PRIO, NULL, tskNO_AFFINITY);



   // blue_tooth_start();
   // while (1)
   // {
   // char example_data[] = "12,12,12,12,12,12,12,12,";  // 替换为您要传递的数据

   // vTaskDelay(pdMS_TO_TICKS(1000));
   // sendData(example_data);
   // ESP_LOGI("TAG", "发送蓝牙数据1111");
   // }
   
}