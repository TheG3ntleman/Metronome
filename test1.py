import numpy as np
import matplotlib.pyplot as plt
from sklearn.neighbors import KernelDensity


def plot_kde(important_list, bandwidth=0.75, kernel="gaussian"):
    """
    Plots the kernel density estimation for a given list of integers.

    Parameters:
        important_list (list): List of integer values to perform KDE on.
        bandwidth (float): The bandwidth of the kernel. Default is 0.75.
        kernel (str): The kernel to use for KDE. Default is 'gaussian'.
                      Options include 'gaussian', 'tophat', 'epanechnikov',
                      'exponential', 'linear', 'cosine'.
    """
    # Convert the list to a numpy array and reshape for scikit-learn
    data = np.array(important_list).reshape(-1, 1)

    # Create and fit the KDE model
    kde = KernelDensity(kernel=kernel, bandwidth=bandwidth).fit(data)

    # Create a range of values for the x-axis
    x_d = np.linspace(min(important_list) - 1, max(important_list) + 1, 1000).reshape(
        -1, 1
    )

    # Evaluate the KDE on the range of values
    log_dens = kde.score_samples(x_d)

    # Plot the results
    plt.fill_between(x_d[:, 0], np.exp(log_dens), alpha=0.5)
    plt.plot(
        data[:, 0], -0.01 - 0.1 * np.random.rand(len(data)), "ro", label="Data points"
    )
    plt.title(f"Kernel Density Estimation with {kernel.capitalize()} Kernel")
    plt.xlabel("Value")
    plt.ylabel("Density")
    plt.legend()
    plt.show()


# Example usage
important_list = [1, 2, 2, 3, 3, 3, 4, 5, 5, 6, 7, 8, 8, 8, 9, 10]
plot_kde(important_list, bandwidth=0.75, kernel="epanechnikov")
