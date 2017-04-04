#ifndef TCC0_CONFIG_H
#define TCC0_CONFIG_H

#include <Arduino.h>
#include <PWM_Timer.h>

//#include <peripheral_clk_config.h>
#ifndef CONF_TCC0_ENABLE
#define CONF_TCC0_ENABLE 1
#endif

#ifndef CONF_TCC0_PWM_ENABLE
#define CONF_TCC0_PWM_ENABLE 1
#endif

// <h> Basic settings
// <y> TCC0 Prescaler
// <TCC_CTRLA_PRESCALER_DIV1_Val"> No division
// <TCC_CTRLA_PRESCALER_DIV2_Val"> Divide by 2
// <TCC_CTRLA_PRESCALER_DIV4_Val"> Divide by 4
// <TCC_CTRLA_PRESCALER_DIV8_Val"> Divide by 8
// <TCC_CTRLA_PRESCALER_DIV16_Val"> Divide by 16
// <TCC_CTRLA_PRESCALER_DIV64_Val"> Divide by 64
// <TCC_CTRLA_PRESCALER_DIV256_Val"> Divide by 256
// <TCC_CTRLA_PRESCALER_DIV1024_Val"> Divide by 1024
// <i> This defines the TCC0 prescaler value
// <id> tcc_prescaler

// Assuming a 48 MHz clock signal... that's 20 nanoseconds per tick
// A prescale of 16 means each count is 1/3 of a microsecond.
#ifndef CONF_TCC0_PRESCALER
#define CONF_TCC0_PRESCALER TCC_CTRLA_PRESCALER_DIV16_Val
#endif




//<o> TCC0 Period Value <0x000000-0xFFFFFF>
// <id> tcc_per
#ifndef CONF_TCC0_PER
#define CONF_TCC0_PER (uint32_t)60000//(uint32_t)(REFRESH_INTERVAL/(16.0/VARIANT_MCK*1000.0))
#endif

// </h>


// <h> Advanced settings
/* Commented intentionally. Timer uses fixed value of the following bit(s)/bitfield(s) of CTRL A register.
 * May be used by other abstractions based on TC. */
//#define CONF_TCC0_RESOLUTION TCC_CTRLA_RESOLUTION_NONE_Val
// <q> Run in standby
// <i> Indicates whether the TCC0 will continue running in standby sleep mode or not
// <id> tcc_arch_runstdby
#ifndef CONF_TCC0_RUNSTDBY
#define CONF_TCC0_RUNSTDBY 0
#endif

// <y> TCC0 Prescaler and Counter Synchronization Selection
// <TCC_CTRLA_PRESCSYNC_GCLK_Val"> Reload or reset counter on next GCLK
// <TCC_CTRLA_PRESCSYNC_PRESC_Val"> Reload or reset counter on next prescaler clock
// <TCC_CTRLA_PRESCSYNC_RESYNC_Val"> Reload or reset counter on next GCLK and reset prescaler counter
// <i> These bits select if on retrigger event, the Counter should be cleared or reloaded on the next GCLK_TCCx clock or on the next prescaled GCLK_TCCx clock.
// <id> tcc_arch_prescsync
#ifndef CONF_TCC0_PRESCSYNC
#define CONF_TCC0_PRESCSYNC TCC_CTRLA_PRESCSYNC_GCLK_Val
#endif

// <y> TCC0 Waveform Generation Selection
// <TCC_WAVE_WAVEGEN_NPWM_Val"> Single-slope PWM
// <TCC_WAVE_WAVEGEN_DSCRITICAL_Val"> Dual-slope, critical interrupt/event at ZERO (DSCRITICAL)
// <TCC_WAVE_WAVEGEN_DSBOTTOM_Val"> Dual-slope, interrupt/event at ZERO (DSBOTTOM)
// <TCC_WAVE_WAVEGEN_DSBOTH_Val"> Dual-slope, interrupt/event at Top and ZERO (DSBOTH)
// <TCC_WAVE_WAVEGEN_DSTOP_Val"> Dual-slope, interrupt/event at Top (DSTOP)
// <id> tcc_arch_wavegen
#ifndef CONF_TCC0_WAVEGEN
#define CONF_TCC0_WAVEGEN TCC_WAVE_WAVEGEN_NPWM_Val
#endif

