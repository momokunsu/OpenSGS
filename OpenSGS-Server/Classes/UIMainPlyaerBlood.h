#pragma once
#include "cocos2d.h"


class UIMainPlyaerBlood : public cocos2d::Layer
{
	public:
		UIMainPlyaerBlood(void);
		virtual ~UIMainPlyaerBlood(void);

		void setMaxLifePiont(int p);
		inline int getMaxLifePiont() const{return _maxhp;}

		void setLifePiont(int p);
		inline int getLifePiont() const{return _hp;}


		CREATE_FUNC(UIMainPlyaerBlood)

	private:
		virtual bool init();
		void _setBloodNum(int blood, int maxblood);

		static float _ALLROLL;

		cocos2d::Sprite *_blood;
		cocos2d::DrawNode *_stencil;
		cocos2d::ClippingNode *_bloodclip;
		cocos2d::Node *_bloodinfo;

		int _hp, _maxhp; 
};

