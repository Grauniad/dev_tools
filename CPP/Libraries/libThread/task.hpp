// Default behaviour
template<class T, TASK_POLICY POLICY>
Task& Task<T,POLICY>::operator >> (T& item); {
    resultStream >> item;
    return *this;
}

// On-demand case
template<class T>
Task& Task<T,TASK_POLICY_ON_DEMAND>::operator >> (T& item); {
    if ( !Started() ) {
        Task_Master::Scheduler().StartDedicated(this);
    }
    resultStream >> item;
    return *this;
}
