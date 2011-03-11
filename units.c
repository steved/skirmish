#include "units.h"
#include "camera.h"
#include "collision.h"

#include "util/astar.h"
#include "util/random.h"
#include "util/terrain.h"

#include "units/states/waiting.h"

#include <assert.h>
#include <math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>

static void delta_height_scale(gsl_vector *, gsl_vector *);
static int roll(int, int);

unit *create_empty_unit() {
  unit *empty_unit = (unit *) malloc(sizeof(unit));
  assert(empty_unit != NULL);
  empty_unit->type = infantry;

  empty_unit->state = NULL;
  push_unit_state(empty_unit, &waiting, NULL);

  empty_unit->position = gsl_vector_calloc(3);
  empty_unit->heading = gsl_vector_calloc(3);
  empty_unit->side = gsl_vector_calloc(3);

  weapons empty = {none, none, 0};
  empty_unit->weapons = empty; 

  attributes attrs = {
    0, 0, 0, // strength, speed, stamina
    0, 0, // health, armor
    1 // experience
  };

  empty_unit->display_radius[0] = 0;
  empty_unit->display_radius[1] = 0;
  empty_unit->collision_radius = 0;

  empty_unit->attributes = attrs;

  return empty_unit;
}

void place(unit *unit, int x, int y) {
  gsl_vector_set(unit->position, 0, x);
  gsl_vector_set(unit->position, 1, y);
  gsl_vector_set(unit->position, 2, height_at(x, y));
}

void place_at_vector(unit *unit, gsl_vector *v) {
  gsl_vector_memcpy(unit->position, v);
}

void print_unit(unit *unit) {
  printf("type %d, state %s, vector (%f, %f, %f)\n", 
      unit->type, 
      ((state *)unit->state->value)->name, 
      x(unit->position), 
      y(unit->position),
      z(unit->position));
  printf("\t");
  print_attributes(unit->attributes);
  printf("\t");
  print_weapons(unit->weapons);
}

void print_attributes(attributes attributes) {
  printf("strength %d, speed %d, stamina %d, health %d, armor %d\n",
      attributes.strength, attributes.speed, attributes.stamina,
      attributes.health, attributes.armor);
}

void print_weapons(weapons weapons) {
  printf("primary weapon %d, left %d. secondary weapon %d\n",
      weapons.primary_weapon, weapons.primary_weapon_left,
      weapons.secondary_weapon);
}

void remove_unit(unit *u) {
  gsl_vector_free(u->position);
  gsl_vector_free(u->heading);
  gsl_vector_free(u->side);
  ll_clear(u->state);
  free(u);
}

unit *check_for_unit_near(gsl_vector *location, PLAYERS *players, unit *unit_except, bool only_human, bool only_enemies) {
  for(int i = 0; i < players->num; i++) {
    player *pl = players->players[i];

    if((only_human && !pl->human) || (only_enemies && pl->human))
      continue;

    for(int j = 0; j < pl->num_divisions; j++) {
      division *div = pl->divisions[j];
      for(int k = 0; k < div->size; k++) {
        unit *un = div->units[k];

        if(un == unit_except || is_unit_dead(un))
          continue;

        if(bounding_circle_collision(location, (unit_except == NULL ? un : unit_except)->collision_radius, un->position, un->collision_radius))
          return un;
      }
    }
  }
  return NULL;
}

// returns true if @ destination, otherwise false
bool move_unit_towards(unit *subj, gsl_vector *dest, PLAYERS *players) {
  // set the z coordinate because it gets set incorrectly when
  // placing the unit in the beginning 
  gsl_vector_set(subj->position, 2, height_at(x(subj->position), y(subj->position)));

  gsl_vector *go_to = gsl_vector_alloc(3);
  gsl_vector_memcpy(go_to, dest);
  gsl_vector_sub(go_to, subj->position);

  double norm = gsl_blas_dnrm2(go_to);
  if(norm == 0)
    return true;

  gsl_vector_scale(go_to, 1 / norm);

  delta_height_scale(go_to, subj->position);
  gsl_vector_add(go_to, subj->position);

  //bool unit_at = check_for_unit_near(go_to, players, subj, false, false) != NULL;
  //if(!unit_at) {
    // XXX find a way around?
    gsl_vector_memcpy(subj->position, go_to);
  //}


  gsl_vector_memcpy(go_to, dest);
  gsl_vector_sub(go_to, subj->position);
  bool there = ((int) round(x(go_to))) == 0 && ((int) round(y(go_to))) == 0;
  gsl_vector_free(go_to);

  return there;
}

static void delta_height_scale(gsl_vector *new_location, gsl_vector *location) {
  gsl_vector *delta_height = gsl_vector_alloc(3);
  gsl_vector_memcpy(delta_height, new_location);
  gsl_vector_add(delta_height, location);

  double delta = height_at_vector(delta_height) - gsl_vector_get(location, 2);
  gsl_vector_set(new_location, 2, delta);

  float sign; 
  delta *= 100;
  if(delta < -0.1)
    sign = 1;
  else if(delta > 0.1)
    sign = -1;
  else
    sign = 0;

  // TODO scale by how high delta height is
  gsl_vector_scale(new_location, 1 + (sign * 0.3)); 
  gsl_vector_set(new_location, 2, height_at_vector(delta_height) - gsl_vector_get(location, 2));
}

void update_unit(unit *u, camera *cam, PLAYERS *players) {
  if(is_unit_dead(u))
    return;

  if(!((state *) u->state->value)->update(players, cam, u)) {
    pop_unit_state(u);
  }
}

bool attack_unit(unit *attacker, unit *defender) {
  int attack_strength = roll(attacker->attributes.strength, attacker->attributes.experience);
  int defend_strength = roll(defender->attributes.armor, defender->attributes.experience);

  int damage = attack_strength - defend_strength;
  if(damage < 0)
    damage = 0;
  printf("damage to defender %d\n", damage);
  defender->attributes.health -= damage; 
  printf("defender has %d health left\n", defender->attributes.health);

  return defender->attributes.health <= 0;
}

static int roll(int size, int number) {
  int result = 1;
  for(int i = 0; i < number; i++) {
    result *= random_int_max(size) + 1; 
  }

  return result;
}

void unit_dead(unit *un) {
  if(is_unit_dead(un))
    return;
  state *current_state = (state *) un->state->value;
  current_state->cleanup(un);

  un->state = ll_clear(un->state);
}

bool is_unit_dead(unit *un) {
  return un->state == NULL;
}

// TODO
int unit_range(unit *un) {
  return un->collision_radius + 5;
}
