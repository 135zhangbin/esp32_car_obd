#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "OBD_simulator.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_log.h"
#include "example_ble_sec_gatts_demo.h"
#include <string.h>

// #define TX_GPIO_NUM GPIO_NUM_5
// #define RX_GPIO_NUM GPIO_NUM_4
#define TX_GPIO_NUM GPIO_NUM_5
#define RX_GPIO_NUM GPIO_NUM_4

#define MSG_ID_EXP 0x18DB33F1 // 29 bit standard format ID
#define MSG_ID 0x7DF// 11 bit standard format ID
#define RE_ID 0x7E8// 11 bit standard format ID
#define LENGTH 8


static const twai_timing_config_t t_config_250 = TWAI_TIMING_CONFIG_250KBITS();
static const twai_timing_config_t t_config_500 = TWAI_TIMING_CONFIG_500KBITS();
static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
static const twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NORMAL);
struct  SendCommond commondList[LENGTH]={};



// 构造函数的模拟
void initialize(struct SendCommond *sendcommond, twai_message_t tx_msg, enum CommondType commondType) {
    sendcommond->tx_msg=tx_msg;
    sendcommond->commondType = commondType;
}


void init_sendcommond(){
    int i=0;
    //发动机冷却液温度
    struct SendCommond Engine_Temperature;
    twai_message_t tx_msg = {.identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00}};
    enum CommondType commondType = Engine_Temperature_Type;
    initialize(&Engine_Temperature,tx_msg,commondType);
    commondList[i]=Engine_Temperature;
    i++;

    //发动机转速
    struct SendCommond Engine_Speed;
    twai_message_t tx_msg1 = {.identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00}};
    enum CommondType commondType1 = Engine_Speed_Type ;
    initialize(&Engine_Speed,tx_msg1,commondType1);
    commondList[i]=Engine_Speed;
    i++;

    //车速
    struct SendCommond Car_Speed;
    twai_message_t tx_msg2 = {.identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00}};
    enum CommondType commondType2 = Car_Speed_Type;
    initialize(&Car_Speed,tx_msg2,commondType2);
    commondList[i]=Car_Speed;
     i++;


     //进气温度
    struct SendCommond Air_Temperature;
    twai_message_t tx_msg3 = {.identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00}};
    enum CommondType commondType3 = Air_Temperature_Type;
    initialize(&Air_Temperature,tx_msg3,commondType3);
    commondList[i]=Air_Temperature;
     i++;


    //发动机起动后时间
    struct SendCommond Engine_Start_Time;
    twai_message_t tx_msg4 = {.identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00}};
    enum CommondType commondType4 = Engine_Start_Time_Type;
    initialize(&Engine_Start_Time,tx_msg4,commondType4);
    commondList[i]=Engine_Start_Time;
    i++;


     //发送故障码
    struct SendCommond Error_Code;
    twai_message_t tx_msg5 = {.identifier = MSG_ID, .data_length_code = 8, .data = {0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    enum CommondType commondType5 = Error_Code_Type;
    initialize(&Error_Code,tx_msg5,commondType5);
    commondList[i]=Error_Code;
    i++;
     

     //距离
    struct SendCommond Distance;
    //twai_message_t tx_msg6 = {.identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00}};
    twai_message_t tx_msg6 = {.identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0xA6, 0x00, 0x00, 0x00, 0x00, 0x00}};
    enum CommondType commondType6 = Distance_Type;
    initialize(&Distance,tx_msg6,commondType6);
    commondList[i]=Distance;
    i++;


    //油量计算
    struct SendCommond Strees;
    twai_message_t tx_msg7 = {.identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00}};
    enum CommondType commondType7 = Strees_Type;
    initialize(&Strees,tx_msg7,commondType7);
    commondList[i]=Strees;
}

void OBD_twai_init_250(void)
{
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config_250, &f_config));
    printf("Driver installed\n");
    ESP_ERROR_CHECK(twai_start());
    printf("Driver started\n");
}
void OBD_twai_init_500(void)
{
        printf("Driver open\n");
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config_500, &f_config));
    printf("Driver installed\n");

    ESP_ERROR_CHECK(twai_start());
    printf("Driver started\n");
}

