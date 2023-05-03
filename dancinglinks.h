//
//  dancinglinks.h
//  DancingLinkSudoku
//
//  Created by flutterdro on 03.05.2023.
//

#ifndef dancinglinks_h
#define dancinglinks_h

#include <vector>

namespace {
struct Node {
    Node* left;
    Node* up;
    Node* head;
    Node* down;
    Node* right;
    int size;
    int row;
};
}
class DancingLinks {
private:
    Node* m_HeadNode;
    std::vector<int> m_RowsCover;
public:
    DancingLinks(std::vector<std::vector<int>> matrix);
    void cover(Node* column);
    void uncover(Node* column);
    std::vector<int> getExactCoverProblemSolution();
private:
    void DLX(int step, std::vector<Node*>& solution);
    void extractData(std::vector<Node*> solution);
    void createList(std::vector<std::vector<int>> matrix);
};

#endif /* dancinglinks_h */
