from matplotlib import pyplot

with open('C:/points.txt', 'r') as file:
    lines = file.readlines()
    file.close()
x_axis = []
y_axis = []

for line in lines:
    coords = line.rstrip("\n").split(" ")
    x_axis.append(float(coords[0]))
    y_axis.append(float(coords[1]))

pyplot.figure("Imitation model plot")
pyplot.xlabel("X axis")
pyplot.ylabel("Y axis")
pyplot.title("Imitation model plot")
pyplot.plot(x_axis, y_axis, color='r')
pyplot.show()
