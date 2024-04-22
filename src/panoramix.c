/*
** EPITECH PROJECT, 2024
** panoramix
** File description:
** panoramix
*/

#include "panoramix.h"

#include <bits/pthreadtypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
    for (int i = 1; i < argc; i++) {
        char *ptr = argv[i];
        long tmp = strtol(argv[i], &ptr, 10);
        if (tmp <= 0 || *ptr != '\0') {
            fprintf(stderr, "Invalid argument: %s\n", argv[i]);
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

int panoramix(int argc, char **argv)
{
    gaule_t pano = {0};

    if (!parse_args(argc, argv, &pano))
        return Error;
    pano.villagers = malloc(pano.nb_villagers * sizeof *pano.villagers);
    if (pano.villagers == NULL) {
        perror("malloc");
        return Error;
    }

    free(pano.villagers);
    return Valid;
}
