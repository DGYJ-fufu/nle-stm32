;/*****************************************************************************/
;/* STM32L1xx_HD_OPT.s: STM32L1xx High-Density Flash Option Bytes             */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/*  This file is part of the uVision/ARM development tools                   */
;/*  Copyright (c) 2012 Keil - An ARM Company.                           */
;/*  All rights reserved.                                                     */
;/*****************************************************************************/

; Option byte organization
;-------------------------
;   Address     [31:24] [23:16] [15:8] [7:0]
; 0x1FF80000       -     nRDP      -    RDP	    (0xFF5500AA)
; 0x1FF80004       -     nUSER     -    USER    (0xFF870078)
; 0x1FF80008     nWRP1   nWRP0   WRP1   WRP0    (0xFFFF0000)
; 0x1FF8000C     nWRP3   nWRP2   WRP3   WRP2    (0xFFFF0000)
; 0x1FF80010     nWRP5   nWRP4   WRP5   WRP4    (0xFFFF0000)
; 0x1FF80014     nWRP7   nWRP6   WRP7   WRP6    (0xFFFF0000)
; 0x1FF80018     nWRP9   nWRP8   WRP9   WRP8    (0xFFFF0000)
; 0x1FF8001C     nWRP11  nWRP10  WRP11  WRP10   (0xFFFF0000)


;// <e> Flash Option Bytes
FLASH_OPT       EQU     1

;// <h> Flash Read Protection
;//     <i> Read protection is used to protect the software code stored in Flash memory
;//   <o0> Read Protection Level
;//     <i> Level 0: No Protection 
;//     <i> Level 1: Read Protection of Memories (memory read protection enabled!)
;//     <i> Level 2: Chip Protection (memory read protection enabled and all debug features disabled!)
;//     <i> Note: Mass Erase is performed when Level 1 is active and Level 0 is requested
;//          <0xAA=> Level 0 (No Protection) 
;//          <0x00=> Level 1 (Read Protection of Memories)
;//          <0xCC=> Level 2 (Chip Protection)
;// </h>
RDP             EQU     0xAA
nRDP            EQU     RDP:EOR:0xFF

;// <h> Flash Write Protection
;//   <h> WRP0..WRP3
;//     <o0.0> Sector 0
;//     <o0.1> Sector 1
;//     <o0.2> Sector 2
;//     <o0.3> Sector 3
;//     <o0.4> Sector 4
;//     <o0.5> Sector 5
;//     <o0.6> Sector 6
;//     <o0.7> Sector 7
;//     <o1.0> Sector 8
;//     <o1.1> Sector 9
;//     <o1.2> Sector 10
;//     <o1.3> Sector 11
;//     <o1.4> Sector 12
;//     <o1.5> Sector 13
;//     <o1.6> Sector 14
;//     <o1.7> Sector 15
;//     <o2.0> Sector 16
;//     <o2.1> Sector 17
;//     <o2.2> Sector 18
;//     <o2.3> Sector 19
;//     <o2.4> Sector 20
;//     <o2.5> Sector 21
;//     <o2.6> Sector 22
;//     <o2.7> Sector 23
;//     <o3.0> Sector 24
;//     <o3.1> Sector 25
;//     <o3.2> Sector 26
;//     <o3.3> Sector 27
;//     <o3.4> Sector 28
;//     <o3.5> Sector 29
;//     <o3.6> Sector 30
;//     <o3.7> Sector 31
;//   </h>
WRP00           EQU     0x00
WRP01           EQU     0x00
WRP02           EQU     0x00
WRP03           EQU     0x00
nWRP00          EQU     WRP00:EOR:0xFF
nWRP01          EQU     WRP01:EOR:0xFF
nWRP02          EQU     WRP02:EOR:0xFF
nWRP03          EQU     WRP03:EOR:0xFF
;//   <h> WRP4..WRP7
;//     <o0.0> Sector 32
;//     <o0.1> Sector 33
;//     <o0.2> Sector 34
;//     <o0.3> Sector 35
;//     <o0.4> Sector 36
;//     <o0.5> Sector 37
;//     <o0.6> Sector 38
;//     <o0.7> Sector 39
;//     <o1.0> Sector 40
;//     <o1.1> Sector 41
;//     <o1.2> Sector 42
;//     <o1.3> Sector 43
;//     <o1.4> Sector 44
;//     <o1.5> Sector 45
;//     <o1.6> Sector 46
;//     <o1.7> Sector 47
;//     <o2.0> Sector 48
;//     <o2.1> Sector 49
;//     <o2.2> Sector 50
;//     <o2.3> Sector 51
;//     <o2.4> Sector 52
;//     <o2.5> Sector 53
;//     <o2.6> Sector 54
;//     <o2.7> Sector 55
;//     <o3.0> Sector 56
;//     <o3.1> Sector 57
;//     <o3.2> Sector 58
;//     <o3.3> Sector 59
;//     <o3.4> Sector 60
;//     <o3.5> Sector 61
;//     <o3.6> Sector 62
;//     <o3.7> Sector 63
;//   </h>
WRP04           EQU     0x00
WRP05           EQU     0x00
WRP06           EQU     0x00
WRP07           EQU     0x00
nWRP04          EQU     WRP04:EOR:0xFF
nWRP05          EQU     WRP05:EOR:0xFF
nWRP06          EQU     WRP06:EOR:0xFF
nWRP07          EQU     WRP07:EOR:0xFF
;//   <h> WRP8..WRP11
;//     <o0.0> Sector 64
;//     <o0.1> Sector 65
;//     <o0.2> Sector 66
;//     <o0.3> Sector 67
;//     <o0.4> Sector 68
;//     <o0.5> Sector 69
;//     <o0.6> Sector 70
;//     <o0.7> Sector 71
;//     <o1.0> Sector 72
;//     <o1.1> Sector 73
;//     <o1.2> Sector 74
;//     <o1.3> Sector 75
;//     <o1.4> Sector 76
;//     <o1.5> Sector 77
;//     <o1.6> Sector 78
;//     <o1.7> Sector 79
;//     <o2.0> Sector 80
;//     <o2.1> Sector 81
;//     <o2.2> Sector 82
;//     <o2.3> Sector 83
;//     <o2.4> Sector 84
;//     <o2.5> Sector 85
;//     <o2.6> Sector 86
;//     <o2.7> Sector 87
;//     <o3.0> Sector 88
;//     <o3.1> Sector 89
;//     <o3.2> Sector 90
;//     <o3.3> Sector 91
;//     <o3.4> Sector 92
;//     <o3.5> Sector 93
;//     <o3.6> Sector 94
;//     <o3.7> Sector 95
;//   </h>
WRP08           EQU     0x00
WRP09           EQU     0x00
WRP10           EQU     0x00
WRP11           EQU     0x00
nWRP08          EQU     WRP08:EOR:0xFF
nWRP09          EQU     WRP09:EOR:0xFF
nWRP10          EQU     WRP10:EOR:0xFF
nWRP11          EQU     WRP11:EOR:0xFF
;// </h>

