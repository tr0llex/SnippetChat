#include <iostream>

void search_indexes(int N, const int *A, const int *B, int &chosen_A, int &chosen_B) {
    int i_max_A = 0;
    for (int i = 1; i < N; i++) {
        if (A[i] > A[i_max_A]) {
            i_max_A = i;
        }
        if ((A[i_max_A] + B[i] > A[chosen_A] + B[chosen_B]) && (i >= i_max_A)) {
            chosen_A = i_max_A;
            chosen_B = i;
        }
    }
}

int main() {
    int N;
    std::cin >> N;

    int *A = new int[N];
    int *B = new int[N];
    for (int i = 0; i < N; i++) {
        std::cin >> A[i];
    }
    for (int i = 0; i < N; i++) {
        std::cin >> B[i];
    }

    int c=0;
    for(int i=0;i<10000000000;i++){
        c+=i;
    }
    int chosen_A = 0;
    int chosen_B = 0;
    search_indexes(N, A, B, chosen_A, chosen_B);

    std::cout << chosen_A << " " << chosen_B << std::endl;

    delete[] A;
    delete[] B;
    return 0;
}