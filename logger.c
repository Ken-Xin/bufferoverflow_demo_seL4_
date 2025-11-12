#include <microkit.h>
#include <stdint.h>
#include "util.h"

#define MAX_LOG_ENTRIES 1000
#define MAX_LOG_LENGTH 256
#define CHANNEL_CLIENT 1
#define CHANNEL_SERVER 2

// ログバッファのアドレス
uintptr_t log_buffer_vaddr;
size_t log_buffer_size;

// ログエントリ
typedef struct {
    uint64_t timestamp;
    uint8_t source_pd;
    uint8_t log_level;
    char message[MAX_LOG_LENGTH];
} log_entry_t;

// ログバッファ構造体
typedef struct {
    uint64_t entry_count;
    uint64_t write_index;
    log_entry_t entries[MAX_LOG_ENTRIES];
} log_buffer_t;

static uint64_t current_timestamp = 0;

void init(void) {
    microkit_dbg_puts("\n");
    microkit_dbg_puts("##############################################\n");
    microkit_dbg_puts("# LOGGER: Component Isolation Demonstration  #\n");
    microkit_dbg_puts("##############################################\n");
    microkit_dbg_puts("\nLOGGER: Initializing logger protection domain\n");
    
    // ログバッファの初期化
    log_buffer_t *log_buf = (log_buffer_t *)log_buffer_vaddr;
    my_memset(log_buf, 0, sizeof(log_buffer_t));
    
    microkit_dbg_puts("\n--- VSpace Isolation ---\n");
    microkit_dbg_puts("LOGGER: This PD has its own isolated virtual address space\n");
    microkit_dbg_puts("LOGGER: Cannot access client/server memory\n");
    
    microkit_dbg_puts("\n--- CSpace Isolation ---\n");
    microkit_dbg_puts("LOGGER: This PD has its own capability space\n");
    microkit_dbg_puts("LOGGER: Only has capabilities granted in system.xml\n");
    
    microkit_dbg_puts("\nLOGGER: Ready to log events\n\n");
}

void notified(microkit_channel ch) {
    log_buffer_t *log_buf = (log_buffer_t *)log_buffer_vaddr;
    const char *source = (ch == CHANNEL_CLIENT) ? "CLIENT" : "SERVER";
    
    microkit_dbg_puts("LOGGER: Received notification from ");
    microkit_dbg_puts(source);
    microkit_dbg_puts("\n");
    
    // ログエントリの作成
    if (log_buf->write_index < MAX_LOG_ENTRIES) {
        log_entry_t *entry = &log_buf->entries[log_buf->write_index];
        entry->timestamp = current_timestamp++;
        entry->source_pd = (ch == CHANNEL_CLIENT) ? 0 : 1;
        entry->log_level = 0;
        
        const char *msg = "Event from ";
        my_strncpy(entry->message, msg, MAX_LOG_LENGTH);
        
        log_buf->write_index++;
        log_buf->entry_count++;
        
        microkit_dbg_puts("LOGGER: Entry logged\n");
    }
}

microkit_msginfo protected(microkit_channel ch, microkit_msginfo msginfo) {
    return microkit_msginfo_new(0, 0);
}
