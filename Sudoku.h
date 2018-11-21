#ifndef SUDOKU_H
#define SUDOKU_H


#include <iostream>
#include <math.h>
#include <vector>
//#include <algorithm>
#include <set>
#include "Searchable.h"

using namespace std;

class Cell{
		public:
			set<short int> solutions;
			short int value;
			
			Cell(const short int& boardSizeIn):value(-1){
				for(short int i = 1; i<=boardSizeIn; ++i){
					solutions.insert(i);
				}
			}
			
			void setValue(const short int& valueIn){
				value = valueIn;
				solutions.clear();
				solutions.insert(valueIn);
			}
			
			//return if this cell has no value but one possible solution
			bool oneSolution()const{
				return(value == -1 && solutions.size()==1);
			}
			
			//return true if this cell has no value, and many possible solutions
			bool hasManySolutions()const{
				return(solutions.size()>1);
			}
			
			void removeSolution(const short int& number){
				if(solutions.size()>1){
					solutions.erase(number);
				}
			}
			
			short int getSolution()const{
				return *solutions.begin();
			}
			
			short int size()const{
				return solutions.size();
			}
		
			bool contains(const short int& i)const{
				return solutions.find(i)!=solutions.end();
			}
};

class Sudoku : public Searchable{
	public:
		vector<vector<Cell>> board;
		short int boardSize;
		short int boxSize;
		
		Sudoku(const short int& boardSizeIn)
		:boardSize(boardSizeIn),board(boardSizeIn,vector<Cell>(boardSizeIn,Cell(boardSizeIn))),boxSize(sqrt(boardSize)){}
		
		Sudoku(const Sudoku& other):boardSize(other.boardSize),board(other.board),boxSize(other.boxSize){}
		
		const short int getSquare(const short int& row, const short int& col)const{
			return board[row][col].value;
		}
		
		short int getBigSquare(const short int& reference)const{
			short int box = sqrt(boardSize);
			return (reference/box)*box;
		}
		
		void removeSolutionsRow(const short int& row,const short int& value){
			for(short int i = 0; i < boardSize; i++){
				board[row][i].removeSolution(value);
			}
		}
		
		void removeSolutionsCol(const short int& col,const short int& value){
			for(short int i = 0; i < boardSize; i++){
				board[i][col].removeSolution(value);
			}
		}
		
		void removeSolutionsBox(const short int& row, const short int& col, const short int& value){
			short int bigR = getBigSquare(row);
			short int bigC = getBigSquare(col);
			short int boxSize = sqrt(boardSize);
			for(short int i = 0; i < boxSize; i++){
				for( short int j = 0; j< boxSize; j++){
					board[bigR + i][bigC + j].removeSolution(value);
				}
			}
		}
		void removeSolutions(const short int& row, const short int& col, const short int& value){
			removeSolutionsRow(row,value);
			removeSolutionsCol(col,value);
			removeSolutionsBox(row,col,value);
		}
		
