#include <microkit.h>
#include <stdint.h>

#define CHANNEL_LOGGER 1

void init(void) {
    microkit_dbg_puts("\n");
    microkit_dbg_puts("##############################################\n");
    microkit_dbg_puts("# FAULT TEST: Fault Tolerance Demonstration  #\n");
    microkit_dbg_puts("##############################################\n");
    microkit_dbg_puts("\nFAULT_TEST: Starting fault tolerance test\n");
    microkit_dbg_puts("FAULT_TEST: Phase 0 - Normal operation\n");
    
    // 正常な操作をシミュレート
    microkit_dbg_puts("FAULT_TEST: Performing normal operations...\n");
    
    // 少し待つ
    for (volatile int i = 0; i < 1000000; i++);
    
    microkit_dbg_puts("\n");
    microkit_dbg_puts("================================================\n");
    microkit_dbg_puts("FAULT_TEST: Phase 1 - TRIGGERING FAULT\n");
    microkit_dbg_puts("================================================\n");
    microkit_dbg_puts("FAULT_TEST: Attempting invalid memory access...\n\n");
    
    // 意図的にフォールトを発生させる
    volatile uint32_t *invalid_ptr = (uint32_t *)0xDEADBEEF;
    *invalid_ptr = 0x12345678;
    
    microkit_dbg_puts("FAULT_TEST: ERROR - This should not be reached!\n");
}

void notified(microkit_channel ch) {
    microkit_dbg_puts("FAULT_TEST: Notification received\n");
}

microkit_msginfo protected(microkit_channel ch, microkit_msginfo msginfo) {
    return microkit_msginfo_new(0, 0);
}
