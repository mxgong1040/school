#include <iostream>
#include <vector>
#include <thread>
#include <utility>
#include <functional>

typedef void (*mergesort_ptr_t)(std::vector<double> &v, int l, int r);

void merge(std::vector<double> &v, int l, int m, int r) {
    int i = l;
    int j = m + 1;
    std::vector<double> temp;
    while (i <= m && j <= r) {
        if (v.at(i) <= v.at(j)) {
            temp.push_back(v.at(i++));
        }
        else {
            temp.push_back(v.at(j++));
        }
    }
    while (i <= m) {
        temp.push_back(v.at(i++));
    }
    while (j <= r) {
        temp.push_back(v.at(j++));
    }
    std::copy(temp.begin(), temp.end(), v.begin() + l);
}

void mergesort(std::vector<double> &v, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        std::thread sort_thread1((mergesort_ptr_t)(mergesort), std::ref(v), l, m);
        std::thread sort_thread2((mergesort_ptr_t)(mergesort), std::ref(v), m + 1, r);
        sort_thread1.join();
        sort_thread2.join();
        merge(v, l, m, r);
    }
}

int main(void) {
    int n;
    std::vector<double> v;
    std::cout << "Enter num values:"<<std::endl;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        double a;
        std::cin >> a;
        v.push_back(a);
    }
    mergesort(v, 0, n - 1);
    for (const double &a : v) {
        std::cout << a << " ";
    }
    int k;
    std::cin>>k;
    std::cout << std::endl;
    return 0;
}
