#include "BonusWheelTest.h"
#include "BonusWheelScene.h"

BonusWheelTest::BonusWheelTest(BonusWheelScene* scene) : scene(scene) {}

void BonusWheelTest::runSpinTest() {
    std::map<int, int> prizeCounts;
    for (int i = 0; i < 1000; ++i) {
        int index = scene->simulateSpin();
        prizeCounts[index]++;
    }

    // Open a file to write the results
    std::ofstream outFile("spin_results.txt");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    // Output the results to the file
    for (const auto& pair : prizeCounts) {
        outFile << "Prize: " << pair.first << ", Count: " << pair.second << std::endl;
    }

    // Close the file
    outFile.close();
    CCLOG("File spin_results.txt created");
}
