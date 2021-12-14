#pragma once
#include "singleton.h"


struct Event
{
	EVENT_TYPE event_type;
	DWORD_PTR lParam;
	DWORD_PTR wParam;
};

class CGameObject;

class CEventManager :
    public CSingleton<CEventManager>
{
	SINGLE(CEventManager);
public:
	void Update();

public:
	void AddEvent(const Event& _event) { event_vector_.push_back(_event); }
	bool HasOccuredObjectEvent() { return occured_object_event_; }
private:
	void excute(const Event& _event);
private:
	vector<Event> event_vector_;
	vector<CGameObject*> dead_object_vector_;
	bool occured_object_event_;
};

