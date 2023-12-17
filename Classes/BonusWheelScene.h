#ifndef __BONUS_WHEEL_SCENE_H__
#define __BONUS_WHEEL_SCENE_H__

#include "cocos2d.h"
#include "BonusWheelTest.h"
#include <vector>
#include <string>

class BonusWheelScene : public cocos2d::Scene {
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

private:
    cocos2d::Node* wheelNode;
    cocos2d::Sprite* wheelBorder;
    cocos2d::Sprite* wheelBackground;
    cocos2d::Sprite* wheelArrow;
    cocos2d::Menu* wheelMenu;
    cocos2d::MenuItemImage* spinButton;
    cocos2d::MenuItemImage* claimButton;

    void setupWheel();
    void setupPrizes();
    void setupButtons();
    void setupSpinButton(const cocos2d::Vec2& screenCenter, float maxWheelSize, float buttonScaleFactor);
    void setupClaimButton(const cocos2d::Vec2& screenCenter, float maxWheelSize, float buttonScaleFactor);
    cocos2d::Vec2 getScreenCenter() const;
    float getMaxWheelSize() const;
    float calculateScaleFactor() const;
    void setupWheelArrow(float scaleFactor, const cocos2d::Vec2& screenCenter);
    void initializePrizes();
    float calculateWheelRadius() const;
    void positionPrize(int index, float wheelRadius, float anglePerSector, float prizeScaleFactor);
    std::string formatPrizeAmountText(const Prize& prize) const;

    void clickSpinButton();
    void onSpinEnd(int winningIndex);
    void claim();
};

#endif // __BONUS_WHEEL_SCENE_H__
