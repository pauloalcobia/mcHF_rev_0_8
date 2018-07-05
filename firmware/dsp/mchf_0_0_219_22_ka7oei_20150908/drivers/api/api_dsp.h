/************************************************************************************
**                                                                                 **
**                             mcHF Pro QRP Transceiver                            **
**                         Krassi Atanassov - M0NKA 2013-2018                      **
**                              djchrismarc@gmail.com                              **
**                                      @M0NKA_                                    **
**---------------------------------------------------------------------------------**
**                                                                                 **
**  File name:      api_dsp.h                                                      **
**  Description:    the very first version of the hook api driver                  **
**  Last Modified:  05 July 2018                                                   **
**  Licence:		For radio amateurs experimentation, non-commercial use only!   **
************************************************************************************/

#ifndef __API_DSP_H
#define __API_DSP_H

#ifdef DSP_MODE

// Exports
void api_dsp_init(void);
void api_dsp_thread(void);

void api_dsp_post(q15_t *fft);

#endif

#endif
