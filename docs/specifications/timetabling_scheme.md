# The feasiblity problem

## Two stages of the problem

### Stage - I, not in the format for the solvers, easy to enter as input though

This stage is easier for user to enter and understand but is not in the format for the solver to solve. 

Here we have, 
    1. Students
    2. Faculty
    3. Raw Course Information: 
        Ex.
            - How many hours per week?
            - Batch Partitions
            - Batch scheduling info (how manmy classes does each batch need)
            - Who will be taking which batch
            - Parties needed in attendance for particular session/period
            - Period venue
            - Period frequency and lenghts.
            ...
    4. additional problem specifications

### Stage - II, in format for solver, not easy to read and input

The second stage is the resultant output after the first Stage has gone through processing. Here we only have the following tables:
    1. Parties
    2. Atomic Courses,
    3. additional problem specifications:
        max cap on hours students can attend
        max cap on hours faculties will work
        time slot/cycle information
        ...

#### Criticisms with first specfication
Does not conform to the essence behind 1 or 2. Essentially it is noth completely this or completely that. It is difficult for entry and at the same time not in format for the solver
