#include <balancing_hist.h>
#include <iostream>

int main()
{
        // Create and ppulate a histogram which has bins of size 2
        balancing_histogram::hist<float, 2> h;
        for(int i = 0; i < 10; i++) h.insert(float(i));
        h.insert(100);
        for(int i = 5; i < 10; i++) h.insert(float(i));

        // Print the histogram
        h.print();

        // Print out the learned cuts
        std::vector<float> cuts = h.get_bin_boundaries();
        for(const float& f: cuts) std::cout << f << " ";
        std::cout << std::endl;

        return 0;
}
