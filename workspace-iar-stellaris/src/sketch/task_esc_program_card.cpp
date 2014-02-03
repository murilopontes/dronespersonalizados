
//ENERGIA
#include "Arduino.h"
#include "pitches.h"

//FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//TASK IT-SELF
#include "task_esc_program_card.h"

//PWM-Write
#include "wiring_private.h"


int pino=PF_1;

void TPC_PIN_INPUT(void){ 
pinMode(pino, INPUT); 
}

void TPC_PIN_OUTPUT(void){
pinMode(pino, OUTPUT);
}

void TPC_PIN_ON(void){ 
digitalWrite(pino,1); 
}

void TPC_PIN_OFF(void){ 
digitalWrite(pino,0); 
}
int TPC_PIN_IS_ON(void){ 
 if (digitalRead(pino)){ 
  return 1; 
 } else {
  return 0;
 } 
}
int TPC_PIN_IS_OFF(void){ 
  if (digitalRead(pino)){ 
    return 0; 
  } else {
    return 1;
  } 
}


void TPC_delay_us(int x){ 
  delay(x); 
}
int TPC_READ_BIT_TIME_WIDTH      (void){ return 2437; }
int TPC_READ_HALF_BIT_TIME_WIDTH (void){ return (TPC_READ_BIT_TIME_WIDTH()/2); }
int TPC_WRITE_BIT_TIME_WIDTH     (void){ return TPC_READ_BIT_TIME_WIDTH(); }
int TPC_WRITE_HALF_BIT_TIME_WIDTH(void){ return (TPC_WRITE_BIT_TIME_WIDTH()/2); };

  
  
enum TPC_battType_t
{
    TPC_battType_Li = 0,
    TPC_battType_Ni = 1
};
  
enum TPC_cutoffType_t
{
    TPC_cutoffType_softCut = 0,
    TPC_cutoffType_cutoff = 1
};
  
enum TPC_cutoffVoltage_t
{
    TPC_cutoffVoltage_low = 0,
    TPC_cutoffVoltage_middle = 1,
    TPC_cutoffVoltage_high = 2
};
  
enum TPC_startMode_t
{
    TPC_startMode_normal = 0,
    TPC_startMode_soft = 1,
    TPC_startMode_verySoft = 2
};
  
enum TPC_timingMode_t
{
    TPC_timingMode_low = 0,
    TPC_timingMode_middle = 1,
    TPC_timingMode_high = 2
};
  
enum TPC_lipoCells_t
{
    TPC_lipoCells_autoDetect = 0,
    TPC_lipoCells_2 = 1,
    TPC_lipoCells_3 = 2,
    TPC_lipoCells_4 = 3,
    TPC_lipoCells_5 = 4,
    TPC_lipoCells_6 = 5,
    TPC_lipoCells_7 = 6,
    TPC_lipoCells_8 = 7,
    TPC_lipoCells_9 = 8,
    TPC_lipoCells_10 = 9,
    TPC_lipoCells_11 = 10,
    TPC_lipoCells_12 = 11
};
  
typedef struct
{
    uint8_t brake;
    //enum  TPC_battType_t 
    uint8_t battType;
    //enum TPC_cutoffType_t 
    uint8_t cutoffType;
    //enum TPC_cutoffVoltage_t 
    uint8_t cutoffVoltage;
    //enum TPC_startMode_t 
    uint8_t startMode;
    //enum TPC_timingMode_t 
    uint8_t timingMode;
    //enum TPC_lipoCells_t 
    uint8_t lipoCells;
    uint8_t governorMode;
      
} TPC_settings_t;


// initializes the settings to default values
void TPC_loadDefault(TPC_settings_t* x)
{
    x->brake = 0;
    x->battType = TPC_battType_Li;
    x->cutoffType = TPC_cutoffType_softCut;
    x->cutoffVoltage = TPC_cutoffVoltage_middle;
    x->startMode = TPC_startMode_normal;
    x->timingMode = TPC_timingMode_low;
    x->lipoCells = TPC_lipoCells_autoDetect;
    x->governorMode = 0;
}
  
