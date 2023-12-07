#include "executable.h"
#include <unordered_map>

TEST(erase_iterator) {
    Typegen t;
    for(size_t i = 0; i < TEST_ITER; i++) {

        size_t n_pairs = t.range(1000ul);

        std::vector<std::pair<double, double>> pairs(n_pairs);
        t.fill_unique(pairs.begin(), pairs.end());

        size_t n = t.range(100ull);
        UnorderedMap<double, double> map(n);
        shadow_map<double, double>   gt_map(n);

        std::vector<std::pair<double, double>> cpy_pairs = pairs;
        for(auto const & pair : cpy_pairs) {
            map.insert(pair);
            gt_map.insert(pair);
        }

        t.shuffle(pairs.begin(), pairs.end());

        using Map = UnorderedMap<double, double>;
        using iter = typename Map::iterator;
        print_map(map);
        size_t remaining = n_pairs;
        while(remaining--) {
            iter it = map.begin();

            size_t k = t.range(map.size());
            for(size_t i = 0; i < k; i++) {
                it++;
            }

            iter after = it;
            std::cout<<"it: "<<it->first<<std::endl;
            after++;

            gt_map.erase(it->first);

            iter ret;
            {
                Memhook mh;
                ret = map.erase(it);
                ASSERT_EQ(0ULL, mh.n_allocs());
                ASSERT_EQ(1ULL, mh.n_frees());
            }
            std::cout<<"ret: "<<ret->first<<std::endl;
            std::cout<<"after: "<<after->first<<std::endl;
            ASSERT_TRUE((ret == after));
            ASSERT_EQ(gt_map.size(), map.size());

            print_map(map);

            ASSERT_PAIRS_FOUND_IN_CORRECT_BUCKETS(gt_map, map);
        }
    }
}
