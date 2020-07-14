#include <atmel_start.h>
#include <util/delay.h>
#include <avr/sleep.h>

uint8_t  Number_of_LEDS = 16; // Selects the number of RGB leds connected
uint8_t  ON             = 1;
uint8_t  OFF            = 0;
uint16_t address        = EEPROM_START; // Adress in the eeprom the color and intensity are stored

uint8_t portB_intflags = 0;
uint8_t status_flags   = 0; // Variable used to determine witch interrupts that have been triggered

struct {
	uint8_t red; // RGB variables
	uint8_t green;
	uint8_t blue;
	uint8_t color;     // Used as bitfield. Keeps track witch of the RGB diodes that should on or off
	uint8_t intensity; // Intensity value variable
} leds;

void setup_interrupt_and_sleepmode(void);
void blink_selected(void);
void change_color(void);
void update_LEDS(uint8_t, uint8_t);
void SPI_Exchange8bit(uint8_t);

void get_stored_color_from_EE(void);
void store_color_to_EE(void);

ISR(PORTB_PORT_vect)
{
	portB_intflags = PORTB.INTFLAGS;

	if (portB_intflags & 0x01) { // Button 1 has been pressed and generated a low level interrupt
		status_flags |= 0x01;    // The status flag is set
		PORTB.INTFLAGS = 0x01;   // Interrupt flag cleared
	}

	if (portB_intflags & 0x02) { // Button 2 has been pressed and generated a low level interrupt
		status_flags |= 0x02;    // The status flag is set
		PORTB.INTFLAGS = 0x02;   // Interrupt flag cleared
	}

	if (portB_intflags & 0x10) {
		if ((PORTB.IN & PIN4_bm) == 0x10) // Edge interrupt triggered, check if pin if high or low
		{                                 // If pin is high, CCL LUT0 has generated a rising edge interrupt
			status_flags |= 0x03;
		} else { // if pin is low, CCL LUT0 has generated a falling edge interrupt
			status_flags |= 0x04;
		}
		PORTB.INTFLAGS = 0x10; // Interrupt flag cleared
	}
}

// In main the
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	// Configure interrupts and sleepmode
	setup_interrupt_and_sleepmode();

	// Make sure all LEDs are off
	update_LEDS(OFF, Number_of_LEDS);

	// Get the stored color and intensity from eeprom
	get_stored_color_from_EE();

	while (1) {
		cli();
		if (status_flags == 0) // If no interrupts have been triggered, goto sleep
		{
			sei();
			sleep_cpu(); // The device will remain in sleep until any of the buttons are pressed
		}                // to change color or intensity or when LUT0 output goes high.
		else {
			Check_flags(); // Check witch flags have been set
		}
	}
}

void Check_flags(void)
{
	switch (status_flags) {
	case 0x01:                            // Button 1 have been pressed to change the LED color
		while (Button_1_get_level() == 0) // Remain in the loop as long as the button is held low
		{
			change_color();                  // Change to next color
			update_LEDS(ON, Number_of_LEDS); // Write color to LEDs
			_delay_ms(300);                  // Delay to allow user to observe the color
		}
		store_color_to_EE();   // When button is released, the color is chosen and stored in eeprom
		blink_selected();      // Selected color is displayed on the LEDs
		status_flags &= ~0x01; // Delete this status flag

		break;

	case 0x02:                            // Button 2 have been pressed to change the LED intensity
		while (Button_2_get_level() == 0) // Remain in the loop as long as the button is held low
		{
			leds.intensity += 0x04;     // Change intensity
			if (leds.intensity >= 0x30) // If intensity is set equal or higher than 0x30 it will be set back to 0x08
			{ // This is done because the LEDs consume a lot of power, spesially when using white light
				leds.intensity = 0x04;
			}
			update_LEDS(ON, Number_of_LEDS); // Write color to LEDs using new intensity
			_delay_ms(300);                  // Delay to allow user to observe the intensity
		}
		store_color_to_EE();   // When button is released, the intensity is chosen and stored in eeprom
		blink_selected();      // Selected color is displayed on the LEDs
		status_flags &= ~0x02; // Delete this status flag

		break;

	case 0x03:
		update_LEDS(ON, Number_of_LEDS); // Turn LEDs on
		status_flags &= ~0x03;           // Delete this status flag
		break;

	case 0x04:
		update_LEDS(OFF, Number_of_LEDS); // Turn LEDs off
		status_flags &= ~0x04;            // Delete this status flag
		break;

	default:
		update_LEDS(OFF, Number_of_LEDS); // Turn LEDs on
		status_flags = 0;                 // Delete all status flags
		break;
	}
}

void get_stored_color_from_EE(void)
{
	// Read color and intensity values from eeprom into the leds struct
	leds.red       = *((volatile uint8_t *)(address));
	leds.green     = *((volatile uint8_t *)(address + 1));
	leds.blue      = *((volatile uint8_t *)(address + 2));
	leds.intensity = *((volatile uint8_t *)(address + 3));
	leds.color     = *((volatile uint8_t *)(address + 4));

	// The very first time the code is run on a device the eeprom might be erased
	// Check if the values are 0xFF and if they are set them so the lowest setting and store them into the eeprom
	if ((leds.red == 0xff) | (leds.green == 0xff) | (leds.blue == 0xff) | (leds.intensity == 0xff)
	    | (leds.color == 0xff)) {
		leds.red       = 0x08;
		leds.green     = 0x08;
		leds.blue      = 0x08;
		leds.intensity = 0x08;
		leds.color     = 1;

		store_color_to_EE();
	}
}

