#include <stdio.h>

int unic_words(char **s) {
  int ans = 0;
  for (int i = 0; s[i] != NULL; i++) {
    int unic = 1;
    for (int j = 0; j < i; j++) {
      if (strcmp(s[i], s[j]) == 0) {
        unic = 0;
        break;
      }
    }
    ans += unic;
  }
  return ans;
}
int my_strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  if (*s1 == *s2){
    return 0;
  }
} 