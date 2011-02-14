#include "file_load.h"
#include "romans.h"
#include "units.h"

#include <assert.h>
#include <stdio.h>

static int read_int(FILE *);

PLAYERS *read_file(char *name) {
  FILE *fp = fopen(name, "r");
  if(fp == NULL) {
    perror("Could not open file");
    exit(1);
  }

  PLAYERS *players = (PLAYERS *) malloc(sizeof(PLAYERS));
  assert(players != NULL);

  char line[255];
  players->num = read_int(fp);
  players->players = (player **) malloc(sizeof(player *) * players->num);
  assert(players != NULL);

  printf("found %d players\n", players->num);
  for(int i = 0; i < players->num; i++) {

    // human? and player idx
    fgets(line, 255, fp);

    player *p = create_ai_player(read_int(fp));
    printf("player %d: found %d divisions\n", i, p->num_divisions);

    for(int j = 0; j < p->num_divisions; j++) {
      division *div = malloc(sizeof(division));
      assert(div != NULL);

      // division num
      fgets(line, 255, fp);

      div->size = read_int(fp);
      printf("player %d division %d: found %d units\n", i, j, div->size);

      div->units = (unit **) malloc(sizeof(unit *) * div->size);
      assert(div->units != NULL);

      unit *u;
      int x, y;
      for(int k = 0; k < div->size; k++) {
        fgets(line, 255, fp);
        u = create_legionary_unit();
        x = read_int(fp);
        y = read_int(fp);
        printf("placing unit @ (%d, %d)\n", x, y);
        place(u, x, y);
        div->units[k] = u;
      }
      p->divisions[j] = div;
    }
    players->players[i] = p;
  }

  return players;
}

static int read_int(FILE *fp) {
  char line[255], copy[255];
  fgets(line, 255, fp);

  size_t comment_index = strcspn(line, " #");

  errno = 0;
  char *p;
  int read_in;

  // if there isn't a comment, then
  // make the last char the idx of the \0
  int len = strlen(line);
  if(comment_index >= len)
    comment_index = len - 1;

  strncpy(copy, line, comment_index);
  copy[comment_index] = '\0';
  read_in = strtol(copy, &p, 0);

  if(errno != 0 || *p != '\0' || p == copy) {
    fprintf(stderr, "there was an error reading an int in\n");
    exit(1);
  }

  return read_in;
}
