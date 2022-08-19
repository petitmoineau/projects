#include "ActReactTranslator.h"

ActReactTranslator::~ActReactTranslator()
{
	analyzer.detach();
	delete action;
}

void ActReactTranslator::setAnalyzer(EmotionAnalyzer& analyzer)
{
	analyzer.attach(this);
}

void ActReactTranslator::update(AsisstantAction actionToDo)
{
	switch (actionToDo)
	{
	case AsisstantAction::cheerUp:
		this->action = "cheerUp";
		break;
	case AsisstantAction::keepMood:
		this->action = "keepMood";
		break;
	case AsisstantAction::takeBreak:
		this->action = "takeBreak";
		break;
	case AsisstantAction::userAway:
		this->action = "userAway";
		break;
	default:
		this->action = "nothing";
		break;
	};
}