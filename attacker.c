#include <microkit.h>
#include <stdint.h>
#include "util.h"

#define CHANNEL_VULNERABLE 1
#define CHANNEL_LOGGER 3

static int attack_phase = 0;

void init(void) {
    microkit_dbg_puts("\n");
    microkit_dbg_puts("##################################################\n");
    microkit_dbg_puts("# ATTACKER: Buffer Overflow Attack Simulator     #\n");
    microkit_dbg_puts("##################################################\n");
    microkit_dbg_puts("\nATTACKER: Initializing attack simulator\n");
    microkit_dbg_puts("ATTACKER: This component simulates malicious input\n");
    microkit_dbg_puts("ATTACKER: Educational purpose only!\n\n");
    
    // 攻撃を開始
    microkit_dbg_puts("ATTACKER: Launching buffer overflow attack...\n");
    microkit_dbg_puts("ATTACKER: Target: vulnerable_pd\n");
    microkit_dbg_puts("ATTACKER: Attack vector: Stack buffer overflow\n\n");
    
    // Vulnerable PDに攻撃を送信
    microkit_notify(CHANNEL_VULNERABLE);
}

void notified(microkit_channel ch) {
    attack_phase++;
    
    if (attack_phase == 1) {
        microkit_dbg_puts("\n");
        microkit_dbg_puts("ATTACKER: First attack completed\n");
        microkit_dbg_puts("ATTACKER: Analyzing results...\n\n");
        
        // 少し待つ
        for (volatile int i = 0; i < 1000000; i++);
        
        microkit_dbg_puts("ATTACKER: Preparing second attack wave...\n");
        microkit_dbg_puts("ATTACKER: Attack vector: Heap buffer overflow\n\n");
        
        // 2回目の攻撃
        microkit_notify(CHANNEL_VULNERABLE);
    } else if (attack_phase == 2) {
        microkit_dbg_puts("\n");
        microkit_dbg_puts("ATTACKER: All attacks completed\n");
        microkit_dbg_puts("ATTACKER: Attempting to exploit other components...\n\n");
        
        // seL4の隔離により、他のコンポーネントには影響しない
        microkit_dbg_puts("ATTACKER: ❌ Cannot access safe_server (isolated)\n");
        microkit_dbg_puts("ATTACKER: ❌ Cannot access logger (isolated)\n");
        microkit_dbg_puts("ATTACKER: ❌ seL4 isolation prevents lateral movement\n\n");
        
        microkit_notify(CHANNEL_LOGGER);
    }
}

microkit_msginfo protected(microkit_channel ch, microkit_msginfo msginfo) {
    return microkit_msginfo_new(0, 0);
}
