#include <stdio.h>
#include "coordinates.h"
#include "database_reader.h"



int main(void){
    FILE *fichier = fopen("../data/mat_adj_distance.txt","w");
    for (int i = 1; i < 55321; i++)
    {
        coord_pt temp = getCoordonnees(i);
        for (int j = 1; j < i +1; j++)
        {
            coord_pt temp_2 = getCoordonnees(j);
            double test = distance(temp.longitude,temp.latitude,temp_2.longitude,temp_2.latitude);
            fprintf(fichier,"%f -> (%d,%d)\n",test,i,j);

        }
        
    }
    
    fclose(fichier);
}
