#ifndef __BONUS_WHEEL_SCENE_H__
#define __BONUS_WHEEL_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include <string>

class BonusWheelScene : public cocos2d::Scene
{
public:
    struct Prize {
        std::string prizeName;
        std::string iconFilename;
        int amount;
        float dropChance;

        Prize(const std::string& name, const std::string& icon, const int& amt, float chance)
            : prizeName(name), iconFilename(icon), amount(amt), dropChance(chance) {}
    };

    std::vector<Prize> prizes;

    static cocos2d::Scene* createScene();

    virtual bool init() override;

    // Implement the "static create()" method manually
    CREATE_FUNC(BonusWheelScene);

    int simulateSpin();
    void testSpinFunction(); // unit test

private:
    cocos2d::Node* wheelNode;
    cocos2d::Sprite* wheelBorder;
    cocos2d::Sprite* wheelBackground;
    cocos2d::Sprite* wheelArrow;
    cocos2d::Menu* wheelMenu;
    cocos2d::MenuItemImage* spinButton;
    cocos2d::MenuItemImage* claimButton;

    void clickSpinButton();
    void onSpinEnd(int winningIndex);
    void claim();
};

#endif // __BONUS_WHEEL_SCENE_H__
