#define CAC_PROJ_NAME "Trigger Thing"
#include <CacKit>
#include <iostream>
#define DREF(exp) (*((uint64_t*)(exp)))

template <typename T>
std::string to_string(T val) {
    std::stringstream stream;
    stream << val;
    return stream.str();
}

using namespace cocos2d;

namespace CocosHelper {
    template <typename T, typename F, typename E>
    void createIDTextInput(T, F, double, double, const char *, CCTextInputNode **, E, E);
    template <typename T, typename F, typename E>
    void createTogglerInput(T, F, double, double, double, const char *, CCLabelBMFont **, CCMenuItemToggler **, E);
    template <typename T, typename F, typename E>
    void createOKButton(T, F, double, double, E);
    template <typename T, typename F>
    void createTitle(T, F, double, double, const char *);
    template <typename T, typename F>
    void createBackdrop(T, F, double, double, double, double);
}

namespace CompareTriggerObject {
    constexpr int objectID = 3201;

    enum CompareMode {
        Smaller,
        Equals,
        Larger
    };

    template <typename T, typename F>
    void renderCustomUI(T self, F mainLayer);
    

    class SetupComparePopup : public FLAlertLayer {
    public:
        SetupComparePopup() : FLAlertLayer() {
            m_controlConnected = -1;
            m_joystickConnected = -1;
            m_ZOrder = 0;
            m_noElasticity = true;
            m_touchTriggeredCheckbox = nullptr;
            m_spawnTriggeredCheckbox = nullptr;
            m_multiTriggerCheckbox = nullptr;
            m_multiTriggerLabel = nullptr;
            m_multiTriggerLabel = nullptr;
            m_smallerCheckbox = nullptr;
            m_equalsCheckbox = nullptr;
            m_largerCheckbox = nullptr;
            m_activateCheckbox = nullptr;
        }
        static SetupComparePopup* create(EffectGameObject* ego) {
            SetupComparePopup* scp = new SetupComparePopup();
            uint64_t ok1 = DREF(scp);
            uint64_t ok2 = DREF(ok1-8)+16;

            uint64_t n_typinfo = getBase()+0x65d870; // FLAlertLayer vtable or some shit
            MemoryContainer(ok2, 8, reinterpret_cast<char*>(&n_typinfo)).enable();
            if (scp && scp->init(ego)) {
                scp->m_scene = NULL;
                scp->autorelease();
            } else {
                CC_SAFE_DELETE(scp);
            }
            return scp;
        }
        void onClose(CCObject* callback) {
            auto itemA = std::atoi(m_itemAInput->getString_s());
            auto itemB = std::atoi(m_itemBInput->getString_s());
            auto targetGroup = std::atoi(m_targetIDInput->getString_s());
            m_effectObject->_itemBlockID() = itemA;
            m_effectObject->_itemBlockBID() = itemB;
            m_effectObject->_targetGroup() = targetGroup;

            m_helper->keyBackClicked();
            setKeypadEnabled(false);
            setTouchEnabled(false);
            removeFromParentAndCleanup(true);
        }
        virtual void keyBackClicked() {
            onClose(NULL);
        }

