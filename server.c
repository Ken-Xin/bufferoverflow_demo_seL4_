#include <microkit.h>
#include <stdint.h>
#include <stdbool.h>
#include "util.h"

#define MAX_MESSAGE_SIZE 4096
#define CHANNEL_CLIENT 1

// 共有メモリのアドレス
uintptr_t shared_mem_vaddr;
size_t shared_mem_size;

// メッセージ構造体
typedef struct {
    uint64_t request_id;
    uint64_t data_length;
    uint64_t timestamp;
    char data[MAX_MESSAGE_SIZE];
} message_t;

// サーバー統計情報
static uint64_t request_count = 0;
static uint64_t total_bytes_processed = 0;

void init(void) {
    microkit_dbg_puts("==============================================\n");
    microkit_dbg_puts("SERVER: Initializing server protection domain\n");
    microkit_dbg_puts("==============================================\n");
    microkit_dbg_puts("SERVER: VSpace isolation enabled\n");
    microkit_dbg_puts("SERVER: CSpace isolation enabled\n");
    microkit_dbg_puts("SERVER: Shared memory initialized\n");
    
    // 共有メモリの初期化
    if (shared_mem_vaddr != 0) {
        my_memset((void *)shared_mem_vaddr, 0, shared_mem_size);
        microkit_dbg_puts("SERVER: Ready to accept requests\n");
    } else {
        microkit_dbg_puts("SERVER: ERROR - Shared memory not mapped\n");
    }
}

void notified(microkit_channel ch) {
    if (ch == CHANNEL_CLIENT) {
        microkit_dbg_puts("\n--- SERVER: Processing Request ---\n");
        
        // 共有メモリからメッセージを読み取る
        message_t *msg = (message_t *)shared_mem_vaddr;
        
        // リクエストの検証
        if (msg->data_length > MAX_MESSAGE_SIZE) {
            microkit_dbg_puts("SERVER: ERROR - Invalid message length\n");
            return;
        }
        
        request_count++;
        total_bytes_processed += msg->data_length;
        
        microkit_dbg_puts("SERVER: Request received\n");
        microkit_dbg_puts("SERVER: Original data: ");
        microkit_dbg_puts(msg->data);
        microkit_dbg_puts("\n");
        
        // データ処理（文字列の反転）
        for (size_t i = 0; i < msg->data_length / 2; i++) {
            char temp = msg->data[i];
            msg->data[i] = msg->data[msg->data_length - 1 - i];
            msg->data[msg->data_length - 1 - i] = temp;
        }
        
        microkit_dbg_puts("SERVER: Processed data: ");
        microkit_dbg_puts(msg->data);
        microkit_dbg_puts("\n");
        
        // クライアントに処理完了を通知
        microkit_notify(CHANNEL_CLIENT);
        microkit_dbg_puts("SERVER: Response sent\n");
    }
}

microkit_msginfo protected(microkit_channel ch, microkit_msginfo msginfo) {
    return microkit_msginfo_new(0, 0);
}
