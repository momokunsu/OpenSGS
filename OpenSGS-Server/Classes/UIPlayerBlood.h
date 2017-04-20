#pragma once
#include "cocos2d.h"


class UIPlayerBlood : public cocos2d::Layer
{
	public:
		virtual void setMaxLifePiont(int p) = 0;
		inline int getMaxLifePiont() const{return _maxhp;}

		virtual void setLifePiont(int p) = 0;
		inline int getLifePiont() const{return _hp;}

		inline void addLifePiont(int p = 1){setLifePiont(_hp+p);}
		inline void subLifePiont(int p = 1){setLifePiont(_hp-p);}
		inline void addMaxLifePiont(int p = 1){setMaxLifePiont(_maxhp+p);}
		inline void subMaxLifePiont(int p = 1){setMaxLifePiont(_maxhp-p);}
		inline void RecoverLife(){setLifePiont(_maxhp);}

	protected:
		cocos2d::DrawNode *_stencil;
		cocos2d::ClippingNode *_bloodclip;
		cocos2d::Node *_bloodinfo;

		int _hp, _maxhp; 
};

class UIMainPlayerBlood : public UIPlayerBlood
{
	public:
		virtual void setMaxLifePiont(int p);
		virtual void setLifePiont(int p);

		CREATE_FUNC(UIMainPlayerBlood)

	private:
		UIMainPlayerBlood(void);
		virtual ~UIMainPlayerBlood(void);

		static float _ALLROLL;
		static cocos2d::Sprite *_greenblood;
		static cocos2d::Sprite *_yellowblood;
		static cocos2d::Sprite *_redblood;

		virtual bool init();
		void _setBloodNum(int blood, int maxblood);
};

class UIOtherPlayerBlood : public UIPlayerBlood
{
	public:
		virtual void setMaxLifePiont(int p);
		virtual void setLifePiont(int p);

		CREATE_FUNC(UIOtherPlayerBlood)

	private:
		UIOtherPlayerBlood(void);
		virtual ~UIOtherPlayerBlood(void);
		
		void _setBloodNum(int blood, int maxblood);
		virtual bool init();

		static cocos2d::Sprite *_greenblood;
		static cocos2d::Sprite *_yellowblood;
		static cocos2d::Sprite *_redblood;
};