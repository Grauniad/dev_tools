template<class T>
rTask<T>& rTask<T>::operator >> (T& item) {
    switch((*this)->policy) {
    case TASK_POLICY_DEDICATED:
    case TASK_POLICY_IMMEDIATE:
    case TASK_POLICY_STANDARD:
        (*this)->resultStream >> item;
        return *this;
    case TASK_POLICY_ON_DEMAND:
        /*
         * We have to be sure no-one has started us...
         * 
         * This extra lock is the reason ON_DEMAN is not the 
         * default mode...
         */
        (*this)->Lock();
        if ( !(*this)->Started() ) {
            (*this)->Unlock();
            Task_Master::Scheduler().StartDedicated(*this);
        } else {
            (*this)->Unlock();
        }

        (*this)->resultStream >> item;
        return *this;
    }
    LOG_FROM( LOG_ERROR, 
              "rTask::operator>>", 
              "Hit non existent default branch, nothing read from the queue!"
            )
    return *this;
}

template<class T>
Channel<T>& rTask<T>::Results() {
    switch((*this)->policy) {
    case TASK_POLICY_DEDICATED:
    case TASK_POLICY_IMMEDIATE:
    case TASK_POLICY_STANDARD:
        return (*this)->resultStream;
    case TASK_POLICY_ON_DEMAND:
        /*
         * We have to be sure no-one has started us...
         * 
         * This extra lock is the reason ON_DEMAND is not the 
         * default mode...
         */
        (*this)->Lock();
        if ( !(*this)->Started() ) {
            (*this)->Unlock();
            Task_Master::Scheduler().StartDedicated(*this);
        } else {
            (*this)->Unlock();
        }
        return (*this)->resultStream;
    }
    LOG_FROM( LOG_ERROR, 
              "Task::operator>>", 
              "Hit non existent default branch, nothing read from the queue!"
            )
    return (*this)->resultStream;
}
