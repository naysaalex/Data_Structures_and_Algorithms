#include <algorithm>
#include <list>
#include "executable.h"

TEST(constructor_move) {
    Typegen t;

    for(size_t i = 0; i < TEST_ITER; i++) {
        {
            const size_t n = i == 0 ? 0 : t.range(0x999ULL);
            List<int> ll(n);
            std::list<int> gt_ll(n);

            t.fill(gt_ll.begin(), gt_ll.end());
            std::copy(gt_ll.cbegin(), gt_ll.cend(), ll.begin());

            Memhook mh;
            List<int> ll_cpy { std::move(ll) };
            
            // List object should be in an "empty" state
            #ifndef ALT_LIST_VALID_MOVE_STATE
            // These checks will run by default.
            // Technically, the object only needs to be in a valid state.
            // If you come up with an alternative imp, you may disable this
            // check
            // cout<<"1"<<endl;
            ASSERT_EQ(0ULL, ll.size());
            // cout<<"2"<<endl;
            ASSERT_EQ(true,ll.begin() == ll.end());
            // cout<<"3"<<endl;
            #endif
            
            // Ensure pointers are wired correctly
            // Should handle empty case & stack nodes properly
            ASSERT_EQ(false,++(--ll.end()) == ll_cpy.end());
            // cout<<"4"<<endl;
            ASSERT_EQ(false,++(--ll.end()) == ll_cpy.begin());
            // cout<<"5"<<endl;
            ASSERT_EQ(false,ll.end() == ll_cpy.begin());
            // cout<<"6"<<endl;

            ASSERT_EQ(0ULL, mh.n_allocs());
            // cout<<"7"<<endl;
            ASSERT_EQ(0ULL, mh.n_frees());
            // cout<<"8"<<endl;

            // Check consistancy
             ASSERT_EQ(gt_ll.size(), ll_cpy.size());
            // cout<<"9"<<endl;

            auto it = ll_cpy.cbegin();
            auto gt_it = gt_ll.cbegin();

            while(gt_it != gt_ll.cend())
                ASSERT_EQ_(*gt_it++, *it++, "An inconsistency was found when iterating forward");
            // cout<<"10"<<endl;
            while(gt_it != gt_ll.cbegin())
                ASSERT_EQ_(*--gt_it, *--it, "An inconsistency was found when iterating backward");
            // cout<<"11"<<endl;
        }
    }
}