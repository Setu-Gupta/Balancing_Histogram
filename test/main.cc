#include <balancing_hist.h>
#include <iostream>

int main()
{
        // Create and populate a histogram which has bins of size 2
        balancing_histogram::hist<float, 2> h;
        for(float i = 0.0f; i < 10.0f; i++) h.insert(i);
        h.insert(100.0f);
        for(float i = 5.0f; i < 10.0f; i++) h.insert(i);

        // Print the histogram
        h.print();

        // Print out the learned cuts
        std::vector<float> cuts = h.get_bin_boundaries();
        for(const float& f: cuts) std::cout << f << " ";
        std::cout << std::endl;

        return 0;
}
