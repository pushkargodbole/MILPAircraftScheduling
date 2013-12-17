This is the code for finding the global-best solution of an aircraft scheduling snapshot problem. It takes as input the airport map (interpreted as a graph) and parameters (which include the origin, destination, start time, speed, trailing separation and priority) for each aircraft and generates as output the path allocations and the correspoinding nodal-time-stamps for each aircraft such that the sum of the destination-times for all aircraft is minimized.
The algorithm works in 2 steps:
- It finds all possible simple paths for each aircraft for the given origin-destination pair
- For every such flight-path allocation, it runs an MILP (using CPLEX) to find the best solution for that flight-path allocation.
Finally it compares the costs for each and narrows down on the global-best solution.
It also executes a feasibility check after the solution is generated and an animation and a 3D time-evolution plot of the schedule using Python.

REQUIRES ILOG CPLEX

