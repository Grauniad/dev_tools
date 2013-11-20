template<class T>
rTask<T>& rTask<T>::operator >> (T& item) {
    switch(this->policy) {
    case TASK_POLICY_DEDICATED:
    case TASK_POLICY_IMMEDIATE:
    case TASK_POLICY_STANDARD:
        this->resultStream >> item;
        return *this;
    case TASK_POLICY_ON_DEMAND:
        if ( !this->Started() ) {
            Task_Master::Scheduler().StartDedicated(*this);
        }
        this->resultStream >> item;
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
        if ( !(*this)->Started() ) {
            Task_Master::Scheduler().StartDedicated(*this);
        }
        return (*this)->resultStream;
    }
    LOG_FROM( LOG_ERROR, 
              "Task::operator>>", 
              "Hit non existent default branch, nothing read from the queue!"
            )
    return (*this)->resultStream;
}
