#ifndef __FOREACH_IN_TUPLE_H__
#define __FOREACH_IN_TUPLE_H__

#include <tuple>
#include <string>
#include <sstream>

namespace TupleForEach {

    /*
     * A trivial loop over every item in a tuple...
     */
    template<size_t N, class...Tn> 
    class TupleForEach {
    public:
        static int Loop(std::stringstream& input, std::tuple<Tn...>& data) {
            int ret = TupleForEach<N-1,Tn...>::Loop(input,data);
            if ( ret == 1 ) {
                input >> std::get<N>(data);
                ret = !input.fail();
            }
            return ret;
        }
    };
    
    template<class...Tn> 
    class TupleForEach<0,Tn...> {
    public:
        static int Loop(std::stringstream& input, std::tuple<Tn...>& data) {
            input >> std::get<0>(data);
            return !input.fail();
        }
    };

    template<class...Tn> 
    int PopulateTuple(std::stringstream& input, std::tuple<Tn...>& data) {
        return TupleForEach<sizeof...(Tn)-1,Tn...>::Loop(input,data);
    }
}


#endif
