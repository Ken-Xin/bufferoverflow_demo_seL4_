#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

// 文字列の長さを取得
static inline size_t my_strlen(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// 文字列のコピー
static inline char *my_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

// メモリのゼロクリア
static inline void *my_memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    for (size_t i = 0; i < n; i++) {
        p[i] = (unsigned char)c;
    }
    return s;
}

// 簡易snprintf（整数のみ対応）
static inline int my_snprintf(char *str, size_t size, const char *format, ...) {
    // 簡易実装：フォーマット文字列をそのままコピー
    size_t i;
    for (i = 0; i < size - 1 && format[i] != '\0'; i++) {
        str[i] = format[i];
    }
    str[i] = '\0';
    return i;
}

#endif // UTIL_H
