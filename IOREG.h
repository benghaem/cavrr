/* ATtiny 25 / 45 / 85 IO Register Names
 * generated from: http://www.atmel.com/Images/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf
 *
 * Named registers also have named values for their bits
 * Named bits are shown below as REGISTERNAME_BITNAME
 */

#ifndef IOREG_H
#define IOREG_H

#define SREG          0x3F
#define SREG_I        7
#define SREG_T        6
#define SREG_H        5
#define SREG_S        4
#define SREG_V        3
#define SREG_N        2
#define SREG_Z        1
#define SREG_C        0
#define SPH           0x3E
#define SPH_SP9       1
#define SPH_SP8       0
#define SPL           0x3D
#define SPL_SP7       7
#define SPL_SP6       6
#define SPL_SP5       5
#define SPL_SP4       4
#define SPL_SP3       3
#define SPL_SP2       2
#define SPL_SP1       1
#define SPL_SP0       0
#define GIMSK         0x3B
#define GIMSK_INT0    6
#define GIMSK_PCIE    5
#define GIFR          0x3A
#define GIFR_INTF0    6
#define GIFR_PCIF     5
#define TIMSK         0x39
#define TIMSK_OCIE1A  6
#define TIMSK_OCIE1B  5
#define TIMSK_OCIE0A  4
#define TIMSK_OCIE0B  3
#define TIMSK_TOIE1   2
#define TIMSK_TOIE0   1
#define TIFR          0x38
#define TIFR_OCF1A    6
#define TIFR_OCF1B    5
#define TIFR_OCF0A    4
#define TIFR_OCF0B    3
#define TIFR_TOV1     2
#define TIFR_TOV0     1
#define SPMCSR        0x37
#define SPMCSR_RSIG   5
#define SPMCSR_CTPB   4
#define SPMCSR_RFLB   3
#define SPMCSR_PGWRT  2
#define SPMCSR_PGERS  1
#define SPMCSR_SPMEN  0
#define MCUCR         0x35
#define MCUCR_BODS    7
#define MCUCR_PUD     6
#define MCUCR_SE      5
#define MCUCR_SM1     4
#define MCUCR_SM0     3
#define MCUCR_BODSE   2
#define MCUCR_ISC01   1
#define MCUCR_ISC00   0
#define MCUSR         0x34
#define MCUSR_WDRF    3
#define MCUSR_BORF    2
#define MCUSR_EXTRF   1
#define MCUSR_PORF    0
#define TCCR0B        0x33
#define TCCR0B_FOC0A  7
#define TCCR0B_FOC0B  6
#define TCCR0B_WGM02  3
#define TCCR0B_CS02   2
#define TCCR0B_CS01   1
#define TCCR0B_CS00   0
#define TCNT0         0x32
#define OSCCAL        0x31
#define TCCR1         0x30
#define TCCR1_CTC1    7
#define TCCR1_PWM1A   6
#define TCCR1_COM1A1  5
#define TCCR1_COM1A0  4
#define TCCR1_CS13    3
#define TCCR1_CS12    2
#define TCCR1_CS11    1
#define TCCR1_CS10    0
#define TCNT1         0x2F
#define OCR1A         0x2E
#define OCR1C         0x2D
#define GTCCR         0x2C
#define GTCCR_TSM     7
#define GTCCR_PWM1B   6
#define GTCCR_COM1B1  5
#define GTCCR_COM1B0  4
#define GTCCR_FOC1B   3
#define GTCCR_FOC1A   2
#define GTCCR_PSR1    1
#define GTCCR_PSR0    0
#define OCR1B         0x2B
#define TCCR0A        0x2A
#define TCCR0A_COM0A1 7
#define TCCR0A_COM0A0 6
#define TCCR0A_COM0B1 5
#define TCCR0A_COM0B0 4
#define TCCR0A_WGM01  1
#define TCCR0A_WGM00  0
#define OCR0A         0x29
#define OCR0B         0x28
#define PLLCSR        0x27
#define PLLCSR_LSM    7
#define PLLCSR_PCKE   2
#define PLLCSR_PLLE   1
#define PLLCSR_PLOCK  0
#define CLKPR         0x26
#define CLKPR_CLKPCE  7
#define CLKPR_CLKPS3  3
#define CLKPR_CLKPS2  2
#define CLKPR_CLKPS1  1
#define CLKPR_CLKPS0  0
#define DT1A          0x25
#define DT1A_DT1AH3   7
#define DT1A_DT1AH2   6
#define DT1A_DT1AH1   5
#define DT1A_DT       4
#define DT1A_1AH0     3
#define DT1A_DT1AL3   2
#define DT1A_DT1AL2   1
#define DT1A_DT1AL1   0
#define DT1B          0x24
#define DT1B_DT1BH3   7
#define DT1B_DT1BH2   6
#define DT1B_DT1BH1   5
#define DT1B_DT       4
#define DT1B_1BH0     3
#define DT1B_DT1BL3   2
#define DT1B_DT1BL2   1
#define DT1B_DT1BL1   0
#define DTPS1         0x23
#define DTPS1_DTPS11  1
#define DTPS1_DTPS10  0
#define DWDR          0x22
#define WDTCR         0x21
#define WDTCR_WDIF    7
#define WDTCR_WDIE    6
#define WDTCR_WDP3    5
#define WDTCR_WDCE    4
#define WDTCR_WDE     3
#define WDTCR_WDP2    2
#define WDTCR_WDP1    1
#define WDTCR_WDP0    0
#define PRR           0x20
#define PRR_PRTIM1    3
#define PRR_PRTIM0    2
#define PRR_PRUSI     1
#define PRR_PRADC     0
#define EEARH         0x1F
#define EEARH_EEAR8   0
#define EEARL         0x1E
#define EEARL_EEAR7   7
#define EEARL_EEAR6   6
#define EEARL_EEAR5   5
#define EEARL_EEAR4   4
#define EEARL_EEAR3   3
#define EEARL_EEAR2   2
#define EEARL_EEAR1   1
#define EEARL_EEAR0   0
#define EEDR          0x1D
#define EECR          0x1C
#define EECR_EEPM1    5
#define EECR_EEPM0    4
#define EECR_EERIE    3
#define EECR_EEMPE    2
#define EECR_EEPE     1
#define EECR_EERE     0
#define PORTB         0x18
#define PORTB_PORTB5  5
#define PORTB_PORTB4  4
#define PORTB_PORTB3  3
#define PORTB_PORTB2  2
#define PORTB_PORTB1  1
#define PORTB_PORTB0  0
#define DDRB          0x17
#define DDRB_DDB5     5
#define DDRB_DDB4     4
#define DDRB_DDB3     3
#define DDRB_DDB2     2
#define DDRB_DDB1     1
#define DDRB_DDB0     0
#define PINB          0x16
#define PINB_PINB5    5
#define PINB_PINB4    4
#define PINB_PINB3    3
#define PINB_PINB2    2
#define PINB_PINB1    1
#define PINB_PINB0    0
#define PCMSK         0x15
#define PCMSK_PCINT5  5
#define PCMSK_PCINT4  4
#define PCMSK_PCINT3  3
#define PCMSK_PCINT2  2
#define PCMSK_PCINT1  1
#define PCMSK_PCINT0  0
#define DIDR0         0x14
#define DIDR0_ADC0D   5
#define DIDR0_ADC2D   4
#define DIDR0_ADC3D   3
#define DIDR0_ADC1D   2
#define DIDR0_AIN1D   1
#define DIDR0_AIN0D   0
#define GPIOR2        0x13
#define GPIOR1        0x12
#define GPIOR0        0x11
#define USIBR         0x10
#define USIDR         0x0F
#define USISR         0x0E
#define USISR_USISIF  7
#define USISR_USIOIF  6
#define USISR_USIPF   5
#define USISR_USIDC   4
#define USISR_USICNT3 3
#define USISR_USICNT2 2
#define USISR_USICNT1 1
#define USISR_USICNT0 0
#define USICR         0x0D
#define USICR_USISIE  7
#define USICR_USIOIE  6
#define USICR_USIWM1  5
#define USICR_USIWM0  4
#define USICR_USICS1  3
#define USICR_USICS0  2
#define USICR_USICLK  1
#define USICR_USITC   0
#define ACSR          0x08
#define ACSR_ACD      7
#define ACSR_ACBG     6
#define ACSR_ACO      5
#define ACSR_ACI      4
#define ACSR_ACIE     3
#define ACSR_ACIS1    1
#define ACSR_ACIS0    0
#define ADMUX         0x07
#define ADMUX_REFS1   7
#define ADMUX_REFS0   6
#define ADMUX_ADLAR   5
#define ADMUX_REFS2   4
#define ADMUX_MUX3    3
#define ADMUX_MUX2    2
#define ADMUX_MUX1    1
#define ADMUX_MUX0    0
#define ADCSRA        0x06
#define ADCSRA_ADEN   7
#define ADCSRA_ADSC   6
#define ADCSRA_ADATE  5
#define ADCSRA_ADIF   4
#define ADCSRA_ADIE   3
#define ADCSRA_ADPS2  2
#define ADCSRA_ADPS1  1
#define ADCSRA_ADPS0  0
#define ADCH          0x05
#define ADCL          0x04
#define ADCSRB        0x03
#define ADCSRB_BIN    7
#define ADCSRB_ACME   6
#define ADCSRB_IPR    5
#define ADCSRB_ADTS2  2
#define ADCSRB_ADTS1  1
#define ADCSRB_ADTS0  0

#endif /* IOREG.h */
