#include "iog.h"

int **generate()
{
    int **mat = (int**)calloc(N, sizeof(int*));
    for (int i = 0; i < N; i++)
        mat[i] = (int*)calloc(N, sizeof(int));
    for (int f = 0; f < N; f++) {
        int i = rand() % N;
        int j = rand() % N;
        mat[i][j] = rand();
    }
    return mat;
}


void print(matr *new_matr)
{
    cout << "\nrows ";
    for (int i = 0; i < int(new_matr->rows_index.size()); i++) {
        cout << new_matr->rows_index[i] << ' ';
    }
    cout << "\ncols ";
    for (int i = 0; i < int(new_matr->columns.size()); i++) {
        cout << new_matr->columns[i] << ' ';
    }

    cout << "\neles ";
    for (int i = 0; i < int(new_matr->elements.size()); i++) {
        cout << new_matr->elements[i] << ' ';
    }
    cout << '\n';
}

vector<vector<int>> *input(int n, int m) 
{
    vector<vector<int>> *mat = new(vector<vector<int>>);
    for (int i = 0; i < n; i++) {
        vector<int> *col = new(vector<int>);
        for (int j = 0; j < m; j++) {
            int x = 0;
            cin >> x;
            col->push_back(x);
        }
        mat->push_back(*col);
    }
    return mat;
}