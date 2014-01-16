/*
 * $FU-Copyright$
 */

#include "StandardClock.h"
#include "StandardTime.h"
#include "StandardTimer.h"

ARA_NAMESPACE_BEGIN

Time* StandardClock::makeTime(){
    return new StandardTime();
}

//TimerPtr StandardClock::getNewTimer(char timerType, void* contextObject){
Timer* StandardClock::getNewTimer(TimerType timerType, void* contextObject){
//    return std::make_shared<StandardTimer>(timerType, contextObject);
    return new StandardTimer(timerType, contextObject);
}

ARA_NAMESPACE_END
