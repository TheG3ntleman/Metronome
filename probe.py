from src.problems.morpian_solitaire import MorpionSolitaire5D
from src.primitives.state_space_tree import StateSpaceTreeNode
from src.probes.LegacyProbe import LegacyProbe
from src.primitives.probe import Probe
# We now see the evolution of the mean, variance, skewness and kurtosis of the rewards of each child node

# Setting up the problem
morpion_solitaire = MorpionSolitaire5D(20)

# Setting up the state space tree
# root = StateSpaceTreeNode(None)

# # We perform an expansion to get the children
# valid_moves = morpion_solitaire.get_valid_moves()
# for move in valid_moves:
#     child = StateSpaceTreeNode(move, root)
#     child.properties["visits"] = 0
#     child.properties["reward"] = 0
#     root.add_child(child)

# # We now probe the state space tree
# dataframes = LegacyProbe.probe(root, morpion_solitaire, 1000)
# for dataframe in dataframes:
#     print(dataframe)
#     print("\n\n")


probe_instance = Probe(morpion_solitaire, 100)
dataframe = probe_instance.probe()
stats = probe_instance.compute_statistics() # This will compute the statistics for the probe
print("Statistics: ", stats)
probe_instance.scatter_plot_rewards()
# probes = probe_instance.get_distributions_for_each_sample(None)
# for i in range(len(probes)):
#     print('Length of probe: ', len(probes[i]))
    
print("Showing the final distributions")
probe_instance.show_final_distributions()