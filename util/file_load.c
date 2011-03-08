#include "util/file_load.h"

#include "units.h"
#include "units/romans.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static int read_int(FILE *);
static char *read_string(FILE *);

PLAYERS *read_file(char *name) {
  FILE *fp = fopen(name, "r");
  if(fp == NULL) {
    perror("Could not open file");
    exit(1);
  }

  PLAYERS *players = (PLAYERS *) malloc(sizeof(PLAYERS));
  assert(players != NULL);

  players->num = read_int(fp);
  // for now make sure the max number of players
  assert(players->num <= MAX_PLAYERS);

  players->players = (player **) malloc(sizeof(player *) * players->num);
  players->setup = false;
  assert(players->players != NULL);

  printf("found %d players\n", players->num);
  for(int i = 0; i < players->num; i++) {

    // human? and player idx
    int index = read_int(fp);
    assert(i == index);

    bool human = read_int(fp);

    // load the number of divisions and create the player
    player *p;
    if(human) {
      // read in the name
      char *name = read_string(fp);
      p = create_human_player(name, read_int(fp));
    } else {
      p = create_ai_player(read_int(fp));
    }

    printf("player %d: found %d divisions\n", i, p->num_divisions);

    for(int j = 0; j < p->num_divisions; j++) {
      division *div = malloc(sizeof(division));
      
      // TODO standardize either in file loaded or elsewhere
      div->structure.num_per_row = 5;
      div->structure.row_padding = 10;
      assert(div != NULL);

      // read in division num
      assert(j == read_int(fp));

      div->size = read_int(fp);
      printf("player %d division %d: found %d units\n", i, j, div->size);

      div->units = (unit **) malloc(sizeof(unit *) * div->size);
      assert(div->units != NULL);

      unit *u;
      char *unit_type;
      //int x, y;
      for(int k = 0; k < div->size; k++) {
        unit_type = read_string(fp);
        free(unit_type);

        u = create_legionary_unit();
        u->division = div;
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

static char *read_string(FILE *fp) {
  char line[255];
  fgets(line, 255, fp);

  char *copy = malloc(255);

  size_t comment_index = strcspn(line, " #");

  // if there isn't a comment, then
  // make the last char the idx of the \0
  int len = strlen(line);
  if(comment_index >= len)
    comment_index = len - 1;

  strncpy(copy, line, comment_index);
  copy[comment_index] = '\0';

  return copy;
}
