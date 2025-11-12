#include <microkit.h>
#include <stdint.h>
#include <stdbool.h>
#include "util.h"

#define MAX_MESSAGE_SIZE 4096
#define CHANNEL_SERVER 1

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

static uint64_t next_request_id = 1;
static volatile bool waiting_for_response = false;
static uint64_t test_count = 0;

// 簡易的なタイムスタンプ
static uint64_t get_timestamp(void) {
    static uint64_t counter = 0;
    return counter++;
}

// テストメッセージの送信
void send_test_message(const char *data) {
    message_t *msg = (message_t *)shared_mem_vaddr;
    
    msg->request_id = next_request_id++;
    msg->timestamp = get_timestamp();
    msg->data_length = my_strlen(data);
    my_strncpy(msg->data, data, MAX_MESSAGE_SIZE - 1);
    msg->data[MAX_MESSAGE_SIZE - 1] = '\0';
    
    microkit_dbg_puts("\n=== CLIENT: Sending Message ===\n");
    microkit_dbg_puts("CLIENT: Data: ");
    microkit_dbg_puts(msg->data);
    microkit_dbg_puts("\n");
    
    waiting_for_response = true;
    microkit_notify(CHANNEL_SERVER);
}

void init(void) {
    microkit_dbg_puts("==============================================\n");
    microkit_dbg_puts("CLIENT: Initializing client protection domain\n");
    microkit_dbg_puts("==============================================\n");
    microkit_dbg_puts("CLIENT: Demonstrating IPC via endpoints\n");
    microkit_dbg_puts("CLIENT: Using shared memory (page mapping)\n");
    microkit_dbg_puts("CLIENT: Testing message passing\n\n");
    
    // テストメッセージ1を送信
    send_test_message("Hello seL4 Microkit!");
}

void notified(microkit_channel ch) {
    if (ch == CHANNEL_SERVER && waiting_for_response) {
        microkit_dbg_puts("\n=== CLIENT: Received Response ===\n");
        
        message_t *msg = (message_t *)shared_mem_vaddr;
        microkit_dbg_puts("CLIENT: Processed data: ");
        microkit_dbg_puts(msg->data);
        microkit_dbg_puts("\n");
        
        waiting_for_response = false;
        test_count++;
        
        // 追加のテストメッセージ
        if (test_count == 1) {
            microkit_dbg_puts("\n--- CLIENT: Sending second test ---\n");
            send_test_message("IPC demonstration complete");
        } else if (test_count == 2) {
            microkit_dbg_puts("\n--- CLIENT: Sending third test ---\n");
            send_test_message("Testing endpoint capabilities");
        } else {
            microkit_dbg_puts("\n==============================================\n");
            microkit_dbg_puts("CLIENT: All tests completed successfully!\n");
            microkit_dbg_puts("==============================================\n");
        }
    }
}

microkit_msginfo protected(microkit_channel ch, microkit_msginfo msginfo) {
    return microkit_msginfo_new(0, 0);
}
