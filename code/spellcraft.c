#include <stdio.h>

enum elements { FIRE, WATER, LIGHT, ENERGY, EARTH, STORM, DARK };
char* word[] = { "Fire", "Water", "Light", "Energy", "Earth", "Storm", "Dark" };

int main() {
  int i, j, k, count = 0;
  for(i = FIRE; i <= DARK; i++) {
    for(j = i; j <= DARK; j++) {
      for(k = j; k <= DARK; k++) {
        printf("%s + %s + %s\n", word[i], word[j], word[k]);
        count++;
      }
    }
  }
  printf("%d combinations\n", count);
}

