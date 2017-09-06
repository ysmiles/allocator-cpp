// custom_allocator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <vector>

#include "mem_pool.h"
#include "y_alloc.h"

using std::cout;
using std::endl;
using std::vector;
using std::list;
using std::map;
using std::pair;
using namespace std::chrono;

void timer_test(const int &sz);

// global used random engine and number generator
std::random_device myrd;
std::default_random_engine myeng(myrd());
std::uniform_int_distribution<int> mydist(1, 1000);

int main() {

    int scales[] = {100, 1000, 10000, 100000}; // , 1000000};

    for (auto scale : scales) {
        timer_test(scale);
    }

    return 0;
}

void timer_test(const int &sz) {
    cout << sz << endl;

    // starting time
    steady_clock::time_point start_time;
    // ending time
    steady_clock::time_point end_time;
    // elapsed time array
    vector<long long> elapse_self_new, elapse_stl_default, elapse_mem_pool;
    struct teststruct {
        char buf[1024];
    };

    {
        // ---------------------------------------
        // self defined new based allocator
        // ---------------------------------------
        {
            // vector
            // cout << "fixed size vector test" << endl;
            // create fix sized container
            start_time = steady_clock::now();
            vector<int, y_alloc<int>> vfix(sz);
            end_time = steady_clock::now();
            elapse_self_new.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            vfix.clear();

            // container manipulation
            // cout << "vector grow test" << endl;
            vector<teststruct, y_alloc<teststruct>> v;
            start_time = steady_clock::now();
            for (auto i = 0; i < sz; ++i) {
                if (mydist(myeng) % 2 || v.empty())
                    v.push_back(teststruct());
                else {
                    int i = mydist(myeng) % v.size();
                    auto it = v.begin();
                    std::advance(it, i);
                    v.erase(it);
                }
            }
            end_time = steady_clock::now();
            elapse_self_new.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            v.clear();
        }
        {
            // list
            // cout << "fixed size list test" << endl;
            // create fix sized container
            start_time = steady_clock::now();
            list<int, y_alloc<int>> lfix(sz);
            end_time = steady_clock::now();
            elapse_self_new.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            lfix.clear();

            // container grow
            // cout << "list grow test" << endl;
            list<teststruct, y_alloc<teststruct>> l;
            start_time = steady_clock::now();
            for (auto i = 0; i < sz; ++i) {
                if (mydist(myeng) % 2 || l.empty())
                    l.push_back(teststruct());
                else {
                    int i = mydist(myeng) % l.size();
                    auto it = l.begin();
                    std::advance(it, i);
                    l.erase(it);
                }
            }
            end_time = steady_clock::now();
            elapse_self_new.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            l.clear();
        }
        {
            // map
            // container grow
            // cout << "map grow test" << endl;
            map<int, int, std::less<int>, y_alloc<pair<int, int>>> m;
            // map<int, int> m;
            start_time = steady_clock::now();
            for (auto i = 0; i < sz; ++i)
                if (mydist(myeng) % 2 || m.empty())
                    ++m[mydist(myeng)];
                else {
                    int i = mydist(myeng) % m.size();
                    auto it = m.begin();
                    std::advance(it, i);
                    m.erase(it);
                }
            end_time = steady_clock::now();
            elapse_self_new.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            m.clear();
        }
        std::copy(elapse_self_new.begin(), elapse_self_new.end(),
                  std::ostream_iterator<long long>(cout, ","));
        cout << endl;
    }

    {
        // ---------------------------------------
        // std default allocator
        // ---------------------------------------
        {
            // vector
            // cout << "fixed size vector test" << endl;
            // create fix sized container
            start_time = steady_clock::now();
            vector<int, std::allocator<int>> vfix(sz);
            end_time = steady_clock::now();
            elapse_stl_default.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            vfix.clear();

            // container manipulation
            // cout << "vector grow test" << endl;
            vector<teststruct, std::allocator<teststruct>> v;
            start_time = steady_clock::now();
            for (auto i = 0; i < sz; ++i) {
                if (mydist(myeng) % 2 || v.empty())
                    v.push_back(teststruct());
                else {
                    int i = mydist(myeng) % v.size();
                    auto it = v.begin();
                    std::advance(it, i);
                    v.erase(it);
                }
            }
            end_time = steady_clock::now();
            elapse_stl_default.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            v.clear();
        }
        {
            // list
            // cout << "fixed size list test" << endl;
            // create fix sized container
            start_time = steady_clock::now();
            list<int, std::allocator<int>> lfix(sz);
            end_time = steady_clock::now();
            elapse_stl_default.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            lfix.clear();

            // container grow
            // cout << "list grow test" << endl;
            list<teststruct, std::allocator<teststruct>> l;
            start_time = steady_clock::now();
            for (auto i = 0; i < sz; ++i) {
                if (mydist(myeng) % 2 || l.empty())
                    l.push_back(teststruct());
                else {
                    int i = mydist(myeng) % l.size();
                    auto it = l.begin();
                    std::advance(it, i);
                    l.erase(it);
                }
            }
            end_time = steady_clock::now();
            elapse_stl_default.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            l.clear();
        }
        {
            // map
            // container grow
            // cout << "map grow test" << endl;
            map<int, int, std::less<int>, std::allocator<pair<int, int>>> m;
            // map<int, int> m;
            start_time = steady_clock::now();
            for (auto i = 0; i < sz; ++i)
                if (mydist(myeng) % 2 || m.empty())
                    ++m[mydist(myeng)];
                else {
                    int i = mydist(myeng) % m.size();
                    auto it = m.begin();
                    std::advance(it, i);
                    m.erase(it);
                }
            end_time = steady_clock::now();
            elapse_stl_default.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            m.clear();
        }
        std::copy(elapse_stl_default.begin(), elapse_stl_default.end(),
                  std::ostream_iterator<long long>(cout, ","));
        cout << endl;
    }

    {
        // ---------------------------------------
        // memory pool based allocator
        // ---------------------------------------
        {
            // vector
            // cout << "fixed size vector test" << endl;
            // create fix sized container
            start_time = steady_clock::now();
            vector<int, y_mp_alloc<int>> vfix(sz);
            end_time = steady_clock::now();
            elapse_mem_pool.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            vfix.clear();

            // container manipulation
            // cout << "vector grow test" << endl;
            vector<teststruct, y_mp_alloc<teststruct>> v;
            start_time = steady_clock::now();
            for (auto i = 0; i < sz; ++i) {
                if (mydist(myeng) % 2 || v.empty())
                    v.push_back(teststruct());
                else {
                    int i = mydist(myeng) % v.size();
                    auto it = v.begin();
                    std::advance(it, i);
                    v.erase(it);
                }
            }
            end_time = steady_clock::now();
            elapse_mem_pool.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            v.clear();
        }
        {
            // list
            // cout << "fixed size list test" << endl;
            // create fix sized container
            start_time = steady_clock::now();
            list<int, y_mp_alloc<int>> lfix(sz);
            end_time = steady_clock::now();
            elapse_mem_pool.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            lfix.clear();

            // container grow
            // cout << "list grow test" << endl;
            list<teststruct, y_mp_alloc<teststruct>> l;
            start_time = steady_clock::now();
            for (auto i = 0; i < sz; ++i) {
                if (mydist(myeng) % 2 || l.empty())
                    l.push_back(teststruct());
                else {
                    int i = mydist(myeng) % l.size();
                    auto it = l.begin();
                    std::advance(it, i);
                    l.erase(it);
                }
            }
            end_time = steady_clock::now();
            elapse_mem_pool.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            l.clear();
        }
        {
            // map
            // container grow
            // cout << "map grow test" << endl;
            map<int, int, std::less<int>, y_mp_alloc<pair<int, int>>> m;
            // map<int, int> m;
            start_time = steady_clock::now();
            for (auto i = 0; i < sz; ++i)
                if (mydist(myeng) % 2 || m.empty())
                    ++m[mydist(myeng)];
                else {
                    int i = mydist(myeng) % m.size();
                    auto it = m.begin();
                    std::advance(it, i);
                    m.erase(it);
                }
            end_time = steady_clock::now();
            elapse_mem_pool.push_back(
                duration_cast<microseconds>(end_time - start_time).count());
            m.clear();
        }
        std::copy(elapse_mem_pool.begin(), elapse_mem_pool.end(),
                  std::ostream_iterator<long long>(cout, ","));
        cout << endl;
    }
}