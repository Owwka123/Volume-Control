#include <Geode/Geode.hpp>

#include "ui.hpp"

using namespace geode::prelude;

VolumeUI* VolumeUI::create() {
	auto ret = new VolumeUI();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool VolumeUI::init() {
	if (!CCLayer::init()) return false;
	if (CCScene::get()->getChildByIDRecursive("VolumeUI")) {
		auto bg = CCScene::get()->getChildByIDRecursive("VolumeUI")->getChildByIDRecursive("bg");
		bg->stopAllActions();
		bg->runAction(CCSequence::create(
			CCEaseBackOut::create(CCMoveTo::create(0.5f, ccp(bg->getPositionX(), 0.f))),
			CCDelayTime::create(Mod::get()->getSettingValue<double>("duration")),
			CCEaseBackIn::create(CCMoveTo::create(0.5f, ccp(bg->getPositionX(), -51.f))),
			nullptr
		));
		return false;
	}

	scheduleUpdate(); // calls update(float dt) every frame
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	
	bg = CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
	bg->setColor({0, 0, 0});
	bg->setOpacity(170);
	bg->setPosition(500, -50);
	bg->setContentSize({ 100, 80 });
	bg->setID("bg");

	musicLabel = CCLabelBMFont::create("a", "bigFont.fnt");
	musicLabel->setScale(0.45f);
	musicLabel->setAnchorPoint({ 0.f, 0.5f });
	musicLabel->setPosition(5, 52);
	musicLabel->ignoreAnchorPointForPosition(true);
	musicLabel->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);

	sfxLabel = CCLabelBMFont::create("a", "bigFont.fnt");
	sfxLabel->setScale(0.45f);
	sfxLabel->setAnchorPoint({ 0.f, 0.5f });
	sfxLabel->setPosition(5, 34.5);
	sfxLabel->ignoreAnchorPointForPosition(true);
	sfxLabel->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);

	bg->addChild(musicLabel);
	bg->addChild(sfxLabel);

	this->addChild(bg);
	this->setID("VolumeUI");

	CCScene::get()->addChild(this);

	bg->runAction(CCSequence::create(
		CCEaseBackOut::create(CCMoveTo::create(0.5f, ccp(bg->getPositionX(), 0.f))),
		CCDelayTime::create(Mod::get()->getSettingValue<double>("duration")),
		CCEaseBackIn::create(CCMoveTo::create(0.5f, ccp(bg->getPositionX(), -51.f))),
		nullptr
	));

	return true;
}

void VolumeUI::changeVolume(bool sfx, double v) {
	double getMusic = FMODAudioEngine::get()->getBackgroundMusicVolume() - v;
	double getSfx = FMODAudioEngine::get()->getEffectsVolume() - v;
	// doing it in int to prevent float inaccuracy
	if (getMusic < 0.f) getMusic = 0.f;
	else if (getMusic > 1.f) getMusic = 1.f;
	if (getSfx < 0.f) getSfx = 0.f;
	else if (getSfx > 1.f) getSfx = 1.f;

	(sfx)
		? FMODAudioEngine::sharedEngine()->setEffectsVolume(std::fabs(getSfx))
		: FMODAudioEngine::sharedEngine()->setBackgroundMusicVolume(std::fabs(getMusic));
}

void VolumeUI::update(float dt) {
	musicLabel->setString(fmt::format("Music: {}%", std::round(FMODAudioEngine::sharedEngine()->m_musicVolume * 100)).c_str());
	sfxLabel->setString(fmt::format("Sfx: {}%", std::round(FMODAudioEngine::sharedEngine()->m_sfxVolume * 100)).c_str());

	if (bg->getPositionY() == -51)
		this->removeFromParentAndCleanup(true);
}