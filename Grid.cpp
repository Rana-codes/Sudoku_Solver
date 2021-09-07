// Name: Harsh Rana
// Instructor and contributor : Dr. Gladys Monagan

#include <iostream>
#include <fstream>  // for file input and output
#include <iomanip>  // std::setw
#include <cmath>
#include "Grid.h"

// width of a number
const int Grid::OUT_WIDTH = 2;
// output that the position is empty
const char Grid::OUT_BLANK = ' ';
// should be as long as WIDTH + 1
const std::string Grid::OUT_ROOF = "---"; 
// output a wall around the number
const char Grid::OUT_WALL = '|';
// corner of a number
const char Grid::OUT_CORNER = '+';

/**
 * CONSTRUCTOR
 * we do not allocate any memory while construction as it is pointless.
 * Our grid will have a purpose only after it receives input
 */
Grid::Grid() { 
   backtracked = 0;
   solutions = 0;
   dimensions = 0;
   array = nullptr;
}

void Grid::deleteArray(int**& array, int size){
   for(int i = 0; i < size; i++) {
      delete [] array[i];
   }
   delete [] array;
}

/**
 * returns the number of possible solutions to a grid
 * PRECONDITION: grid.solve() must be called before to know the number of solutions
 * POSTCONDITION: An integer equivalent to the number of solutions is returned.
 */
int Grid::numberOfSolutions() const {
   return solutions;
}

/**
 * returns the number of times values were assigned into the grid throughout
 * the process of solving.
 * PRECONDITION: grid.solve() must be called before to know works assigned
 * POSTCONDITION: An integer equivalent to works assigned is returned.
 */
long Grid::timesBackTracked() const {
   return backtracked;
}

/**
 * Returns the dimensions of the grid.
 * PRECONDITION: Properly formatted input must be provided to the grid.
 * POSTCONDITION: An integer equivalent to the dimensions of the grid is returned.
 */
int Grid::dimension() const {
   return dimensions;
}




//PRECONDITION: ALL THE INPUTS ARE INT AND IN CORRECT FORMAT
// ALL THE INPUTS ARE IN ASCENDING ORDER.
/**
 * if there was an allocated array already, it is deallocated first.
 * then, new inputs are read. and a new grid is made
 * PRECONDITION: ALL THE INPUTS ARE INT AND IN CORRECT FORMAT.
 * PRECONDITION: The grid must be of square dimensions (X^2) by(x^2) where x^2 is the dimension
 * POSTCONDITION: Input stream used is returned, The current values of the grid are modified according to the input.
 */
std::istream& operator >> (std::istream& in, Grid& G) {

   Grid::deleteArray(G.array, G.dimensions);
   G.backtracked = 0;
   G.solutions = 0;
   G.dimensions = 0;


   int input;
   in >> input;
   G.dimensions = input;
   in >> input;
   if(input!=G.dimensions){
         return in;
   }
   /////////////////////////////////NOW WE ARE SURE DIMENSIONS ARE CORRECT
   const int SIZE = G.dimensions;
   G.array = new int*[SIZE];
   for(int i = 0; i < SIZE; i++) {
      G.array[i] = new int[SIZE];
   }

   //////////////////// MAKING EACH ELEMENT 0 TO BE DOUBLLY SURE;
   for (int i = 0; i < G.dimensions; i++){
      for(int j = 0; j < G.dimensions; j++){
         G.array[i][j] = 0;
      }

   }
   /////////////////// MAKING THE CHANGES REQUESTED BY USER
   int i; int j;
   while(in >> input){
      i = input - 1;// indices
      in >> input;
      j = input - 1;
      in >> input;
      G.array[i][j] = input;
   }
   
   // I COULD'VE DONE BOTH MAKING 0 AND MAKING USER REQUESTED CHANGES USING JUST 1 NESTED ARRAY,
   // BUT IF i DID SO , i WOULD'VE HAD TO MAKE COMPARISONS TO SEE IF I AND J ARE EQUAL TO GIVEN I AND J,
   // COMPLEXITY WOULD STILL REMAIN N^2 BUT i WOULD BE TAKING MORE TIME TO COMPARE.
   // THEREFORE, i JUST ADDED A WHILE LOOP OUTSIDE THE NESTED LOOP. NOW, COMPLEXITY IS N^2 BUT I'M MAKING 0 COMPARISONS.
   return in;
}

// pretty printing
// PROVIDED BY: Dr. Gladys Monagan
void Grid::printHorizontalLine(int n, std::ostream& out) {
   out << OUT_CORNER;
   for (int i = 0; i < n; i++)  {
      out << OUT_ROOF << OUT_CORNER;
   }
   out << std::endl;
}

// pretty printing
// PROVIDED BY: Dr. Gladys Monagan
void Grid::printGrid(std::ostream& out) const {
   // print a top line
   printHorizontalLine(dimensions, out);
   for (int i = 0; i < dimensions; i++) {
      out << OUT_WALL;
      for (int j = 0; j < dimensions; j++) {
         if (array[i][j] == 0) {
            out << std::setw(Grid::OUT_WIDTH) << Grid::OUT_BLANK;
         }
         else {
            out << std::setw(Grid::OUT_WIDTH) << array[i][j];
         }
         out << OUT_BLANK << OUT_WALL;
      } // for j
      out << std::endl;
      printHorizontalLine(dimensions, out);
   } // for i
   out << std::endl;
}

