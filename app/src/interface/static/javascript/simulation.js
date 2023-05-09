
var clean = document.getElementById("clean");

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

<<<<<<< HEAD
function ticke_suivant(){
  
}
=======

function addVector(longitude,latitude){
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
            src: "https://docs.maptiler.com/openlayers/default-marker/marker-icon.png",
          }),
        }),
      });
      map.addLayer(layer);
      
    }

function removeVectors(){
    var layers = map.getLayers();
    var firstLayer = layers.getArray()[1];
    map.removeLayer(firstLayer);

    while (layers.getArray()[1] != undefined){
        var firstLayer = layers.getArray()[1];
        map.removeLayer(firstLayer);
    }
    console.log(firstLayer);
}

clean.addEventListener('click', function (){
    removeVectors();
})
>>>>>>> dc0ad16374c981761f4685317505c0fd7aa55327
