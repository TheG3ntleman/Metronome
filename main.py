from src.sudoku import SudokuNoGuarantee
from src.tree_search import TreeSearch

# Example usage:

sudoku3 = SudokuNoGuarantee(2)
sudoku3.print_puzzle()

tree_search = TreeSearch(sudoku3)
tree_search.bfs(700)
tree_search.root_node.print()#.plot()
tree_search.root_node.plot()
sudoku3.print_puzzle()