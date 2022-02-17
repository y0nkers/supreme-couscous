import matplotlib.pyplot as plt

x = []
y = []
rec_w, rec_h, circle_radius = 0, 0, 0
half_width, half_height = 0, 0
fig_coords = [[], []] # left up, right down (X,Y)

with open('C:/montecarlo.txt', 'r') as file:
    for i, line in enumerate(file):
        if i == 0:
            cfg = line.rstrip("\n").split(" ")
            rec_w, rec_h, circle_radius = float(cfg[0]), float(cfg[1]), float(cfg[2])
            half_width, half_height = rec_w / 2, rec_h / 2
        elif i > 0:
            coords = line.rstrip("\n").split(" ")
            x.append(float(coords[0]))
            y.append(float(coords[1]))
    file.close()


if circle_radius <= half_width and circle_radius <= half_height:    # if circle inside rectangle
    fig_coords = [[-half_width, half_height], [half_width, -half_height]]
elif circle_radius > half_width and circle_radius > half_height:    # if rectangle inside circle
    fig_coords = [[-circle_radius, circle_radius], [circle_radius, -circle_radius]]
elif half_width > circle_radius and half_height < circle_radius:    # if the rectangle is wider than the circle, but lower than it
    fig_coords = [[-half_width, circle_radius], [half_width, -circle_radius]]
elif half_width < circle_radius and half_height > circle_radius:    # if the rectangle is taller than the circle, but narrower than it
    fig_coords = [[-circle_radius, half_height], [circle_radius, -half_height]]

fig, ax = plt.subplots()
plt.scatter(x, y, color = 'k', marker = '.', s = [1.0]*len(x))
circle1 = plt.Circle((0, 0), circle_radius, color='r', fill = False)
ax.add_patch(circle1)
ax.margins(0.2, 0.2)

plt.plot([-rec_w/2, rec_w/2], [rec_h/2, rec_h/2], [-rec_w/2, rec_w/2], [-rec_h/2, -rec_h/2], [-rec_w/2, -rec_w/2], [rec_h/2, -rec_h/2], [rec_w/2, rec_w/2], [rec_h/2, -rec_h/2], color = 'r')
plt.plot([fig_coords[0][0], fig_coords[1][0]], [fig_coords[0][1], fig_coords[0][1]], color = 'g')
plt.plot([fig_coords[0][0], fig_coords[1][0]], [fig_coords[1][1], fig_coords[1][1]], color = 'g')
plt.plot([fig_coords[0][0], fig_coords[0][0]], [fig_coords[0][1], fig_coords[1][1]], color = 'g')
plt.plot([fig_coords[1][0], fig_coords[1][0]], [fig_coords[0][1], fig_coords[1][1]], color = 'g')

manager = plt.get_current_fig_manager()
manager.set_window_title('Monte-Carlo visualization')
plt.show()