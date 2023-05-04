### Currently working on
  - Probably migrate argument parsing code to it's own file.
  1.5 Add a script parser that will accept timetable specifications from files. (FROM INTERFACE, 1)
    Make lexer
      - Optimize lexer 
        - possible switch case optimization
        - Better control flow
        - Get rid of the semicolon syntax and let the new_line be the TOKEN_END.      
    Make timetabling structures to fill
    Make parser
  2. Actual timetabling stuff. 

### BUGS
  - Lexer
    - Final token of the file is at some weird point.

### Interface
1. Make a language interpreter to accept specifications for a time table and then proceed to make it.

### Core functionality related to actually making a time table.

TODO (LEVELS OF FUNCTIONALITY):

  # class room capacity is not considered (handle that 
    by dividing people into groups and assigning physical 
    classroom's on the user's end).

  LEVEL - I:
    1. Take groups list
    2. Take instructor list 
    1. Take classes:
      a. duration
      b. attending groups
      c. instructors assigned for a class

  LEVEL - I:
    FINAL GOAL:
      1. Given some information then generate the best possible time table
         according to some criteria.
      2. Allow for best time-table generation given incomplete information.
         that would then be used to generate revised input information to make
         a better timetable.
    GOAL: 
      1. Work for single-year timetables
    OBJECTIVE:
      1. Create and elagant data-format
         that accepts the following information:
          a. Work timings (timings and days of the week)
          b. List of classes
          c. List of instructors
          d. List of student groups

# COMPLETED
  1. Basic project structure and setup
  2. Basic argument parser (bare bones, functionally null).
