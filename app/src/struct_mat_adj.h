#ifndef STRUCT_MAT_ADJ_H__
#define STRUCT_MAT_ADJ_H__

#include <stdlib.h>
#include <stdio.h>

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

typedef struct mat_adj
{
    double* distance;
    int length;
} mat_adj;

mat_adj* create_mat_adj(int taille); 

double get(mat_adj* mat,int i, int j);

void define(mat_adj* mat, int i, int j,double value);

void mat_destroy(mat_adj* mat);

#endif /* STRUCT_MAT_ADJ__ */
