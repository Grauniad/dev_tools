#ifndef __FOREACH_IN_TUPLE_H__
#define __FOREACH_IN_TUPLE_H__

#include <tuple>
#include <string>
#include "tokenizor.h"

namespace TupleForEach {

    /*
     * A trivial loop over every item in a tuple...
     */
    template<size_t N, class...Tn> 
    class TupleForEach {
    public:
        static int Loop(Tokens& input, std::tuple<Tn...>& data) {
            int ret = TupleForEach<N-1,Tn...>::Loop(input,data);
            if ( ret == 1 ) {
                ret = input.Get(N,std::get<N>(data));
            }
            return ret;
        }
    };
    
    template<class...Tn> 
    class TupleForEach<0,Tn...> {
    public:
        static int Loop(Tokens& input, std::tuple<Tn...>& data) {
            return input.Get(0,std::get<0>(data));
        }
    };

    template<class...Tn> 
    int PopulateTuple(Tokens& input, std::tuple<Tn...>& data) {
        return TupleForEach<sizeof...(Tn)-1,Tn...>::Loop(input,data);
    }
}


#endif
