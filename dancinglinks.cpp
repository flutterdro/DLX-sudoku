//
//  dancinglinks.cpp
//  DancingLinkSudoku
//
//  Created by flutterdro on 03.05.2023.
//

#include "dancinglinks.h"
DancingLinks::DancingLinks(std::vector<std::vector<int>> matrix) {
    createList(matrix);
}

DancingLinks::~DancingLinks() {
    deleteList();
}

//Standart stuff which you will find in every single paper about dancing links in pseudocode
void DancingLinks::DLX(int step, std::vector<Node*>& solution) {
    if (m_HeadNode->right == m_HeadNode) {
        extractData(solution);
        return;
    }
    Node* columnToRemove = m_HeadNode->right;
    for (Node* it_col = columnToRemove->right; it_col != m_HeadNode; it_col = it_col->right) {
        if (columnToRemove->size > it_col->size) columnToRemove = it_col;
    }

    cover(columnToRemove);
    for (Node* it_row = columnToRemove->down; it_row != columnToRemove; it_row = it_row->down) {
        solution.push_back(it_row);
        for (Node* it_col = it_row->right; it_col != it_row; it_col = it_col->right) {
            cover(it_col->head);
        }
        DLX(step + 1, solution);
        it_row = solution[step];
        solution.pop_back();
        columnToRemove = it_row->head;
        for (Node* it_col = it_row->left; it_col != it_row; it_col = it_col->left) {
            uncover(it_col->head);
        }
    }
    uncover(columnToRemove);
}

void DancingLinks::cover(Node* column) {
    
    column->left->right = column->right;
    column->right->left = column->left;
    
    for (Node* i = column->down; i != column; i = i->down) {
        for (Node* j = i->right; j != i; j = j->right) {
            
            j->up->down = j->down;
            j->down->up = j->up;
            j->head->size--;
            
        }
    }
}

void DancingLinks::uncover(Node* column) {
    
    for (Node* i = column->up; i != column; i = i->up) {
        for (Node* j = i->left; j != i; j = j->left) {
            
            j->head->size++;
            j->up->down = j;
            j->down->up = j;
            
        }
    }
    
    column->left->right = column;
    column->right->left = column;
}

std::vector<int> DancingLinks::getExactCoverProblemSolution(){
    std::vector<Node*> solution{};
    DLX(0, solution);
    return m_RowsCover;
}


/*
 If I understood this correctly DLX gives all the possible solutions while this function extracts all of them into a single vector
 so in case there are multiple solutions outputting one of them would turn into an unfixable mess. Ideally I should store solutions in the
 vector of vectors and in the begining of exteracting pushback a vector. This will easily allow to keep track of the amount of solutions as
 well as outputting specific ones but as of right now I have an exam tomorrow so I couldn't be bothered.
*/
void DancingLinks::extractData(std::vector<Node*> solution) {
    for (auto sol : solution) {
        m_RowsCover.push_back(sol->row);
    }
}

//I'm sorry about this mess but I couldn't come up with anything better
void DancingLinks::createList(std::vector<std::vector<int>> matrix) {
    
    m_HeadNode = new Node;
    m_HeadNode->left   = m_HeadNode;
    m_HeadNode->up     = m_HeadNode;
    m_HeadNode->down   = m_HeadNode;
    m_HeadNode->head   = m_HeadNode;
    m_HeadNode->right  = m_HeadNode;
    m_HeadNode->size   = -1;
    Node* temp          = m_HeadNode;
    
    for (int i = 0; i < matrix[0].size(); i++) {
        
        Node* new_node  = new Node;
        new_node->up    = new_node;
        new_node->down  = new_node;
        new_node->head  = new_node;
        new_node->right = m_HeadNode;
        new_node->size  = 0;
        new_node->left  = temp;
        temp->right     = new_node;
        temp = new_node;
        
    }
    for (int i = 0; i < matrix.size(); i++) {
        
        Node* curr_col = m_HeadNode->right;
        Node* prev_node = nullptr;
        
        for (int j = 0; j < matrix[0].size(); j++, curr_col = curr_col->right) {
            
            if (matrix[i][j] != 0) {
                
                Node* new_node = new Node;
                new_node->row = i;
                
                if (prev_node == nullptr) {
                    prev_node = new_node;
                    prev_node->right = new_node;
                }
                
                new_node->left      = prev_node;
                new_node->right     = prev_node->right;
                new_node->right->left = new_node;
                prev_node->right    = new_node;
                new_node->head      = curr_col;
                new_node->down      = curr_col;
                new_node->up        = curr_col->up;
                curr_col->up->down  = new_node;
                curr_col->up        = new_node;
                curr_col->size     += 1;
                
                if (curr_col->down == curr_col) {
                    curr_col->down = new_node;
                }
                
                prev_node = new_node;
            }
        }
    }
}

/*
 I read cpp core guidelines the other day and the funny thing it said is that in case of simple input->output programs (basically what my program does)
 you can leak away without a second thought. But it doesn't sit right with me so I decided to clean up regardless. I hope I did it right and it won't
 come bitting me on the tail.
*/
void DancingLinks::deleteList(){
    for (Node* column = m_HeadNode->left; column != m_HeadNode;) {
        for (Node* row = column->up; row != column;) {
            Node* temp = row;
            
            row->left->right    = row->right;
            row->right->left    = row->left;
            row->down->up       = row->up;
            row->up->down       = row->down;
            row->head->size    -= 1;
            row                 = row->up;
            
            delete temp;
        }
        Node* temp = column;
        column->left->right     = column->right;
        column->right->left     = column->left;
        column                  = column->left;
        
        delete temp;
    }
    delete m_HeadNode;
}
