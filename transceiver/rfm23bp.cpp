#include <SPI.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <avr/pgmspace.h>

#include "rfm23bp.h"

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))


#define RH_SPI_WRITE_MASK                                  0x80

#define RFM23BP_TIMEOUT                                    100

#define SS_PORT                                            2

static rfm23bp_params * internal_params;
rfm23bp_params params;
uint8_t RFM_DATA[RFM23_FIFO_LENGTH];

void rfm23bp_init(rfm23bp_params * params)
{
  internal_params = params;
  memset(internal_params,0x00,sizeof *internal_params);
  }

void rfm23bp_temperature_init()
{
  rfm23bp_enable_adc_input();
  rfm23bp_set_adc_reference();
  rfm23bp_set_temperature_range(m64_p64C);
  rfm23bp_enable_adc_reading();
}

void rfm23_reset_rx_fifo()
{
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0x02);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0);
}

void rfm23bp_transmit(uint8_t* data, uint8_t data_length)
{
  rfm23_reset_rx_fifo();
  //while (!rfm23bp_tx_available()){};
  rfm23bp_write_data_to_fifo(data,data_length);
  rfm23bp_enable_tx();
}


uint8_t rfm23bp_received()
{
  uint8_t reg;
  rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_2);
  reg = rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_1);
  if(reg=255) return 0;
  if ((BIT_CHECK(reg,4))/* && (BIT_CHECK(reg,4))*/)
    return 1;
  else return 0;
}

void rfm23bp_set_address(uint8_t address)
{
  if (address<=MAX_I2C_ADDRESS)
    internal_params->address = address;
  else internal_params->address = 0;
}

uint8_t rfm23bp_set_register(uint8_t reg,uint8_t value)
{
    //BIT_CLEAR(PORTB,0);//BIT_CLEAR(PORTE,7);
    digitalWrite(SS,LOW);
    SPI.transfer(reg | RH_SPI_WRITE_MASK);
    SPI.transfer(value);
    //BIT_SET(PORTB,0);//BIT_SET(PORTE,7);
    digitalWrite(SS,HIGH);
    //delay(100);
    return 0;
}


uint8_t rfm23bp_read_register(uint8_t reg)
{
    //BIT_CLEAR(PORTB,0);//BIT_CLEAR(PORTE,7);
    digitalWrite(SS,LOW);
    SPI.transfer(reg);
    uint8_t value = SPI.transfer(0);

    //while (1);
    digitalWrite(SS,HIGH);
    //BIT_SET(PORTB,0);//BIT_SET(PORTE,7);
    //delay(100);
    return value;
}

void rfm23bp_enable_rx()
{
  /*uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  BIT_SET(creg,2);
  BIT_CLEAR(creg,3);*/
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x04);//creg);
  //rfm23bp_set_register(RFM23BP_GPIO0_CONFIGURATION,0xD5);
}


void rfm23bp_enable_tx()
{
  /*uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  BIT_SET(creg,3);
  BIT_CLEAR(creg,2);*/
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x09);//creg);
  //rfm23bp_set_register(RFM23BP_GPIO1_CONFIGURATION,0xD2);
}


void rfm23bp_standby()
{
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0);
}


void rfm23bp_sleep()
{
  uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  BIT_SET(creg,6);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,creg);
}

void rfm23bp_ready()
{
  uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  //Serial.println(creg,HEX);
  BIT_SET(creg,0);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,creg);
}


void rfm23bp_mode_spi(uint8_t slave_select)
{
  internal_params->slave_select_pin = slave_select;
  internal_params->spi_or_twi = 1;
  //SS_PIN = internal_params->slave_select_pin;
  SPI.begin();

  uint8_t addr = 0;
  uint8_t outVal = 0;

  outVal = rfm23bp_read_register(addr);

  /*digitalWrite(slave_select, LOW);
  SPI.transfer(addr);
  outVal = SPI.transfer(0x00);
  digitalWrite(slave_select, HIGH);*/

  //Serial.println(outVal);
  //delay(100);
}

void rfm23bp_reset()
{
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x80);
  delayMicroseconds(1);
}

