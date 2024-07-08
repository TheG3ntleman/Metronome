# Infinite grid is difficult to implement.
# Nodes with up, down, left, right, up-left, ...
# fields and references would be the best way
# to make an infinite grid.

# The difficulty lies in finding feasible moves,
# and updating feilds when needed as that would
# probably require the implementation of a neighbor
# search mechanism which would most likely be a
# pure pain.

# Another way to do it is to maintain a 2D array
# and resize it whenever necessary.

from src.primitives.problem import Problem

class MorpianSolitaire5D(Problem):
    def __init__(self):
        pass
