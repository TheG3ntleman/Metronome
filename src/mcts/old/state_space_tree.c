#include "state_space_tree.h"


StateSpaceTree *StateSpaceTree_make() {
    StateSpaceTree *tree = malloc(sizeof(StateSpaceTree));
    
    tree->root = (StateNode*) malloc(sizeof(StateNode));

    // Going to have to redo this entire function.

    return tree;
}

void StateSpaceTree_free(StateSpaceTree *tree) {

}