        void onTouchTriggered(CCObject*) {
            m_effectObject->_touchTriggered() = !m_effectObject->_touchTriggered();
            m_effectObject->_spawnTriggered() = false;
            m_multiTriggerCheckbox->setVisible(false);
            m_multiTriggerLabel->setVisible(false);
            m_spawnTriggeredCheckbox->toggle(false);
        }
        void onSpawnTriggered(CCObject*) {
            m_effectObject->_touchTriggered() = false,
            m_effectObject->_spawnTriggered() = !m_effectObject->_spawnTriggered();
            m_multiTriggerCheckbox->setVisible(m_effectObject->_spawnTriggered());
            m_multiTriggerLabel->setVisible(m_effectObject->_spawnTriggered());
            m_touchTriggeredCheckbox->toggle(false);
        }
        void onMultiTrigger(CCObject*) {
            m_effectObject->_multiTrigger() = !m_effectObject->_multiTrigger();
        }
        void onDecreaseItemA(CCObject*) {
            auto itemA = std::atoi(m_itemAInput->getString_s());
            if (itemA > 1) --itemA;
            m_effectObject->_itemBlockID() = itemA;
            m_itemAInput->setString(to_string(itemA));
        }
        void onIncreaseItemA(CCObject*) {
            auto itemA = std::atoi(m_itemAInput->getString_s());
            if (itemA < 999) ++itemA;
            m_effectObject->_itemBlockID() = itemA;
            m_itemAInput->setString(to_string(itemA));
        }
        void onDecreaseItemB(CCObject*) {
            auto itemB = std::atoi(m_itemBInput->getString_s());
            if (itemB > 1) --itemB;
            m_effectObject->_itemBlockBID() = itemB;
            m_itemBInput->setString(to_string(itemB));
        }
        void onIncreaseItemB(CCObject*) {
            auto itemB = std::atoi(m_itemBInput->getString_s());
            if (itemB < 999) ++itemB;
            m_effectObject->_itemBlockBID() = itemB;
            m_itemBInput->setString(to_string(itemB));
        }
        void onDecreaseTargetID(CCObject*) {
            auto targetID = std::atoi(m_targetIDInput->getString_s());
            if (targetID > 1) --targetID;
            m_effectObject->_targetGroup() = targetID;
            m_targetIDInput->setString(to_string(targetID));
        }
        void onIncreaseTargetID(CCObject*) {
            auto targetID = std::atoi(m_targetIDInput->getString_s());
            if (targetID < 999) ++targetID;
            m_effectObject->_targetGroup() = targetID;
            m_targetIDInput->setString(to_string(targetID));
        }

        void onSmaller(CCObject*) {
            m_effectObject->_compareType() = (int)CompareMode::Smaller;
            m_equalsCheckbox->toggle(false);
            m_largerCheckbox->toggle(false);
        }

        void onEquals(CCObject*) {
            m_effectObject->_compareType() = (int)CompareMode::Equals;
            m_smallerCheckbox->toggle(false);
            m_largerCheckbox->toggle(false);
        }

        void onLarger(CCObject*) {
            m_effectObject->_compareType() = (int)CompareMode::Larger;
            m_smallerCheckbox->toggle(false);
            m_equalsCheckbox->toggle(false);
        }

        void onActivateGroup(CCObject*) {
            m_effectObject->_activateGroup() = !m_effectObject->_activateGroup();
        }

        void restoreDefaults() {
            std::string s;
            s = to_string(m_effectObject->_itemBlockID());
            if (s.length() > 0 && m_effectObject->_itemBlockID() != 0) m_itemAInput->setString(s);
            s = to_string(m_effectObject->_itemBlockBID());
            if (s.length() > 0 && m_effectObject->_itemBlockBID() != 0) m_itemBInput->setString(s);
            s = to_string(m_effectObject->_targetGroup());
            if (s.length() > 0 && m_effectObject->_targetGroup() != 0) m_targetIDInput->setString(s);

            if (m_touchTriggeredCheckbox) m_touchTriggeredCheckbox->toggle(m_effectObject->_touchTriggered());
            if (m_spawnTriggeredCheckbox) m_spawnTriggeredCheckbox->toggle(m_effectObject->_spawnTriggered());
            if (m_multiTriggerCheckbox) m_multiTriggerCheckbox->toggle(m_effectObject->_multiTrigger());
            if (m_multiTriggerCheckbox) m_multiTriggerCheckbox->setVisible(m_effectObject->_spawnTriggered());
            if (m_multiTriggerLabel) m_multiTriggerLabel->setVisible(m_effectObject->_spawnTriggered());

            if (m_smallerCheckbox) m_smallerCheckbox->toggle(m_effectObject->_compareType() == CompareMode::Smaller);
            if (m_equalsCheckbox) m_equalsCheckbox->toggle(m_effectObject->_compareType() == CompareMode::Equals);
            if (m_largerCheckbox) m_largerCheckbox->toggle(m_effectObject->_compareType() == CompareMode::Larger);

            if (m_activateCheckbox) m_activateCheckbox->toggle(m_effectObject->_activateGroup());
        }

