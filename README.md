# Sudoku_Solver

Hello, Thank you for showing interest.

This is a C++ program that finds all the possible solutions to a given sudoku using the principles of backtracking.
Also uses file IO.
This assignment was assigned by Dr. Gladys Monagan for the course Algorithms and Data Structures II.
An executable has been provided( sudokuSolver.exe)
All the Source files have also been provided.

### Requirements - 

   - The sudoku must be a (perfect square)x(perfect square)
    
     for eg. 4x4 , 9x9, 16x16........

   - The input must be in correct format (for both the command line and the sudoku.)


### Using the program - 
   
Sudoku Solver uses command line input for the sake of convinience to the user.
Input is provided through a file.
Outpt is provided on the console unless an output file is specified.


- An Input is compulsory.
- For output on console 

      numbers.exe -input InFileName
      
- For output on a file

      numbers.exe -input InFileName -output OutFileName
      

### Inputing the sudoku - 

The basic format is

size size 

row column numberInSudoku

row column numberInSudoku

row column numberInSudoku

for a 4x4 sudoku like -

     +---+---+---+---+
     |   | 4 |   |   |
     +---+---+---+---+
     |   |   |   |   |
     +---+---+---+---+
     |   | 2 |   |   |
     +---+---+---+---+
     |   |   | 1 |   |
     +---+---+---+---+

The input file will have - 

    4 4
    1 2 4
    3 2 2
    4 3 1 
