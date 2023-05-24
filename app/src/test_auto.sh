#!/bin/bash

OUTPUT_FILE="../data/execution_times.csv"

for M in 10 20 50 75
do
    for N in $(seq 75 75 1050)
    do
        echo "Running simulation for N=$N"

        total_time=0

        # Lance la simu 10 fois et récupère le temps de début et de fin
        for i in $(seq 0 1 9)
        do
            START_TIME=$(date +%s%N)
            ./main "$N" "$M"
            END_TIME=$(date +%s%N)

            # Calcul le temps d'exécution en secondes
            ELAPSED_TIME=$(echo "scale=6; ($END_TIME - $START_TIME) / 1000000000" | bc)

            total_time=$(echo "$total_time + $ELAPSED_TIME" | bc)
        done

        # Calcul le temps moyen d'exécution en secondes
        AVERAGE_TIME=$(echo "scale=6; $total_time / 10" | bc)

        # Ajoute les résultats au fichier de sortie
        echo "$M,$N,$AVERAGE_TIME" >> "$OUTPUT_FILE"
    done
done