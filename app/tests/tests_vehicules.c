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

describe(test_create_voiture){
    it("Conformance test"){
        voiture* test_voiture = create_voiture(55);
        asserteq(test_voiture->id,55);
        asserteq(test_voiture->autonomie,425);
        asserteq(test_voiture->autonomie_actuelle,425);
        asserteq(test_voiture->puissance,90600);
        asserteq(test_voiture->puissance_actuelle,90600);
        asserteq_str(test_voiture->nom,"Mercedes EQE SUV AMG 53 4MATIC+");
        defer(destroy_voiture(test_voiture));
    }
}

describe(test_update_charge){
    it("Conformance test"){
        voiture* test_voiture = create_voiture(55);
        update_charge(test_voiture,150.0);
        asserteq(test_voiture->autonomie_actuelle,425-150.0);
        defer(destroy_voiture(test_voiture));
    }
}

describe(test_update_puissance){
    it ("Conformance test : without reserve"){
        voiture* test_voiture = create_voiture(55);
        test_voiture->autonomie_actuelle = 200;
        update_puissance(test_voiture);
        asserteq(test_voiture->puissance_actuelle,42635);
        defer(destroy_voiture(test_voiture));
    }
    it ("Conformance test : with reserve"){
        voiture* test_voiture = create_voiture(55);
        test_voiture->reserve_equivalent_autonomie = 150;
        test_voiture->autonomie_actuelle = 200;
        update_puissance(test_voiture);
        asserteq(test_voiture->puissance_actuelle,74611);
        defer(destroy_voiture(test_voiture));
    }
}

describe(test_update_autonomie){
    it("Conformance test"){
        voiture* test_voiture = create_voiture(55);
        test_voiture->reserve_equivalent_autonomie = 150;
        update_autonomie(test_voiture);
        asserteq(test_voiture->autonomie_actuelle,275);
        defer(destroy_voiture(test_voiture));
    }
}

describe(test_recharge){
    it("Conformance test : no recharge time minimum"){
        voiture* test_voiture = create_voiture(55);
        test_voiture->autonomie_actuelle = 300;
        test_voiture->temps_recharge_max_minutes = 0;
        update_puissance(test_voiture);
        recharge(test_voiture,150000);
        asserteq(test_voiture->autonomie_actuelle,425);
        defer(destroy_voiture(test_voiture));
    }
    it("Conformance test : recharge more than needed"){
        voiture* test_voiture = create_voiture(55);
        test_voiture->autonomie_actuelle = 300;
        test_voiture->temps_recharge_max_minutes = 10000;
        update_puissance(test_voiture);
        recharge(test_voiture,150000);
        asserteq(test_voiture->autonomie_actuelle,425);
        defer(destroy_voiture(test_voiture));
    }
    it("Conformance test : doesn't recharge to maximum"){
        voiture* test_voiture = create_voiture(55);
        test_voiture->autonomie_actuelle = 300;
        test_voiture->temps_recharge_max_minutes = 2;
        update_puissance(test_voiture);
        recharge(test_voiture,150000);
        asserteq(test_voiture->autonomie_actuelle,323);
        defer(destroy_voiture(test_voiture));
    }
}

describe(test_set_autonomie){
    it("Conformance test"){
        voiture* test_voiture = create_voiture(55);
        set_autonomie(test_voiture,150);
        asserteq(test_voiture->autonomie_actuelle,150);
        defer(destroy_voiture(test_voiture));
    }
}

describe(test_simulation_recharge){
    it("Conformance test : no recharge time minimum"){
        voiture* test_voiture = create_voiture(55);
        test_voiture->autonomie_actuelle = 300;
        test_voiture->temps_recharge_max_minutes = 0;
        update_puissance(test_voiture);
        long double autonomie = simulation_recharge(test_voiture,150000, 150);
        update_charge(test_voiture,150);
        recharge(test_voiture,150000);
        assert(fabsl(autonomie-test_voiture->autonomie_actuelle)<0.0001);
        defer(destroy_voiture(test_voiture));
    }
    it("Conformance test : recharge more than needed"){
        voiture* test_voiture = create_voiture(55);
        test_voiture->autonomie_actuelle = 300;
        test_voiture->temps_recharge_max_minutes = 10000;
        update_puissance(test_voiture);
        long double autonomie = simulation_recharge(test_voiture,150000, 150);
        update_charge(test_voiture,150);
        recharge(test_voiture,150000);
        assert(fabsl(autonomie-test_voiture->autonomie_actuelle)<0.0001);
        defer(destroy_voiture(test_voiture));
    }
    it("Conformance test : doesn't recharge to maximum"){
        voiture* test_voiture = create_voiture(55);
        test_voiture->autonomie_actuelle = 300;
        test_voiture->temps_recharge_max_minutes = 2;
        update_puissance(test_voiture);
        long double autonomie = simulation_recharge(test_voiture,150000, 150);
        update_charge(test_voiture,150);
        recharge(test_voiture,150000);
        assert(fabsl(autonomie-test_voiture->autonomie_actuelle)<0.0001);
        defer(destroy_voiture(test_voiture));
    }
}

snow_main();
