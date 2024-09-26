#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <omp.h>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

int query_size = 10000;

using namespace std;

int main() {
  // NOTE: This is a microbenchmark on tag-matching algorithms
  // key = uint64_t, value = uint64_t
  // container holds 5M elements
  // need to find 10K elements
  //

  // create random data
  std::random_device rd;
  std::mt19937 gen(rd());
  // do not overlap

  uniform_int_distribution<uint64_t> dis(0, 100'000'000);
  std::vector<uint64_t> data(5'000'000);
  for (int i = 0; i < 5'000'000; i++) {
    data[i] = dis(gen);
  }

  std::vector<uint64_t> queries(query_size);
  for (int i = 0; i < query_size; i++) {
    queries[i] = dis(gen);
  }

  std::vector<uint64_t> results(query_size);

  // =======================
  // unordered_map
  std::unordered_map<uint64_t, uint64_t> m_tags_umap;
  for (int i = 0; i < 5'000'000; i++) {
    m_tags_umap[data[i]] = data[i];
  }

  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < query_size; i++) {
    auto it = m_tags_umap.find(queries[i]);
    if (it != m_tags_umap.end()) {
      results[i] = it->second;
    }
  }
  auto end = chrono::high_resolution_clock::now();
  cout << "container size: " << m_tags_umap.size() << endl;
  cout << "query size: " << query_size << endl;
  cout << "unordered_map: "
       << chrono::duration_cast<chrono::microseconds>(end - start).count()
       << "us" << endl;

  // use result to avoid compiler optimization
  int64_t dummy = 0;
  for (int i : results) {
    dummy += i;
  }
  cout << "dummy: " << dummy << endl;

  // unordered_map with parallel
  start = chrono::high_resolution_clock::now();
#pragma omp parallel for
  for (int i = 0; i < query_size; i++) {
    auto it = m_tags_umap.find(queries[i]);
    if (it != m_tags_umap.end()) {
      results[i] = it->second;
    }
  }
  end = chrono::high_resolution_clock::now();
  cout << "unordered_map with parallel: "
       << chrono::duration_cast<chrono::microseconds>(end - start).count()
       << "us" << endl;

  // use result to avoid compiler optimization
  dummy = 0;
  for (int i : results) {
    dummy += i;
  }
  cout << "dummy: " << dummy << endl;

  // map
  std::map<uint64_t, uint64_t> m_tags_map;
  for (int i = 0; i < 5'000'000; i++) {
    m_tags_map[data[i]] = data[i];
  }

  start = chrono::high_resolution_clock::now();
  for (int i = 0; i < query_size; i++) {
    auto it = m_tags_map.find(queries[i]);
    if (it != m_tags_map.end()) {
      results[i] = it->second;
    }
  }
  end = chrono::high_resolution_clock::now();
  cout << "map: "
       << chrono::duration_cast<chrono::microseconds>(end - start).count()
       << "us" << endl;
  dummy = 0;
  for (int i : results) {
    dummy += i;
  }
  cout << "dummy: " << dummy << endl;

  // map with parallel
  start = chrono::high_resolution_clock::now();
#pragma omp parallel for
  for (int i = 0; i < query_size; i++) {
    auto it = m_tags_map.find(queries[i]);
    if (it != m_tags_map.end()) {
      results[i] = it->second;
    }
  }
  end = chrono::high_resolution_clock::now();
  cout << "map with parallel: "
       << chrono::duration_cast<chrono::microseconds>(end - start).count()
       << "us" << endl;
  dummy = 0;
  for (int i : results) {
    dummy += i;
  }
  cout << "dummy: " << dummy << endl;

  return 0;
}
