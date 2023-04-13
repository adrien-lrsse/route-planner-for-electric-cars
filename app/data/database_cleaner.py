
# Premier script python pour nettoyer la base de données de ses doublons

# import sqlite3

# # connect to database
# db = sqlite3.connect("database.db")
# cur = db.cursor()

# #for each element of the database, erase all sames entries
# cur.execute("select id_unique,consolidated_longitude, consolidated_latitude from bornes")
# donnees = cur.fetchall()

# for element in donnees:
#     id_unique = element[0]
#     longitude = element[1]
#     latitude = element[2]
#     cur.execute("select id_unique,consolidated_longitude, consolidated_latitude from bornes where consolidated_longitude=? and consolidated_latitude=?",(longitude,latitude))
#     donnees2 = cur.fetchall()
#     if len(donnees2)>1:
#         print("élément numéro",id_unique,"a",len(donnees2),"doublons")
#         for element2 in donnees2:
#             id_unique2 = element2[0]
#             if id_unique2!=id_unique:
#                 print("j'ai trouvé un doublon numéro",id_unique2,"qui est le même que",id_unique)
#                 cur.execute("delete from bornes where id_unique=?",(id_unique2,))
#                 db.commit()

# Deuxième script python pour ajouter le nbr de points de recharge à chaque endroit

# import sqlite3

# # connect to database
# db = sqlite3.connect("database_clean.db")
# db_originale = sqlite3.connect("database_originale.db")
# cur = db.cursor()
# cur_org = db_originale.cursor()

# cur.execute("select consolidated_longitude, consolidated_latitude from bornes")
# data = cur.fetchall()

# for element in data:
#     longitude = element[0]
#     latitude = element[1]
#     cur_org.execute("select count(*) from bornes where consolidated_longitude=? and consolidated_latitude=?",(longitude,latitude))
#     data2 = cur_org.fetchall()
#     nbr_points = data2[0][0]
#     #print("il y a",nbr_points,"points de recharge à",longitude,latitude)
#     cur.execute("update bornes set nbr_pt_recharge=? where consolidated_longitude=? and consolidated_latitude=?",(nbr_points,longitude,latitude))
#     db.commit()

# Troisième script pour vérifier que la base de données est bien nettoyée et que l'on n'a pas perdu d'informations

import sqlite3

# connect to database
db = sqlite3.connect("database_clean.db")
cur = db.cursor()

cur.execute("select sum(nbr_pt_recharge) from bornes")
data = cur.fetchall()
print("il y a",data[0][0],"points de recharge dans la base de données (normalement il y en a 55320)")