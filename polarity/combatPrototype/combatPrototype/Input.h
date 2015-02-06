#ifndef _Input_h_
#define _Input_h_

// TODO: can we get rid of input class?
//		 - later, how will we handle remapping of keys and hotkeys and such?
//		 - maybe just make this a friend class of Application?
class Input
{
public:
	Input() {}
	virtual ~Input() {}

	bool handleInput();
};

#endif // _Input_h_