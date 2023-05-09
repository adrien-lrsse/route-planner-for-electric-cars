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

    const map = new ol.Map({
      layers: [
        new ol.layer.Tile({
          source: source,
        }),
      ],
      controls: ol.control.defaults
        .defaults({ attribution: false })
        .extend([attribution]),
      target: "map",
      view: new ol.View({
        constrainResolution: true,
        center: ol.proj.fromLonLat([2.471829, 46.677531]),
        zoom: 6,
      }),
    });
}