#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

bool p1 = false;
bool p2 = false;
bool p3 = false;

class $modify(PauseLayerExt, PauseLayer) {

	void hide()
	{
		this->setVisible(false);

		getChildOfType<UILayer>(PlayLayer::get(), 0)->setVisible(false);

		CCArrayExt<CCNode*> objects = PlayLayer::get()->getChildren();

		for (auto* obj : objects) {
			if (obj->getID() != "main-node")
			{
				obj->setPosition(obj->getPosition() + ccp(10000, 10000));
			}
		}

		if (auto s = CCScene::get()->getChildByID("android-ball"))
		{
			s->setVisible(false);
		}

		if (Mod::get()->getSettingValue<bool>("hide-icons"))
		{
			auto plr = PlayLayer::get()->m_player1;
			plr->setVisible(false);
			plr->m_regularTrail->setVisible(false);
		}
	}

	void show()
	{
		this->setVisible(true);

		getChildOfType<UILayer>(PlayLayer::get(), 0)->setVisible(true);

		CCArrayExt<CCNode*> objects = PlayLayer::get()->getChildren();

		for (auto* obj : objects) {
			if (obj->getID() != "main-node")
			{
				obj->setPosition(obj->getPosition() - ccp(10000, 10000));
			}
		}

		if (auto s = CCScene::get()->getChildByID("android-ball"))
		{
			s->setVisible(true);
		}

		if (Mod::get()->getSettingValue<bool>("hide-icons"))
		{
			auto plr = PlayLayer::get()->m_player1;
			plr->setVisible(true);
			plr->m_regularTrail->setVisible(true);
		}
	}

	void onScreenshot(CCObject*)
	{
		CCDirector* director = CCDirector::sharedDirector();
		CCScene* scene = director->getRunningScene();
		//CCRenderTexture* renderTexture = CCRenderTexture::create(scene->getContentSize().width, scene->getContentSize().height);
		CCRenderTexture* renderTexture = CCRenderTexture::create(scene->getContentSize().width, scene->getContentSize().height);

		hide();

		renderTexture->begin();
		scene->visit();
		renderTexture->end();

		show();

		// Get the captured image
		CCImage* image = renderTexture->newCCImage();

		// Save the image to file
		if (image != nullptr) {
			auto path = Mod::get()->getConfigDir().string() + std::string("/") + numToString<int>(PlayLayer::get()->m_level->m_levelID.value(), 0) + std::string(".png");
			image->saveToFile(path.c_str());

			image->release();
		}
	}

	virtual void customSetup()
	{
		PauseLayer::customSetup();

		auto menu = CCMenu::create();
		menu->setPosition(ccp(60, CCDirector::get()->getWinSize().height / 2));
		menu->setAnchorPoint(ccp(0, 0));
		menu->setContentSize(ccp(0, 0));
		menu->setScale(0.5f);

		this->addChild(menu);

		auto ss = CCMenuItemSpriteExtra::create(ButtonSprite::create("Screenshot"), this, menu_selector(PauseLayerExt::onScreenshot));
		menu->addChild(ss);

		menu->setLayout(RowLayout::create()->setAutoScale(false)->setGap(30));
	}
};
