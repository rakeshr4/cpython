import matplotlib.pyplot as plt
import numpy as np

ind = []
gfllist = []
glllist = []
sumVal = 0
with open('/Users/akhilasankar/Downloads/set_get_data.csv', 'r') as f:
	for line in f:
		x = int(line.split(',')[0])
		gll = float(line.split(',')[1])
		gfl = float(line.split(',')[2].strip())
		ind.append(x)
		glllist.append(gll)	
		gfllist.append(gfl)
		print glllist
		print gfllist


plt.ylabel('Time averaged over 50 iterations')
plt.plot(ind, gfllist, 'b', ind, glllist, 'r', linestyle='-', linewidth=.5)
plt.xlabel('Num of objects')
plt.title('set and get')
plt.show()