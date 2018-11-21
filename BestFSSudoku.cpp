#include "Sudoku.h"
#include "BestFirstSearch.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

#include <iostream>

using std::cout;
using std::endl;

void fillBoard(Sudoku * board, const vector<string> & fillWith) {
    
    for (size_t row = 0; row < fillWith.size(); ++row) {
        for (size_t col = 0; col < fillWith.size(); ++col) {
            if (fillWith[row][col] != ' ') {
                const int setTo = std::stoi(fillWith[row].substr(col,1));
                board->setSquare(row, col, setTo);
            }
        }
    }
}

int main() {
        
    {
        //insert the new board here
        vector<string> hardBoard{"8        ",
                                 "  36     ",
                                 " 7  9 2  ",
                                 
                                 " 5   7   ",
                                 "    457  ",
                                 "   1   3 ",
                                 
                                 "  1    68",
                                 "  85   1 ",
                                 " 9    4  "};
                                 
        unique_ptr<Sudoku> board(new Sudoku(9));
        fillBoard(board.get(), hardBoard);
        cout << "Solving board:\n";        
        board->write(cout);
        
        BestFirstSearch search(std::move(board));
        
        Searchable * solution = search.solve();
        
        Sudoku * solvedBoard = static_cast<Sudoku*>(solution);
        
        cout << "\n\n\nSolution is:\n";
        solvedBoard->write(cout);
    }
    
    
}