import xml.dom.minidom
import sys
import time

start = time.time()
input_file = sys.argv[1]
output_file = sys.argv[2]
with open(input_file, 'r') as in_file:
    doc = xml.dom.minidom.parse(in_file)

nets = doc.getElementsByTagName("net")
ids = []
for net in nets:
    ids.append(int(net.getAttribute('id')))

num_of_vertices = ids[-1]
distances = []

for i in range(0, num_of_vertices):
    distances.append([float('Inf')] * num_of_vertices)
    distances[i][i] = 0

electrical_elements = ['resistor', 'capactor', 'diode']
for element in electrical_elements:
    edges = doc.getElementsByTagName(element)
    for edge in edges:
        startv = int(edge.getAttribute('net_from')) - 1
        endv = int(edge.getAttribute('net_to')) - 1
        rev_resistance = resistance = float(edge.getAttribute('resistance'))
        if element == 'diode':
            rev_resistance = float(edge.getAttribute('reverse_resistance'))
        distances[startv][endv] = 1 / (1 / distances[startv][endv] +
                                       1 / resistance)
        distances[endv][startv] = 1 / (1 / distances[endv][startv] +
                                       1 / rev_resistance)

for k in range(0, num_of_vertices):
    for i in range(0, num_of_vertices):
        for j in range(0, num_of_vertices):
            if i != j:
                try:
                    first_resistance = 1 / (distances[k][j] + distances[i][k])
                except ZeroDivisionError:
                    first_resistance = float('Inf')
                try:
                    second_resistance = 1 / distances[i][j]
                except ZeroDivisionError:
                    second_resistance = float('Inf')
                try:
                    distances[i][j] = 1 / (first_resistance
                                           + second_resistance)
                except ZeroDivisionError:
                    distances[i][j] = float('Inf')

ans_file = open(output_file, 'w')
for row in distances:
    for column in row:
        ans_file.write("{:25.6f}, ".format(column))
    ans_file.write("\n")

in_file.close()
ans_file.close()
finish = time.time()
print(finish - start)
