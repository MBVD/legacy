#include <stdio.h>

char *my_strstr(char *haystack, char *needle) {
    if (!*needle) {
        return haystack;
    }
    char *h = haystack;
    while (*h) {
        if (*h == *needle) {
            char *start_h = h;
            char *n = needle;
            while (*h && *n && (*h == *n)) {
                h++;
                n++;
            }
            if (!*n) {
                return start_h;
            }
            h = start_h;
        }
        h++;
    }
    return NULL;
}

int main() {
    char haystack[] = "Hello, World!";
    char needle[] = "World";
    char *res = my_strstr(haystack, needle);
    if (res) {
        printf("Found: %s\n", res);
    } else {
        printf("Not found\n");
    }
    return 0;
}