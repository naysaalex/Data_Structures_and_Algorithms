#include "executable.h"

TEST(bucket_size) {
    Typegen t;

    for(size_t i = 0; i < TEST_ITER; i++) {
        size_t n_pairs = t.range(1000ul);

        using Map = UnorderedMap<double, double>;
        using size_type = typename Map::size_type;
        using value_type = std::pair<double, double>;

        std::vector<value_type> pairs(n_pairs);
        t.fill(pairs.begin(), pairs.end());

        size_t n = t.range(100ull);
        Map map(n);
        shadow_map<double, double>   gt_map(n);

        for(auto const & pair : pairs) {
            map.insert(pair);
            gt_map.insert(pair);

        }

        std::cout<<"1"<<std::endl;
        ASSERT_PAIRS_FOUND_IN_CORRECT_BUCKETS(gt_map, map);
        std::cout<<"2"<<std::endl;
        for(size_type i = 0; i < gt_map.bucket_count(); i++) {
            ASSERT_EQ(gt_map.bucket_size(i), map.bucket_size(i));
        }
        std::cout<<"3"<<std::endl;
    }
}