1. Test convergence of GA : 15th Jan
2. Polish and refine all GA code : 17th Jan, 2 person job
    - Decide on new stage 2 specifications  
    - In this polishing stage use these new stage 2 specifications
3. Decide on the formulation of IP problem, 16th Jan, 1 person job
4. Make code to generate IP problem, 17th Jan, 1 person job
5. Return IP problem, 18th Jan, 1 person job
6. Use appropriate cplex (or alternatives) calls to solve IP problem, 25th Jan, 2 person
7. Polish everything again, 27th Jan, 
8. Polish everything but this time for make optimizations, 29th Jan
9. Build a user interface to generate stage 1 specfications and store them, 30 Jan 
10. Allow the user interface to call our python functions as required, 1 Feb
11. Make the user interface display output in an appropriate format (with something like pdf exports), 5 Feb
12. Polish everything, 5 Feb

# Current objects

1. Understand why the python call gives a segfault. (HITEN & ABHIRATH)
2. Judge the feasibility and optimality of converged timetable solutions. (HITEN  & ABHIRATH)

3. Mystery: why it likes specific numbers. (The timeslots are roughly always the number of sessions).

4. Revamping stage 2 specifications (timeslot support and venue distance information) and data.  (ABHIRATH)
5. Make stage 1 specification loader and converter (CHARAN)

6. Integrate culling via clustering and termination via cluster fully. (CHARAN)
7. Fix fitness functions. (HITEN & ABHIRATH)

