#define SNOW_ENABLED
#include "../src/snow.h"
#include "../src/database_reader.h"
#include "../src/coordinates.h"
#include "../src/borne.h"
#include "../src/etape.h"
#include "../src/vehicules.h"
#include "../src/itinerary.h"
#include "../src/random_points.h"
#include "../src/thread.h"
#include <stdio.h>
#include <stdlib.h>

#define NB_FORKS 20
#define NB_ITINERAIRES 150
#define DIST_MIN 100

describe(test_parametre_aleatoire){
    it("Conformance test of id vehicule"){
        int test_id = 0;
        for (int i = 0; i < 100; i++)
        {
            test_id = rand()%310+1;
            assert(test_id > 0 && test_id < 311);
        }
    }
    it("Conformance test of 'Seuil de décharge'"){
        int test_seuil_decharge = -1;
        for (int i = 0; i < 100; i++)
        {
            test_seuil_decharge = rand()%20;
            assert(test_seuil_decharge>=0 && test_seuil_decharge<20);
        }
    }
    it("Conformance test of recharge time"){
        int test_tps_recharge = 0;
        for (int i = 0; i < 100; i++)
        {
            test_tps_recharge = (rand() % 31) + 30;
            assert(test_tps_recharge > 29 && test_tps_recharge < 61);
        }
    }
    it("Conformance test of initial autonomy"){
        int test_initial_autonomy = 0;
        for (int i = 0 ; i<100 ; i++){
            test_initial_autonomy = (rand()%30) + 70;
            assert(test_initial_autonomy > 69 && test_initial_autonomy < 101);
        }
    }
}

describe(test_recuperation_coord_ligne){
    it("Conformance test"){
        coord_pt* test1_coord = recuperation_coord_ligne(14);
        long double test1_lat = test1_coord->latitude;
        long double test2_long = test1_coord->longitude;
        free(test1_coord);
        assert(abs(test1_lat-45.93196511)<0.001);
        assert(abs(test2_long-5.693133119)<0.001);
    }
}

describe(test_generate_x_random_itinerary){
    it("Conformance test : size"){
        trajets_aleatoires* test_trajet = generate_x_random_itinerary(50,100);
        asserteq(test_trajet->taille,50);
        defer(destroy_trajets_aleatoires(test_trajet));
    }
    it("Conformance test : distance trajet"){
        trajets_aleatoires* test_trajet = generate_x_random_itinerary(30,100);
        double d = 0;
        for (int i = 0; i < 30; i++)
        {
            d = distance(test_trajet->traj[i].depart->longitude,test_trajet->traj[i].depart->latitude,test_trajet->traj[i].arrivee->longitude,test_trajet->traj[i].arrivee->latitude);
            assert(d>=100);
        }
        defer(destroy_trajets_aleatoires(test_trajet));
        
    }

}

snow_main();