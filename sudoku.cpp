//
//  sudoku.cpp
//  DancingLinkSudoku
//
//  Created by flutterdro on 03.05.2023.
//

#include "sudoku.h"

Sudoku::Sudoku(const char* input_sudoku) {
    inputTable(input_sudoku);
    m_sparse_matrix = std::vector<std::vector<int>>(size_sq*size, std::vector<int>(4*size_sq, 0));
    constructSparseMatrix();
}

void Sudoku::constructSparseMatrix() {
    for (int i = 0; i < size; i++) { // iterating through rows
        for (int j = 0; j < size; j++) { //iterating through columns
            
            if (m_sudoku[i][j] != 0) {
                int k = m_sudoku[i][j] - 1;
/*
 Here I want to express my sorrow about this stupidity of mine.
 
 While generating sparse matrix for the empty sudoku I am iterating through every row column and digit. My small brain corrupted by programming
 as the most natural thing iterated those variables from 0 to 8 which is < size=9. At this stage I did not encounter any problems and
 sparse matrix covered every single posibility and this is a good result but I want to solve a specific sudoku board not every single one. To do this
 I need to modify sparse matrix in a way that takes into an account initial sudoku board. You can do this in 2 ways:
 1. On the step of generating sparse matrix if cell contains known digit you set constraints only for this digit in this cell and other posibilities
    in this cell you skip.
 2. After creating sparse matrix for every single posibility you create Dancing Links List and using already written methods of Cover(Node* column)
    you remove some of the constraints.
 2nd method has some serious pitfalls in my case so the obious choice was the first method, also my way of generating the sparse matrix makes it easy to
 implement. All it took is 1 if statement. I happy with myself compile and run and..... nothing which is weird because it means that DLX(Knuth's algorithm X)
 printed solutions of size 0 which can happen only in the case if recursion never happend because there exists a column of size 0. This may sound simple but to
 figure this out I had to spend 2 days. So this meant that the problem is with the sparse matrix. After 1 day of inspecting table of size 728x324 I was ready to
 give up but then I noticed this line //int k = m_sudoku[i][j];//  and you can obviously see the issue... My oonga bunga mind forgot that normal people use
 numbers starting from 1 to 9 in the sudoku while I use them from 0 to 8 to iterate through matrix. So the only thing I had to do is -1. And it worked almost
 instantly without a hitch and that was the only time where I wished that it would not be the case.
 
 And this is the end of the tale how -1 could have saved me 3 days of debugging
 
*/
                m_sparse_matrix[i * size_sq + j * size + k][i * size + j + 0*size_sq] = 1;
                m_sparse_matrix[i * size_sq + j * size + k][i * size + k + 1*size_sq] = 1;
                m_sparse_matrix[i * size_sq + j * size + k][j * size + k + 2*size_sq] = 1;
                m_sparse_matrix[i * size_sq + j * size + k][i/3 * size * size_rt + j/3 * size + k + 3*size_sq] = 1;
                
            } else {
            
                for (int k = 0; k < size; k++) { // iterating through digits
                    
                    //cell constraint
                    m_sparse_matrix[i * size_sq + j * size + k][i * size + j + 0*size_sq] = 1;
                    
                    //row constraint
                    m_sparse_matrix[i * size_sq + j * size + k][i * size + k + 1*size_sq] = 1;
                    
                    //column constraint
                    m_sparse_matrix[i * size_sq + j * size + k][j * size + k + 2*size_sq] = 1;
                    
                    //box constraint
                    m_sparse_matrix[i * size_sq + j * size + k][i/3 * size * size_rt + j/3 * size + k + 3*size_sq] = 1;
                }
            }
        }
    }
}
void Sudoku::inputTable(const char* input_filename) {
    
    std::ifstream sudoku_input;
    sudoku_input.open(input_filename);
    
    for (int i = 0; i < size; i++) {
        
        std::vector<int> row;
        
        for (int j = 0; j < size; j++) {
            
            int buff = 0;
            sudoku_input >> buff;
            row.push_back(buff);
            
        }
        
        m_sudoku.push_back(row);
        
    }
    
    sudoku_input.close();
}

void Sudoku::solve() {
    DancingLinks dll(m_sparse_matrix);
    std::vector<int> solution = dll.getExactCoverProblemSolution();
    for (const auto& sol : solution) {
        int row = sol/size_sq;
        int digit = sol%size + 1; //NEVER FORGET THE +1
        int col = (sol - digit + 1)/size - row*size; //NEVER FORGET THE +1
        m_sudoku[row][col] = digit;
    }
}

void Sudoku::outputTable() {
    std::ofstream sudoku_output;
    sudoku_output.open("solved.out", std::ios::trunc);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            sudoku_output << m_sudoku[i][j] << " ";
        }
        sudoku_output << "\n";
    }
    sudoku_output.close();
}


//Remnants of the debugging warfare
void Sudoku::print() {
    printf("       |-----------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    printf("       |                                                                               box                                                                               |\n");
    printf("       |-----------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    printf("       |        1        |        2        |        3        |        4        |        5        |        6        |        7        |        8        |        9        |\n");
    printf("       |-----------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    printf("       |1|2|3|4|5|6|7|8|9|1|2|3|4|5|6|7|8|9|1|2|3|4|5|6|7|8|9|1|2|3|4|5|6|7|8|9|1|2|3|4|5|6|7|8|9|1|2|3|4|5|6|7|8|9|1|2|3|4|5|6|7|8|9|1|2|3|4|5|6|7|8|9|1|2|3|4|5|6|7|8|9|\n");
    printf("       |-----------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            for (int dig = 0; dig < size; dig++) {
                printf("(%d,%d)|%d|", col, row, dig);
                for (int i = 0; i < size_sq; i++) {
                    printf("%d ", m_sparse_matrix[row * size_sq + col * size + dig][i+0*size_sq]);
                }
                printf("\n");
            }
            printf("\n");
        }
    }
    int sum = 0;
    for (int i = 0; i < size_sq*4; i++) {
        for (int j = 0; j < size_sq*size; j++) {
            sum += m_sparse_matrix[j][i];
        }
        if (sum == 0) {
            std::cout << "section: " << i/size_sq << " index: " << i%size_sq << std::endl;
        }
        sum = 0;
    }
}
