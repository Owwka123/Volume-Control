#pragma once
#include <Geode/Geode.hpp>


using namespace geode::prelude;

class VolumeUI : public CCLayer {
protected:
	virtual bool init();

	CCScale9Sprite* bg;
	CCLabelBMFont* musicLabel;
	CCLabelBMFont* sfxLabel;

public:
	static VolumeUI* create();
	void changeVolume(bool sfx, double v);
	void update(float dt);
};