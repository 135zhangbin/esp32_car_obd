#include <stdbool.h>
#include "esp_err.h"
#include "driver/twai.h"

void init_sendcommond();
void OBD_twai_init_250(void); 
void OBD_twai_init_500(void);
bool OBD_twai_init_auto_baudrate(void);
void OBD_twai_deinit(void);
void twai_transmit_data();
void twai_receive_data();

enum  protocol
{
    ISO15765_11bit_500K, 
    ISO15765_11bit_250K, 
    ISO15765_29bit_500K, 
    ISO15765_29bit_250K
};
enum  CommondType
{
    Engine_Temperature_Type, 
    Engine_Speed_Type, 
    Car_Speed_Type, 
    Air_Temperature_Type,
    Engine_Start_Time_Type,
    Error_Code_Type,
    Distance_Type,
    Strees_Type
};
struct SendCommond {
  twai_message_t tx_msg;
  enum CommondType commondType;
};



