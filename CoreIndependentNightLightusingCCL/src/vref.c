/**
 * \file
 *
 * \brief VREF related functionality implementation.
 *
 (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/**
 * \defgroup doc_driver_vref_init VREF Init Driver
 * \ingroup doc_driver_vref
 *
 * \section doc_driver_vref_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <vref.h>

/**
 * \brief Initialize vref interface
 * \return Initialization status.
 */
int8_t VREF_0_init()
{

	VREF_CTRLA = VREF_ADC0REFSEL_0V55_gc   /* Voltage reference at 0.55V */
	             | VREF_DAC0REFSEL_2V5_gc; /* Voltage reference at 2.5V */

	// VREF_CTRLB = 0 << VREF_DAC1REFEN_bp /* DAC1/AC1 reference enable: disabled */
	//		 | 0 << VREF_ADC0REFEN_bp /* ADC0 reference enable: disabled */
	//		 | 0 << VREF_DAC0REFEN_bp /* DAC0/AC0 reference enable: disabled */
	//		 | 0 << VREF_DAC2REFEN_bp /* DAC2/AC2 reference enable: disabled */
	//		 | 0 << VREF_ADC1REFEN_bp; /* ADC1 reference enable: disabled */

	// VREF_CTRLC = VREF_ADC1REFSEL_0V55_gc /* Voltage reference at 0.55V */
	//		 | VREF_DAC1REFSEL_0V55_gc; /* Voltage reference at 0.55V */

	// VREF_CTRLD = VREF_DAC2REFSEL_0V55_gc; /* Voltage reference at 0.55V */

	return 0;
}
