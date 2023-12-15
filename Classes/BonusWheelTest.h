#ifndef __BONUS_WHEEL_TEST_H__
#define __BONUS_WHEEL_TEST_H__

#include <map>
#include <string>
#include <fstream>
#include <iostream>

class BonusWheelScene;

class BonusWheelTest {
public:
    explicit BonusWheelTest(BonusWheelScene* scene);
    void runSpinTest();

private:
    BonusWheelScene* scene;
};

#endif // __BONUS_WHEEL_TEST_H__
