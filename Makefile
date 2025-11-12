# M1 Mac (Apple Silicon) 用の設定
MICROKIT_SDK := $(PWD)/microkit-sdk
BOARD := qemu_virt_aarch64
CONFIG := debug
MICROKIT_TOOL := $(MICROKIT_SDK)/bin/microkit

# M1 Macはネイティブでarm64
TOOLCHAIN := aarch64-elf-

CC := $(TOOLCHAIN)gcc
LD := $(TOOLCHAIN)ld
AR := $(TOOLCHAIN)ar

# リンカースクリプトのパス（重要！）
LINKER_SCRIPT := $(MICROKIT_SDK)/board/$(BOARD)/$(CONFIG)/lib/microkit.ld

# コンパイルフラグ
CFLAGS := -mcpu=cortex-a53 \
          -mstrict-align \
          -ffreestanding \
          -nostdlib \
          -I$(MICROKIT_SDK)/board/$(BOARD)/$(CONFIG)/include \
          -I.

# リンカーフラグ（リンカースクリプトを追加）
LDFLAGS := -L$(MICROKIT_SDK)/board/$(BOARD)/$(CONFIG)/lib \
           -T$(LINKER_SCRIPT)

# QEMU設定
QEMU := qemu-system-aarch64
QEMU_FLAGS := -machine virt,virtualization=on,highmem=off \
              -cpu cortex-a53 \
              -serial mon:stdio \
              -device loader,file=loader.img,addr=0x70000000,cpu-num=0 \
              -m size=2G \
              -nographic

# デフォルトターゲット
.DEFAULT_GOAL := task2

# タスク2: IPC実装
task2: server.elf client.elf
	@echo "Building Task 2 system image..."
	$(MICROKIT_TOOL) system.xml --search-path . --board $(BOARD) \
	    --config $(CONFIG) -o loader.img -r report.txt
	@echo "✓ Task 2 build complete!"
	@echo "Run with: make run"

# タスク3: Logger追加
task3: server.elf client.elf logger.elf
	@echo "Building Task 3 system image..."
	$(MICROKIT_TOOL) system_with_logger.xml --search-path . --board $(BOARD) \
	    --config $(CONFIG) -o loader.img -r report.txt
	@echo "✓ Task 3 build complete!"
	@echo "Run with: make run"

# タスク4: Fault tolerance
task4: server.elf client.elf logger.elf fault_handler.elf fault_test.elf
	@echo "Building Task 4 system image..."
	$(MICROKIT_TOOL) system_fault.xml --search-path . --board $(BOARD) \
	    --config $(CONFIG) -o loader.img -r report.txt
	@echo "✓ Task 4 build complete!"
	@echo "Run with: make run"

# ELFファイルのビルド（リンカースクリプトを使用）
%.elf: %.o
	@echo "Linking $@..."
	$(LD) $(LDFLAGS) $< -lmicrokit -o $@

%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# M1 Mac上でQEMU実行（自動的にビルド）
run: loader.img
	@echo "Starting seL4 Microkit in QEMU..."
	@echo "Press Ctrl+A then X to exit"
	@echo ""
	$(QEMU) $(QEMU_FLAGS)

# loader.imgがない場合はtask2を自動実行
loader.img: task2

# デバッグモードで実行
debug: loader.img
	$(QEMU) $(QEMU_FLAGS) -s -S &
	@echo "QEMU waiting for GDB connection on localhost:1234"

# クリーンアップ
clean:
	@echo "Cleaning build artifacts..."
	rm -f *.o *.elf loader.img report.txt
	@echo "✓ Clean complete"

# SDKの確認
check-sdk:
	@echo "Checking Microkit SDK..."
	@if [ -f "$(MICROKIT_TOOL)" ]; then \
		echo "✓ Microkit tool found"; \
		file $(MICROKIT_TOOL); \
	else \
		echo "✗ Microkit tool not found at $(MICROKIT_TOOL)"; \
		echo "Please download the macOS ARM64 SDK"; \
	fi
	@echo ""
	@echo "Checking linker script..."
	@if [ -f "$(LINKER_SCRIPT)" ]; then \
		echo "✓ Linker script found at $(LINKER_SCRIPT)"; \
	else \
		echo "✗ Linker script not found at $(LINKER_SCRIPT)"; \
	fi

# ヘルプ
help:
	@echo "Available targets:"
	@echo "  task2       - Build Task 2 (IPC Client-Server)"
	@echo "  task3       - Build Task 3 (Component Isolation with Logger)"
	@echo "  task4       - Build Task 4 (Fault Tolerance)"
	@echo "  run         - Run the current loader.img in QEMU"
	@echo "  clean       - Remove all build artifacts"
	@echo "  check-sdk   - Verify Microkit SDK installation"
	@echo "  help        - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make task2  # Build task 2"
	@echo "  make run    # Run in QEMU"
	@echo "  Ctrl+A, X   # Exit QEMU"

.PHONY: task2 task3 task4 run debug clean check-sdk help

# 自分用タスク: Buffer Overflow Demonstration
task5: safe_server.elf vulnerable_pd.elf attacker.elf logger.elf
	@echo "Building Task 5 system image..."
	$(MICROKIT_TOOL) system_buffer_overflow.xml --search-path . --board $(BOARD) \
	    --config $(CONFIG) -o loader.img -r report.txt
	@echo "✓ Task 5 build complete!"
	@echo "Run with: make run"

# セキュリティ機能を明示的に無効化したCFLAGS
CFLAGS_UNSAFE := -mcpu=cortex-a53 \
                 -mstrict-align \
                 -ffreestanding \
                 -nostdlib \
                 -I$(MICROKIT_SDK)/board/$(BOARD)/$(CONFIG)/include \
                 -I. \
                 -fno-stack-protector \
                 -z execstack \
                 -Wno-format-security \
                 -D_FORTIFY_SOURCE=0

# タスク5（セキュリティ機能なし版）
task5-unsafe: CFLAGS := $(CFLAGS_UNSAFE)
task5-unsafe: safe_server.elf vulnerable_pd.elf attacker.elf logger.elf
	@echo "Building Task 5 (ALL SECURITY FEATURES DISABLED)..."
	@echo "- Stack protector: DISABLED"
	@echo "- DEP/NX: DISABLED"
	@echo "- Format security: DISABLED"
	@echo "- FORTIFY_SOURCE: DISABLED"
	@echo ""
	$(MICROKIT_TOOL) system_buffer_overflow.xml --search-path . --board $(BOARD) \
	    --config $(CONFIG) -o loader.img -r report.txt
	@echo "✓ Task 5 (unsafe) build complete!"
	@echo "Run with: make run"