void rfm23bp_enable_adc_input()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION);
  BIT_CLEAR(reg,4);
  BIT_CLEAR(reg,5);
  BIT_CLEAR(reg,6);
  rfm23bp_set_register(RFM23BP_ADC_CONFIGURATION,reg);
}

void rfm23bp_set_adc_reference()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION);
  BIT_CLEAR(reg,2);
  BIT_CLEAR(reg,3);
  rfm23bp_set_register(RFM23BP_ADC_CONFIGURATION,reg);
}

void rfm23bp_set_temperature_range(rfm23bp_temperature_range range)
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_TEMPERATURE_SENSOR_CONTROL);
  if(range<3)
  {
    reg &= (range << 6 | 0x3F);
    BIT_SET(reg,5);
  }
  else if(range==3)
  {
    BIT_SET(reg,7);
    BIT_SET(reg,6);
    BIT_SET(reg,5);  }
  else
  {
    BIT_SET(reg,7);
    BIT_CLEAR(reg,6);
    BIT_CLEAR(reg,5);
  }
  
  rfm23bp_set_register(RFM23BP_TEMPERATURE_SENSOR_CONTROL,reg);
}

void rfm23bp_enable_adc_reading()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION);
  BIT_SET(reg,7);
  rfm23bp_set_register(RFM23BP_ADC_CONFIGURATION,reg);
}

uint8_t rfm23bp_read_temperature()
{
  rfm23bp_temperature_init();
  uint64_t time = millis();
  while ((!(rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION) & 0x80)) && ((millis()-time)<RFM_TIMEOUT));
  return rfm23bp_read_register(RFM23BP_ADC_VALUE);
}

void rfm23bp_set_power_level(rfm23bp_power_level level)
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_TX_POWER);
  if(BIT_CHECK(level,0)) BIT_SET(reg,0); else BIT_CLEAR(reg,0);
  if(BIT_CHECK(level,1)) BIT_SET(reg,1); else BIT_CLEAR(reg,1);
  if(BIT_CHECK(level,2)) BIT_SET(reg,2); else BIT_CLEAR(reg,2);
  rfm23bp_set_register(RFM23BP_TX_POWER,reg);
}

/*
** РЈСЃС‚Р°РЅРѕРІРёС‚СЊ РїР°СЂР°РјРµС‚СЂС‹ РґР»СЏ РЅРµСЃСѓС‰РµР№ С‡Р°СЃС‚РѕС‚С‹ 433РњР“С† (СЃРј. РєР°Р»СЊРєСѓР»СЏС‚РѕСЂ СЂРµРіРёСЃС‚СЂРѕРІ)
*/
void rfm23bp_set_default_carrier_frequency_433()
{
  rfm23bp_set_register(RFM23BP_FREQUENCY_BAND_SELECT,0x53);
  rfm23bp_set_register(RFM23BP_NOMINAL_CARRIER_FREQUENCY_1,0x4B);
  rfm23bp_set_register(RFM23BP_NOMINAL_CARRIER_FREQUENCY_0,0x00);
}

/*
** РЈСЃС‚Р°РЅРѕРІРёС‚СЊ РїР°СЂР°РјРµС‚СЂС‹ РґР»СЏ РЅРµСЃСѓС‰РµР№ С‡Р°СЃС‚РѕС‚С‹ 435РњР“С† (СЃРј. РєР°Р»СЊРєСѓР»СЏС‚РѕСЂ СЂРµРіРёСЃС‚СЂРѕРІ)
*/
void rfm23bp_set_default_carrier_frequency_435()
{
  rfm23bp_set_register(RFM23BP_FREQUENCY_BAND_SELECT,0x53);
  rfm23bp_set_register(RFM23BP_NOMINAL_CARRIER_FREQUENCY_1,0x7D);
  rfm23bp_set_register(RFM23BP_NOMINAL_CARRIER_FREQUENCY_0,0x00);
}


void rfm23bp_set_default_frequency_deviation()
{
  rfm23bp_set_register(RFM23BP_FREQUENCY_DEVIATION,0x20);
}

