#include <vector>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <array>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

#include "matr.h"
#include "iog.h"


int main()
{
    setbuf(stdout, NULL);
    srand(time(nullptr));
    // int n = 0, m = 0, q = 0;
    // cin >> n;
    // cin >> m;
    // cin >> q;
    
    // auto m1 = input(n, m);
    // auto m2 = input(m, q);

    auto m1 = generate();
    auto m2 = generate();

    matr *new_matr = get_csrrepresent_m(m1);
    matr *new_matr2 = get_csrrepresent_m(m2);

  //  print(new_matr);

   // matr *mul = mutl(new_matr, new_matr2, q);

   // print(mul);

    int **new_mul = NULL;

    auto start = chrono::high_resolution_clock::now();
    auto m3 = mutl_new(new_matr, new_matr2, N, N, N);
    auto stop = chrono::high_resolution_clock::now();
    auto dur1 = chrono::duration_cast<chrono::microseconds>((stop - start));
    cout << "Последовательный: " << dur1.count() << '\n';
    free(m3);

    for (int i = 1; i < 256; i = i * 2) {
        new_mul = mutl_parallel(new_matr, new_matr2, N, N, N, i);
        for (int i = 0; i < N; i++)
            free(new_mul[i]);
        free(new_mul);
    }

    // for (int i = 0; i < n; i++) {
    //     cout << '\n';
    //     for (int j = 0; j < q; j++)
    //         cout << new_mul[i][j] << ' ';
    // }
    // cout << '\n';
    return 0;
}

/*
4 3 5
3 0 0 0 0 3
1 2 3 0 2 0
3 0 0 0 1
0 0 3 0 0
1 2 3 0 0
*/