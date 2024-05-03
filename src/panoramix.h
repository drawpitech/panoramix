/*
** EPITECH PROJECT, 2024
** panoramix
** File description:
** panoramix
*/

#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
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
    const unsigned long nb_villagers;
    const unsigned long pot_size;
    const unsigned long nb_fights;
    const unsigned long nb_refills;

    villager_t *villagers;
    pthread_t tdruid;

    bool druid_awake;
    unsigned long pot;

    pthread_mutex_t mutex;
    sem_t sem_druid;
    sem_t sem_villagers;
} gaule_t;

int panoramix(int argc, char **argv);
