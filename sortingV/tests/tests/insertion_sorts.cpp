#include "executable.h"
#include <algorithm>
#include <vector>

TEST(insertion_sort) {
    Typegen t;

    for(size_t sz = 0; sz < 800; sz++) {
        std::vector<int> vec(sz);
        std::vector<int> gt(sz);

        std::cout<<"1"<<std::endl;
        t.fill(vec.begin(), vec.end());
        std::cout<<"2"<<std::endl;
        std::copy(vec.begin(), vec.end(), gt.begin());
        std::cout<<"3"<<std::endl;

        sort::insertion(vec.begin(), vec.end());
        std::cout<<"4"<<std::endl;
        std::sort(gt.begin(), gt.end());
        std::cout<<"5"<<std::endl;

        bool equal = std::equal(vec.begin(), vec.end(), gt.begin());
        if (!equal) {
            std::cerr << "ERROR: Vector is not sorted!\n";
            if (sz < 30) {
                std::cerr << "\tYour Vector: " << vec
                        << "\n\tCorrect Vector: " << gt << '\n';
            } else {
                std::cerr << "\tVectors are too large to print.\n";
            }
        }
        ASSERT_TRUE(equal);
    }
}