void store_color_to_EE(void)
{
	*((volatile uint8_t *)(address)) = 0;

	CPU_CCP       = CCP_SPM_gc;
	NVMCTRL.CTRLA = NVMCTRL_CMD_PAGEBUFCLR_gc;
	while (NVMCTRL.STATUS & NVMCTRL_EEBUSY_bm)
		;

	/* Load data to write, which triggers the loading of EEPROM page buffer. */

	*((volatile uint8_t *)(address))     = leds.red;
	*((volatile uint8_t *)(address + 1)) = leds.green;
	*((volatile uint8_t *)(address + 2)) = leds.blue;
	*((volatile uint8_t *)(address + 3)) = leds.intensity;
	*((volatile uint8_t *)(address + 4)) = leds.color;

	//  Issue EEPROM Atomic Write (Erase&Write) command. Load command, write
	// the protection signature and execute command.

	CPU_CCP       = CCP_SPM_gc;
	NVMCTRL.CTRLA = NVMCTRL_CMD_PAGEERASEWRITE_gc;
	while (NVMCTRL.STATUS & NVMCTRL_EEBUSY_bm)
		;
}

void SPI_Exchange8bit(uint8_t data)
{
	// Composition of 24bit data:
	// G7 G6 G5 G4 G3 G2 G1 G0 R7 R6 R5 R4 R3 R2 R1 R0 B7 B6 B5 B4 B3 B2 B1 B0
	// Note: Follow the order of GRB to sent data and the high bit sent at first.
	// Clear the Write Collision flag, to allow writing
	SPI0.INTFLAGS = SPI0_INTFLAGS;

	// Reset TCA counter register to ensure the first rising edge of PWM is predictable
	TCA0.SINGLE.CNT = 0 /* Count: 0 */;

	// Start TCA
	TCA0.SINGLE.CTRLA = TCA_SPLIT_CLKSEL_DIV1_gc /* System Clock */
	                    | 1 << TCA_SPLIT_ENABLE_bp /* Module Enable: enabled */;

	// Start SPI by writing a byte to SPI data register
	SPI0.DATA = data;

	// Wait for transfer to complete
	while ((SPI0.INTFLAGS & SPI_RXCIF_bm) == 0) {
	}

	// Stop TCA
	TCA0.SINGLE.CTRLA = TCA_SPLIT_CLKSEL_DIV1_gc /* System Clock */
	                    | 0 << TCA_SPLIT_ENABLE_bp /* Module Enable: disabled */;
}

void update_LEDS(volatile uint8_t on_off, volatile uint8_t led_number)
{
	// Check if LEDs should be turned ON or OFF.
	if (on_off == 1) {
		if (leds.color & 0x01) // If bit 1 in color is set, the green LED should be turned on
		{
			leds.green = leds.intensity;
		}
		if (leds.color & 0x02) // If bit 2 in color is set, the red LED should be turned on
		{
			leds.red = leds.intensity;
		}
		if (leds.color & 0x04) // If bit 4 in color is set, the blue LED should be turned on
		{
			leds.blue = leds.intensity;
		}

		write_to_leds(led_number); // Write to the LEDs
	} else {                       // Turn off the LEDs
		leds.green = 0x00;
		leds.red   = 0x00;
		leds.blue  = 0x00;
		write_to_leds(led_number); // Write to the LEDs
	}
}

void write_to_leds(uint8_t x_leds)
{
	for (uint8_t n = 0; n < x_leds; n++) {
		// Transmit 24-bit RGB color data 8bit at a time using SPI
		SPI_Exchange8bit(leds.green); // GREEN
		SPI_Exchange8bit(leds.red);   // RED
		SPI_Exchange8bit(leds.blue);  // BLUE
	}
}

void setup_interrupt_and_sleepmode(void)
{

	PORTB_PIN0CTRL = 0x0D; // Enable low level interrupt on PB0 low(button 1)
	PORTB_PIN1CTRL = 0x0D; // Enable low level interrupt on PB1 low(button 2)
	PORTB_PIN4CTRL = 0x01; // Enable both edges interrupt on LUT0 output pin

	PORTA_PIN7CTRL = 0x04; // Disable pullup and digital input buffer

	PORTC_PIN1CTRL = 0x00; // Disable pullup

	SLPCTRL.CTRLA = 1 << SLPCTRL_SEN_bp /* Sleep enable: enabled */
	                | SLPCTRL_SMODE_STDBY_gc /* Standby mode */;
}

void change_color(void)
{
	// set all colors to zero
	leds.green = 0x00;
	leds.red   = 0x00;
	leds.blue  = 0x00;

	leds.color++;
	if (leds.color > 7) {
		leds.color = 1;
	}
}

void blink_selected(void)
{
	for (uint8_t i = 0; i < 2; i++) {
		update_LEDS(ON, Number_of_LEDS);
		_delay_ms(200);
		update_LEDS(OFF, Number_of_LEDS);
		_delay_ms(200);
	}
}
