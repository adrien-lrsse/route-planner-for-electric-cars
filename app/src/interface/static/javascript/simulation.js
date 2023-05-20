
// var clean = document.getElementById("clean");
var num_tick = 1;
var tot_tick;
var tick_previous = document.getElementById("previous");
var tick_next = document.getElementById("next");
var display_num_tick = document.getElementById("display_num_tick");
var form_simulation = document.getElementById("f_simulation");
var table_simulation;

const franceCoordinates = [
  [
  [48.8566, 2.3522], // Paris
  [43.2965, 5.3698], // Marseille
  [47.2184, -1.5536] // Nantes
  ], 
  [
    [43.6108, 3.8767], // Montpellier
  [48.1173, -1.6778], // Rennes
  [45.7640, 4.8357], // Lyon
  [44.8378, -0.5792]// Bordeaux
  ], 
  [
  [49.4432, 1.0999], // Rouen
  [48.5734, 7.7521], // Strasbourg
  [47.2184, 6.0188] // Besançon
  ],[
  [48.8566, 2.3522], // Paris
  [43.2965, 5.3698], // Marseille
  [47.2184, -1.5536] // Nantes
  ], 
  [
  [43.6108, 3.8767], // Montpellier
  [48.1173, -1.6778], // Rennes
  [45.7640, 4.8357], // Lyon
  [44.8378, -0.5792]// Bordeaux
  ], 
  [
  [49.4432, 1.0999], // Rouen
  [48.5734, 7.7521], // Strasbourg
  [47.2184, 6.0188]
]];


var map;
function affichageMap(){
    const key = "1PB6znVWrQC3JvrDjX1j";
    const source = new ol.source.TileJSON({
      url: `https://api.maptiler.com/maps/streets-v2/tiles.json?key=${key}`,
      
      tileSize: 512,
      crossOrigin: "anonymous",
    });

    const attribution = new ol.control.Attribution({
      collapsible: false,
    });

    map = new ol.Map({
      layers: [
        new ol.layer.Tile({
          source: source,
        }),
      ],
      controls: ol.control.defaults
        .defaults({ attribution: false })
        .extend([attribution]),
      target: 'map',
      view: new ol.View({
        constrainResolution: true,
        center: ol.proj.fromLonLat([2.471829, 46.677531]),
        zoom: 6,
      }),
    });
}

function afficher_tick(liste) {
  removeVectors();
  for (let entier = 0; entier < liste.length; entier++) {
    addVector(liste[entier][0],liste[entier][1],liste[entier][2]);
  }
  
}

function addVector(longitude,latitude,type_point){
  if (type_point == 1){
    const layer = new ol.layer.Vector({
      source: new ol.source.Vector({
        features: [
          new ol.Feature({
            geometry: new ol.geom.Point(
              ol.proj.fromLonLat([longitude, latitude])
            ),
          }),
        ],
      }),
      style: new ol.style.Style({
        image: new ol.style.Icon({
          anchor: [0.5, 1],
          crossOrigin: "anonymous",
          src: "/static/javascript/point_orange.png",
        }),
      }),
    });
    map.addLayer(layer);
  }
  else if (type_point == 2){
    const layer = new ol.layer.Vector({
      source: new ol.source.Vector({
        features: [
          new ol.Feature({
            geometry: new ol.geom.Point(
              ol.proj.fromLonLat([longitude, latitude])
            ),
          }),
        ],
      }),
      style: new ol.style.Style({
        image: new ol.style.Icon({
          anchor: [0.5, 1],
          crossOrigin: "anonymous",
          src: "/static/javascript/point_rouge.png",
        }),
      }),
    });
    map.addLayer(layer);
  }
    
}

function removeVectors(){
    var layers = map.getLayers();
    var firstLayer = layers.getArray()[1];
    map.removeLayer(firstLayer);

    while (layers.getArray()[1] != undefined){
        var firstLayer = layers.getArray()[1];
        map.removeLayer(firstLayer);
    }
}


function importSimulation(simulation,len){
    table_simulation = simulation;
    tot_tick = len;
}


// clean.addEventListener('click', function (){
//     removeVectors();
// })

tick_next.addEventListener('click',function(){
  if(num_tick<tot_tick){
    num_tick++;
    display_num_tick.innerHTML = ""+num_tick+"/"+tot_tick+" ticks";

    afficher_tick(table_simulation[num_tick]);
  }
  
})

tick_previous.addEventListener('click',function(){
  if(num_tick>0){
    num_tick--;
    display_num_tick.innerHTML = ''+num_tick+"/"+tot_tick+" ticks";

    afficher_tick(table_simulation[num_tick]);
  }
 
})

form_simulation.addEventListener('submit', function(){
  var number_simulation = document.getElementById("status_generation");

  number_simulation.innerHTML = "Génération de la simulation en cours..."
})