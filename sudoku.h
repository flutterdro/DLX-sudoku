//
//  sudoku.h
//  DancingLinkSudoku
//
//  Created by flutterdro on 03.05.2023.
//

#ifndef sudoku_h
#define sudoku_h

#include <vector>
#include <fstream>
#include <iostream>
#include "dancinglinks.h"

class Sudoku {
public:
    Sudoku(const char* input_sudoku);
    void constructSparseMatrix();
    void outputTable();
    void print();
    void solve();
    std::vector<std::vector<int>> m_sparse_matrix{};
private:
    static constexpr int size = 9;
    static constexpr int size_rt = 3;
    static constexpr int size_sq = size*size;
    
    std::vector<std::vector<int>> m_sudoku{};
    
    void inputTable(const char* input_filename);
};

#endif /* sudoku_h */
