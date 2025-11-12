#include <microkit.h>
#include <stdint.h>
#include <stdbool.h>
#include "util.h"

#define CHANNEL_ATTACKER 1
#define CHANNEL_LOGGER 2
#define SMALL_BUFFER_SIZE 8

// 意図的に小さいバッファ（脆弱性のデモ用）
static char vulnerable_buffer[SMALL_BUFFER_SIZE];
static int important_variable = 0xDEADBEEF;  // この値が上書きされる
static bool authenticated = false;

// スタック上のバッファオーバーフロー（教育目的）
void unsafe_string_copy(const char *input) {
    microkit_dbg_puts("VULNERABLE_PD: Copying data without bounds checking...\n");
    microkit_dbg_puts("VULNERABLE_PD: Buffer size: 8 bytes\n");
    
    // 危険！境界チェックなし
    int i = 0;
    while (input[i] != '\0') {
        vulnerable_buffer[i] = input[i];  // バッファオーバーフロー発生！
        i++;
    }
    vulnerable_buffer[i] = '\0';
}

// ヒープ上のバッファオーバーフロー（教育目的）
void demonstrate_heap_overflow(void) {
    microkit_dbg_puts("\n=== Heap Buffer Overflow Demonstration ===\n");
    
    // 小さい領域を確保
    static char heap_buffer[16];
    microkit_dbg_puts("VULNERABLE_PD: Allocated 16 bytes\n");
    
    // 大きなデータを書き込む（オーバーフロー）
    const char *overflow_data = "This string is definitely much longer than 16 bytes and will overflow!";
    microkit_dbg_puts("VULNERABLE_PD: Writing ");
    microkit_dbg_puts(" bytes\n");
    
    // 危険！
    my_strncpy(heap_buffer, overflow_data, 100);  // 16バイトしかないのに100バイト書き込もうとする
}

void init(void) {
    microkit_dbg_puts("\n");
    microkit_dbg_puts("##################################################\n");
    microkit_dbg_puts("# VULNERABLE PD: Buffer Overflow Demonstration   #\n");
    microkit_dbg_puts("##################################################\n");
    microkit_dbg_puts("\nVULNERABLE_PD: Initializing vulnerable component\n");
    microkit_dbg_puts("VULNERABLE_PD: This PD contains intentional vulnerabilities\n");
    microkit_dbg_puts("VULNERABLE_PD: Buffer size: 8 bytes\n");
    microkit_dbg_puts("VULNERABLE_PD: Important variable: 0xDEADBEEF\n");
    microkit_dbg_puts("VULNERABLE_PD: Authenticated: FALSE\n\n");
}

void notified(microkit_channel ch) {
    if (ch == CHANNEL_ATTACKER) {
        microkit_dbg_puts("\n");
        microkit_dbg_puts("================================================\n");
        microkit_dbg_puts("VULNERABLE_PD: Received data from attacker\n");
        microkit_dbg_puts("================================================\n");
        
        // 攻撃シナリオ1: スタックバッファオーバーフロー
        const char *malicious_input = "AAAAAAAAAAAAAAAAAAAAAAAA";  // 24 A's (8バイトのバッファに対して)
        
        microkit_dbg_puts("\n--- Before Buffer Overflow ---\n");
        microkit_dbg_puts("VULNERABLE_PD: vulnerable_buffer: [empty]\n");
        microkit_dbg_puts("VULNERABLE_PD: important_variable: 0xDEADBEEF\n");
        microkit_dbg_puts("VULNERABLE_PD: authenticated: FALSE\n");
        
        // 危険な関数を呼び出し
        unsafe_string_copy(malicious_input);
        
        microkit_dbg_puts("\n--- After Buffer Overflow ---\n");
        microkit_dbg_puts("VULNERABLE_PD: vulnerable_buffer: ");
        microkit_dbg_puts(vulnerable_buffer);
        microkit_dbg_puts("\n");
        
        // important_variableが上書きされたか確認
        if (important_variable != 0xDEADBEEF) {
            microkit_dbg_puts("VULNERABLE_PD: ⚠️  important_variable CORRUPTED!\n");
            microkit_dbg_puts("VULNERABLE_PD: ⚠️  Memory corruption detected!\n");
        }
        
        // authenticatedが上書きされたか確認
        if (authenticated) {
            microkit_dbg_puts("VULNERABLE_PD: ⚠️  authenticated flag CORRUPTED!\n");
            microkit_dbg_puts("VULNERABLE_PD: ⚠️  Security bypass possible!\n");
        }
        
        // 攻撃シナリオ2: ヒープオーバーフロー
        microkit_dbg_puts("\n");
        demonstrate_heap_overflow();
        
        microkit_dbg_puts("\n================================================\n");
        microkit_dbg_puts("VULNERABLE_PD: Buffer overflow occurred!\n");
        microkit_dbg_puts("VULNERABLE_PD: BUT - seL4 isolation prevents spread\n");
        microkit_dbg_puts("================================================\n\n");
        
        // Loggerに通知
        microkit_notify(CHANNEL_LOGGER);
    }
}

microkit_msginfo protected(microkit_channel ch, microkit_msginfo msginfo) {
    return microkit_msginfo_new(0, 0);
}
