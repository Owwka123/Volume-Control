#include <Geode/Geode.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/CCMouseDispatcher.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <cocos2d.h>

#include "ui.hpp"

using namespace geode::prelude;

VolumeUI volume;
CCKeyboardDispatcher keybord;

class $modify(AppDelegate) {
	// alt tab fix
	void applicationWillEnterForeground() {
		CCKeyboardDispatcher::get()->updateModifierKeys(false, false, false, false);
		
		AppDelegate::applicationWillEnterForeground();
	}
};
class $modify(CCMouseDispatcher) {
	bool dispatchScrollMSG(float x, float y) {
		if (x < 0.f && CCKeyboardDispatcher::get()->getAltKeyPressed()) {
			VolumeUI::create();
			(CCKeyboardDispatcher::get()->getControlKeyPressed())
				? volume.changeVolume(true, (!CCKeyboardDispatcher::get()->getShiftKeyPressed()) ? -.1 : -.01)
				: volume.changeVolume(false, (!CCKeyboardDispatcher::get()->getShiftKeyPressed()) ? -.1 : -.01);
			return false;
		}
		else if (x > 0.f && CCKeyboardDispatcher::get()->getAltKeyPressed()) {
			VolumeUI::create();
			(CCKeyboardDispatcher::get()->getControlKeyPressed())
				? volume.changeVolume(true, (!CCKeyboardDispatcher::get()->getShiftKeyPressed()) ? .1 : .01)
				: volume.changeVolume(false, (!CCKeyboardDispatcher::get()->getShiftKeyPressed()) ? .1 : .01);
			return false;
		}

		return CCMouseDispatcher::dispatchScrollMSG(x, y);
	}
};