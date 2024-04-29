/*
** EPITECH PROJECT, 2024
** panoramix
** File description:
** panoramix
*/

#include "panoramix.h"

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"

static const char HELP_MSG[] =
"USAGE:\n"
"\t./panoramix <nb_villagers> <pot_size> <nb_fights> <nb_refills>\n";

static bool parse_args(int argc, char **argv, gaule_t *gaule)
{
    unsigned long *arr = (size_t *)gaule;
    long tmp = 0;
    char *ptr = NULL;

    if (argc != 5)
        return fprintf(stderr, "Invalid number of arguments\n%s", HELP_MSG),
            false;
    errno = 0;
    for (int i = 1; i < argc; i++) {
        ptr = argv[i];
        tmp = strtol(argv[i], &ptr, 10);
        if (tmp <= 0 || *ptr != '\0' || errno != 0)
            return fprintf(stderr, "Invalid argument: `%s` %s\n%s", argv[i],
                    (errno) ? strerror(errno) : "Not a number", HELP_MSG),
                    false;
        arr[i - 1] = tmp;
    }
    return true;
}

static void *run_villager(villager_t *villager)
{
    printf("Villager %lu: Going into battle!\n", villager->id);
    while (villager->fights < villager->gaule->nb_fights) {
        pthread_mutex_lock(&villager->gaule->mutex);
        printf("Villager %lu: I need a drink... I see %lu servings left.\n",
            villager->id, villager->gaule->pot);
        if (villager->gaule->pot == 0) {
            printf("Villager %lu: Hey Pano wake up! We need more potion.\n",
                villager->id);
            sem_post(&villager->gaule->sem_druid);
            sem_wait(&villager->gaule->sem_villagers);
        }
        villager->gaule->pot--;
        pthread_mutex_unlock(&villager->gaule->mutex);
        villager->fights++;
        printf("Villager %lu: Take that roman scum! Only %lu left.\n",
            villager->id, villager->gaule->nb_fights - villager->fights);
    }
    printf("Villager %lu: I'm going to sleep now.\n", villager->id);
    return villager;
}

static void *run_drouid(gaule_t *gaule)
{
    printf("Druid: I'm ready... but sleepy...\n");
    while (true) {
        sem_wait(&gaule->sem_druid);
        if (gaule->refills >= gaule->nb_refills) {
            printf("Druid: I'm out of viscum. I'm going back to... zZz\n");
            break;
        }
        gaule->pot = gaule->pot_size;
        gaule->refills++;
        printf(
            "Druid: Ah! Yes, yes, I'm awake! Working on it! Beware I can "
            "only make %lu more refills after this one.\n",
            gaule->nb_refills - gaule->refills);
        sem_post(&gaule->sem_villagers);
    }
    return gaule;
}

// me when i see maxime
static gaule_t *summon_gaule(gaule_t *gaule)
{
    gaule->villagers = malloc(gaule->nb_villagers * sizeof *gaule->villagers);
    if (gaule->villagers == NULL)
        return perror("malloc"), NULL;
    memset(gaule->villagers, 0,
        gaule->nb_villagers * sizeof *gaule->villagers);
    if (pthread_mutex_init(&gaule->mutex, NULL) != 0)
        return perror("mutex"), NULL;
    if (sem_init(&gaule->sem_druid, 0, 0) == -1 ||
        sem_init(&gaule->sem_villagers, 0, 0) == -1)
        return perror("sem_init"), NULL;
    gaule->pot = gaule->pot_size;
    return gaule;
}

static void close_the_frontiers(gaule_t *gaule, pthread_t *drouid)
{
    for (size_t i = 0; i < gaule->nb_villagers; i++)
        if (pthread_join(gaule->villagers[i].thread, NULL))
            perror("pthread_join");
    pthread_cancel(*drouid);
    if (pthread_join(*drouid, NULL))
        perror("pthread_join");
    pthread_mutex_destroy(&gaule->mutex);
    free(gaule->villagers);
    sem_destroy(&gaule->sem_druid);
    sem_destroy(&gaule->sem_villagers);
}

int panoramix(int argc, char **argv)
{
    gaule_t gaule = {0};
    pthread_t drouid = {0};
    villager_t *vil = NULL;

    if (!parse_args(argc, argv, &gaule) || summon_gaule(&gaule) == NULL)
        return Error;
    for (size_t i = 0; i < gaule.nb_villagers; i++) {
        vil = gaule.villagers + i;
        *vil = (villager_t){.id = i, .gaule = &gaule};
        if (pthread_create(
                &vil->thread, NULL, (void *(*)(void *))&run_villager, vil))
            return perror("pthread_create"), Error;
    }
    if (pthread_create(&drouid, NULL, (void *(*)(void *))&run_drouid, &gaule))
        return perror("pthread_create"), Error;
    close_the_frontiers(&gaule, &drouid);
    return Valid;
}