;// <h> User Configuration
;//   <o0.0..3> BOR_LEV     
;//          < 0=> BOR OFF:     Reset threshold level for 1.45V - 1.55V (power down only)
;//          < 1=> BOR OFF:     Reset threshold level for 1.45V - 1.55V (power down only)
;//          < 2=> BOR OFF:     Reset threshold level for 1.45V - 1.55V (power down only)
;//          < 3=> BOR OFF:     Reset threshold level for 1.45V - 1.55V (power down only)
;//          < 4=> BOR OFF:     Reset threshold level for 1.45V - 1.55V (power down only)
;//          < 5=> BOR OFF:     Reset threshold level for 1.45V - 1.55V (power down only)
;//          < 6=> BOR OFF:     Reset threshold level for 1.45V - 1.55V (power down only)
;//          < 7=> BOR OFF:     Reset threshold level for 1.45V - 1.55V (power down only)
;//          < 8=> BOR LEVEL 1: Reset threshold level for 1.69V - 1.80V (power on)
;//          < 9=> BOR LEVEL 2: Reset threshold level for 1.94V - 2.10V (power on)
;//          <10=> BOR LEVEL 3: Reset threshold level for 2.30V - 2.49V (power on)
;//          <11=> BOR LEVEL 4: Reset threshold level for 2.54V - 2.74V (power on)
;//          <12=> BOR LEVEL 5: Reset threshold level for 2.77V - 3.00V (power on)
;//   <o0.4> IWDG_SW     
;//          <0=> HW Watchdog <1=> SW Watchdog
;//   <o0.5> nRST_STOP  <i> Generate Reset when entering STOP Mode
;//          <0=> Enabled <1=> Disabled
;//   <o0.6> nRST_STDBY <i> Generate Reset when entering Standby Mode
;//          <0=> Enabled <1=> Disabled
;//   <o0.7> BFB2 <i> This bit is used to boot from bank 2
;//          <0=> Boot from system memory <1=> Boot from bank 1
;// </h>
USER            EQU     0x78
nUSER           EQU     USER:EOR:0xFF

;// </e>


                IF      FLASH_OPT <> 0
                AREA    |.ARM.__AT_0x1FF80000|, CODE, READONLY
                DCB     RDP,   0x00,   nRDP,   0xFF      
                DCB     USER,  0x00,   nUSER,  0xFF      
                DCB     WRP00, WRP01,  nWRP00, nWRP01
                DCB     WRP02, WRP03,  nWRP02, nWRP03
                DCB     WRP04, WRP05,  nWRP04, nWRP05
                DCB     WRP06, WRP07,  nWRP06, nWRP07
                DCB     WRP08, WRP09,  nWRP08, nWRP09
                DCB     WRP10, WRP11,  nWRP10, nWRP11
                ENDIF


                END