// <q> TCC0 Auto Lock
// <i> Indicates whether the TCC0 Auto Lock is enabled or not
// <id> tcc_arch_alock
#ifndef CONF_TCC0_ALOCK
#define CONF_TCC0_ALOCK 0
#endif

// <q> TCC0 Capture Channel 0 Enable
// <i> Indicates whether the TCC0 Capture Channel 0 is enabled or not
// <id> tcc_arch_cpten0
#ifndef CONF_TCC0_CPTEN0
#define CONF_TCC0_CPTEN0 1
#endif

// <q> TCC0 Capture Channel 1 Enable
// <i> Indicates whether the TCC0 Capture Channel 1 is enabled or not
// <id> tcc_arch_cpten1
#ifndef CONF_TCC0_CPTEN1
#define CONF_TCC0_CPTEN1 1
#endif

// <q> TCC0 Capture Channel 2 Enable
// <i> Indicates whether the TCC0 Capture Channel 2 is enabled or not
// <id> tcc_arch_cpten2
#ifndef CONF_TCC0_CPTEN2
#define CONF_TCC0_CPTEN2 1
#endif

// <q> TCC0 Capture Channel 3 Enable
// <i> Indicates whether the TCC0 Capture Channel 3 is enabled or not
// <id> tcc_arch_cpten3
#ifndef CONF_TCC0_CPTEN3
#define CONF_TCC0_CPTEN3 0
#endif

// <q> TCC0 Lock update
// <i> Indicates whether the TCC0 Lock update is enabled or not
// <id> tcc_arch_lupd
#ifndef CONF_TCC0_LUPD
#define CONF_TCC0_LUPD 1
#endif

// <q> TCC0 Debug Running Mode
// <i> Indicates whether the TCC0 Debug Running Mode is enabled or not
// <id> tcc_arch_dbgrun
#ifndef CONF_TCC0_DBGRUN
#define CONF_TCC0_DBGRUN 0
#endif

// <e> Event control
// <id> timer_event_control
#ifndef CONF_TCC0_EVENT_CONTROL_ENABLE
#define CONF_TCC0_EVENT_CONTROL_ENABLE 0
#endif

// <q> Match or Capture Channel 0 Event Output
// <i> This bit indicates whether match/capture event on channel 0 is enabled and will be generated
// <id> tcc_arch_mceo0
#ifndef CONF_TCC0_MCEO0
#define CONF_TCC0_MCEO0 0
#endif

// <q> Match or Capture Channel 0 Event Input
// <i> This bit indicates whether match/capture 0 incoming event is enabled
// <id> tcc_arch_mcei0
#ifndef CONF_TCC0_MCEI0
#define CONF_TCC0_MCEI0 0
#endif
// <q> Match or Capture Channel 1 Event Output
// <i> This bit indicates whether match/capture event on channel 1 is enabled and will be generated
// <id> tcc_arch_mceo1
#ifndef CONF_TCC0_MCEO1
#define CONF_TCC0_MCEO1 0
#endif

// <q> Match or Capture Channel 1 Event Input
// <i> This bit indicates whether match/capture 1 incoming event is enabled
// <id> tcc_arch_mcei1
#ifndef CONF_TCC0_MCEI1
#define CONF_TCC0_MCEI1 0
#endif
// <q> Match or Capture Channel 2 Event Output
// <i> This bit indicates whether match/capture event on channel 2 is enabled and will be generated
// <id> tcc_arch_mceo2
#ifndef CONF_TCC0_MCEO2
#define CONF_TCC0_MCEO2 0
#endif

// <q> Match or Capture Channel 2 Event Input
// <i> This bit indicates whether match/capture 2 incoming event is enabled
// <id> tcc_arch_mcei2
#ifndef CONF_TCC0_MCEI2
#define CONF_TCC0_MCEI2 0
#endif
// <q> Match or Capture Channel 3 Event Output
// <i> This bit indicates whether match/capture event on channel 3 is enabled and will be generated
// <id> tcc_arch_mceo3
#ifndef CONF_TCC0_MCEO3
#define CONF_TCC0_MCEO3 0
#endif

// <q> Match or Capture Channel 3 Event Input
// <i> This bit indicates whether match/capture 3 incoming event is enabled
// <id> tcc_arch_mcei3
#ifndef CONF_TCC0_MCEI3
#define CONF_TCC0_MCEI3 0
#endif

