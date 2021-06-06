#ifndef __COCOS_HELPER_HPP__
#define __COCOS_HELPER_HPP__

using namespace cocos2d;

namespace CocosHelper {
	template <typename T, typename F, typename E>
	void createIDTextInput(T self, F mainLayer, double x, double y, const char *title, CCTextInputNode **objectToSet, E onDecrease, E onIncrease) {
		typedef void (CCObject::*menusel)(CCObject *);
		
		auto l_idText = CCLabelBMFont::create(title, "goldFont.fnt");

		auto p_idText = Cacao::addedPosition(x, y + 30.0);
		l_idText->setPosition(mainLayer->convertToNodeSpace(p_idText));
		l_idText->setScale(2.0 / 3.0);

		mainLayer->addChild(l_idText);

		auto s_idTextInput = extension::CCScale9Sprite::create("square02b_small.png");
		s_idTextInput->setOpacity(100);
		s_idTextInput->setColor(ccc3(0, 0, 0));
		s_idTextInput->setContentSize(CCSizeMake(50.0, 30.0));

		auto t_idTextInput = CCTextInputNode::create(50.0, 40.0, "0", "Thonburi", 24, "bigFont.fnt");
		t_idTextInput->setAllowedChars("0123456789");
		t_idTextInput->setMaxLabelScale(4.0 / 7.0);
		t_idTextInput->m_maxLabelLength = 3;

		auto p_idTextInput = Cacao::addedPosition(x, y);
		t_idTextInput->setPosition(p_idTextInput);
		s_idTextInput->setPosition(p_idTextInput);
		mainLayer->addChild(s_idTextInput);
		mainLayer->addChild(t_idTextInput);

		*objectToSet = t_idTextInput;

		auto m_idInputLeft = CCMenu::create();

		auto s_idInputLeft = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
		auto a_idInputLeft = CCMenuItemSpriteExtra::create(s_idInputLeft, s_idInputLeft, self, (SEL_MenuHandler)(onDecrease));

		auto p_idInputLeft = Cacao::addedPosition(x - 48.0, y);
		m_idInputLeft->setPosition(mainLayer->convertToNodeSpace(p_idInputLeft));
		s_idInputLeft->setScale(6.0 / 5.0);

		mainLayer->addChild(m_idInputLeft);
		m_idInputLeft->addChild(a_idInputLeft);

		auto m_idInputRight = CCMenu::create();

		auto s_idInputRight = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
		auto a_idInputRight = CCMenuItemSpriteExtra::create(s_idInputRight, s_idInputRight, self, (SEL_MenuHandler)(onIncrease));

		auto p_idInputRight = Cacao::addedPosition(x + 48.0, y);
		m_idInputRight->setPosition(mainLayer->convertToNodeSpace(p_idInputRight));
		s_idInputRight->setScale(6.0 / 5.0);

		mainLayer->addChild(m_idInputRight);
		m_idInputRight->addChild(a_idInputRight);
	}

	template <typename T, typename F, typename E>
	void createTogglerInput(T self, F mainLayer, double x, double dx, double y, const char *title, CCLabelBMFont **labelToSet, CCMenuItemToggler **togglerToSet, E onChange) {
		auto l_toggler = CCLabelBMFont::create(title, "bigFont.fnt");

		l_toggler->setPosition(mainLayer->convertToNodeSpace(Cacao::addedPosition(x + dx, y)));
		l_toggler->setScale(1.0 / 3.0);

		mainLayer->addChild(l_toggler);

		if (labelToSet != nullptr) *labelToSet = l_toggler;

		auto m_toggler = CCMenu::create();

		m_toggler->setPosition(mainLayer->convertToNodeSpace(Cacao::addedPosition(x, y)));

		auto t_toggler = Cacao::createToggler(self, (SEL_MenuHandler)onChange);
		t_toggler->setScale(4.0 / 5.0);

		mainLayer->addChild(m_toggler);
		m_toggler->addChild(t_toggler);

		if (togglerToSet != nullptr) *togglerToSet = t_toggler;
	}

	template <typename T, typename F, typename E>
	void createOKButton(T self, F mainLayer, double x, double y, E onClick) {
		// OK button 
		auto m_okButton = CCMenu::create();

		auto s_okButton = ButtonSprite::create("OK", 47.0, 1, 1, true);
		auto a_okButton = CCMenuItemSpriteExtra::create(s_okButton, s_okButton, self, (SEL_MenuHandler)onClick);

		auto p_okButton = Cacao::addedPosition(x, y);
		m_okButton->setPosition(p_okButton);
		s_okButton->setScale(9.0/10.0);

		mainLayer->addChild(m_okButton);
		m_okButton->addChild(a_okButton);
	}

	template <typename T, typename F>
	void createTitle(T self, F mainLayer, double x, double y, const char *title) {
		// Popup title
		auto l_popupTitle = CCLabelBMFont::create(title, "goldFont.fnt");
		auto p_popupTitle = Cacao::addedPosition(x, y);
		l_popupTitle->setPosition(mainLayer->convertToNodeSpace(p_popupTitle));
		l_popupTitle->setScale(4.0 / 5.0);

		mainLayer->addChild(l_popupTitle);
	}

	template <typename T, typename F>
	void createBackdrop(T self, F mainLayer, double x, double y, double w, double h) {
		auto s_backdrop = extension::CCScale9Sprite::create("GJ_square01.png");
        s_backdrop->setContentSize(CCSizeMake(w, h));
        s_backdrop->setPosition(Cacao::relativePosition(x,y));
        mainLayer->addChild(s_backdrop, -2);
	}
}

#endif
