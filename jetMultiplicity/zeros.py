from __future__ import division
import csv

all_particles = "../PFCandidate.csv"
zeros_events = "zeros_events.csv"
# zeros_events = "nonzeros_events.csv"


zeros_events_list = []
with open(zeros_events, 'rb') as csvfile:
	zeros = csv.reader(csvfile, delimiter=',')
	for row in zeros:
		zeros_events_list.extend(row)

particles_list = []
total_particles = 0
with open(all_particles, 'rb') as csvfile:
	events = csv.reader(csvfile, delimiter=',')
	for row in events:
		if row[1] in zeros_events_list:
			particles_list.append(row)
			total_particles += 1

particle_type = {'1': 0.0, '2': 0.0, '3': 0.0, '4': 0.0, '5': 0.0, '6': 0.0, '7': 0.0}

with open('zeros_particles.csv', 'wb') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',')
    for particle in particles_list:
    	particle_type[particle[9]] += float(1);
    	spamwriter.writerow(particle)

for key in particle_type:
	particle_type[key] = int((particle_type[key] / total_particles) * 100)


print particle_type
