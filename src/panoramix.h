/*
** EPITECH PROJECT, 2024
** panoramix
** File description:
** panoramix
*/

#pragma once

#include <pthread.h>
#include <unistd.h>

typedef struct gaule_s gaule_t;

typedef struct {
    size_t id;
    pthread_t thread;
    gaule_t *gaule;

    unsigned long fights;
} villager_t;

typedef struct gaule_s {
    // Those variables MUST NOT be moved in the structure
    unsigned long nb_villagers;
    unsigned long pot_size;
    unsigned long nb_fights;
    unsigned long nb_refills;

    pthread_mutex_t mutex;
    villager_t *villagers;
    pthread_t tdruid;
} gaule_t;

int panoramix(int argc, char **argv);
