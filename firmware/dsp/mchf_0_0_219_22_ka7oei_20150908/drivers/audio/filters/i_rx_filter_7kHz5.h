/************************************************************************************
**                                                                                 **
**                               mcHF QRP Transceiver                              **
**                             K Atanassov - M0NKA 2014                            **
**                                                                                 **
**---------------------------------------------------------------------------------**
**                                                                                 **
**  File name:                                                                     **
**  Description:                                                                   **
**  Last Modified:                                                                 **
**  Licence:		For radio amateurs experimentation, non-commercial use only!   **
************************************************************************************/

#ifndef __I_RX_FILTER_7K5_H
#define __I_RX_FILTER_7K5_H

#define I_BLOCK_SIZE		1
#define I_NUM_TAPS			89

/*
 	 * Hilbert 0 Degree, "Phase-added" bandpass filter
     * Kaiser Window FIR Filter, Beta = 5.75, Raised Cosine = 0.91
     * Fc = 2.26 kHz (0.094)
     * BW = 10.3 kHz (0.428)
     * 89 Taps
     *
     * 20140926 by KA7OEI using Iowa Hills Hilbert Filter Designer
     *
     * This filter used in the "7.5 kHz" RX filter position of the mcHF
*/

const float i_rx_7k5_coeffs[I_NUM_TAPS] =
{
		-0.000047152981653544,
		-0.000110026565186158,
		-0.000095405108317349,
		0.000037624035064354,
		0.000192202262504789,
		0.000171234872189907,
		-0.000122839931661074,
		-0.000499087466789727,
		-0.000554573184328879,
		-0.000060588292528025,
		0.000668772060340029,
		0.000888593187081358,
		0.000102515515551741,
		-0.001249921161362290,
		-0.001903095045735830,
		-0.000868621193042000,
		0.001346264630606470,
		0.002745142468753870,
		0.001509633500856800,
		-0.001948801772718740,
		-0.004687224218310920,
		-0.003581927332951620,
		0.001381645765114430,
		0.006138876544307210,
		0.005629613782664030,
		-0.001162538364411810,
		-0.008986602959997310,
		-0.010039404665373800,
		-0.001363884161919830,
		0.010763050651804400,
		0.014785374341762900,
		0.004299482561992500,
		-0.013990436914319800,
		-0.023502567244261900,
		-0.011858171107267000,
		0.015392025256479800,
		0.035034901926729100,
		0.023919333282717500,
		-0.018102510217819800,
		-0.059883515107186500,
		-0.056272796648812800,
		0.018325151509416600,
		0.144877690937171000,
		0.264565982473051000,
		0.313582971052170000,
		0.264565982473059000,
		0.144877690937182000,
		0.018325151509425500,
		-0.056272796648809600,
		-0.059883515107188600,
		-0.018102510217823600,
		0.023919333282715300,
		0.035034901926729700,
		0.015392025256481900,
		-0.011858171107265400,
		-0.023502567244262100,
		-0.013990436914321200,
		0.004299482561991270,
		0.014785374341762900,
		0.010763050651805400,
		-0.001363884161918720,
		-0.010039404665373400,
		-0.008986602959997760,
		-0.001162538364412480,
		0.005629613782663730,
		0.006138876544307410,
		0.001381645765114760,
		-0.003581927332951560,
		-0.004687224218311200,
		-0.001948801772719100,
		0.001509633500856680,
		0.002745142468754090,
		0.001346264630606850,
		-0.000868621193041729,
		-0.001903095045735810,
		-0.001249921161362460,
		0.000102515515551548,
		0.000888593187081267,
		0.000668772060340053,
		-0.000060588292527955,
		-0.000554573184328831,
		-0.000499087466789723,
		-0.000122839931661093,
		0.000171234872189892,
		0.000192202262504788,
		0.000037624035064360,
		-0.000095405108317345,
		-0.000110026565186159,
		-0.000047152981653547
};

/*
static float32_t 		FirState_I[128];
arm_fir_instance_f32 	FIR_I;
*/
#endif
