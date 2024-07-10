from src.problems.morpian_solitaire import MorpionSolitaire5D
from src.primitives.state_space_tree import StateSpaceTreeNode
from src.probes.LegacyProbe import LegacyProbe

# We now see the evolution of the mean, variance, skewness and kurtosis of the rewards of each child node

# Setting up the problem
morpion_solitaire = MorpionSolitaire5D(20)

# Setting up the state space tree
root = StateSpaceTreeNode(None)

# We perform an expansion to get the children
valid_moves = morpion_solitaire.get_valid_moves()
for move in valid_moves:
    child = StateSpaceTreeNode(move, root)
    child.properties["visits"] = 0
    child.properties["reward"] = 0
    root.add_child(child)

# We now probe the state space tree
dataframes = LegacyProbe.probe(root, morpion_solitaire, 1000)
for dataframe in dataframes:
    print(dataframe)
    print("\n\n")