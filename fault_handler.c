#include <microkit.h>
#include <sel4/sel4.h>
#include <stdint.h>

#define CHILD_FAULT_TEST 0

static uint64_t fault_count = 0;

void init(void) {
    microkit_dbg_puts("\n");
    microkit_dbg_puts("##############################################\n");
    microkit_dbg_puts("# FAULT HANDLER: Monitoring Child PDs        #\n");
    microkit_dbg_puts("##############################################\n");
    microkit_dbg_puts("\nFAULT_HANDLER: Initializing\n");
    microkit_dbg_puts("FAULT_HANDLER: Monitoring child PDs for faults\n\n");
}

void notified(microkit_channel ch) {
    microkit_dbg_puts("FAULT_HANDLER: Notification received\n");
}

seL4_Bool fault(microkit_child child, microkit_msginfo msginfo, microkit_msginfo *reply_msginfo) {
    fault_count++;
    
    microkit_dbg_puts("\n");
    microkit_dbg_puts("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    microkit_dbg_puts("!  FAULT HANDLER: FAULT DETECTED            !\n");
    microkit_dbg_puts("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    microkit_dbg_puts("\n");
    
    seL4_Word label = microkit_msginfo_get_label(msginfo);
    
    microkit_dbg_puts("FAULT_HANDLER: Fault details received\n");
    
    if (label == seL4_Fault_VMFault) {
        microkit_dbg_puts("FAULT_HANDLER: VM Fault detected\n");
        microkit_dbg_puts("\n--- Isolation Success ---\n");
        microkit_dbg_puts("FAULT_HANDLER: Fault contained\n");
        microkit_dbg_puts("FAULT_HANDLER: Other components continue\n");
        microkit_dbg_puts("FAULT_HANDLER: System integrity maintained\n\n");
        
        microkit_dbg_puts("FAULT_HANDLER: Stopping faulty PD...\n");
        microkit_pd_stop(child);
        microkit_dbg_puts("FAULT_HANDLER: Faulty PD stopped\n\n");
        
        microkit_dbg_puts("================================================\n");
        microkit_dbg_puts("  FAULT CONTAINMENT DEMONSTRATION SUCCESSFUL\n");
        microkit_dbg_puts("================================================\n\n");
        
        return seL4_False;
    }
    
    microkit_dbg_puts("FAULT_HANDLER: Unknown fault type\n");
    microkit_pd_stop(child);
    
    return seL4_False;
}

microkit_msginfo protected(microkit_channel ch, microkit_msginfo msginfo) {
    return microkit_msginfo_new(0, 0);
}