// configs the settings struct with 2 bytes (which you should get from the ESC, but you can also store it in your own EEPROM or something)
void TPC_word_to_struct(TPC_settings_t* x, uint16_t y)
{
    // map the bits to the settings
    x->brake = (y & (1 << 0)) == 0 ? 0 : 1;
    x->battType = (y & (1 << 1)) == 0 ? TPC_battType_Li : TPC_battType_Ni;
    x->cutoffType = (y & (1 << 2)) == 0 ? TPC_cutoffType_softCut : TPC_cutoffType_cutoff;
    x->cutoffVoltage = ((y & (0x03 << 3)) >> 3);
    x->startMode = ((y & (0x03 << 5)) >> 5);
    x->timingMode = ((y & (0x03 << 8)) >> 8);
    x->lipoCells = ((y & (0x0F << 10)) >> 10);
    x->governorMode = (y & (1 << 7)) == 0 ? 0 : 1;
}

// translates the settings struct into 2 bytes (which you can send to the ESC, or store in your own EEPROM or something)
uint16_t TPC_struct_to_word(TPC_settings_t* x)
{
    return 0 | 
           ((x->brake ? 1 : 0) << 0) |
           ((x->battType ? 1 : 0) << 1) |
           ((x->cutoffType ? 1 : 0) << 2) |
           ((x->cutoffVoltage & 0x03) << 3) |
           ((x->startMode & 0x03) << 5) |
           ((x->timingMode & 0x03) << 8) |
           ((x->lipoCells & 0x0F) << 10) |
           ((x->governorMode ? 1 : 0) << 7);
}

// reads a byte from a psuedo 10-bit UART
unsigned char TPC_ser_read()
{
    unsigned char i, x = 0;
      
    TPC_PIN_ON(); // input
    TPC_PIN_INPUT();
      
    while (TPC_PIN_IS_OFF()); // wait for powerup if not already
    while (TPC_PIN_IS_ON()); // wait until start of frame
    while (TPC_PIN_IS_OFF()); // this period indicates start of frame
    while (TPC_PIN_IS_ON()); // the first bit always seems to be 1
      
    TPC_delay_us(TPC_READ_BIT_TIME_WIDTH() + TPC_READ_HALF_BIT_TIME_WIDTH()); // skip
      
    // read the 8 bits LSB first
    for (i = 0; i < 8; i++)
    {
        x |= (TPC_PIN_IS_ON() ? 1 : 0) << i;
        TPC_delay_us(TPC_READ_BIT_TIME_WIDTH());
    }
      
    return x;
}



// writes a byte to a psuedo 10-bit UART
void TPC_ser_write(unsigned char x)
{   
    TPC_PIN_ON(); // make sure
    TPC_PIN_OUTPUT();
    TPC_delay_us(TPC_WRITE_BIT_TIME_WIDTH());
      
    TPC_PIN_OFF(); // signal start
    TPC_delay_us(TPC_WRITE_BIT_TIME_WIDTH());
      
    TPC_PIN_ON(); // first bit always 1
    TPC_delay_us(TPC_WRITE_BIT_TIME_WIDTH());
      
    TPC_PIN_OFF(); // 2nd bit always 0
    TPC_delay_us(TPC_WRITE_BIT_TIME_WIDTH());
      
    // send the byte LSB first
    char i;
    for (i = 0; i < 8; i++)
    {
        if ((x & (1 << i)) == 0)
        {
            TPC_PIN_OFF();
        }
        else
        {
            TPC_PIN_ON();
        }
        TPC_delay_us(TPC_WRITE_BIT_TIME_WIDTH());
    }
    TPC_PIN_ON(); // leave as input
    TPC_PIN_INPUT();
}
  
// must be sent after receiving configuration from ESC upon initialization
void TPC_send_init_ack()
{
    TPC_PIN_ON();
    TPC_PIN_OUTPUT();
    TPC_delay_us(TPC_WRITE_BIT_TIME_WIDTH());
      
    // send pulses
    char i;
    for (i = 0; i < 6; i++)
    {
        TPC_PIN_OFF();
        TPC_delay_us(TPC_WRITE_BIT_TIME_WIDTH());
        TPC_PIN_ON();
        TPC_delay_us(TPC_WRITE_BIT_TIME_WIDTH());
    }
      
    TPC_PIN_INPUT(); // leave clean
}
  
