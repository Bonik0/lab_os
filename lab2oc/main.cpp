#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>


std::vector<int> arrey;
const int RUN = 32;


void insertion_sort(int left, int right){
    for (int i = left + 1; i <= right; i++) {
        int temp = arrey[i];
        int j = i - 1;
        while (j >= left && arrey[j] > temp) {
            arrey[j + 1] = arrey[j];
            j--;
        }
        arrey[j + 1] = temp;
    }
}

void merge(int l, int m, int r){
    int len1 = m - l + 1, len2 = r - m;
    std::vector<int> left, right;
    left.assign(arrey.begin() + l, arrey.begin() + l + len1);
    right.assign(arrey.begin() + m + 1, arrey.begin() + m + len2 + 1);
    int i = 0;
    int j = 0;
    int k = l;

    while (i < len1 && j < len2) {
        if (left[i] <= right[j]) {
            arrey[k] = left[i];
            i++;
        }
        else {
            arrey[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < len1) {
        arrey[k] = left[i];
        k++;
        i++;
    }

    while (j < len2) {
        arrey[k] = right[j];
        k++;
        j++;
    }
}

void sort(int n, int thread_count){
    for (int i = 0; i < n; i += RUN * thread_count){
        std::vector<std::thread> threads;
        for(int j = i; j < i + RUN * thread_count; j += RUN){
            threads.push_back(std::thread(insertion_sort, j, std::min((j + RUN - 1), (n - 1))));
        }
        for (int j = 0; j < threads.size(); j++) {
            threads[j].join();
        }
    }
    for (int size = RUN; size < n; size = 2 * size) {
        std::vector<std::thread> threads;
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min((left + 2 * size - 1), (n - 1));

            if (mid < right){
                threads.push_back(std::thread(merge, left, mid, right));
            }
            if (thread_count == threads.size()){
                for(int i = 0; i < thread_count; i++){
                    threads[i].join();
                }
                threads.clear();
            }
        }
        for(int i = 0; i < threads.size(); i++){
            threads[i].join();
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        throw std::logic_error("Указано неполное число ключей");
    }
    int n, thread_count;
    thread_count = atoi(argv[1]);
    if (thread_count < 1){
        throw std::logic_error("Количество потоков должно быть больше 1");
    }
    n = atoi(argv[2]);
    if (n < 1){
        throw std::logic_error("Размер массива должен быть больше 1");
    }
    arrey.resize(n);
    for(int i = 0; i < n; i++){
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 100000);
        arrey[i] = dis(gen);
    }
    auto start = std::chrono::steady_clock::now();
    sort(n, thread_count);
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<< "ms" << "\n";
}