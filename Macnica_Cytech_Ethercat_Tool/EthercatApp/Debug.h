#ifndef DEBUG_H
#define DEBUG_H

// (WORD) ADDRESS MAP FOR DEBUG MEMORY
// Write-only registers will be polled by System Console
// Read-only registers (including button registers) will be polled in main loop

// General Drive Status (Write only)
#define DOC_DBG_DRIVE_STATE             0
#define DOC_DBG_RUNTIME                 1
#define DOC_DBG_DSP_MODE                2
#define DOC_DBG_DEMO_MODE               3
#define DOC_DBG_LATENCY                 4
#define DOC_DBG_DUMP_MODE               5
#define DOC_DBG_TRIG_SEL                6
#define DOC_DBG_TRIG_EDGE               7
#define DOC_DBG_TRIG_VALUE              8

// Drive Performance Status (Write only)
#define DOC_DBG_SPEED                   10
#define DOC_DBG_SPEED_FILT              11
#define DOC_DBG_POSITION                12
#define DOC_DBG_V_RMS                   13
#define DOC_DBG_I_RMS                   14
#define DOC_DBG_POWER_INST              15
#define DOC_DBG_POWER_CUM               16

// Drive Command Inputs (Read-only except for buttons handshake)
#define DOC_DBG_NUM_BUTTONS             8
#define DOC_DBG_BUTTONS                 17
#define DOC_DBG_OL_EN                   (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 1)
#define DOC_DBG_OL_SPEED                (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 2)
#define DOC_DBG_OL_VOLT                 (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 3)

#define DOC_DBG_I_PI_TUNE_EN            (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 4)
#define DOC_DBG_I_PI_KP                 (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 5)
#define DOC_DBG_I_PI_KI                 (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 6)

#define DOC_DBG_SPEED_PI_TUNE_EN        (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 7)
#define DOC_DBG_SPEED_PI_KP             (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 8)
#define DOC_DBG_SPEED_PI_KI             (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 9)

#define DOC_DBG_SPEED_SETP0             (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 10)
#define DOC_DBG_SPEED_SETP1             (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 11)
#define DOC_DBG_SPEED_SW_PERIOD         (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 12)

#define DOC_DBG_AXIS_SELECT             (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 13)
#define DOC_DBG_CONTROLWORD             (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 14)
#define DOC_DBG_UNUSED1                 (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 15)

#define DOC_DBG_POS_SETP0               (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 16)
#define DOC_DBG_POS_SETP1               (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 17)
#define DOC_DBG_POS_SW_PERIOD           (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 18)

#define DOC_DBG_I_PI_FB_LIM             (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 19)
#define DOC_DBG_I_PI_OP_LIM             (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 20)

#define DOC_DBG_SPEED_PI_FB_LIM         (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 21)
#define DOC_DBG_SPEED_PI_OP_LIM         (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 22)

#define DOC_DBG_POS_MODE                (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 23)
#define DOC_DBG_POS_SPEED               (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 24)

#define DOC_DBG_POS_PI_KP               (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 25)
#define DOC_DBG_POS_SPDFF_KP            (DOC_DBG_NUM_BUTTONS + DOC_DBG_BUTTONS + 26)

#endif // DEBUG_H
