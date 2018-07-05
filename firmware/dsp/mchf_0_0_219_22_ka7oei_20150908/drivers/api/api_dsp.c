/************************************************************************************
**                                                                                 **
**                             mcHF Pro QRP Transceiver                            **
**                         Krassi Atanassov - M0NKA 2013-2018                      **
**                              djchrismarc@gmail.com                              **
**                                      @M0NKA_                                    **
**---------------------------------------------------------------------------------**
**                                                                                 **
**  File name:      api_dsp.c                                                      **
**  Description:    the very first version of the hook api driver                  **
**  Last Modified:  05 July 2018                                                   **
**  Licence:		For radio amateurs experimentation, non-commercial use only!   **
************************************************************************************/

// Common
#include "mchf_board.h"

#ifdef DSP_MODE

#include "audio_driver.h"
#include "ui_rotary.h"

// Transceiver state public structure
extern __IO TransceiverState 		ts;

// ------------------------------------------------
// Spectrum display
extern __IO	SpectrumDisplay			sd;

// ------------------------------------------------
// Frequency public
extern __IO DialFrequency 			df;


// ------------------------------------------------
uchar ou_buffer[300];
uchar in_buffer[300];

uchar api_drv_dissabled = 0;
uchar pub_v = 0;
uchar led_s = 0;
ulong pro_s = 0;

//*----------------------------------------------------------------------------
//* Function Name       : UiLcdHy28_SpiInit
//* Object              :
//* Input Parameters    :
//* Output Parameters   :
//* Functions called    :
//*----------------------------------------------------------------------------
static void api_dsp_spi_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	// Enable the SPI periph
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	// Common SPI settings
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	// SPI SCK pin configuration
	GPIO_InitStructure.GPIO_Pin = LCD_SCK;
	GPIO_Init(LCD_SCK_PIO, &GPIO_InitStructure);

	// SPI  MOSI pins configuration
	GPIO_InitStructure.GPIO_Pin =  LCD_MOSI;
	GPIO_Init(LCD_MOSI_PIO, &GPIO_InitStructure);

	// SPI  MISO pins configuration
	GPIO_InitStructure.GPIO_Pin =  LCD_MISO;
	GPIO_Init(LCD_MISO_PIO, &GPIO_InitStructure);

	// Set as alternative
	GPIO_PinAFConfig(LCD_SCK_PIO,  LCD_SCK_SOURCE,  GPIO_AF_SPI2);
	GPIO_PinAFConfig(LCD_MISO_PIO, LCD_MISO_SOURCE, GPIO_AF_SPI2);
	GPIO_PinAFConfig(LCD_MOSI_PIO, LCD_MOSI_SOURCE, GPIO_AF_SPI2);

	// SPI configuration
	SPI_I2S_DeInit(SPI2);
	SPI_InitStructure.SPI_Direction 		= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize 			= SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL 				= SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA 				= SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS 				= SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit 			= SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode 				= SPI_Mode_Master;
	SPI_Init(SPI2, &SPI_InitStructure);

	// Enable SPI2
	SPI_Cmd(SPI2, ENABLE);

	// Common misc pins settings
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// Configure GPIO PIN for Chip select
	//GPIO_InitStructure.GPIO_Pin = LCD_CS;
	//GPIO_Init(LCD_CS_PIO, &GPIO_InitStructure);

	// PA9 as chip select
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure GPIO PIN for Reset
	//GPIO_InitStructure.GPIO_Pin = LCD_RESET;
	//GPIO_Init(LCD_RESET_PIO, &GPIO_InitStructure);

	// Deselect : Chip Select high
	//GPIO_SetBits(LCD_CS_PIO, LCD_CS);
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
}

static uchar api_dsp_SendByteSpiA(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)  == RESET);
	SPI_I2S_SendData(SPI2, byte);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	return (uchar)SPI_I2S_ReceiveData(SPI2);
}

void api_dsp_init(void)
{
	//printf("api driver init...\n\r");
	ts.api_band = 0;
	api_dsp_spi_init();
}

