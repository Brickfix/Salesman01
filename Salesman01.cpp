#include<iostream>
// #include<blend2d.h>
// #include"blend2d.h"
#include <utility>
#include <vector>
#include <string>

#include "Libraries/blend2d/src/blend2d.h"
#include "imgfuncs.h"
#include "pointgeneration.h"
// #include "pathcalcs.h"
#include "Traveler.h"

#define PRADIUS 20
#define NPOINTS 13


int main()
{

    // int points[10] = { 100, 100, 300, 300, 150, 400, 50, 300, 350, 50 };
    // std::deque<int> points = { 100, 100, 300, 300, 150, 400, 50, 300, 350, 50 };
    std::vector<int> points = createPoints(NPOINTS, 480);

    Traveler DaBoss(points);

    DaBoss.createDistMat();

    std::pair<std::vector<int>,double> result = DaBoss.iterateThroughPoints();
    std::vector<int> bestIndexes = result.first;
    double bestDistance = result.second;

    std::vector<int>::iterator iter;
    std::cout << "Best indexes: 0";
    for (iter = bestIndexes.begin(); iter != bestIndexes.end(); iter++) {
        std::cout << ", " << * iter;
    }
    std::cout << std::endl;


    BLImage img = createImage(points, points.size(), PRADIUS);
    
    std::string pointsstring = std::to_string(NPOINTS);
    std::string name("_points.bmp");
    
    name = pointsstring + name;
    
    saveBLImg(img, name.c_str());
    
    drawBestRoute(img, points, bestIndexes);
    
    std::string drawName = ("_best_route.bmp");
    name = pointsstring + drawName;
    
    saveBLImg(img, name.c_str());
    
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
