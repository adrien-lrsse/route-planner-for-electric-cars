var estMasque = true;


var btn = document.getElementById("settings");
var select_car = document.getElementById("select_car");
var select_modele = document.getElementById("selectmodele");
var slider_pct = document.getElementById("pct_autonomie_initial");
var slider_delimiter = document.getElementById("limite_decharge");
var form_recherche = document.getElementById("form_recherche");
var limit_temps = document.getElementById("input_limit_temps");
var limit_or_not = document.getElementById("limit");
var init_autonomie = document.getElementById("pct_autonomie_initial");

var i = 0;

var file_car;
var marques

function initialisation_voiture(voiture){
  file_car = JSON.parse(voiture);
  var marques = Object.keys(file_car)
  // for (var i = 0; i < marques.length; i++) {
  //   var marque = marques[i];
  //   // console.log(marque)
  //   for (var j = 0; j < parsed[marque].length; j++) {
  //     var modele = parsed[marque][j][0]; // Nom du modèle
  //     var puissance = parsed[marque][j][1]; // Puissance en chevaux-vapeur
  //     console.log(marque + " " + modele + " (" + puissance + " km)");
  //   }
  // }

}

btn.onclick = function(){
  if (estMasque) {
    document.getElementById("form_param").style.display = "flex";
    estMasque = false;
    document.getElementById("settings").innerHTML = "Masquer les paramètres"
  }
  else {
    document.getElementById("form_param").style.display = "none";
    estMasque = true;
    document.getElementById("settings").innerHTML = "Afficher les paramètres"


  } 
}


function afficher_modele(marque) {
  var modele_select = document.getElementById("selectmodele");
  modele_select.innerHTML = "";
  for (var j = 0; j < file_car[marque].length; j++) {
    var modele = file_car[marque][j][0]; // Nom du modèle
    var id_unique = file_car[marque][j][2]
    var puissance = file_car[marque][j][1]; // Puissance en chevaux-vapeur
    console.log(marque + " " + modele + " (" + puissance + " km)");
    var option = document.createElement("option");
    console.log(id_unique);
    option.value = id_unique;
    option.text = modele;
    modele_select.add(option);
  }
}


function getAutonomie(marque,id_unique){
  for (var i = 0; i<file_car[marque].length;i++){
    if (file_car[marque][i][2] == id_unique){
      return file_car[marque][i][1];
    }
  }
  return -1;
}


function affichageAutonomie(){
  var auto_max = document.getElementById("auto_max");
  var id = select_modele.value;
  var marque = select_car.value;
  auto_max.innerHTML = "Autonomie maximale : " +getAutonomie(marque,id)  +" km" 
  var sliderPct = document.getElementById("pct_autonomie_initial");
  var auto_start = document.getElementById("auto_initial");
  auto_start.innerHTML = "Autonomie de départ : " +Math.floor(getAutonomie(marque,id)*((sliderPct.value)/100)) +" km" ;
  
}

select_car.addEventListener('change', function () {
  afficher_modele(this.value);
  affichageAutonomie();
});

window.addEventListener('load', function () {
  afficher_modele(select_car.value)
  affichageAutonomie();
  slider_delimiter.value = 0;
  init_autonomie.value = 100;
  limit_or_not.checked = true;
});

select_modele.addEventListener('change',function(){
  var auto_max = document.getElementById("auto_max");
  var id = select_modele.value;
  var marque = select_car.value;
  auto_max.innerHTML = "Autonomie maximale : " +getAutonomie(marque,id) +" km" 
  var sliderPct = document.getElementById("pct_autonomie_initial");
  var auto_start = document.getElementById("auto_initial");
  auto_start.innerHTML = "Autonomie de départ : " +Math.floor(getAutonomie(marque,id)*((sliderPct.value)/100)) +" km" ;

})


slider_pct.addEventListener('input',function(){
  var int_pct = document.getElementById("pct_int_charge");
  var auto_start = document.getElementById("auto_initial");
  var id = select_modele.value;
  var marque = select_car.value;
  auto_start.innerHTML = "Autonomie de départ : " +Math.floor(getAutonomie(marque,id)*((this.value)/100)) +" km" ;
  int_pct.innerHTML = "Charge de la batterie : "+this.value+" %";
})

slider_delimiter.addEventListener('input',function(){
  var txt = document.getElementById("text_delimiter");
  txt.innerHTML = "Seuil de décharge : " +this.value+ " %";
})



slider_delimiter.addEventListener('change',function(){
    slider_pct.min = this.value;
    console.log(slider_pct.min,slider_pct.value);

    var int_pct = document.getElementById("pct_int_charge");
    var auto_start = document.getElementById("auto_initial");
    var id = select_modele.value;
    var marque = select_car.value;
    auto_start.innerHTML = "Autonomie de départ : " +Math.floor(getAutonomie(marque,id)*((slider_pct.value)/100)) +" km" ;
    int_pct.innerHTML = "Charge de la batterie : "+slider_pct.value+" %";

})

init_autonomie.addEventListener('input',function(){
  var txt = document.getElementById("pct_int_charge");
  txt.innerHTML = "Charge de la batterie : " +this.value+ " %";
})


form_recherche.addEventListener('submit', function (event) {
  event.preventDefault();
  id_voit = select_modele.value;
  delimiter = slider_delimiter.value;
  console.log(delimiter);
  init = init_autonomie.value;
  isLimited = limit_or_not.checked;
  time = limit_temps.value;
  const formData = new FormData(form_recherche);
    formData.append('selectmodele', id_voit);
    formData.append('reserve', delimiter);
    formData.append('limit',isLimited);
    formData.append('input_limit_temps',time);
    formData.append('pct_autonomie_initial',init)
    fetch('/find_itinerary', { method: 'POST', body: formData })
    .then(response => {
      if (response.ok) {
        location.reload(); // recharger la page si la réponse est OK
      } else {
        throw new Error('Réponse réseau non OK');
      }
    })
    .catch(error => console.error(error));
    
  });

  limit_temps.addEventListener('input', function (){
    if (this.value > 240){
      this.value = 240;
    } else if (this.value<0){
      this.value = 0;
    }
      var txt = document.getElementById("txt_limit_temps");
      txt.innerHTML = "Temps : " + this.value +" min";  

  });
  
limit_or_not.addEventListener('input', function (){
  var txt = document.getElementById("txt_limit_temps");

  if (this.checked){
    limit_temps.style.display = "none";
    txt.style.display = "none";
    
  } else {
    limit_temps.style.display = "flex";
    txt.style.display = "block";

  }

});