from src.sudoku import SudokuNoGuarantee
from src.breadth_first_search import SudokuSolverBFS
from src.node import Node


# Instantiate the puzzle
sudoku3 = SudokuNoGuarantee(3)

# Instantiate the solver
solver = SudokuSolverBFS(sudoku3)
solver.run()
solver.root_node.plot()
sudoku3.print_puzzle()
