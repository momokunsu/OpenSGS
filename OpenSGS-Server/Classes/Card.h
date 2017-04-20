#pragma once

#include "cocos2d.h"
#include "GameDef.h"
#include "SmartPtr.h"

class Card : public SmartPtr
{
	public:
		Card(void);
		virtual ~Card(void);

		inline const char *getName() const{return _name.c_str();}
		inline void setName(const char *name){_name = name;};

		inline eCardType getType() const{return _type;}
		inline void setType(eCardType type){_type = type;};

		inline std::string *getScript() const{return _script;}
		inline void setScript(std::string *scr){_script = scr;}
		
		inline ulong getUseTime() const{return _usetime;}
		inline void setUseTime(ulong time){_usetime = time;}

		inline bool isOffical() const{return _isoffical;}
		inline void setOfficalState(bool flag){_isoffical = flag;}

	protected:
		std::string _name;
		std::string *_script;
		ulong _usetime;
		eCardType _type;
		bool _isoffical;

	private:
};

