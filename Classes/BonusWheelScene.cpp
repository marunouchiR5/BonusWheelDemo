#include "BonusWheelScene.h"

cocos2d::Scene* BonusWheelScene::createScene()
{
    return BonusWheelScene::create();
}

bool BonusWheelScene::init() {
    if (!Scene::init()) {
        return false;
    }

    setupWheel();
    setupPrizes();
    setupButtons();

    #ifdef DEBUG
        BonusWheelTest test(this);
        test.runSpinTest();
    #endif

    return true;
}

void BonusWheelScene::setupWheel() {
    cocos2d::Vec2 screenCenter = getScreenCenter();

    // Initialize the wheel game object
    wheelNode = Node::create();
    wheelNode->setPosition(screenCenter);
    this->addChild(wheelNode);

    // Add the wheel background
    wheelBackground = cocos2d::Sprite::create("wheel_sections_8.png");
    wheelBackground->setPosition(cocos2d::Vec2::ZERO);
    wheelNode->addChild(wheelBackground);

    // Scale wheel elements
    float scaleFactor = calculateScaleFactor();
    wheelNode->setScale(scaleFactor);

    // Set the anchor point to the center of the wheel
    wheelNode->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

    // Add the wheel border
    wheelBorder = cocos2d::Sprite::create("wheel_border.png");
    wheelBorder->setPosition(screenCenter);
    this->addChild(wheelBorder);
    wheelBorder->setScale(scaleFactor * .99f);

    // Add the wheel arrow
    setupWheelArrow(scaleFactor, screenCenter);
}

cocos2d::Vec2 BonusWheelScene::getScreenCenter() const {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    return cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
}

float BonusWheelScene::getMaxWheelSize() const {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    float maxWheelSizeFactor = 0.9f; // Adjust this value as needed
    return visibleSize.width * maxWheelSizeFactor;
}

float BonusWheelScene::calculateScaleFactor() const {
    float maxWheelSize = getMaxWheelSize();
    return maxWheelSize / wheelBackground->getContentSize().width;
}

void BonusWheelScene::setupWheelArrow(float scaleFactor, const cocos2d::Vec2& screenCenter) {
    wheelArrow = cocos2d::Sprite::create("wheel_arrow.png");

    // Calculate the position for the wheel arrow
    float arrowYOffset = (wheelBorder->getContentSize().height * scaleFactor / 2) * .88f;
    wheelArrow->setPosition(cocos2d::Vec2(screenCenter.x, screenCenter.y + arrowYOffset));
    wheelArrow->setScale(scaleFactor);

    // Add the wheel arrow directly to the scene, not as a child of the wheelNode
    this->addChild(wheelArrow);
}

void BonusWheelScene::setupPrizes() {
    initializePrizes(); // Function to initialize the 'prizes' vector

    float wheelRadius = calculateWheelRadius();
    float anglePerSector = 360.0f / prizes.size();
    float prizeScaleFactor = 1.0f; // Adjust as needed

    // Position prize icons and labels
    for (int i = 0; i < prizes.size(); ++i) {
        positionPrize(i, wheelRadius, anglePerSector, prizeScaleFactor);
    }
}

void BonusWheelScene::initializePrizes() {
    // Clear existing prizes, if any
    prizes.clear();

    // Add new prizes
    prizes.emplace_back("Life", "heart.png", 30, 0.2f);
    prizes.emplace_back("Brush", "brush.png", 3, 0.1f);
    prizes.emplace_back("Gems", "gem.png", 35, 0.1f);
    prizes.emplace_back("Hammer", "hammer.png", 3, 0.1f);
    prizes.emplace_back("Coins", "coin.png", 750, 0.05f);
    prizes.emplace_back("Brush", "brush.png", 1, 0.2f);
    prizes.emplace_back("Gems", "gem.png", 75, 0.05f);
    prizes.emplace_back("Hammer", "hammer.png", 1, 0.2f);

    // Additional initialization as needed
}

float BonusWheelScene::calculateWheelRadius() const {
    float scaleFactor = calculateScaleFactor();
    return (wheelBorder->getContentSize().width * scaleFactor * 0.8f) / 2; // The 0.8f factor can be adjusted as needed
}

