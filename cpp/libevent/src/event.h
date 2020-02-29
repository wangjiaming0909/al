#pragma once
#include "cstdint"
#include "utils.h"
#include <list>
#include "boost/intrusive/list_hook.hpp"

namespace libevent
{	
	using boost_list_base_hook_t = 
		boost::intrusive::list_base_hook<
			boost::intrusive::link_mode<boost::intrusive::auto_unlink>
		>;
class Event
{
public:
	enum Event_t : uint8_t
	{
		EV_TIMEOUT = 0x01,
		EV_READ = 0x02,
		EV_WRITE = 0x04,
		EV_SIGNAL = 0x08,
		EV_PERSIST = 0x10,
		EV_ET = 0x20
	};
	struct EventCallback : public boost_list_base_hook_t
	{
		using Callback = void (socket_t, short, void*);
		using SelfCB = void(EventCallback*, void*);
		short flags;
		uint8_t pri;
		uint8_t closure;
		union {
			Callback cb;
			SelfCB Scb;
		}cb_union;
		void* args;
	};
public:
	Event();
	~Event();
public:


private:
	EventCallback callback_;
	short ev_events_;
	short ev_res_;
	//Base* base_;
};
}