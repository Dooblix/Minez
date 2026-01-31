// vectors.c
// Copyright (c) 2026 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

#include <stdlib.h>
#include <stdio.h>
#include "../include/vectors.h"

vector_size_t make_vector_size_t(size_t initial_capacity) {
    vector_size_t v;
    v.size = 0;
    v.capacity = initial_capacity;
    v.data = malloc(initial_capacity * sizeof(size_t));
    if (!v.data) {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }
    return v;
}

void vector_size_t_push(vector_size_t* v, size_t val) {
    if (v->size >= v->capacity) {
        v->capacity = (v->capacity == 0) ? 1 : v->capacity * 2;
        size_t* tmp = realloc(v->data, v->capacity * sizeof(size_t));
        if (!tmp) exit(1);
        v->data = tmp;
    }
    v->data[v->size++] = val;
}

size_t vector_size_t_pop(vector_size_t* v) {
    if (v->size > 0) {
        v->size--;
        return v->data[v->size];
    }
    fprintf(stderr, "Error: tried to pop an empty vector.");
    exit(1);
}

vector_int make_vector_int(size_t initial_capacity) {
    vector_int v;
    v.size = 0;
    v.capacity = initial_capacity;
    v.data = malloc(initial_capacity * sizeof(int));
    if (!v.data) {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }
    return v;
}

void vector_int_push(vector_int* v, int val) {
    if (v->size >= v->capacity) {
        v->capacity = (v->capacity == 0) ? 1 : v->capacity * 2;
        int* tmp = realloc(v->data, v->capacity * sizeof(int));
        if (!tmp) exit(1);
        v->data = tmp;
    }
    v->data[v->size++] = val;
}

int vector_int_pop(vector_int* v) {
    if (v->size > 0) {
        v->size--;
        return v->data[v->size];
    }
    fprintf(stderr, "Error: tried to pop an empty vector.");
    exit(1);
}

vector_char make_vector_char(size_t initial_capacity) {
    vector_char v;
    v.size = 0;
    v.capacity = initial_capacity;
    v.data = malloc(initial_capacity * sizeof(unsigned char));
    if (!v.data) {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }
    return v;
}

void vector_char_push(vector_char* v, unsigned char val) {
    if (v->size >= v->capacity) {
        v->capacity = (v->capacity == 0) ? 1 : v->capacity * 2;
        unsigned char* tmp = realloc(v->data, v->capacity * sizeof(unsigned char));
        if (!tmp) exit(1);
        v->data = tmp;
    }
    v->data[v->size++] = val;
}

unsigned char vector_char_pop(vector_char* v) {
    if (v->size > 0) {
        v->size--;
        return v->data[v->size];
    }
    fprintf(stderr, "Error: tried to pop an empty vector.");
    exit(1);
}