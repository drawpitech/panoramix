/*
** EPITECH PROJECT, 2024
** panoramix
** File description:
** utils
*/


#pragma once

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define ATTR(x) __attribute__((x))
#define UNUSED ATTR(unused)

#define MAX_NAME_LENGTH 32
#define MAX_DESCRIPTION_LENGTH 255
#define MAX_BODY_LENGTH 512

#define LEN_OF(x) (sizeof(x) / sizeof(*x))

enum {
    Valid = 0,
    Error = 84,
};
