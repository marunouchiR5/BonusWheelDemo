#include "BonusWheelScene.h"
#include <fstream>
#include <iostream>

USING_NS_CC;

Scene* BonusWheelScene::createScene()
{
    return BonusWheelScene::create();
}

bool BonusWheelScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 screenCenter = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

    // Initialize the wheel game object
    wheelNode = Node::create();
    wheelNode->setPosition(screenCenter);
    this->addChild(wheelNode);

    // Add the wheel background
    wheelBackground = Sprite::create("wheel_sections_8.png");
    wheelBackground->setPosition(Vec2::ZERO);
    wheelNode->addChild(wheelBackground);

    // Scale wheel elements
    float maxWheelSize = visibleSize.height * 0.5f; // Example scaling factor
    float scaleFactor = maxWheelSize / wheelBackground->getContentSize().height;
    wheelNode->setScale(scaleFactor);

    // Set the anchor point of wheelGameObject to the center of the wheel
    wheelNode->setAnchorPoint(Vec2(0.5f, 0.5f));

    // Add the wheel border
    wheelBorder = Sprite::create("wheel_border.png");
    wheelBorder->setPosition(screenCenter);
    this->addChild(wheelBorder);
    wheelBorder->setScale(scaleFactor * .99f);

    // Initialize prizes
    prizes.emplace_back("Life", "heart.png", 30, 0.2f);
    prizes.emplace_back("Brush", "brush.png", 3, 0.1f);
    prizes.emplace_back("Gems", "gem.png", 35, 0.1f);
    prizes.emplace_back("Hammer", "hammer.png", 3, 0.1f);
    prizes.emplace_back("Coins", "coin.png", 750, 0.05f);
    prizes.emplace_back("Brush", "brush.png", 1, 0.2f);
    prizes.emplace_back("Gems", "gem.png", 75, 0.05f);
    prizes.emplace_back("Hammer", "hammer.png", 1, 0.2f);

    // Calculate wheel radius and sector angle
    float wheelRadius = (wheelBorder->getContentSize().width * scaleFactor) / 2 + 135.0f;
    float anglePerSector = 360.0f / prizes.size(); // Assuming 8 prizes
    float initialOffsetAngle = anglePerSector / 2;

    // Scale factor for the prizes
    float prizeScaleFactor = 1.0f; // Adjust this value as needed

    // Position prize icons and labels
    for (size_t i = 0; i < prizes.size(); ++i) {
        const auto& prize = prizes[i];

        float angleDegrees = initialOffsetAngle + anglePerSector * i;
        float angleRadians = CC_DEGREES_TO_RADIANS(angleDegrees);

        float x = cos(angleRadians) * wheelRadius;
        float y = sin(angleRadians) * wheelRadius;

        auto iconSprite = Sprite::create(prize.iconFilename);
        iconSprite->setPosition(Vec2(x, y));
        iconSprite->setRotation(-angleDegrees + 90);
        iconSprite->setTag(i);
        wheelNode->addChild(iconSprite);

        std::string amountText;
        if (prize.prizeName == "Life") {
            amountText = std::to_string(prize.amount) + "min";
        } else {
			amountText = "x" + std::to_string(prize.amount);
		}
        auto label = Label::createWithTTF(amountText, "fonts/Marker Felt.ttf", 64);
        label->setPosition(Vec2(iconSprite->getContentSize().width / 2, -30));
        label->setTag(100 + i);
        iconSprite->addChild(label);

        iconSprite->setScale(prizeScaleFactor); // Apply new scale factor for prizes
    }

    // Scale factor for the spin button
    float spinButtonScaleFactor = 0.5f; // Adjust this value as needed

    // Add and position the spin button with new scale
    spinButton = MenuItemImage::create("spin_button.png", "", CC_CALLBACK_0(BonusWheelScene::clickSpinButton, this));
    spinButton->setScale(spinButtonScaleFactor); // Apply new scale factor for spin button

    // Create a label for the spin button
    auto spinButtonLabel = Label::createWithTTF("Play On", "fonts/Marker Felt.ttf", 128); // Adjust font and size as needed
    spinButtonLabel->setPosition(Vec2(spinButton->getContentSize().width / 2, spinButton->getContentSize().height / 2));
    spinButton->addChild(spinButtonLabel); // Add label as a child of the spin button

    // Calculate position for the spin button
    auto buttonYPos = screenCenter.y - (maxWheelSize / 2 + spinButton->getContentSize().height * spinButtonScaleFactor);
    spinButton->setPosition(Vec2(screenCenter.x, buttonYPos));

    // Add the wheel arrow
    wheelArrow = Sprite::create("wheel_arrow.png");

    // Calculate the position for the wheel arrow
    float arrowYOffset = (wheelBorder->getContentSize().height * scaleFactor / 2) * .88f;
    wheelArrow->setPosition(Vec2(screenCenter.x, screenCenter.y + arrowYOffset));
    wheelArrow->setScale(scaleFactor);

    // Add the wheel arrow directly to the scene, not as a child of the wheelNode
    this->addChild(wheelArrow);
    
    // Initialize the claim button
    claimButton = MenuItemImage::create("spin_button.png", "", CC_CALLBACK_0(BonusWheelScene::claim, this));
    claimButton->setScale(spinButtonScaleFactor); // Use the same scale factor as the spin button

    // Create a label for the claim button
    auto claimButtonLabel = Label::createWithTTF("Claim", "fonts/Marker Felt.ttf", 128); // Adjust font and size as needed
    claimButtonLabel->setPosition(Vec2(claimButton->getContentSize().width / 2, claimButton->getContentSize().height / 2));
    claimButton->addChild(claimButtonLabel); // Add label as a child of the claim button

    // Calculate position for the claim button (adjust as needed)
    auto claimButtonYPos = buttonYPos;
    claimButton->setPosition(Vec2(screenCenter.x, claimButtonYPos));

    // Hide the claim button initially
    claimButton->setVisible(false);

    // Create a menu and add both spin and claim buttons
    wheelMenu = Menu::create(spinButton, claimButton, NULL);
    wheelMenu->setPosition(Vec2::ZERO);
    this->addChild(wheelMenu); // Add menu to the scene

    #ifdef DEBUG
        BonusWheelTest test(this);
        test.runSpinTest();
    #endif

    return true;
}

