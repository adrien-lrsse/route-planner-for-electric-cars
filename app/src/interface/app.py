from flask import Flask, render_template, request,redirect,session
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
    
    

    return render_template("index.html",resultat = parseResultat())

@app.route("/find_itinerary",methods=["GET","POST"])
def find_itinerary():
    if request.method == "POST":
        depart = request.form.get("depart")
        arrivee = request.form.get("arrivee")
        execute_app(depart,arrivee)
    return redirect('/')


    

def execute_app(depart,arrivee):
    geolocator = Nominatim(user_agent="itinerary")
    depart = geolocator.geocode(depart)
    arrivee = geolocator.geocode(arrivee)
    script_dir = os.path.dirname(os.path.abspath(__file__))
    cwd = os.path.abspath(os.path.join(script_dir, '..'))
    subprocess.run(["./main",str(depart.longitude),str(depart.latitude),str(arrivee.longitude),str(arrivee.latitude)],cwd=cwd)



def parseResultat():
    f = open("../../data/etape.txt","r")
    lignes = f.readlines()

    parsage = []

    for ligne in lignes :
        parsage_i = []
        chaine = ""
        for j in range(len(ligne)):
            
            if (ligne[j]!='$'):
                chaine+=ligne[j]
                
            else :
                parsage_i.append(chaine)
                chaine = ""
        parsage.append(parsage_i)

    return (parsage)