void BonusWheelScene::positionPrize(int index, float wheelRadius, float anglePerSector, float prizeScaleFactor) {
    const auto& prize = prizes[index];
    
    float initialOffsetAngle = -anglePerSector / 2 + 90;
    float angleDegrees = initialOffsetAngle - anglePerSector * index;
    float angleRadians = CC_DEGREES_TO_RADIANS(angleDegrees);

    // Calculate position for the prize icon
    float x = cos(angleRadians) * wheelRadius;
    float y = sin(angleRadians) * wheelRadius;

    // Create and position the prize icon
    auto iconSprite = cocos2d::Sprite::create(prize.iconFilename);
    iconSprite->setPosition(cocos2d::Vec2(x, y));
    iconSprite->setRotation(-angleDegrees + 90); // Adjust rotation so icons face outwards
    iconSprite->setTag(index);
    wheelNode->addChild(iconSprite);

    // Set up the label for the prize
    std::string amountText = formatPrizeAmountText(prize); // Implement this function to format the prize amount text
    auto label = cocos2d::Label::createWithTTF(amountText, "fonts/Marker Felt.ttf", 64);
    label->setPosition(cocos2d::Vec2(iconSprite->getContentSize().width / 2, -30)); // Position can be adjusted as needed
    label->setTag(100 + index);
    iconSprite->addChild(label);

    // Apply scale factor to the prize icon
    iconSprite->setScale(prizeScaleFactor);
}

std::string BonusWheelScene::formatPrizeAmountText(const Prize& prize) const {
    std::string amountText;

    // Format the text based on the prize type
    if (prize.prizeName == "Life") {
        // For 'Life', append 'min' to the amount
        amountText = std::to_string(prize.amount) + " min";
    } else {
        // For other prizes, prepend 'x' to the amount
        amountText = "x" + std::to_string(prize.amount);
    }

    // Additional formatting can be added here for different prize types

    return amountText;
}

void BonusWheelScene::setupButtons() {
    cocos2d::Vec2 screenCenter = getScreenCenter();
    float maxWheelSize = getMaxWheelSize();
    float spinButtonScaleFactor = 0.5f; // Adjust this value as needed
    
    // Setup spin button
    setupSpinButton(screenCenter, maxWheelSize, spinButtonScaleFactor);
    
    // Setup claim button
    setupClaimButton(screenCenter, maxWheelSize, spinButtonScaleFactor);
    
    // Create a menu and add both spin and claim buttons
    wheelMenu = cocos2d::Menu::create(spinButton, claimButton, NULL);
    wheelMenu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(wheelMenu);
}

void BonusWheelScene::setupSpinButton(const cocos2d::Vec2& screenCenter, float maxWheelSize, float buttonScaleFactor) {
    // Create and scale the spin button
    spinButton = cocos2d::MenuItemImage::create("spin_button.png", "", CC_CALLBACK_0(BonusWheelScene::clickSpinButton, this));
    spinButton->setScale(buttonScaleFactor);

    // Create a label for the spin button
    auto spinButtonLabel = cocos2d::Label::createWithTTF("Play On", "fonts/Marker Felt.ttf", 128);
    spinButtonLabel->setPosition(cocos2d::Vec2(spinButton->getContentSize().width / 2, spinButton->getContentSize().height / 2));
    spinButton->addChild(spinButtonLabel);

    // Calculate position for the spin button
    auto buttonYPos = screenCenter.y - (maxWheelSize / 2 + spinButton->getContentSize().height * buttonScaleFactor);
    spinButton->setPosition(cocos2d::Vec2(screenCenter.x, buttonYPos));
}

