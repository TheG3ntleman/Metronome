import numpy as np
import matplotlib.pyplot as plt
from sklearn.neighbors import KernelDensity

# Sample data: replace this with your own list of integers
important_list = [1, 2, 2, 3, 3, 3, 4, 5, 5, 6, 7, 8, 8, 8, 9, 10]

# Convert the list to a numpy array and reshape for scikit-learn
data = np.array(important_list).reshape(-1, 1)

# Create and fit the KDE model
kde = KernelDensity(kernel="gaussian", bandwidth=0.75).fit(data)

# Create a range of values for the x-axis
x_d = np.linspace(min(important_list) - 1, max(important_list) + 1, 1000).reshape(-1, 1)

# Evaluate the KDE on the range of values
log_dens = kde.score_samples(x_d)

# Plot the results
plt.fill_between(x_d[:, 0], np.exp(log_dens), alpha=0.5)
plt.plot(data[:, 0], -0.01 - 0.1 * np.random.rand(len(data)), "ro", label="Data points")
plt.title("Kernel Density Estimation")
plt.xlabel("Value")
plt.ylabel("Density")
plt.legend()
plt.show()
