/*
** EPITECH PROJECT, 2024
** panoramix
** File description:
** panoramix
*/

#include "panoramix.h"

#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "debug.h"
#include "utils.h"

char HELP_MSG[] =
    "USAGE:\n"
    "\t./panoramix <nb_villagers> <pot_size> <nb_fights> <nb_refills>\n";

static bool parse_args(int argc, char **argv, gaule_t *gaule)
{
    unsigned long *arr = (size_t *)gaule;

    if (argc != 5) {
        fprintf(stderr, "Invalid number of arguments\n");
        fprintf(stderr, "%s", HELP_MSG);
        return false;
    }
    errno = 0;
    for (int i = 1; i < argc; i++) {
        char *ptr = argv[i];
        long tmp = strtol(argv[i], &ptr, 10);
        if (tmp <= 0 || *ptr != '\0' || errno != 0) {
            fprintf(
                stderr, "Invalid argument: `%s` %s\n", argv[i],
                (errno) ? strerror(errno) : "Not a number");
            fprintf(stderr, "%s", HELP_MSG);
            return false;
        }
        arr[i - 1] = tmp;
    }
    DEBUG("nb_villagers: %lu", gaule->nb_villagers);
    DEBUG("pot_size:     %lu", gaule->pot_size);
    DEBUG("nb_fights:    %lu", gaule->nb_fights);
    DEBUG("nb_refills:   %lu", gaule->nb_refills);
    return true;
}

static void *run_villager(villager_t *villager)
{
    DEBUG("[%lu]\tRunning villager", villager->id);
    printf("Villager %lu: Going into battle!\n", villager->id);

    printf("Villager %lu: I'm going to sleep now.\n", villager->id);
    DEBUG("[%lu]\tExit villager", villager->id);
    return villager;
}

static int gaulois_land(gaule_t *gaule)
{
    if (pthread_mutex_init(&gaule->mutex, NULL) != 0) {
        perror("mutex");
        return Error;
    }

    for (size_t i = 0; i < gaule->nb_villagers; i++) {
        villager_t *vil = gaule->villagers + i;
        *vil = (villager_t){.id = i, .gaule = gaule};

        if (pthread_create(
                &vil->thread, NULL, (void *(*)(void *)) & run_villager, vil))
            return perror("pthread_create"), Error;
    }

    for (size_t i = 0; i < gaule->nb_villagers; i++)
        if (pthread_join(gaule->villagers[i].thread, NULL))
            return perror("pthread_join"), Error;

    pthread_mutex_destroy(&gaule->mutex);
    return Valid;
}

int panoramix(int argc, char **argv)
{
    gaule_t gaule = {0};

    if (!parse_args(argc, argv, &gaule))
        return Error;

    gaule.villagers = malloc(gaule.nb_villagers * sizeof *gaule.villagers);
    if (gaule.villagers == NULL) {
        perror("malloc");
        return Error;
    }
    memset(gaule.villagers, 0, gaule.nb_villagers * sizeof *gaule.villagers);

    int ret = gaulois_land(&gaule);
    free(gaule.villagers);
    return ret;
}
