#pragma once
#include "EmotionAnalyzer.h"

class UserStateObserver
{
public:
	virtual ~UserStateObserver() {};
	virtual void update(AsisstantAction action) = 0;
};

class UserStateSubject {
public:
	virtual ~UserStateSubject() {};
	virtual void attach(UserStateObserver* observer) = 0;
	virtual void detach() = 0;
	virtual void notify() = 0;
};