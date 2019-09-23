import pandas as pd
import matplotlib.pyplot as plt

# Load data
data = pd.read_csv("particle_shapes.csv")

#print(data.head())
data = data.drop(["particle_number", "contour_number"], axis = 1)
data = data[data["elongation"] <= 100]
print(data.head())
data.corr(method = "pearson")
corr_plot = pd.plotting.scatter_matrix(data, figsize=(10,10))
plt.savefig("correlation_plot.png")

plt.show()

fig, ax = plt.subplots(figsize=(20,10))

ax.matshow(data.corr())
ax.set_xticks(range(0, len(data.columns) - 1))  # Set locations and labels
ax.set_xticklabels(data.columns)
ax.set_yticks(range(0, len(data.columns) - 1))  # Set locations and labels
ax.set_yticklabels(data.columns)
plt.show()
