/*
 *  The bridge between Lua and JOESTAR.
 *  Copyright
 *      (C) 2020 Ryan Jeffrey
 *
 *  This file is part of JOESTAR.
 */

#ifndef JOES_BRIDGE_H
#define JOES_BRIDGE_H

#define HASH_MAX 479UL

unsigned long var_hash(const char *key);

struct joe_var
{

};

struct
{
    struct joe_var hmap[HASH_MAX];
} var_hashmap;

 #endif /* JOES_BRIDGE_H */