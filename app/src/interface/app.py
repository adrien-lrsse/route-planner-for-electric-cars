import os
import subprocess
from geopy.geocoders import Nominatim



geolocator = Nominatim(user_agent="itinerary")



depart = "Marseille"
location = geolocator.geocode(depart)
print((location.latitude,location.longitude))

arrivee = "Nancy"
location_arrivee = geolocator.geocode(arrivee)
print((location_arrivee.latitude,location_arrivee.longitude))

script_dir = os.path.dirname(os.path.abspath("./main"))
subprocess.run(["./main",str(location.longitude),str(location.latitude),str(location_arrivee.longitude),str(location_arrivee.latitude)], cwd=script_dir)

f = open("./../data/etape.txt","r")
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

print(parsage)