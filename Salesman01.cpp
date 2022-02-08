#include<iostream>
// #include<blend2d.h>
// #include"blend2d.h"
#include <utility>
#include <vector>
#include <string>
#include <exception>

#include "Libraries/blend2d/src/blend2d.h"
#include "imgfuncs.h"
#include "pointgeneration.h"
// #include "pathcalcs.h"
#include "Traveler.h"

int main(int argc, char* argv[])
{

    int pointsTotal = 10;
    int imgSize = 480;
    bool randPoints = true;
    std::vector<int> points;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--points") {
            if (i + 1 < argc) {
                pointsTotal = std::stoi(argv[i + 1]);
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
            randPoints = false;
            if (i + 1 < argc) {
                pointsTotal = std::stoi(argv[i + 1]);
                i++;
            }
        }
    }

    if (randPoints) {
        points = createPoints(pointsTotal, imgSize);
    }
    else {
        for (int p = 0; p < 2 * pointsTotal; p++) {
            int nextPointCoord;
            std::cin >> nextPointCoord;
            points.push_back(nextPointCoord);
        }
    }

    Traveler DaBoss(points);

    DaBoss.iterateThroughPoints();

    std::vector<int> bestIndexes = DaBoss.getBestPathIndizes();
    double bestDistance = DaBoss.getBestDist();

    /*std::vector<int>::iterator iter;
    std::cout << "Best indexes: 0";
    for (iter = bestIndexes.begin(); iter != bestIndexes.end(); iter++) {
        std::cout << ", " << * iter;
    }
    std::cout << std::endl;
    std::cout << "Shortest distance is: " << bestDistance << std::endl;
    std::cout << "Best solution found after: " << DaBoss.getTimeToFindBest() << "s" << std::endl;
    std::cout << "Total time to permutate all: " << DaBoss.getTimeToFinish() << "s" << std::endl;

    DaBoss.reset();*/

    DaBoss.permutateHalf();

    bestIndexes = DaBoss.getBestPathIndizes();
    bestDistance = DaBoss.getBestDist();

    std::vector<int>::iterator iter;
    std::cout << /*"Best indexes: 0"*/ 0;
    for (iter = bestIndexes.begin(); iter != bestIndexes.end(); iter++) {
        std::cout << /*", " <<*/ *iter;
    }
    std::cout << std::endl;
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

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
