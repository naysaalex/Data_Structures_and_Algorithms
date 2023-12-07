#include "executable.h"

using namespace std;

TEST(access_operator) {
    Typegen t;
    for(size_t i = 0; i < TEST_ITER; i++) {
        
        size_t n_pairs = t.range(1000ul);
        std::vector<std::pair<double, double>> pairs(n_pairs);
        t.fill(pairs.begin(), pairs.end());
        //cout<<"1"<<endl;
        size_t n = t.range(100ull);
        UnorderedMap<double, double> map(n);
        //cout<<"2"<<endl;
        shadow_map<double, double> shadow_map(n);
        //cout<<"3"<<endl;
    
        for(auto const & pair : pairs) {
            shadow_map.insert(pair);
            //cout<<"1"<<endl;
            {
                Memhook mh;
                map[pair.first] = pair.second;
                ASSERT_EQ(1ULL, mh.n_allocs());
                ASSERT_EQ(0ULL, mh.n_frees());
                //cout<<"2"<<endl;
            }
            //cout<<"3"<<endl;
            ASSERT_EQ(pair.second, map[pair.first]);
            // cout<<shadow_map<<endl;
            //cout<<"4"<<endl;//map<<endl;
            //print_map(map, cout);
            ASSERT_PAIRS_FOUND_IN_CORRECT_BUCKETS(shadow_map, map);
            //int t;
            //cin >> t;
            //cout<<"5"<<endl;
        }
        //cout<<"4"<<endl;
    }
}