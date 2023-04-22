from flask import Flask, render_template, request,redirect,session, url_for
from flask_session import Session

import os
import subprocess
import csv
from geopy.geocoders import Nominatim

app = Flask(__name__)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)



@app.route("/")
def index():
    
    status = ""
    print(parseResultat())
    if parseResultat() == [['empty']]:
        status = "Adresse incorrecte ou non trouvée par le module Geopy"
    return render_template("index.html",resultat = parseResultat(),status=status, voiture=parsageVoiture())

@app.route("/find_itinerary",methods=["GET","POST"])
def find_itinerary():
    if request.method == "POST":
        depart = request.form.get("depart")
        arrivee = request.form.get("arrivee")
        voiture = request.form.get("selectmodele")
        reserve = request.form.get("reserve")
        isLimit = request.form.get("limit")
        time = request.form.get("input_limit_temps")
        if (isLimit == "true"):
            temps = 0
        else :
            temps = time
        print(voiture,reserve,isLimit,time)
        error_status = execute_app(depart,arrivee,voiture,reserve,temps)
    return redirect('/')


    

def execute_app(depart,arrivee,id_voiture,pourcentage_reserve,temps):
    status = ""
    geolocator = Nominatim(user_agent="itinerary")
    depart = geolocator.geocode(depart)
    arrivee = geolocator.geocode(arrivee)
    if (depart is None):
        status += "Adresse de départ invalide"
    if (arrivee is None):
        status += "Adresse d'arrivée invalide"
    print(status)

    if (status == ""):
        script_dir = os.path.dirname(os.path.abspath(__file__))
        cwd = os.path.abspath(os.path.join(script_dir, '..'))
        temps_max_attente = 0
        type = 1
        subprocess.run(["./main",str(depart.longitude),str(depart.latitude),str(arrivee.longitude),str(arrivee.latitude),str(id_voiture),str(pourcentage_reserve),str(temps),str(type)],cwd=cwd)
    else :
            f = open("../../data/etape.txt","w")
            f.write("empty\n")
            f.close()
    return status


def parseResultat():
    f = open("../../data/etape.txt","r")
    lignes = f.readlines()
    
    parsage = []

    if (lignes[0] == "empty\n"):
        parsage.append(["empty"])
    else :
        for ligne in lignes :
            parsage_i = []
            chaine = ""
            colonne = 0;
            for j in range(len(ligne)):
                
                if (ligne[j]!='$'):
                    chaine+=ligne[j]
                    
                else :
                    if (colonne == 0):
                        ajout = int(chaine)
                        parsage_i.append(ajout)
                        colonne+=1
                    elif (colonne == 1):
                        parsage_i.append(chaine)
                        colonne+=1
                    else :
                        ajout = float(chaine)
                        parsage_i.append(ajout)
                        colonne+=1
                    chaine = ""
            parsage.append(parsage_i)

    return (parsage)

def parsageVoiture():
    f = open('../../data/voitures_export.csv','r')
    reader = csv.reader(f,delimiter=',',quotechar='"')
    tab = []
    for row in reader:
        tab.append(row)
    for i in range (len(tab)):
        tab_marque_modele = []
        marque = ""
        modele = ""
        find_marque = False
        for el in tab[i][0]:
            if (find_marque == False and el != " "):
                marque+=el
            elif (find_marque == False and el == " "):
                find_marque = True
            elif (el!='"'):
                modele += el
        tab_marque_modele.append(marque)
        tab_marque_modele.append(modele)
        tab[i][0] = tab_marque_modele

    dic = {}
    for i in range (len(tab)):
        if tab[i][0][0] not in dic:
            dic[tab[i][0][0]] = []
        dic[tab[i][0][0]].append((tab[i][0][1],int(tab[i][1]),int((tab[i][3]))))
    return dic
