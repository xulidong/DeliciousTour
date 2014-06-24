LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BagData.cpp \
		   ../../Classes/BaseFood.cpp \
		   ../../Classes/BaseState.cpp \
		   ../../Classes/CommonFunction.cpp \
		   ../../Classes/DropState.cpp \
		   ../../Classes/FSM.cpp \
		   ../../Classes/FlashScene.cpp \
		   ../../Classes/FoodSprite.cpp \
		   ../../Classes/LifeManager.cpp \
		   ../../Classes/LostState.cpp \
		   ../../Classes/MainScene.cpp \
		   ../../Classes/MapReader.cpp \
		   ../../Classes/PlayLayer.cpp \
		   ../../Classes/PlayScene.cpp \
		   ../../Classes/PlayerData.cpp \
		   ../../Classes/PreOverState.cpp \
		   ../../Classes/Props.cpp \
		   ../../Classes/ReadyState.cpp \
		   ../../Classes/RemoveState.cpp \
		   ../../Classes/SoundManager.cpp \
		   ../../Classes/TextManager.cpp \
		   ../../Classes/TimeManager.cpp \
		   ../../Classes/WelcomeScene.cpp \
		   ../../Classes/WinState.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/extensions

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
# $(call import-module,network)
$(call import-module,extensions)
