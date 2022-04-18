#include<iostream>
// #include<blend2d.h>
// #include"blend2d.h"
#include <utility>
#include <vector>
#include <string>
#include <exception>

// #include "Libraries/blend2d/src/blend2d.h"
// #include "imgfuncs.h"
#include "pointgeneration.h"
// #include "pathcalcs.h"
#include "Traveler.h"
#include "BruteForce.h"
#include "Shortest.h"
#include "NoCross.h"

/**
* \mainpage
* Welcome to my small project for finding a short path connecting a given amount of generated points.
* 
* 
* \section intro_sec Introduction
* This project was created as a hand-in for the university course *efficient programming* for my masters degree.
* 
* It is made up of two components:
*  - the C++ code for generating points and finding a valid path
*  - the Python code to create an interactive GUI
* 
* The main function can be used without the GUI as a command line tool.
* See Salesman01.cpp for further reference.
* 
* \section install_sec Installation
* To run and use the GUI, the C++ code must be compiled first. 
* Easiest way to do this is to open the project in Microsoft Visual Studio Code (vscode)
* in the .vscode folder, the *tasks.json* folder contains all neccessary commands to compile the code using the mignw  g++ compiler.
* It might be neccessary to change "command" to point to the correct g++ executable.
* 
* After successfull installation, run the gui_builder.py
* 
* To create the docs, an installation of doxygen is required.
*/

/**
* @file
* 
* Finds a short path connecting all points
* Command line parameters:
* @param --points         number of points to generate
* @param --size           size of the area to generate points in
* @param --inputPoints    Number of points input via console
* @param --seed           seed for point generation
*/
int main(int argc, char* argv[])
{

    int nPointsGenerated = 10;
    int nPointsInput = 0;
    int imgSize = 480;
    bool randPoints = true;
    int seed = 711;
    int mode = 0;
    std::vector<int> points;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--points") {
            if (i + 1 < argc) {
                nPointsGenerated = std::stoi(argv[i + 1]);
                // No need to iterate over the command input again
                i++;
            }
            else {
                throw std::runtime_error("--points requires input");
            }
        }
        else if (std::string(argv[i]) == "--size") {
            if (i + 1 < argc) {
                imgSize = std::stoi(argv[i + 1]);
                i++;
            }
            else {
                throw std::runtime_error("--size requires input");
            }
        }
        else if (std::string(argv[i]) == "--inputPoints") {
            if (i + 1 < argc) {
                nPointsInput = std::stoi(argv[i + 1]);
                i++;
            }
        }
        else if (std::string(argv[i]) == "--seed") {
            if (i+1 < argc) {
                seed = std::stoi(argv[i+1]);
                i++;
            }
        }
        else if (std::string(argv[i]) == "--mode") {
            if (i+1 < argc) {
                mode = std::stoi(argv[i+1]);
                i++;
            }
        }
    }

    // read in and/or generate points
    for (int p = 0; p < 2 * nPointsInput; p++) {
        int nextPointCoord;
        std::cin >> nextPointCoord;
        points.push_back(nextPointCoord);
    }
    if (nPointsGenerated > 0){
        createPoints(nPointsGenerated, points, imgSize, seed);
    }

    // Declare and initialize outputs
    std::vector<int> bestIndexes;
    for(int i=1; i<points.size()/2; i++) bestIndexes.push_back(i);

    double minDistance = 0;
    double timeToBest = 0;
    double timeToFinish = 0;

    if(mode==0){
        BruteForce Permutator = BruteForce(points);
        Permutator.iterateThroughPoints();

        bestIndexes = Permutator.getBestPathIndizes();
        minDistance = Permutator.getBestDist();
        timeToBest = Permutator.getTimeToFindBest();
        timeToFinish = Permutator.getTimeToFinish();
    }
    else if(mode==1){
        BruteForce Permutator = BruteForce(points);
        Permutator.permutateHalf();

        bestIndexes = Permutator.getBestPathIndizes();
        minDistance = Permutator.getBestDist();
        timeToBest = Permutator.getTimeToFindBest();
        timeToFinish = Permutator.getTimeToFinish();
    }
    else if (mode == 2) {
        Shortest traveler = Shortest(points);
        traveler.takeClosestPoint();
        
        bestIndexes = traveler.getBestPathIndizes();
        minDistance = traveler.getBestDist();
        timeToBest = traveler.getTimeToFindBest();
        timeToFinish = traveler.getTimeToFinish();
    }
    else if (mode == 3) {
        NoCross crossOut = NoCross(points);
        crossOut.NextPointNoCross();

        bestIndexes = crossOut.getBestPathIndizes();
        minDistance = crossOut.getBestDist();
        timeToBest = crossOut.getTimeToFindBest();
        timeToFinish = crossOut.getTimeToFinish();
    }

    std::vector<int>::iterator iter;
    std::cout << /*"Best indexes: 0"*/ 0 << std::endl;
    for (iter = bestIndexes.begin(); iter != bestIndexes.end(); iter++) {
        std::cout << /*", " <<*/ *iter << std::endl;
    }

    std::vector<int>::iterator pointIter;
    for (pointIter = points.begin(); pointIter != points.end(); pointIter++){
        std::cout << *pointIter << std::endl;
    }

    std::cout << minDistance << std::endl;
    std::cout << timeToBest << std::endl;
    std::cout << timeToFinish << std::endl;
    // std::cout << std::endl;
    /*
    std::cout << "Shortest distance is: " << bestDistance << std::endl;
    std::cout << "Best solution found after: " << DaBoss.getTimeToFindBest()<< "s" << std::endl;
    std::cout << "Total time to permutate all: " << DaBoss.getTimeToFinish() << "s" << std::endl;


    int pointRadius = 20;
    BLImage img = createImage(points, points.size(), pointRadius);
    
    std::string pointsstring = std::to_string(pointsTotal);
    std::string name("_points.bmp");
    
    name = pointsstring + name;
    
    saveBLImg(img, name.c_str());
    
    drawBestRoute(img, points, bestIndexes);
    
    std::string drawName = ("_best_route.bmp");
    name = pointsstring + drawName;
    
    saveBLImg(img, name.c_str());
    */
    return 0;
}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

