## Final Exam in Algorithms and Data Structures 2022-2023
Consider a highway described as a sequence of service stations. Each service station is located at a distance from the beginning of the highway expressed in kilometers by a positive integer. No two service stations are at the same distance, and each station is uniquely identified by its distance from the beginning of the highway.

_Example highway: [3, 15, 430, 1400]_

Each service station is equipped with a rental electric vehicle park (no more than 512). Each vehicle is characterized by the autonomy of a fully charged battery, expressed in kilometers, as an integer value. It is possible to rent a car at a station, and it is possible to reach any other station within a distance less than or equal to the autonomy of the car.

_Example station: [distance = 3km, vehicle park = 100km, 100km, 100km, 50km]_

A trip is defined as a sequence of service stations, with the driver stopping at each intermediate station and picking up one of the available vechicles of the park (car switching is forced). A station `ns` is reachable iif the autonomy of the car `autonomy >= ns - cs`, where `cs` is the current station.

The project objective is as follows: given a pair of stations, plan the route with the fewest stops between them. In case multiple routes have the same minimum number of stops, the route with the shortest first stops (i.e. route `a = {a_1, a_2, ..., a_n}` should be chosen over route `b = {b_1, a_2, ..., a_n}` if `a_1 < b_1`).


## Input Files and Expected Output
The input file consists of a sequence of commands, each on a new line, with the following format. All integer values are signed 32-bit numbers.

* #### aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n 
  Adds a station at the specified distance, with a vehicle park containing vehicles with the given autonomies.

  Example:
  aggiungi-stazione 10 3 100 200 300
  Adds a station at distance 10km from the start of the highway, with a vehicle park consisting of vehicles with 100 km, 200 km, and 300 km autonomies.

  Expected output: aggiunta or non aggiunta.

* #### demolisci-stazione distanza
  Removes the station at the given distance, if it exists.

  Expected output: demolita or non demolita.

* #### aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere
  If the station exists, adds a vehicle with the specified autonomy to the station. It is possible to have multiple cars with the same autonomy.

  Expected output: aggiunta or non aggiunta.

* #### rottama-auto distanza-stazione autonomia-auto-da-rottamare
  Removes a car with the specified autonomy from the station at the given distance, if the station has at least one car with that autonomy.

  Expected output: rottamata or non rottamata.

* #### pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo
   Asks for the route plan between the start and end stations.

  Expected output: The stops along the route, separated by spaces, followed by a new line. If no route exists, print nothing. If the start or end stations are the same, print the station itself.


Sample input
```
aggiungi-stazione 91 9 5 4 5 5 4 6 5 4 5
aggiungi-stazione 24 6 5 4 5 4 4 5
pianifica-percorso 24 91
aggiungi-stazione 92 6 5 4 4 5 5 5
demolisci-stazione 59
aggiungi-stazione 52 9 13 14 13 17 17 15 18 15 16
aggiungi-auto 91 39
aggiungi-auto 52 39
aggiungi-auto 24 28
pianifica-percorso 24 92
```

Sample output
```
aggiunta
aggiunta
nessun percorso
aggiunta
non demolita
aggiunta
aggiunta
aggiunta
aggiunta
24 52 91 92
```