// <q> Timer/Counter Event Input 0
// <i> This bit is used to enable input event 0 to the TCC
// <id> tcc_arch_tcei0
#ifndef CONF_TCC0_TCEI0
#define CONF_TCC0_TCEI0 0
#endif

// <q> Timer/Counter Event Input 0 Invert
// <i> This bit inverts the event 0 input
// <id> tcc_arch_tceinv0
#ifndef CONF_TCC0_TCINV0
#define CONF_TCC0_TCINV0 0
#endif
// <q> Timer/Counter Event Input 1
// <i> This bit is used to enable input event 1 to the TCC
// <id> tcc_arch_tcei1
#ifndef CONF_TCC0_TCEI1
#define CONF_TCC0_TCEI1 0
#endif

// <q> Timer/Counter Event Input 1 Invert
// <i> This bit inverts the event 1 input
// <id> tcc_arch_tceinv1
#ifndef CONF_TCC0_TCINV1
#define CONF_TCC0_TCINV1 0
#endif

// <q> Timer/Counter Event Output
// <i> This bit is used to enable the counter cycle event.
//<id> tcc_arch_cnteo
#ifndef CONF_TCC0_CNTEO
#define CONF_TCC0_CNTEO 0
#endif

// <q> Re-trigger Event Output
// <i> This bit is used to enable the counter re-trigger event.
//<id> tcc_arch_trgeo
#ifndef CONF_TCC0_TRGEO
#define CONF_TCC0_TRGEO 0
#endif

// <q> Overflow/Underflow Event Output
// <i> This bit is used to enable enable event on overflow/underflow.
//<id> tcc_arch_ovfeo
#ifndef CONF_TCC0_OVFEO
#define CONF_TCC0_OVFEO 0
#endif

// <o> Timer/Counter Interrupt and Event Output Selection
// <0=> An interrupt/event is generated when a new counter cycle starts
// <1=> An interrupt/event is generated when a counter cycle ends
// <2=> An interrupt/event is generated when a counter cycle ends, except for the first and last cycles
// <3=> An interrupt/event is generated when a new counter cycle starts or a counter cycle ends
// <i> These bits define on which part of the counter cycle the counter event output is generated
// <id> tcc_arch_cntsel
#ifndef CONF_TCC0_CNTSEL
#define CONF_TCC0_CNTSEL 0
#endif

// <o> Timer/Counter Event Input 0 Action
// <0=>Event action disabled
// <1=>Start restart or re-trigger on event
// <2=>Count on event
// <3=>Start on event
// <4=>Increment on event
// <5=>Count on active state of asynchronous event
// <6=>Capture overflow times (Max value)
// <7=>Non-recoverable fault
// <i> These bits define the action the TCC performs on TCE0 event input 0
// <id> tcc_arch_evact0
#ifndef CONF_TCC0_EVACT0
#define CONF_TCC0_EVACT0 0
#endif

// <o> Timer/Counter Event Input 1 Action
// <0=>Event action disabled
// <1=>Re-trigger on event
// <2=>Direction control
// <3=>Stop counter on event
// <4=>Decrement on event
// <5=>Period capture value in CC0 register, pulse width capture value in CC1 register
// <6=>Period capture value in CC1 register, pulse width capture value in CC0 register
// <7=>Non-recoverable fault
// <i> These bits define the action the TCC performs on TCE0 event input 0
// <id> tcc_arch_evact1
#ifndef CONF_TCC0_EVACT1
#define CONF_TCC0_EVACT1 0
#endif
// </e>

//<o> TCC0 Compare and Capture value 0 <0x000000-0xFFFFFF>
// <id> tcc_arch_cc0
#ifndef CONF_TCC0_CC0
#define CONF_TCC0_CC0 0x0
#endif

//<o> TCC0 Compare and Capture value 1 <0x000000-0xFFFFFF>
// <id> tcc_arch_cc1
#ifndef CONF_TCC0_CC1
#define CONF_TCC0_CC1 0x0
#endif

//<o> TCC0 Compare and Capture value 2 <0x000000-0xFFFFFF>
// <id> tcc_arch_cc2
#ifndef CONF_TCC0_CC2
#define CONF_TCC0_CC2 0x0
#endif

//<o> TCC0 Compare and Capture value 3 <0x000000-0xFFFFFF>
// <id> tcc_arch_cc3
#ifndef CONF_TCC0_CC3
#define CONF_TCC0_CC3 0x0
#endif


#endif // HPL_TCC_CONFIG_H