        bool init(EffectGameObject* ego) {

            if (CCLayerColor::initWithColor(ccc4(0,0,0,150))) {
                m_effectObject = ego;

                m_helper = FLAlertLayer::create("","","");
                m_helper->m_buttonMenu->setVisible(false);
                m_helper->m_noElasticity = true;
                m_helper->show();

                registerWithTouchDispatcher();
                CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);

                setTouchEnabled(true);
                setKeypadEnabled(true);

                m_mainLayer = CCLayer::create();
                addChild(m_mainLayer);
                renderCustomUI(this, m_mainLayer);
                restoreDefaults();
            }
            return true;
        }
    protected:
        template <typename T, typename F, typename E>
        friend void CocosHelper::createIDTextInput(T, F, double, double, const char *, CCTextInputNode **, E, E);
        template <typename T, typename F, typename E>
        friend void CocosHelper::createTogglerInput(T, F, double, double, double, const char *, CCLabelBMFont **, CCMenuItemToggler **, E);
        template <typename T, typename F, typename E>
        friend void CocosHelper::createOKButton(T, F, double, double, E);
        template <typename T, typename F>
        friend void CocosHelper::createTitle(T, F, double, double, const char *);
        template <typename T, typename F>
        friend void createBackdrop(T, F, double, double, double, double);
        template <typename T, typename F>
        friend void renderCustomUI(T, F);

        EffectGameObject *m_effectObject;
        CCTextInputNode *m_itemAInput;
        CCTextInputNode *m_itemBInput;
        CCTextInputNode *m_targetIDInput;
        FLAlertLayer *m_helper;

        CCMenuItemToggler *m_touchTriggeredCheckbox;
        CCMenuItemToggler *m_spawnTriggeredCheckbox;
        CCMenuItemToggler *m_multiTriggerCheckbox;
        CCLabelBMFont *m_multiTriggerLabel;

        CCMenuItemToggler *m_activateCheckbox;

        CCMenuItemToggler *m_smallerCheckbox;
        CCMenuItemToggler *m_equalsCheckbox;
        CCMenuItemToggler *m_largerCheckbox;
    };

    template <typename T, typename F>
    void renderCustomUI(T self, F mainLayer) {
        typedef void (CCObject::*menusel)(CCObject *);

        CocosHelper::createBackdrop(self, mainLayer, 50.0, 50.0, 300.0, 280.0);

        CocosHelper::createTitle(self, mainLayer, 0, 120.0, "Compare Trigger");

        CocosHelper::createOKButton(self, mainLayer, 0, -112.0, &SetupComparePopup::onClose);

        CocosHelper::createIDTextInput(self, mainLayer, -72.0, 64.0, "ItemA ID", &self->m_itemAInput, &SetupComparePopup::onDecreaseItemA, &SetupComparePopup::onIncreaseItemA);
        CocosHelper::createIDTextInput(self, mainLayer, 72.0, 64.0, "ItemB ID", &self->m_itemBInput, &SetupComparePopup::onDecreaseItemB, &SetupComparePopup::onIncreaseItemB);
        CocosHelper::createIDTextInput(self, mainLayer, -72.0, 0, "Target ID", &self->m_targetIDInput, &SetupComparePopup::onDecreaseTargetID, &SetupComparePopup::onIncreaseTargetID);

        CocosHelper::createTogglerInput(self, mainLayer, -120.0, 48.0, -108.0, "Spawn   \nTriggered", nullptr, &self->m_spawnTriggeredCheckbox, &SetupComparePopup::onSpawnTriggered);
        CocosHelper::createTogglerInput(self, mainLayer, -120.0, 47.0, -72.0, "Touch   \nTriggered", nullptr, &self->m_touchTriggeredCheckbox, &SetupComparePopup::onTouchTriggered);
        CocosHelper::createTogglerInput(self, mainLayer, 72, 43.0, -108.0, "Multi   \nTrigger", &self->m_multiTriggerLabel, &self->m_multiTriggerCheckbox, &SetupComparePopup::onMultiTrigger);

        CocosHelper::createTogglerInput(self, mainLayer, 48.0, 50.0, 0, "Activate   \nGroup", nullptr, &self->m_activateCheckbox, &SetupComparePopup::onActivateGroup);
        CocosHelper::createTogglerInput(self, mainLayer, -120.0, 43.0, -40.0, "Smaller", nullptr, &self->m_smallerCheckbox, &SetupComparePopup::onSmaller);
        CocosHelper::createTogglerInput(self, mainLayer, -24.0, 40.0, -40.0, "Equals", nullptr, &self->m_equalsCheckbox, &SetupComparePopup::onEquals);
        CocosHelper::createTogglerInput(self, mainLayer, 72.0, 40.0, -40.0, "Larger", nullptr, &self->m_largerCheckbox, &SetupComparePopup::onLarger);
    }

    std::string objectToString(GameObject *go, std::string st) {
        auto ego = reinterpret_cast<EffectGameObject *>(go);

        if (ego->_itemBlockID() > 0) st += ",80," + to_string(ego->_itemBlockID());
        if (ego->_itemBlockBID() > 0) st += ",95," + to_string(ego->_itemBlockBID());
        if (ego->_targetGroup() > 0) st += ",51," + to_string(ego->_targetGroup());
        if (ego->_compareType() > 0) st += ",88," + to_string(ego->_compareType());
        if (ego->_activateGroup() > 0) st += ",56," + to_string(ego->_activateGroup());
        // std::cout << st << "?2\n\n";
        return st;
    }

    GameObject* objectFromString(GameObject* go, std::string st) {
        auto ego = reinterpret_cast<EffectGameObject *>(go);
        auto mp = GameToolbox::stringSetupToMap(st, ",");

        // for (auto const &pair : mp) {
        //     std::cout << "{" << pair.first << ": " << pair.second << "}\n";
        // }

        if (mp.count("80")) ego->_itemBlockID() = std::atoi(mp["80"].c_str());
        if (mp.count("95")) ego->_itemBlockBID() = std::atoi(mp["95"].c_str());
        if (mp.count("51")) ego->_targetGroup() = std::atoi(mp["51"].c_str());
        if (mp.count("56") && mp["56"] == "1") ego->_activateGroup() = true;
        if (mp.count("88")) ego->_compareType() = std::atoi(mp["88"].c_str());

        return go;
    }

    void callback(GameObject *self, GJBaseGameLayer *gameLayer) {
        int *itemValues = &(gameLayer->_effectManager()->_itemValues());
        auto ego = reinterpret_cast<EffectGameObject *>(self);
        int itemA = ego->_itemBlockID();
        int itemACount = itemValues[itemA];
        int itemB = ego->_itemBlockBID();
        int itemBCount = itemValues[itemB];
        switch ((CompareMode)ego->_compareType()) {
            case CompareMode::Smaller:
                if (itemACount < itemBCount) gameLayer->toggleGroupTriggered(ego->_targetGroup(), ego->_activateGroup());
                break;
            case CompareMode::Equals:
                if (itemACount == itemBCount) gameLayer->toggleGroupTriggered(ego->_targetGroup(), ego->_activateGroup());
                break;
            case CompareMode::Larger:
                if (itemACount > itemBCount) gameLayer->toggleGroupTriggered(ego->_targetGroup(), ego->_activateGroup());
                break;
        }
    }

    void editPopup(EditorUI *self) {
        auto obs = self->getSelectedObjects();
        auto ob = self->_lastSelectedObject();
        if (obs->count() == 1 && ob->_id() == CompareTriggerObject::objectID) {
            CompareTriggerObject::SetupComparePopup::create(reinterpret_cast<EffectGameObject*>(ob))->show();
        }
    }
}

