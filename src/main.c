/*
** EPITECH PROJECT, 2024
** panoramix
** File description:
** main
*/

#include <unistd.h>

#include "debug.h"
#include "panoramix.h"

int main(int argc, char **argv)
{
    DEBUG_MSG("Running in debug mode");
    return panoramix(argc, argv);
}
