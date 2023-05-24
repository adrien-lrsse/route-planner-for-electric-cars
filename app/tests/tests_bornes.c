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

describe(test_list_bornes_visitees_create){
    it("Conformance test"){
        list_bornes_visitees* test_list = list_bornes_visitees_create();
        asserteq(list_bornes_visitees_is_empty(test_list),true);
        defer(list_bornes_visitees_destroy(test_list));
    }
}

describe(test_list_bornes_visitees_append){
    it("Conformance test"){
        list_bornes_visitees* test_list = list_bornes_visitees_create();
        list_bornes_visitees_append(test_list,6);
        asserteq(test_list->id_borne_visitee,6);
        defer(list_bornes_visitees_destroy(test_list));
    }
}

describe(test_borne_deja_visitee){
    it("Conformance test"){
        list_bornes_visitees* test_list = list_bornes_visitees_create();
        assertneq(borne_deja_visitee(test_list,5),true);
        list_bornes_visitees_append(test_list,6);
        asserteq(borne_deja_visitee(test_list,6),true);
        assertneq(borne_deja_visitee(test_list,8),true);
        defer(list_bornes_visitees_destroy(test_list));
    }
}

snow_main();