# PENDING
1. Single Player MCTS for Combinatorial Optimization Problems (DOMAIN). Build on the idea of NRPS, NMCS, GNRPS, ...
    - The performance of an MCTS algorithm is primarily dependent on two factors:
        - The rollout : This affects the distribution (what sort of nodes characterize the quality of a subtree) (NMCS, NRPA, GNRPA, ...)
            - Using a sophisticate selection policy by modifying it to focus on the better part of a subtree.
            - The are all inherently "nested", i.e the selection policy is called recursively.
            - Saving Space
            - Probably applying the probability state tree abstraction to reduce the state space size.
            - Probably using some DL  techniques to maek this better. (Last Priority)
        - The selection policy: This affects how quickly you converge to the true distribution (Boltzmann, UCT, Single Player, PUCT, Langevin, ...)
2. Read all the paper on Policy Adaptation, if you find new important literature add it to the list.
3. Possible directions we can move along are,
    - Truncating/Pruning tree during rollout stage.
    - Using Distribution Approximation algos in rollout
    - Some integrating an idea of focusing on the best and only the best 
      in some other meaningful way.
    - Probably a dynamic programming approach.
    - Some DL shit for better rollouts?
    - Beam Search with Adaptive Policies for Rollouts.
# CURRENT


# COMPLETED