void BonusWheelScene::clickSpinButton()
{
    int winningIndex = simulateSpin();
    if (winningIndex == -1) {
		log("Error: Invalid winning index.");
		return;
	}

    // Logging the details of the selected prize
    const auto& selectedPrize = prizes[winningIndex];
    log("Selected Item: Index=%i, Name=%s, Amount=%i, DropChance=%f",
        winningIndex,
        selectedPrize.prizeName.c_str(),
        selectedPrize.amount,
        selectedPrize.dropChance);

    float anglePerSector = 360.0f / prizes.size(); // Assuming 8 prizes

    float initialReverseSpinAngle = -anglePerSector;
    float finalExtraSpinAngle = anglePerSector / 5;

    // Calculate the angle for the winning sector and adjustment
    float winningSectorAngle = anglePerSector * winningIndex;
    float initialOffset = anglePerSector * 1.5f; // Initial arrow offset
    float adjustmentAngle = winningSectorAngle - initialOffset;

    // Normalize the adjustment angle
    adjustmentAngle = fmod(adjustmentAngle + 360.0f, 360.0f);

    // Total rotation angle including initial reverse spin
    int extraRotations = 3; // Extra rotations for visual effect
    float finalRotationAngle = 360.0f * extraRotations + adjustmentAngle - initialReverseSpinAngle;

    // Create the rotation actions
    auto initialReverseSpin = RotateBy::create(0.5, initialReverseSpinAngle);
    auto mainSpin = RotateBy::create(3, finalRotationAngle + finalExtraSpinAngle);
    auto easedMainSpin = EaseOut::create(mainSpin, 4); // Ease out for gradual slowdown
    auto reverseBack = RotateBy::create(0.5, -finalExtraSpinAngle); // Reverse back to the winning sector

    // Create a sequence of initial spin, main spin, and callback
    auto delay = DelayTime::create(0.5f);
    auto callback = CallFunc::create(CC_CALLBACK_0(BonusWheelScene::onSpinEnd, this, winningIndex));
    auto sequence = Sequence::create(initialReverseSpin, easedMainSpin, reverseBack, delay, callback, nullptr);

    // Run the action sequence on the wheel node
    wheelNode->runAction(sequence);

    // Hide the spin button when the wheel starts spinning
    spinButton->setVisible(false);
}

int BonusWheelScene::simulateSpin() {
    float totalChance = 0.0f;
    for (const auto& prize : prizes) {
        totalChance += prize.dropChance;
    }

    float randomChance = RandomHelper::random_real(0.0f, totalChance);
    float accumulatedChance = 0.0f;

    for (int i = 0; i < prizes.size(); ++i) {
        accumulatedChance += prizes[i].dropChance;
        if (randomChance <= accumulatedChance) {
            return i;
        }
    }
    return -1;
}

void BonusWheelScene::onSpinEnd(int winningIndex)
{
    wheelBackground->setVisible(false);
    wheelBorder->setVisible(false);
    wheelArrow->setVisible(false);

    // Hide all prizes except the winning one
    for (int i = 0; i < prizes.size(); ++i) {
        if (i != winningIndex) {
            Node* iconNode = wheelNode->getChildByTag(i);
            Node* labelNode = wheelNode->getChildByTag(100 + i);
            if (iconNode) {
                iconNode->setVisible(false);
            }
            if (labelNode) {
                labelNode->setVisible(false);
            }
        }
    }

    Node* selectedItem = wheelNode->getChildByTag(winningIndex);
    if (selectedItem) {
        // Convert the local position of the selected item to a global position
        Vec2 globalPosition = wheelNode->convertToWorldSpace(selectedItem->getPosition());

        // Get the screen center vector
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 screenCenterDirection = Vec2(visibleSize.width / 2 - globalPosition.x, visibleSize.height / 2 - globalPosition.y);

        // Create move and scale actions
        auto moveToCenter = MoveBy::create(1.0f, screenCenterDirection);
        auto scaleUp = ScaleBy::create(1.0f, 1.2f);

        // Combine actions
        auto action = Spawn::create(moveToCenter, scaleUp, nullptr);

        // Run the action on the wheel node
        wheelNode->runAction(action);
    }

    // Show the claim button after the wheel stops spinning
    claimButton->setVisible(true);
}

void BonusWheelScene::claim()
{
    Director::getInstance()->replaceScene(BonusWheelScene::createScene());
}
