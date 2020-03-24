import numpy as np 
import matplotlib.pyplot as plt
import cv2, sys

if (len(sys.argv) < 2):
	print("command line argument should be image to open")
	exit(1)

img = sys.argv[1]
path = "./configs/vertex_coords/lines.txt"

img = cv2.imread(img)

with open(path, 'w'):
	pass


# plt.ion()

fig, ax = plt.subplots()
ax.plot(np.random.rand(10))

counter = 0

points = []

def onclick(event):
	global points, counter, colours
	x, y = event.xdata, event.ydata
	print('point -> xdata=%f, ydata=%f' %
		  (x, y))

	points.append([x, y])
	counter += 1

	x = [i[0] for i in points]
	y = [i[1] for i in points]

	if counter % 2 == 0:
		with open(path, 'a') as f:
			f.write(str(points)[1:-1] + "\n")
		plt.plot(x, y, '--o')
		points = []

	plt.scatter(x, y, s=100, c='r')
	plt.draw()

cid = fig.canvas.mpl_connect('button_press_event', onclick)

ax.imshow(img)
plt.show()
