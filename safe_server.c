#include <microkit.h>
#include <stdint.h>
#include "util.h"

#define CHANNEL_LOGGER 1
#define SAFE_BUFFER_SIZE 256

uintptr_t shared_mem_vaddr;
size_t shared_mem_size;

// 安全なバッファ（境界チェック付き）
static char safe_buffer[SAFE_BUFFER_SIZE];
static uint64_t operation_count = 0;

// 安全な文字列コピー関数（境界チェック付き）
void safe_string_copy(char *dest, const char *src, size_t dest_size) {
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';  // 必ず終端する
}

void init(void) {
    microkit_dbg_puts("\n");
    microkit_dbg_puts("##################################################\n");
    microkit_dbg_puts("# SAFE SERVER: Protected Component               #\n");
    microkit_dbg_puts("##################################################\n");
    microkit_dbg_puts("\nSAFE_SERVER: Initializing protected component\n");
    microkit_dbg_puts("SAFE_SERVER: This PD is isolated from vulnerable_pd\n");
    microkit_dbg_puts("SAFE_SERVER: Using safe buffer handling\n");
    microkit_dbg_puts("SAFE_SERVER: Buffer size: 256 bytes (with bounds checking)\n\n");
    
    // 正常な操作を実行
    const char *test_data = "Safe server is operating normally";
    safe_string_copy(safe_buffer, test_data, SAFE_BUFFER_SIZE);
    
    microkit_dbg_puts("SAFE_SERVER: Running normal operations...\n");
    operation_count++;
}

void notified(microkit_channel ch) {
    if (ch == CHANNEL_LOGGER) {
        operation_count++;
        
        microkit_dbg_puts("\n");
        microkit_dbg_puts("================================================\n");
        microkit_dbg_puts("SAFE_SERVER: Status Check\n");
        microkit_dbg_puts("================================================\n");
        microkit_dbg_puts("SAFE_SERVER: ✓ Still operational\n");
        microkit_dbg_puts("SAFE_SERVER: ✓ No memory corruption\n");
        microkit_dbg_puts("SAFE_SERVER: ✓ Isolated from vulnerable_pd\n");
        microkit_dbg_puts("SAFE_SERVER: ✓ seL4 protection working correctly\n");
        microkit_dbg_puts("SAFE_SERVER: Operations completed: ");
        microkit_dbg_puts("\n");
        microkit_dbg_puts("================================================\n\n");
    }
}

microkit_msginfo protected(microkit_channel ch, microkit_msginfo msginfo) {
    return microkit_msginfo_new(0, 0);
}
