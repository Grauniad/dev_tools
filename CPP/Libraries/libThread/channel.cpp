#include "channel.h"

long ChannelId() {
    static long nextId =0;
    nextId +=1;
    return nextId;
}
