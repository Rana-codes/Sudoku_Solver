// Student Name: Harsh Rana

#include <iostream>
#include <fstream>
#include <string>
#include "Grid.h"

void solveIt(std::istream &in, std::ostream &out) {
   Grid grid;
   in >> grid;
   // for debugging purposes
   std::cout << "the input grid\n"
             << grid;

   grid.solve(out);

   std::cout << "for a " << grid.dimension();
   std::cout << " by " << grid.dimension() << " grid\n";
   std::cout << "number of solutions = ";
   std::cout << grid.numberOfSolutions() << std::endl;
   std::cout << "Times Backtracked = " << grid.timesBackTracked();
   std::cout << std::endl;
}

/**
 * outputs available commands and a brief explaination on the console
 */
void printUsage() {
   std::cout << "** USAGE ** \n";
   std::cout << "-input inFile :infile is the file from which input is to be provided\n";
   std::cout << "**NOTE** -input infile : mandatory command\n\n";
   std::cout << "-output outfile :outfile is the file into which the data is to be output\n";
   std::cout << "**NOTE** -output outfile : optional. Will output to console if no input is provided.\n\n";
   std::cout << "-help :provides a list and description of commands\n"
             << std::endl;
}

/**
 * Takes input from the provided file and outputs on the console
 * PRECONDITION: All the elements in the provided fie must be integers
 * POSTCONDITION: If the file wasn't found an error message is displayed. Else the elements are output on the console.
 */
void input(std::string file) {
   std::ifstream input(file);
   if (input.is_open()) { // checks if the file is accessible

      Grid x;

      solveIt(input,std::cout);

      input.clear(); // to bring the file out of failed state
      input.close();
   }
   else { //the file isn't accessible
      std::cerr << "** ERROR ** THE PROVIDED INPUT FILE COULD NOT BE FOUND" << std::endl;
      printUsage();
   }
}

/**
 * Takes input from the provided file and outputs it in the provided output file
 * PRECONDITION: All the elements in the provided fie must be integers
 * POSTCONDITION: A new output file is created if the provided output file DNE. prints out error if either file is inaccessible
 * copies data if both the files are accessible.
 */
void outputToFile(std::string inputFile, std::string outputFile) {
   std::ifstream input(inputFile);
   std::ofstream output(outputFile); // creates the file if it is not already present

   if (input.is_open() && output.is_open()) { // while intyegers are present in the file

      Grid x;
      
      solveIt(input,output);

      output.clear();
      input.clear();
      output.close();
      input.close();
   }

   else {
      if (!(input.is_open())) {
         std::cerr << "** ERROR ** THE PROVIDED INPUT FILE COULD NOT BE FOUND" << std::endl;
      }
      else {
         std::cerr << "** ERROR ** THE PROVIDED OUTPUT FILE COULD NOT BE FOUND" << std::endl;
      }
      printUsage();
   }
}

int main(int argc, char *argv[]) {

   // if there are any additional elements, print out usage.
   if (argc <= 1 || argc > 5) {
      printUsage();
   }

   else {
      bool allCommandsValid = true;
      std::string inputFile = "";
      std::string outputfile = "";
      int i = 1;

      while (i < argc) {
         std::string command = argv[i];

         if (command == "-input") {
            if (i != argc - 1) { // means an argument is present after "-input"
               std::string nxtC = argv[i + 1];
               // checks the argument after "-input". if arguments are "-output" or "-help" means there is no file name
               if (nxtC[0] == '-') {
                  allCommandsValid = false;
                  i = argc; // to exit loop
                  std::cerr << "** ERROR ** THE INPUT FILE IS MISSING" << std::endl;
                  printUsage();
               }
               else {
                  i++;
                  inputFile = argv[i];
               }
            }

            else { // means no argument is present after "-input"
               allCommandsValid = false;
               i = argc;
               std::cerr << "** ERROR ** THE INPUT FILE IS MISSING" << std::endl;
               printUsage();
            }
         }

         else if (command == "-output") {

            if (i != argc - 1) { // means an argument is present after "-output"
               std::string nxtC = argv[i + 1];
               // checks the argument after "-output". if arguments are "-input" or "-help" means there is no file name
               if (nxtC[0] == '-') {
                  allCommandsValid = false;
                  i = argc;
                  std::cerr << "** ERROR ** THE OUTPUT FILE IS MISSING" << std::endl;
                  printUsage();
               }
               else {
                  i++;
                  outputfile = argv[i];
               }
            }
            
            else { // means no argument is present after "-output"
               allCommandsValid = false;
               i = argc;
               std::cerr << "** ERROR ** THE OUTPUT FILE IS MISSING" << std::endl;
               printUsage();
            }
         }

         else { // now it is either an invalid command or -help. In either case we print usage
            if (command != "-help") {
               std::cerr << "** ERROR ** INVALID INPUT" << std::endl;
            }
            printUsage();
            allCommandsValid = false;
         }

         i++;
      }

      // we check if we have to print on the console or copy dta into a file.
      if (inputFile != "" && allCommandsValid) {
         if (outputfile != "") {
            outputToFile(inputFile, outputfile);
         }
         else {
            input(inputFile);
         }
      }
   }
   return 0;
}
