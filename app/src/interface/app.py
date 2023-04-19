from flask import Flask, render_template, request,redirect,session, url_for
from flask_session import Session

import os
import subprocess
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
    return render_template("index.html",resultat = parseResultat(),status=status)

@app.route("/find_itinerary",methods=["GET","POST"])
def find_itinerary():
    if request.method == "POST":
        depart = request.form.get("depart")
        arrivee = request.form.get("arrivee")
        error_status = execute_app(depart,arrivee)
    return redirect('/')


    

def execute_app(depart,arrivee):
    status = ""
    geolocator = Nominatim(user_agent="itine    rary")
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
        subprocess.run(["./main",str(depart.longitude),str(depart.latitude),str(arrivee.longitude),str(arrivee.latitude),"100.0"],cwd=cwd)
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