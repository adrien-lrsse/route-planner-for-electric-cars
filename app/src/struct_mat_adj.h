#ifndef STRUCT_MAT_ADJ_H__
#define STRCUT_MAT_ADJ_H__

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