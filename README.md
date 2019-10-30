#Second attempt at N-body simulation
import math
import matplotlib.pyplot as plot
from mpl_toolkits.mplot3d import Axes3D
import random
import time
import json
import os.path

G = 6.67408e-11 #m3 kg-1 s-2
outfile = None

class Body:
	body_count = 0

	body_list = []

	def __init__ (self, name, mass, x, y, z, v_x, v_y, v_z):

		self.name = name
		self.mass = mass
		self.x = x
		self.y = y
		self.z = z
		self.v_x = v_x
		self.v_y = v_y
		self.v_z = v_z

		self.loc_hist = {"x":[], "y":[], "z":[], "name":self.name}

		self.id = Body.body_count
		Body.body_list.append(self)

		Body.body_count += 1

		print("Object ", self.name, "created with parameters (x,y,z,v_x,v_y,v_z): ", self.x,self.y,self.z,self.v_x,self.v_y,self.v_z)

	def info(self):
		print(self.id, ",", self.name, ",", self.x)

	def set_accelaration(self, step):
		k1_x = k1_y = k1_z = k2_x = k2_y = k2_z = k3_x = k3_y = k3_z = k4_x = k4_y = k4_z = 0

		for body in Body.body_list:
			if body.id != self.id:
				#print(body.name, body.mass, self.x-body.x, self.y-body.y, self.z-body.z)
				r = math.sqrt((self.x-body.x)**2+(self.y-body.y)**2+(self.z-body.z)**2)
				#print(r)
				f_r = -G*body.mass/r**3
				#print(f_r)
				k1_x += f_r*(self.x-body.x)
				k1_y += f_r*(self.y-body.y)
				k1_z += f_r*(self.z-body.z)
			
		tmp_x = self.x+(self.v_x+k1_x*step*0.5)*step*0.5
		tmp_y = self.y+(self.v_y+k1_x*step*0.5)*step*0.5
		tmp_z = self.z+(self.v_z+k1_x*step*0.5)*step*0.5

		for body in Body.body_list:
			if body.id != self.id:
				r = math.sqrt((tmp_x-body.x)**2+(tmp_y-body.y)**2+(tmp_z-body.z)**2)
				f_r = -G*body.mass/r**3
				k2_x += f_r*(tmp_x-body.x)
				k2_y += f_r*(tmp_y-body.y)
				k2_z += f_r*(tmp_z-body.z)

		tmp_x = self.x+(self.v_x+k2_x*step*0.5)*step*0.5
		tmp_y = self.y+(self.v_y+k2_x*step*0.5)*step*0.5
		tmp_z = self.z+(self.v_z+k2_x*step*0.5)*step*0.5

		for body in Body.body_list:
			if body.id != self.id:
				r = math.sqrt((tmp_x-body.x)**2+(tmp_y-body.y)**2+(tmp_z-body.z)**2)
				f_r = -G*body.mass/r**3
				k3_x += f_r*(tmp_x-body.x)
				k3_y += f_r*(tmp_y-body.y)
				k3_z += f_r*(tmp_z-body.z)

		tmp_x = self.x+(self.v_x+k2_x*step)*step
		tmp_y = self.y+(self.v_y+k2_x*step)*step
		tmp_z = self.z+(self.v_z+k2_x*step)*step

		for body in Body.body_list:
			if body.id != self.id:
				r = math.sqrt((tmp_x-body.x)**2+(tmp_y-body.y)**2+(tmp_z-body.z)**2)
				f_r = -G*body.mass/r**3
				k4_x += f_r*(tmp_x-body.x)
				k4_y += f_r*(tmp_y-body.y)
				k4_z += f_r*(tmp_z-body.z)

		self.a_x = (k1_x+2*k2_x+2*k3_x+k4_x)/6
		self.a_y = (k1_y+2*k2_y+2*k3_y+k4_y)/6
		self.a_z = (k1_z+2*k2_z+2*k3_z+k4_z)/6
		#print("a = ",self.a_x, self.a_y, self.a_z)

		###Repeat this whole process for velocity and then position dick head
		###We're not going it thatway first

	def set_velocity(self, step):
		self.v_x += self.a_x*step
		self.v_y += self.a_y*step
		self.v_z += self.a_z*step

	def update_location(self, disp_freq, time, step, output):
		if time % disp_freq and output == True:
			self.loc_hist['x'].append(self.x)
			self.loc_hist['y'].append(self.y)
			self.loc_hist['z'].append(self.z)

		self.x += self.v_x*step
		self.y += self.v_y*step
		self.z += self.v_z*step

	@classmethod
	def step_all(cls, time_step, display_frequency, time, output, save, savefile=None):
		for body in Body.body_list:
			body.set_accelaration(time_step)

		for body in Body.body_list:
			body.set_velocity(time_step)

		for body in Body.body_list:
			body.update_location(display_frequency, time, time_step, output)

		if time % display_frequency:
			#print(time)
			if (save == True) and (savefile != None):
				with open(savefile, "a") as file:
					sv_string = dict()#time : {name_1 : [x_1, y_1, z_1, v_x_1, v_y_1, v_z_1]...}
					for body in Body.body_list:
						body_string = [body.x, body.y, body.z, body.v_x, body.v_y, body.v_z]
						sv_string[body.name] = body_string
					file_string = str(time)+":"+str(json.dumps(sv_string))+","
					file.write(str(file_string))

	@classmethod
	def output_all(cls, display_bodies=[]):
		fig = plot.figure()
		colours = ['r','b','g','y','m','c']
		ax = fig.add_subplot(1,1,1, projection='3d')
		max_range = 0
		if display_bodies == []:
			for body in Body.body_list:
				display_bodies.append(body.name)
		for body in Body.body_list:
			if body.name in display_bodies:
				max_dim = max(max(body.loc_hist['x']),max(body.loc_hist['y']),max(body.loc_hist['z']))
				if max_dim > max_range:
					max_range = max_dim
				ax.plot(body.loc_hist["x"], body.loc_hist["y"], body.loc_hist["z"], c = random.choice(colours), label = body.loc_hist["name"])	  
		
		ax.set_xlim([-max_range,max_range])	
		ax.set_ylim([-max_range,max_range])
		ax.set_zlim([-max_range,max_range])
		ax.legend()		

		if outfile:
			plot.savefig(outfile)
		else:
			plot.show()

	@classmethod
	def simulate(self, start_time, end_time, time_step, display_frequency, output, save, savefile=None):
		if save == False and output == False:
			input("You have chosen not to output or save results, therefore this excersise is futile, press enter to continue anyway or ctl-c to exit")
		if save == True and savefile == None:
			savefile = "n_body_sim_"+str(time.time())+".txt"
			print("No save file name found, falling back on: ", savefile)
		if save == True and savefile != None:
			if os.path.isfile(savefile) != True:
				with open(savefile, "w+") as file:
					file.write("{")
					file.close()
			with open(savefile, "a") as file:
				sv_string = dict()#time : {name_1 : [x_1, y_1, z_1, v_x_1, v_y_1, v_z_1]...}
				for body in Body.body_list:
					body_string = [body.x, body.y, body.z, body.v_x, body.v_y, body.v_z]
					sv_string[body.name] = body_string
				file_string = str(time)+":"+str(json.dumps(sv_string))+","
				file.write(str(file_string))

		time = start_time
		while time<=end_time:
			Body.step_all(time_step, display_frequency, time, output, save, savefile)
			time+=time_step

	@classmethod
	def save_output(self, output_file):
		output_all = dict()
		for body in Body.body_list:
			output_all[body.name] = body.loc_hist
		with open(output_file, 'w') as file:
			file.write(json.dumps(output_all))