//*----------------------------------------------------------------------------
//* Function Name       : api_dsp_thread
//* Object              : blink leds and update alive flag
//* Notes    			:
//* Notes   			:
//* Notes    			:
//* Context    			: CONTEXT_RESET_VECTOR
//*----------------------------------------------------------------------------
void api_dsp_thread(void)
{
	if(pro_s < 50000)
	{
		pro_s++;
		return;
	}
	pro_s = 0;

	if(api_drv_dissabled)
	{
		__asm(".word 0x46004600");
		__asm(".word 0x46004600");
		__asm(".word 0x46004600");
		__asm(".word 0x46004600");
	}
	else
	{
		//if(led_s)
		//	mchf_board_red_led(1);
		//else
		//	mchf_board_red_led(0);
	}

	led_s = !led_s;
}

//*----------------------------------------------------------------------------
//* Function Name       : api_dsp_post
//* Object              : send and receive
//* Notes    			:
//* Notes   			:
//* Notes    			:
//* Context    			: CONTEXT_UI_DRIVER
//*----------------------------------------------------------------------------
void api_dsp_post(q15_t *fft)
{
	ulong k;
	ulong tune_loc;

	if(api_drv_dissabled)
		return;

	tune_loc = df.tune_new;

	// Clear buffer
	for(k = 0; k < 300;k++)
		ou_buffer[k] = 0;

	// ----------------------
	// Header
	ou_buffer[0x00] = 0x12; 				// sig
	ou_buffer[0x01] = 0x34;					// sig
	ou_buffer[0x02] = led_s;				// alive flag
	ou_buffer[0x03] = pub_v;				// seq cnt

	// DSP Version
	ou_buffer[0x04] = TRX4M_VER_MAJOR;
	ou_buffer[0x05] = TRX4M_VER_MINOR;
	ou_buffer[0x06] = TRX4M_VER_RELEASE;
	ou_buffer[0x07] = TRX4M_VER_BUILD;

	// Frequency
	ou_buffer[0x08] = tune_loc >> 24;
	ou_buffer[0x09] = tune_loc >> 16;
	ou_buffer[0x0A] = tune_loc >>  8;
	ou_buffer[0x0B] = tune_loc >>  0;

	ou_buffer[0x0C] = ts.dmod_mode;
	//ou_buffer[0x0D] = ts.band;

	if(fft != NULL)
	{
		// Left part of screen
		for(k = 0; k < 128;k++)
			ou_buffer[k + 0x28] = (uchar)*(fft + k + 128);

		// Right part of screen
		for(k = 0; k < 128;k++)
			ou_buffer[k + 128 + 0x28] = (uchar)*(fft + k + 0);
	}

	// Footer
	ou_buffer[298] = 0x55;
	ou_buffer[299] = 0xAA;

	// -------------------------------------
	// Transfer
	//
	// CS Low
	GPIO_ResetBits(GPIOA, GPIO_Pin_9);

	// Leading delay
	for(k = 0; k < 10000; k++)
		__asm(".word 0x46004600");

	// Send buffer
	for(k = 0; k < 300; k++)
		in_buffer[k] = api_dsp_SendByteSpiA(ou_buffer[k]);

	// CS high
	__asm(".word 0x46004600");
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	// -- End transfer

	// Set volume test
	if(in_buffer[1] < MAX_AUDIO_GAIN)
	{
		// value = DEFAULT_AUDIO_GAIN;
		ts.audio_gain = in_buffer[1];
	}

	// Set demodulator mode
	if(in_buffer[2] <= DEMOD_MAX_MODE)
		ts.dmod_mode = in_buffer[2];
	else
	{
		// Test - stop driver
		//--if(in_buffer[2] == 0x77)
		//--	api_drv_dissabled = 1;
	}

	// ---------------------------------------------------------------
	// Make it change band
	if((in_buffer[3] != ts.band) && (in_buffer[3] <= MAX_BANDS))
		ts.api_band = in_buffer[3];

	pub_v++;
}

#endif
