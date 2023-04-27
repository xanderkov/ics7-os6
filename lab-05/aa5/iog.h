#pragma once

#include <vector>
#include <iostream>
#include <stdio.h>

#include "matr.h"

using namespace std;

int **generate();
void print(matr *new_matr);
vector<vector<int>> *input(int n, int m);
