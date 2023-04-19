# import csv
# import re
# import urllib.request


# #extract the html
# u2 = urllib.request.urlopen('https://ev-database.org/cheatsheet/range-electric-car')
# html = u2.read().decode('utf-8')


# #extract the names of the cars
# data = re.findall(r'(<a href.*target="_blank">)(.*?)(</a>)', html)
# data_list = []
# for d in data:
#     data_list.append(d[1])


# #extract the range of the cars
# data2 = re.findall(r'(<span class="bar" data-avgClass="Range_E_Real">)(.*?)(</span>)', html)
# data_list2 = []
# for d in data2:
#     data_list2.append(d[1])


# #create dictionary
# data_dict = {}
# for i in range(len(data_list)):
#     data_dict[data_list[i]] = int(data_list2[i])


# #to csv
# with open('data.csv', 'w') as f:
#     writer = csv.writer(f)
#     for key, value in data_dict.items():
#         writer.writerow([key, value])




# import csv
# import re
# import urllib.request


# #extract the html
# u2 = urllib.request.urlopen('https://ev-database.org/cheatsheet/useable-battery-capacity-electric-car')
# html = u2.read().decode('utf-8')

# #extract the names of the cars
# data = re.findall(r'(<a href.*target="_blank">)(.*?)(</a>)', html)
# data_list = []
# for d in data:
#     data_list.append(d[1])


# #extract the range of the cars
# data2 = re.findall(r'(<span class="bar" data-avgClass="Battery_kWh">)(.*?)(</span>)', html)
# data_list2 = []
# for d in data2:
#     data_list2.append(d[1])


# #create dictionary
# data_dict = {}
# for i in range(len(data_list)):
#     value = float(data_list2[i])
#     data_dict[data_list[i]] = int(value*1000)


# #to csv
# with open('data_power_vehicles.csv', 'w') as f:
#     writer = csv.writer(f)
#     for key, value in data_dict.items():
#         writer.writerow([key, value])



#imporing the data to the database

# import sqlite3
# import csv

# db = sqlite3.connect('database_puissances_nominales_copy.db')
# cursor = db.cursor()


# # with open('data_power_vehicles.csv', 'r') as f:
# #     reader = csv.reader(f)
# #     for row in reader:
# #         cursor.execute('INSERT INTO vehicules (nom_vehicule,puissance_batterie_Wh) VALUES (?, ?)', (row[0], row[1]))
# #         db.commit()

# with open('data_range_vehicles.csv', 'r') as f:
#     reader = csv.reader(f)
#     for row in reader:
#         cursor.execute("select id from vehicules where nom_vehicule = ?", (row[0],))
#         id_vehicule = cursor.fetchall()
#         if len(id_vehicule) == 0:
#             cursor.execute('INSERT INTO vehicules (nom_vehicule,autonomie_km) VALUES (?, ?)', (row[0], row[1]))
#             db.commit()
#         else:
#             cursor.execute('UPDATE vehicules SET autonomie_km = ? WHERE id = ?', (row[1], id_vehicule[0][0]))
#             db.commit()