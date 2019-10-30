"""Starlink example
This example serves little purpose except to show that this is:
a) a very slow method of calculation and that this should be written in a more efficent language,
b) n body simulation is redundant for such small objects,
and c) there's a lot of stuff in space"""
from mainV0_old import *

Body("Earth", 5.97219e24, 0,0,0,0,0,0)
Body("Moon", 734.9e20, 170266429.305, -312650550.581, 28769968.167, 966.524, 46.919, 32.350)

with open("starlink_state_vectors.csv","r") as f:
	starlink_data = f.read().split("\n")[1:]

count = 0
for sat in starlink_data:
	sat = sat.split(",")
	#According to - https://www.spacex.com/sites/spacex/files/starlink_press_kit.pdf - mass is 227kg
	if count < 100:
		Body(sat[0],227,float(sat[3]),float(sat[4]),float(sat[5]),float(sat[6]),float(sat[7]),float(sat[8]))
	count += 1
input()
Body.simulate(0, 10000, 0.1, 10, True, False)

Body.output_all()