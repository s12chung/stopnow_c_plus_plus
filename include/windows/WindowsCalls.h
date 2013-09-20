#ifndef WINDOWSCALLS_H_
#define WINDOWSCALLS_H_

namespace WindowsCalls {

//! Returns the user idle time of the machine in seconds
int getUserIdleTime();

//! Returns whether the user has reached an idle state
bool isUserIdle(unsigned int secondsUntilIdle);

}

#endif /* WINDOWSCALLS_H_ */