// OVERLOADED OUTPUT OPERATOR
std::ostream& operator << (std::ostream& out, const Grid& g) {
   g.printGrid(out);
   return out;
}


/**
 * Checks if there are any repetitions present that violate the rules of sudoku.
 * PERCONDITION: DIMENSIONS MUST BE PERFECT SQUARE
 * PRECONDITION: index i,j will also be checked. It should be prefferably empty first to get desired result
 */
bool Repetition(int i, int j, int** array, int n, int dimensions){
   // THIS CHECKS THE BOX I,J TOO/
   for(int a = 0; a < dimensions; a++){
      if(array[i][a] == n || array[a][j] == n){
         return true;
      }
   }
   // rows and columns checked. time to check small boxes of size root dim* root dim
   int boxDimensions = sqrt(dimensions);
   int startingI = (i/boxDimensions)*boxDimensions;
   int startingJ = (j/boxDimensions)*boxDimensions;
   // startingI and startingJ are the starting i,j of the small box. for eg, small boxes for 4by $ will be 2 by 2s
   int b = startingJ;
   int a = startingI;
   while(a < startingI+boxDimensions){
      if(array[a][b] == n){
         return true;
      }
      a++;
      if(a == startingI+boxDimensions){
         a = startingI;
         b++;
      }
      if(b == startingJ+boxDimensions){
         a = startingI+boxDimensions;
      }
   }
   //we have checked each element of the small box;
   return false;
}

/**
 * Used as a helper function for solve to make it recursive.
 * fills the provided grid and prints out all the possible solutions
 * POSTCONDITION: all the possible solutions are output to the output stream.
 */
void Grid::fillGrid (std::ostream& out, Grid& grid, int i, int j){
   
   if(j >= grid.dimensions){ // means every j for that i has been covered. so we move to the next i
      j = 0;
      i++;
   }
   if(i >= grid.dimensions){// i will only increase when each j is covered. so at this point all i's and js are covered.
      grid.printGrid(out);
      grid.solutions++;
      return;
   }

   bool constant = false;
   for(int k=1; k < grid.dimensions+1 && !(constant); k++){
      if(grid.array[i][j] != 0 && k == 1){ // if sm value present already,move to the next box, do nothing.
         constant = true;
         fillGrid(out, grid, i, j+1);// if it is a constant, remember it, (so that we don't run the loop again) and just skip that value.
      }
      else{
         if(!(Repetition(i,j,grid.array,k,grid.dimensions))){
            grid.array[i][j] = k;
            //grid.printGrid(out);////////////////////////// testing////////////////////////////########################################################
            grid.backtracked++;
            fillGrid(out, grid, i, j+1);
         }
      }
   }
   if(!constant){// when we backtrack, we need to make sure that all the future values are 0, so we can get all possible solutions.
      grid.array[i][j] = 0;
      grid.backtracked++;
      //grid.printGrid(out);////////////////////////// testing////////////////////////////########################################################
   }
}

void Grid::solve(std::ostream& out) {
   fillGrid(out, *this,0,0);// start from 0,0
   // out << "For  a " << dimensions << " by " << dimensions << " grid \n";
   // out << "Number of Solutions = " << solutions << "\n";
   // out << "Work assignments = " << backtracked << "\n";
   // // these output statements have been covered by solve it in numbers.cpp
}

// makes a deep copy of the first array uptil n elements
void copyArray(int** array, int**& copyarray, int dimensions){
   int j = 0;
   for(int i = 0; i <= dimensions; i++){
      if(i == dimensions){
         i = 0;
         j++;
      }
      if(j == dimensions){
         i = dimensions+1;
      }
      if(i < dimensions && j < dimensions){
         copyarray[i][j] = array[i][j];
      }
   }
}

// copy constructor
Grid::Grid(const Grid& other) {
   const int SIZE = other.dimensions;
   array = new int*[SIZE];
   for(int i = 0; i < SIZE; i++) {
      
      array[i] = new int[SIZE];
   }

   copyArray(other.array, array, other.dimensions);
   backtracked = other.backtracked;
   solutions = other.solutions;
   dimensions = other.dimensions;
}

// overloaded assignment operator
Grid& Grid::operator = (const Grid& rtSide) {
   deleteArray(array, dimensions);
   backtracked = 0;
   solutions = 0;
   dimensions = 0;

   const int SIZE = rtSide.dimensions;
   array = new int*[SIZE];
   for(int i = 0; i < SIZE; i++) {
      
      array[i] = new int[SIZE];
   }

   copyArray(rtSide.array, array, rtSide.dimensions);
   backtracked = rtSide.backtracked;
   solutions = rtSide.solutions;
   dimensions = rtSide.dimensions;

   return *this;
}

//DESTRUCTOR
Grid::~Grid() {

   deleteArray(array, dimensions);
   backtracked = 0;
   solutions = 0;
   dimensions = 0;
}
/////////////##########################################################################################################################################################################################
