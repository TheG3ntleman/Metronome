from src.sudoku import SudokuNoGuarantee
from src.breadth_first_search import SudokuSolverBFS, Sudoku
from src.node import Node


# Instantiate the puzzle
sudoku3 = SudokuNoGuarantee(3)
sudoku_correct = Sudoku(3)
sudoku_correct.from_string("007009050040000930059740080000016790083000002710000000830060020000395018605020070")#"048301560360008090910670003020000935509010200670020010004002107090100008150834029")#"301086504046521070500000001400800002080347900009050038004090200008734090007208103")#"070000043040009610800634900094052000358460020000800530080070091902100005007040802")

# Instantiate the solver
solver = SudokuSolverBFS(sudoku_correct)#3)
print("Solution:\n", solver.run(1000, single_solution=False))
sudoku3.print_puzzle()
solver.root_node.plot()