"""
#Solar System
sun = Body("Sun", 1.988544e30, 1.288434560262645E+08, 1.016672287106661E+09, -1.460513324984163E+07, -1.192685083932455E1, 6.859058889648139E0, 2.877423917810483E-01)
earth = Body("Earth", 5.97219e24, -5.826290294858550E+10, -1.388957265033708E+11, -8.789464758947492E+06, 2.698413775232139E+04, -1.157192512486361E+04, 1.063440919711667e0)
jupiter = Body("Jupiter", 1898.13e24, -5.234601683295876E+11, -6.150114990481849E+11, 1.425959347740144E+10, 9.795174437658890E+03, -7.847564273931063E+03, -1.865625486674438E+02)
saturn = Body("Saturn", 5.68319e26, 1.237471336197857E+11, -1.499529588683352E+12, 2.114685620792848E+10, 9.095483273075599E+03, 7.622217983921383E+02, -3.746814320723464E+02)
bz509 = Body("2015 BZ509", 2e21, 3.288856909489848E+11, -7.131247506872107E+11, 5.266674999617246E+10, -8.395526864006353E+03, -8.948698870356200E+03, 3.699155000513569E+03)
mercury = Body("Mercury", 3.302e23, 4.404001144314776E+10, 2.291717502741127E+10, -2.253423443392134E+09, -3.124957703361783E+04, 4.574963141665005E+04, 6.603804717389332E+03)
venus = Body("Venus", 48.685e23, -9.513826367793775E+10, 5.048489310007670E+10, 6.161517555643119E+9, -1.630178490436402E+04, -3.124178461183138E+04, 5.116659176684930E+02)
mars = Body("Mars", 6.4185e23, -5.097767434300846E+09, -2.170396002247052E+11, -4.455258719224483E+09, 2.512404836677673E+04, 1.506990273399958E+03, -5.851091980191134E+02)
uranus = Body("Uranus", 86.8103e24, 2.609995358717298E+12, 1.428710528616270E+12, -2.850660505426764E+10, -3.319798468342193E+03, 5.656094133145377E+03, 6.384219014497239E+01)
neptune = Body("Neptune", 102.41e24, 4.309672713800700E+12, -1.218149099014301E+12, -7.423532203289109E+10, 1.442081599148088E+03, 5.261938296258387E+03, -1.421511260513890E+02)
#pluto = Body("Pluto", 1.307e22, 1.679550630732988E+12, -4.731985971715788E+12, 2.052422874419022E+10, 5.223801822733935E+03, 6.689163424009280E+02, -1.571316960072888E+03)
#charon = Body("Charon", 1.53e21, 1.679556810104384E+12, -4.731971764940992E+12, 2.053622660107946E+10, 5.371926975020155E+03, 7.341706209326483E+02, -1.724917125209133E+03)
starman = Body("SpaceX Starman", 5000, -1.047764775387199E+11, -1.653487721150364E+11, -3.649541062776364E+09, 1.775425980936795E+04, -1.907492228107692E+04, -3.517435988779738E+01)
asteroid = Body("Made up asteroid", 2e21, 1.375E+11, 0, 0, 0, 40228,0)
to_show = [sun.name, earth.name, mars.name, starman.name, mercury.name, venus.name, jupiter.name, bz509.name, asteroid.name]


Body.simulate(0, 378691200, 1000, 10000)

Body.output_all(to_show)"""