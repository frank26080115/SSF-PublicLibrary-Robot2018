#ifndef _BOOKWORMSELFBALANCE_H_
#define _BOOKWORMSELFBALANCE_H_

class cBookWormSelfBalance
{
	public:
		bool init(void);
		void selfBalance(signed int throttle = 0, signed int steer = 0);
};

#endif