/*
** EPITECH PROJECT, 2024
** panoramix
** File description:
** panoramix
*/

#pragma once

#include <pthread.h>

typedef struct {
    pthread_t thread;
} villager_t;

typedef struct {
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
