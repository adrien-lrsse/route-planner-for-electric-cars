var estMasque = true;

var btn = document.getElementById("settings");

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