bool OBD_twai_init_auto_baudrate(void)
{
    // 定义支持的比特率数组
    twai_timing_config_t t_config_array[] = {
        TWAI_TIMING_CONFIG_25KBITS(),
        TWAI_TIMING_CONFIG_50KBITS(),
        TWAI_TIMING_CONFIG_100KBITS(),
        TWAI_TIMING_CONFIG_125KBITS(),
        TWAI_TIMING_CONFIG_250KBITS(),
        TWAI_TIMING_CONFIG_500KBITS(),
        TWAI_TIMING_CONFIG_800KBITS(),
        TWAI_TIMING_CONFIG_1MBITS()
    };

    // 获取比特率数组的长度
    size_t num_configs = sizeof(t_config_array) / sizeof(t_config_array[0]);

    // 遍历比特率数组进行侦测
    for (size_t i = 0; i < num_configs; ++i) {
        // 使用当前比特率配置进行初始化
        ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config_array[i], &f_config));
        // 尝试启动驱动
        ESP_ERROR_CHECK(twai_start());
        return true;
    }
    return false;
}



void OBD_twai_deinit(void)
{
    ESP_ERROR_CHECK(twai_stop());
     printf("Driver stopped\n");

    ESP_ERROR_CHECK(twai_driver_uninstall());
    printf("Driver uninstalled\n");
}
void twai_transmit_data()
{
      for(int i=0;i<LENGTH;i++)
      {
        twai_message_t tx_msg= commondList[i].tx_msg;
        esp_err_t flag_tran=twai_transmit(&tx_msg, pdMS_TO_TICKS(10000));  
        
        //  ESP_ERROR_CHECK(flag_tran);

         if (flag_tran != ESP_OK) {
           printf("Error occurred: %s\n", esp_err_to_name(flag_tran));
           return;
        } else {
           // printf("Operation succeeded!\n");
         }
      }
    ESP_LOGI("TX_MSG","发送消息成功");


    //测试数据
    // twai_message_t tx_msg= commondList[6].tx_msg;
    // esp_err_t flag_tran=twai_transmit(&tx_msg, pdMS_TO_TICKS(10000));  
    // ESP_ERROR_CHECK(flag_tran);
    // if (flag_tran != ESP_OK) {
    //  printf("Error occurred: %s\n", esp_err_to_name(flag_tran));
    // } else {
    //   //printf("Operation succeeded!\n");
    // }
    // // ESP_LOGI("TX_MSG","发送消息成功");
}

char* computeEngine_Temperature_Type(twai_message_t rx_msg);
char* computeEngine_Speed_Type(twai_message_t rx_msg);
char* computeCar_Speed_Type(twai_message_t rx_msg);
char* computeAir_Temperature_Type(twai_message_t rx_msg);
char* computeEngine_Start_Time_Type(twai_message_t rx_msg);
char* computeError_Code_Type(twai_message_t rx_msg);
char* computeDistance_Type(twai_message_t rx_msg);
char* computeStrees_Type(twai_message_t rx_msg);

//发送数据
static char *myCharArray[LENGTH]={"0","0","0","0","0","0","0","0"};

