/* Includes ------------------------------------------------------------------*/
#include "key_scaner.h"
#include "hal_key.h"

#define KEY_SCAN_TIMER 10

void keyScanner(){
    static unsigned long key_scan_tick = 0;
    key_scan_tick++;
    if(key_scan_tick >= KEY_SCAN_TIMER) {
        key_scan_tick = 0;
        scan_keys();
    }
}