		void checkRowsSets(const short int& setSize){
			set<short int> comparator;
			for(short int row = 0; row < boardSize; row++){
				for(short int col = 0; col< boardSize-setSize;col++){
					if(board[row][col].size() == setSize){
						comparator = board[row][col].solutions;
						short int counter = setSize-1;
						for(short int endCol = col+1;endCol<boardSize;endCol++){
							if(board[row][endCol].solutions == comparator){
								counter--;
								if(counter == 0){
									for(short int newCol = 0; newCol<boardSize;newCol++){
										if(board[row][newCol].solutions != comparator){
											for(short int s : comparator){
												board[row][newCol].removeSolution(s);
											}
											if(board[row][newCol].oneSolution()){
												setSquare(row,newCol,board[row][newCol].getSolution());
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		void checkColsSets(const short int& setSize){
			set<short int> comparator;
			for(short int col = 0; col< boardSize;col++){
				for(short int row = 0; row < boardSize-setSize; row++){
					if(board[row][col].size() == setSize){
						comparator = board[row][col].solutions;
						short int counter = setSize-1;
						for(short int endRow = row+1;endRow<boardSize;endRow++){
							if(board[endRow][col].solutions == comparator){
								counter--;
								if(counter == 0){
									for(short int newRow = 0; newRow<boardSize;newRow++){
										if(board[newRow][col].solutions != comparator){
											for(short int s : comparator){
												board[newRow][col].removeSolution(s);
											}
											if(board[newRow][col].oneSolution()){
												setSquare(newRow,col,board[newRow][col].getSolution());
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		void checkBoxesSets(const short int& setSize){
			set<short int> comparator;
			for(short int bigR = 0; bigR<boardSize; bigR+=boxSize){
				for(short int bigC = 0; bigC<boardSize; bigC+=boxSize){
					for(short int i = 0; i<boardSize; i++){
						if(board[bigR+i%boxSize][bigC+i/boxSize].size()==setSize){
							comparator=board[bigR+i%boxSize][bigC+i/boxSize].solutions;
							for(auto c: comparator){
							}
							short int counter = setSize-1;
							for(short int j =i+1;j<boardSize;j++){
								if(board[bigR+j%boxSize][bigC+j/boxSize].size()==setSize && board[bigR+j%boxSize][bigC+j/boxSize].solutions == comparator){
									counter--;
									if(counter == 0){
										for(short int k =0; k<boardSize;k++){
											if(board[bigR+k%boxSize][bigC+k/boxSize].value == -1 && board[bigR+k%boxSize][bigC+k/boxSize].solutions != comparator){
												for(short int s : comparator){
													board[bigR+k%boxSize][bigC+k/boxSize].removeSolution(s);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		
		void checkRowsSingleValue(const short int& i){
			short int counter=0;
			short int tc=0;
			for(short int row=0;row<boardSize;row++){
				counter = 0;
				tc =0;
				for(short int col = 0;col<boardSize;col++){
					if(board[row][col].value == i){
						col = boardSize;
					}else if(board[row][col].value == -1 && board[row][col].contains(i)){
						tc = col;
						counter++;
					}
				}
				if(counter==1){
					setSquare(row,tc,i);
				}
			}
		}
		
		
		void checkColsSingleValue(const short int& i){
			short int counter=0;
			short int tr=0;
			for(short int col = 0;col<boardSize;col++){
				counter = 0;
				tr =0;
				for(short int row=0;row<boardSize;row++){
					if(board[row][col].value == i){
						row = boardSize;
					}else if(board[row][col].value == -1 && board[row][col].contains(i)){
						tr = row;
						counter++;
					}
				}
				if(counter==1){
					setSquare(tr,col,i);
				}
			}
		}
		
		void checkBoxesSingleValue(const short int& val){
			short int counter=0;
			short int tr=0;
			short int tc=0;
			//boxes loop
			for(short int col = 0;col<boardSize;col+=boxSize){
				for(short int row=0;row<boardSize;row+=boxSize){
					counter=0;
					tr=0;
					tc=0;
					for(short int i = 0; i<boardSize; i++){
						if(board[row][col].value == val){
							i = boardSize;
						}else if(board[row+i%boxSize][col+i/boxSize].value == -1 && board[row+i%boxSize][col+i/boxSize].contains(val)){
							tc = col+i/boxSize;
							tr = row+i%boxSize;
							counter++;
						}
					}
					if(counter==1){
						setSquare(tr,tc,val);
					}
				}
			}
		}
		
		void checkSolutions(const int& row, const int& col){
			for(short int x = 2; x<sqrt(boardSize);x++){
				checkRowsSets(x);
				checkColsSets(x);
				checkBoxesSets(x);//
			}
				//techniques from https://www.kristanix.com/sudokuepic/sudoku-solving-techniques.php
				for(short int i = 1; i<= boardSize;i++){
				checkRowsSingleValue(i);
				checkColsSingleValue(i);
				checkBoxesSingleValue(i);
			}/*
			for(short int i =0;i<boardSize;i++){
				for(short int j = 0; j< boardSize;j++){
					if(board[i][j].oneSolution()){
						setSquare(i,j,board[i][j].getSolution(),true);
						return;
					}
				}
			}*/
		}
		
		bool squareIsSettable(const short int& row, const short int& col, const short int& value){
			for(short int i = 0; i < boardSize; i++){
				if(board[row][i].value == value){
					removeSolutions(row,col,value);
					return false;
				}
			}for(short int i = 0; i < boardSize; i++){
				if(board[i][col].value == value){
					removeSolutions(row,col,value);
					return false;
				}
			}
			short int bigR = getBigSquare(row);
			short int bigC = getBigSquare(col);
			short int boxSize = sqrt(boardSize);
			for(short int i = 0; i < boxSize; i++){
				for(short int j = 0; j< boxSize; j++){
					if(board[bigR+i][bigC+j].value == value){
						removeSolutions(row,col,value);
						return false;
					}
				}
			}
			return true;
		}
		
		bool setSquare(const short int& row, const short int& col, const short int& value, const bool& skipCheck = false){
			if(board[row][col].value == value)return true;
			if(!skipCheck && !squareIsSettable(row,col,value)){
				//board[row][col].removeSolution(value);
				return false;
			}
			board[row][col].setValue(value);
			removeSolutions(row,col,value);
			checkSolutions(row,col);
			return true;
		}
		
		void writePoss()const{
			for(short int i =0;i<boardSize;i++){
				for(short int j = 0; j< boardSize;j++){
					cout<<endl<<"["<<i<<"]["<<j<<"]";
					for(auto& s : board[i][j].solutions){
						cout<<s<<", ";
					}
				}
			}
		}
		
		virtual void write(ostream& o) const override{
			cout<<endl;
			for(short int i =0;i<boardSize;i++){
				cout<<"\n\t ";
				if(i!=0 && i%3==0)cout<<"---------------\n\t ";
				for(short int j = 0; j< boardSize;j++){
					if(j!=0 && j%3==0)cout<<" | ";
					if(board[i][j].value == -1)cout<<" ";
					else cout<<board[i][j].value<<"";
				}
			}
		}
		
		bool checkRow(const short int& row, const short int& value)const{
			for(short int i = 0; i<boardSize; i++){
				if(board[row][i].value == value) return true;
			}
			return false;
		}
		
		bool checkRows(const short int& row)const{
			for(short int i = 1; i<=boardSize;i++){
				if(!checkRow(row,i))return false;
			}
			return true;
		}
		
		int checkCol(const short int& col, const short int& value)const{
			for(short int i = 0; i<boardSize; i++){
				if(board[i][col].value == value) return true;
			}
			return false;
		}
		
		int checkCols(const short int& col)const{
			for(short int i = 1; i<=boardSize;i++){
				if(!checkCol(col,i))return false;
			}
			return true;
		}
		
		int checkBox(const short int& box, const short int& value, const short int& bigR, const short int& bigC, const short int& boxSize)const{
			for(short int row = 0; row<boxSize; row++){
				for(short int col = 0; col<boxSize; col++){
					if(board[row+bigR][col+bigC].value == value)return true;//
				}
			}
			return false;
		}
		
		int checkBoxes(const short int& box)const{
			short int boxSize = sqrt(boardSize);
			for(short int value = 1;value<=boardSize;value++){
				for(short int bigR = 0; bigR<boardSize;bigR+=boxSize){
					for(short int bigC = 0; bigC<boardSize;bigC+=boxSize){
						if(!checkBox(box,value,bigR,bigC,boxSize))return false;
					} 
				}
			}
			return true;
		}
		
		virtual int heuristicValue()const override{
        	short int c = 0;
        	for(short int i = 0; i< boardSize; ++i){
        		for(short int j = 0; j< boardSize; ++j){
        			if(board[i][j].size() == 0){
        				c++;
					}
				}
			}
			return c;
		}		
		
		virtual bool isSolution()const override{
			if(heuristicValue()!=0)return false;
			for(short int i = 0; i< boardSize; ++i){
				if(!checkRows(i))return false;
				if(!checkCols(i))return false;
				if(!checkBoxes(i))return false;//
			}
			return true;
		}
		
		virtual vector<unique_ptr<Searchable>> successors()const override{
			vector<unique_ptr<Searchable>> ptr;
			for(short int row = 0; row<boardSize; ++row){
				for(short int col = 0; col<boardSize; ++col){
					if(board[row][col].value==-1){
						for(short int solution : board[row][col].solutions){
							Sudoku* copyS = new Sudoku(*this);
							if(copyS->setSquare(row,col,solution)){
								ptr.emplace_back(copyS);
							}else{
								delete copyS;
							}
						}
						/*if(ptr.size()==1){
							return ptr[0].get()->successors();
						}*/
						return ptr;
					}
				}
			}
		}
		
        
};


#endif