void BonusWheelScene::setupClaimButton(const cocos2d::Vec2& screenCenter, float maxWheelSize, float buttonScaleFactor) {
    // Create and scale the claim button
    claimButton = cocos2d::MenuItemImage::create("spin_button.png", "", CC_CALLBACK_0(BonusWheelScene::claim, this));
    claimButton->setScale(buttonScaleFactor);

    // Create a label for the claim button
    auto claimButtonLabel = cocos2d::Label::createWithTTF("Claim", "fonts/Marker Felt.ttf", 128);
    claimButtonLabel->setPosition(cocos2d::Vec2(claimButton->getContentSize().width / 2, claimButton->getContentSize().height / 2));
    claimButton->addChild(claimButtonLabel);

    // Calculate position for the claim button (similar to spin button)
    auto buttonYPos = screenCenter.y - (maxWheelSize / 2 + claimButton->getContentSize().height * buttonScaleFactor);
    claimButton->setPosition(cocos2d::Vec2(screenCenter.x, buttonYPos));

    // Hide the claim button initially
    claimButton->setVisible(false);
}

void BonusWheelScene::clickSpinButton()
{
    int winningIndex = simulateSpin();
    if (winningIndex == -1) {
        cocos2d::log("Error: Invalid winning index.");
		return;
	}

    // Logging the details of the selected prize
    const auto& selectedPrize = prizes[winningIndex];
    cocos2d::log("Selected Item: Index=%i, Name=%s, Amount=%i, DropChance=%f",
        winningIndex,
        selectedPrize.prizeName.c_str(),
        selectedPrize.amount,
        selectedPrize.dropChance);

    float anglePerSector = 360.0f / prizes.size(); // Assuming 8 prizes

    float initialReverseSpinAngle = -anglePerSector;
    float finalExtraSpinAngle = anglePerSector / prizes.size();

    // Calculate the angle for the winning sector and adjustment
    float winningSectorAngle = -anglePerSector * winningIndex;
    float initialOffset = anglePerSector * .5f; // Initial arrow offset
    float adjustmentAngle = winningSectorAngle - initialOffset;

    // Normalize the adjustment angle
    adjustmentAngle = fmod(adjustmentAngle + 360.0f, 360.0f);

    // Total rotation angle including initial reverse spin
    int extraRotations = 3; // Extra rotations for visual effect
    float finalRotationAngle = 360.0f * extraRotations + adjustmentAngle - initialReverseSpinAngle;

    // Create the rotation actions
    auto initialReverseSpin = cocos2d::RotateBy::create(0.5, initialReverseSpinAngle);
    auto mainSpin = cocos2d::RotateBy::create(3, finalRotationAngle + finalExtraSpinAngle);
    auto easedMainSpin = cocos2d::EaseOut::create(mainSpin, 4); // Ease out for gradual slowdown
    auto reverseBack = cocos2d::RotateBy::create(0.5, -finalExtraSpinAngle); // Reverse back to the winning sector

    // Create a sequence of initial spin, main spin, and callback
    auto delay = cocos2d::DelayTime::create(0.5f);
    auto callback = cocos2d::CallFunc::create(CC_CALLBACK_0(BonusWheelScene::onSpinEnd, this, winningIndex));
    auto sequence = cocos2d::Sequence::create(initialReverseSpin, easedMainSpin, reverseBack, delay, callback, nullptr);

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

    float randomChance = cocos2d::RandomHelper::random_real(0.0f, totalChance);
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
        cocos2d::Vec2 globalPosition = wheelNode->convertToWorldSpace(selectedItem->getPosition());

        // Get the screen center vector
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        cocos2d::Vec2 screenCenterDirection = cocos2d::Vec2(visibleSize.width / 2 - globalPosition.x, visibleSize.height / 2 - globalPosition.y);

        // Create move and scale actions
        auto moveToCenter = cocos2d::MoveBy::create(1.0f, screenCenterDirection);
        auto scaleUp = cocos2d::ScaleBy::create(1.0f, 1.2f);

        // Combine actions
        auto action = cocos2d::Spawn::create(moveToCenter, scaleUp, nullptr);

        // Run the action on the wheel node
        wheelNode->runAction(action);
    }

    // Show the claim button after the wheel stops spinning
    claimButton->setVisible(true);
}

void BonusWheelScene::claim()
{
    cocos2d::Director::getInstance()->replaceScene(BonusWheelScene::createScene());
}