void twai_receive_data()
{
    twai_message_t rx_msg;
    int flag_rec = twai_receive(&rx_msg, pdMS_TO_TICKS(10000));
    // ESP_LOGI("RX_MSG","identifier true:%"PRId32"",rx_msg.identifier);
    if(rx_msg.identifier == RE_ID)
    {
        uint8_t data_len_rel = rx_msg.data[0];
        if (data_len_rel < 2 || data_len_rel > 7)
        {
          printf("长度错误\n");
          return;
        }
    for(int i=0;i<LENGTH;i++)
     {
            //消息类型解析
        if ((rx_msg.data[2] == commondList[i].tx_msg.data[2]))
        {
        switch (  commondList[i].commondType) {
          case  Engine_Temperature_Type:
         myCharArray[0]=computeEngine_Temperature_Type(rx_msg);
         ESP_LOGI("RX_MSG","发动机冷却液温度:%s", myCharArray[0]);
                     break;
          case  Engine_Speed_Type:
          myCharArray[1]=computeEngine_Speed_Type(rx_msg);
          ESP_LOGI("RX_MSG","发送机转速:%s",myCharArray[1]);
                     break;
          case  Car_Speed_Type:
          myCharArray[2]=computeCar_Speed_Type(rx_msg);
          ESP_LOGI("RX_MSG","车速:%s",myCharArray[2]);
                     break;
          case  Air_Temperature_Type:
          myCharArray[3]=computeAir_Temperature_Type(rx_msg);
          ESP_LOGI("RX_MSG","进气温度: %s",myCharArray[3]);
                     break;
          case  Engine_Start_Time_Type:
          myCharArray[4]=computeEngine_Start_Time_Type(rx_msg);
          ESP_LOGI("RX_MSG", "发动机起动后时间: %s", myCharArray[4]);
                     break;
          case  Distance_Type:
          ESP_LOGI("RX_MSG", "距离: %s", myCharArray[6]);
          myCharArray[6]=computeDistance_Type(rx_msg);
                     break;
          case  Strees_Type:
        ESP_LOGI("RX_MSG", "油量: %s", myCharArray[7]);
          myCharArray[7]=computeStrees_Type(rx_msg);
                     break;
          default:
                     break;
        }
      }
      //故障读取
     if ((rx_msg.data[0] == 0x06&&rx_msg.data[1] == 0x43))
     {
          ESP_LOGI("RX_MSG", "故障码");
          myCharArray[5]=computeError_Code_Type(rx_msg);
          ESP_LOGI("RX_MSG", "故障码: %s", myCharArray[5]);
     }
    }

//    char example_data[] = "12,12,12,12,12";  // 替换为您要传递的数据
//    sendData(example_data);
//    ESP_LOGI("TAG", "发送蓝牙数据1111");


    char send_data_str[100]="";
    // 拼接字符串
    for (int i = 0; i < LENGTH; i++) {
        strcat(send_data_str, myCharArray[i]);
        if (i < LENGTH-1) {
            strcat(send_data_str, ",");
        }
    }
    sendData(send_data_str);

 }else{
      //ESP_LOGI("RX_MSG","identifier error:%"PRId32"",rx_msg.identifier);
 }
}

char* computeEngine_Temperature_Type(twai_message_t rx_msg){
    uint8_t data = rx_msg.data[3]-40;
    static char buffer[10];
    sprintf(buffer, "%u", data);
    return buffer;
}

char* computeEngine_Speed_Type(twai_message_t rx_msg){
    uint16_t engineSpeed = (((uint16_t)rx_msg.data[3] << 8) | rx_msg.data[4])/4;
    static char buffer[10];
    sprintf(buffer, "%u", engineSpeed);
    return buffer;
}

char* computeCar_Speed_Type(twai_message_t rx_msg){
    uint8_t data = rx_msg.data[3];
    static char buffer[10];
    sprintf(buffer, "%u", data);
    return buffer;
}

char* computeAir_Temperature_Type(twai_message_t rx_msg){
    uint8_t data = rx_msg.data[3]-40;
    static char buffer[10];
    sprintf(buffer, "%u", data);
    return buffer;
}

char* computeEngine_Start_Time_Type(twai_message_t rx_msg){
    uint16_t time = (((uint16_t)rx_msg.data[3] << 8) | rx_msg.data[4]);
    static char buffer[10];
    sprintf(buffer, "%u", time);
    return buffer;
}

//错误码详细解析
char* computeError_Code_Type(twai_message_t rx_msg){
    uint16_t code =  rx_msg.data[2];
    static char buffer[10];
    sprintf(buffer, "%u", code);
    return buffer;
}
//故障点亮之后的行驶距离
char* computeDistance_Type(twai_message_t rx_msg){
    uint16_t distance = (((uint16_t)rx_msg.data[3] << 8) | rx_msg.data[4]);
    static char buffer[10];
    sprintf(buffer, "%u", distance);
    return buffer;
}
//燃油液位输入
char* computeStrees_Type(twai_message_t rx_msg){
    uint8_t strees = rx_msg.data[3]*100/255;
    static char buffer[10];
    sprintf(buffer, "%u", strees);
    return buffer;
}