// receive the ack from ESC after writing config to ESC
void TPC_wait_for_ack()
{
    TPC_ser_read();
}
  
// receive current config from ESC
void TPC_read_init(TPC_settings_t* x)
{
    // read in 2 bytes
    unsigned short y;
    y = TPC_ser_read();
    y |= TPC_ser_read() << 8;
      
    TPC_word_to_struct(x, y);
      
    TPC_delay_us(TPC_READ_BIT_TIME_WIDTH()); // a small delay
    TPC_delay_us(TPC_READ_BIT_TIME_WIDTH()); // a small delay
    TPC_delay_us(TPC_READ_BIT_TIME_WIDTH()); // a small delay
    TPC_delay_us(TPC_READ_BIT_TIME_WIDTH()); // a small delay
      
    TPC_send_init_ack(); // must be sent after receiving configuration from ESC upon initialization
}
  
// sends configuration to ESC
void TPC_send_config(TPC_settings_t* x)
{
    unsigned short y = TPC_struct_to_word(x);
      
    // start writing the config, LSB first
    TPC_ser_write(y & 0xFF);
    TPC_ser_write((y >> 8) & 0xFF);
    // just a small note, these two bytes match the received config during initialization
      
    // the settings are sent in this format
    TPC_ser_write((y & (0x01 << 0)) >> 0);
    TPC_ser_write((y & (0x01 << 1)) >> 1);
    TPC_ser_write((y & (0x01 << 2)) >> 2);
    TPC_ser_write((y & (0x03 << 3)) >> 3);
    TPC_ser_write((y & (0x03 << 5)) >> 5);
    TPC_ser_write((y & (0x03 << 8)) >> 8);
    TPC_ser_write((y & (0x01 << 7)) >> 7);
      
    // this is where the string of notes would be, but I don't have that implemented, so these two are just null
    TPC_ser_write(0); 
    TPC_ser_write(0);
      
    TPC_ser_write(11); // this is actually a byte count
      
    TPC_wait_for_ack(); // do not unpower ESCs until the ack has been received, since it's writing to EEPROM during this time
}

/*
static int ser_tx(char c, FILE* f)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}
*/
  


extern "C" void task_esc(void *pvParameters){

  
    TPC_PIN_OUTPUT();
    TPC_PIN_ON();
    
    
    PWMWrite(PF_1,256,26,50);
    
    while(1);
    TPC_PIN_INPUT();
      
    Serial.print("\r\nTesting Begin\r\n");
    // at this point, plug in the ESC to the battery
      
    TPC_settings_t mySettings;
      
    TPC_read_init(&mySettings);
      
    uint16_t x = TPC_struct_to_word(&mySettings);
    Serial.print("Read from ESC: ");
    Serial.println(x,HEX);
      
    // here we change all the settings
    mySettings.brake = mySettings.brake ? 0 : 1;
    mySettings.battType = mySettings.battType ? 0 : 1;
    mySettings.cutoffType = mySettings.cutoffType ? 0 : 1;
    mySettings.cutoffVoltage = (mySettings.cutoffVoltage + 1) % 3;
    mySettings.startMode = (mySettings.startMode + 1) % 3;
    mySettings.timingMode = (mySettings.timingMode + 1) % 3;
    mySettings.lipoCells = (mySettings.lipoCells + 1) % (TPC_lipoCells_12 + 1);
    mySettings.governorMode = mySettings.governorMode ? 0 : 1;  
    x = TPC_struct_to_word(&mySettings);
    
    Serial.print("Sending from ESC: ");
    Serial.println(x,HEX);
    TPC_delay_us(1000);
    TPC_send_config(&mySettings);
      
    Serial.print("configuration is done! reboot to confirm persistence\r\n");
      
}




