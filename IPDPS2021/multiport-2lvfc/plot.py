import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

data = pd.read_csv("result", names=["topo", "time", "datasize"])
data=data.values()

datasize = np.unique(data[2])
topo = np.unique(data[0])
for d in datasize:
    for t in topo:
        for da in data:
            if 

# x-coordinates of left sides of bar
 
# labels for bars
tick_label = ['one', 'two', 'three', 'four', 'five']
 
# plotting a bar chart
plt.bar(left, height, tick_label = tick_label,
        width = 0.4, color = ['red', 'green'])
 
# naming the x-axis
plt.xlabel('x - axis')
# naming the y-axis
plt.ylabel('y - axis')
# plot title
plt.title('My bar chart!')
 
# function to show the plot
plt.show()