#include "cocos_helper.hpp"

void inject() {

    m = new ModContainer(CAC_PROJ_NAME);

    // void(*rAss)(LoadingLayer*) = +[](LoadingLayer* self) {
    //     CCTextureCache::sharedTextureCache()->addImage("GJ_square05.png", false);
    //     ORIG(*rAss, 0x1dfb20)(self);
    // };
    // m->registerHook(getBase()+0x1dfb20, rAss);

    auto editor = Cacao::EditorUIEditor::create(m);
    // screw custom texture
    // CCTextureCache::sharedTextureCache()->addImage("GJ_triggerAdditions.png", false);
    // CCTextureCache::sharedTextureCache()->addImage("GJ_square05-uhd.png", false);
    // CCTextureCache::sharedTextureCache()->addImage("GJ_triggerAdditions-uhd.png", false);
    
    // auto frameCache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    // frameCache->addSpriteFramesWithFile("GJ_triggerAdditions.plist");
    Cacao::addGDObject("edit_eCounterBtn_001.png", CompareTriggerObject::objectID);

    editor
        ->bar(11)
        ->addIndex(12, CompareTriggerObject::objectID)
        ->addObjectsToGameSheet02(CompareTriggerObject::objectID)
        ->addEffectObjects(CompareTriggerObject::objectID)
        ->addTriggerCallback(CompareTriggerObject::objectID, CompareTriggerObject::callback)
        ->addEditPopup(CompareTriggerObject::objectID, CompareTriggerObject::editPopup)
        ->addSaveString(CompareTriggerObject::objectID, CompareTriggerObject::objectFromString, CompareTriggerObject::objectToString);

    editor->applyAll();

    m->enable();
}
