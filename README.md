# FRDM-K82F

Soft Real Time O.S. with Task Non-Preemptive First Come First Served Scheduling "Educational" Project - NXP ARM Cortex M4F

Test some basic functionality for NXP K82F in FRDM board, without libraries.

ATTENTION!! There are many inconsistencies with K82F data sheet to set preemptive priority !! ... this code seems to work well !!

    PIT: interrupt with "high" pre-emptive priority (1), time for delay and state machine, increment internal counter every about 1 ms;
    
    I2S: serial sinc-communication with audio decoder UDA1334ATS.
    
    MAIN: just once, play the 12 musical note in 4° octave, blink led every 1 s in task1, and, for now, nothing to do in task 2.
