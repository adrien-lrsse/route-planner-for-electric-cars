# Itinéraire de trajet en voiture électrique et simulation du réseau électrique

## Installation des dépendances

- Librairie `SQLite3` pour C :

```bash
sudo apt-get install libsqlite3-dev
```

- Dépendances `python` :

Une fois placé dans le répertoire `/app`

```bash
pip install - r requirements.txt
```

## Lancement du projet

Une fois placé dans le répertoire `/app`

```bash
make run
```

- Lancement en mode debug
```bash
make run_debug
```

Une fois le projet lancé, ouvrez votre navigateur est tapé l'adresse
```bash
http://127.0.0.1:5000
``` 
Une fois ouvert, le projet est utilisable sous la forme d'une application Web.

## Utilisation de l'application
### `/` : Utilisation du calcul d'itinéraire
Pour lancer une recherche : 
- adresse de départ
- adresse d'arrivée

Vous pouvez choisir vos options : 
- Une voiture (Marque & Modèle)
- Le type d'optimisation (distance ou temps)
- l'autonomie initiale du véhicule
- le seuil minimal de décharge
- la limite de temps de recharge à la borne

<div style="text-align:center">
<img src="app/data/readme/input_1.png" alt="Texte alternatif" width="100%" height="auto" >
<br>
<i>1. Exemple de recherche d'itinéraire</i>
</div>

Une fois appuyer sur le bouton recherche, soit un message d'erreur apparait car la recherche à échouer, soit une carte avec des points qui indique les bornes de recharge

<div style="text-align:center">
<img src="app/data/readme/res_1.png" alt="Texte alternatif" width="100%" height="auto" >
<br>
<i>2. Exemple d'affichage de requête</i>
</div>

### `/simulation` : Utilisation du module de simulation
En cliquant sur le bouton "Mode simulation" ou en allant sur le route `/simulation`.
Pour lancer une simulation, il faut entrée un entier et appuyer sur générer. 
Le nombre de trajet généré n'est pas exactement celui entrée par l'utilisateur ce qui est normal. Cela vient de la génération aléatoire des trajets.

Après un peu d'attente, vous pouvez naviguez sur la carte en appuyant sur les boutons :
- +10 min
- -10 min

Les bornes occupées apparaîtront. En orange sont représentés les bornes occupées sans file d'attente et en rouge les bornes occupées avec file d'attente.

<div style="text-align:center">
<img src="app/data/readme/output_3.png" alt="Texte alternatif" width="100%" height="auto" >
<br>
<i>3. Exemple d'affichage en cas de succès</i>
</div>

Remarque : On peut naviguez sur 720 ticks mais cela n'implique pas que le dernier trajet ce termine à la 720ème tick.

## Fermeture de l'application
Pour fermer l'application, il faut tuer le processus sur le terminal courant.

## Contexte :

Projet réalisé dans le cadre de la 1ère année d'étude à TELECOM Nancy de mars à mai 2023

## Language :

- Programmation en `c`
- Serveur flask en `Python`
- Interface en `HTML`, `CSS` et `Javascript`

## Ressources :

- Serveur Etalab : https://www.data.gouv.fr/fr/datasets/fichier-consolide-des-bornes-de-recharge-pour-vehicules-electriques/
- Electric Vehicle Database : https://ev-database.org/cheatsheet/range-electric-car
- Modèle de carte : https://cloud.maptiler.com/ basé sur OpenLayers
- Base des communes de France : https://www.data.gouv.fr/fr/datasets/communes-de-france-base-des-codes-postaux/
## Membres du groupe :

- Aurélien GINDRE : aurelien.gindre@telecomnancy.eu
- Héléna BARBILLON : helena.barbillon@telecomnancy.eu
- Mathis Mangold : mathis.mangold@telecomnancy.eu
- Adrien LAROUSSE : adrien.larousse@telecomnancy.eu

## Propriétaire du projet :

TELECOM Nancy : https://telecomnancy.univ-lorraine.fr/