void rfm23bp_set_default_modulation()
{
  rfm23bp_set_register(RFM23BP_MODULATION_MODE_CONTROL_1,0x2C);
//  rfm23bp_set_register(RFM23BP_MODULATION_MODE_CONTROL_2,0x23);
  rfm23bp_set_register(RFM23BP_MODULATION_MODE_CONTROL_2,0x22);
}

void rfm23bp_set_default_baudrate()
{
//  rfm23bp_set_register(RFM23BP_TX_DATA_RATE_1,0x09);
  rfm23bp_set_register(RFM23BP_TX_DATA_RATE_1,0x4E);
  //rfm23bp_set_register(RFM23BP_TX_DATA_RATE_0,0xD5);
  rfm23bp_set_register(RFM23BP_TX_DATA_RATE_0,0xA5);
}

void rfm23bp_set_default_header_settings()
{
  rfm23bp_set_register(RFM23BP_TRANSMIT_PACKET_LENGTH,0x40);
  rfm23bp_set_register(RFM23BP_HEADER_CONTROL_2,0x0A);
  rfm23bp_set_register(RFM23BP_HEADER_CONTROL_1,0x8C);
  rfm23bp_set_register(RFM23BP_DATA_ACCESS_CONTROL,0xA8);
  
  rfm23bp_set_register(RFM23BP_PREAMBLE_DETECTION_CONTROL,0x22);
  
  rfm23bp_set_register(RFM23BP_RX_FIFO_CONTROL,63);//31);//63
}

void rfm23bp_set_default_rx_settings()
{
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_OVERSAMPLING_RATIO,0x68);
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_OFFSET_2,0x01);
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_OFFSET_1,0x3A);
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_OFFSET_0,0x93);
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_TIMING_LOOP_GAIN_1,0x01);
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_TIMING_LOOP_GAIN_0,0x30);
  rfm23bp_set_register(RFM23BP_AFC_LIMITER,0x1E);
  rfm23bp_set_register(RFM23BP_IF_FILTER_BANDWIDTH,0x2E);

}

void rfm23bp_set_default_radio_settings()
{
//  rfm23bp_set_default_carrier_frequency_435();
  rfm23bp_set_default_carrier_frequency_433();
  rfm23bp_set_default_frequency_deviation();
  rfm23bp_set_default_modulation();
  rfm23bp_set_default_baudrate();
  rfm23bp_set_default_header_settings();
  rfm23bp_set_default_rx_settings();

  
  rfm23bp_set_register(RFM23BP_INTERRUPT_ENABLE_1,0x14);
  rfm23bp_set_register(RFM23BP_INTERRUPT_ENABLE_2,0x00);
}

void rfm23bp_set_ook_modulation()
{
  rfm23bp_set_register(RFM23BP_MODULATION_MODE_CONTROL_1,0x24);
  rfm23bp_set_register(RFM23BP_MODULATION_MODE_CONTROL_2,0x21);
}


void rfm23bp_write_byte_to_fifo(uint8_t data)
{
  rfm23bp_set_register(RFM23BP_FIFO_ACCESS,data);
}

uint8_t rfm23bp_read_byte_from_fifo()
{
  return rfm23bp_read_register(RFM23BP_FIFO_ACCESS);
}

void rfm23bp_write_data_to_fifo(uint8_t * data, uint8_t length)
{
  uint8_t i;
  if (length>RFM23_FIFO_LENGTH) return;
  for (i=0;i<length;i++)
    rfm23bp_set_register(RFM23BP_FIFO_ACCESS,data[i]);
}

uint8_t rfm23bp_get_rx_fifo()
{
  uint8_t i;
  
  uint8_t rssi = rfm23bp_read_register(RFM23BP_RECEIVED_SIGNAL_STRENGTH_INDICATOR);

  ///*

  uint8_t data[RFM23_FIFO_LENGTH+3] = "";
    
    for (i=0;i<RFM23_FIFO_LENGTH;i++)
      RFM_DATA[i] = rfm23bp_read_register(RFM23BP_FIFO_ACCESS);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0x02);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0);
  
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x04);
  
  return rssi;
}

uint8_t rfm23bp_tx_available()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_1);
  rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_2);
  if (BIT_CHECK(reg,2))
    return 1;
  else return 0;
}


