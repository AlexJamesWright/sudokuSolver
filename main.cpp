//
//  main.cpp
//  4 by 4 sudoku solver
//
//  Created by Alex Wright on 25/03/2016.
//  Copyright © 2016 Alex Wright. All rights reserved.
//


#include <iostream>
#include <fstream>


using namespace std;


class sudoku
{
    // Friends
    friend ostream & operator<<(ostream &os, const sudoku &grid);
    friend istream & operator>>(istream &is, sudoku &grid);
    
    
private:
    
public:
    int rows;
    int columns;
    int length;
    int data[256][17];
    
    
    // Default constructor
    sudoku(){ rows = columns = 0; }
    // Parameterized constructor
    sudoku(int m, int n) {
        rows = m; columns = n; length = n*m;
        for (int i(1); i < rows + 1; i++ ) {
            for (int j(1); j < columns + 1; j++ ) {
                for (int k(1); k < rows + 1 ; k++ ) {
                    data[ index(i,j) ][ k ] = k;
                }
            }
        }
        
    }
    // Destructor
    ~sudoku() { rows = columns = 0; }
    
    //Copy constructor
    sudoku(const sudoku &s) {
        rows = s.rows; columns = s.columns; length = s.length;
        for (int i(0); i < length + 1; i++) {
            for (int k(0); k < rows + 1; k++) {
                data[i][k] = s.data[i][k];
            }
        }
        
    }
    
    //Equivalence operator
    bool & operator == (const sudoku &s) {
        static bool same = true;
        for (int i(0); i < length; i++) {
            for (int k(0); k < 17; k++) {
                if( data[i][k] != s.data[i][k] ) {
                    //    cout << "Element " << i << "differs..." << endl;
                    same = false;
                }
            }
        }
        return same;
    }
    
    
    
    // Access functions
    int getrows() const { return rows; } // Return number of rows
    int getcols() const { return columns; } // Return number of columns
    int index(int m, int n) const // Return position in array of element (m,n)
    {
        if (m>0 && m <= rows && n>0 && n <= columns) return (n - 1) + (m - 1)*columns;
        else { cout << "Error: out of range" << endl; exit(1); }
    }
    int & operator()(int i) {return data[i][0];};
    int & operator()(int i, int j) {return data[index(i,j)][0];}
    int & operator()(int i, int j, int k) {return data[index(i,j)][k];}
    
    
    // Check if SuDoku is completed
    bool finished() {
        bool fin(true);
        for (int i(0); i < length; i++ ){
            if ( data[i][0] == 0 ) {
                fin = false;
            }
        }
        return fin;
    }
    
    void checkRow(int i, int j) {
        // check sqaure is zero
        if ( data[ index(i, j)][0] == 0 ) {
            
            //eliminate numbers in row from possibilities
            for (int l(1); l < 17; l++) {
                if ( data[ index(i,l) ][ 0 ]!= 0 ) {
                    data [ index(i,j) ][ data[ index(i,l) ][ 0 ] ] = 0;
                }
            }
            
            
        }
    }
    void checkColumn(int i, int j) {
        if ( data[ index(i, j)][0] == 0 ) {
            for (int l(1); l < 17; l++) {
                if ( data[ index(l,j) ][ 0 ]!= 0 ) {
                    data [ index(i,j) ][ data[ index(l,j) ][ 0 ] ] = 0;
                }
            }
        }
    }
    void checkSquare(int i, int j) {
        int x;
        int y;
        if ( data[ index(i, j)][0] == 0 ) {
            if (i < 5) {x = 1; }
            else if (i > 4 && i < 9 ) {x = 5; }
            else if (i > 8 && i < 13) {x = 9; }
            else if (i > 12) {x = 13;}
            else {cout << "Error setting x in 'checkSquare' for i = " << i; exit(1); }
            if (j < 5) {y = 1; }
            else if (j > 4 && j < 9 ) {y = 5; }
            else if (j > 8 && j < 13) {y = 9; }
            else if (j > 12) {y = 13; }
            else {cout << "Error setting y in 'checkSquare', for j = " << j; exit(1); }
            
            for (int l(x); l < x + 4; l++) {
                for (int m(y); m < y + 4; m++ ) {
                    if ( data[ index(l,m) ][ 0 ]!= 0 ) {
                        data [ index(i,j) ][ data[ index(l,m) ][ 0 ] ] = 0;
                    }
                }
            }
        }
    }
    //Check that a square's possible values are in accordance with complete squares
    void checkPossibles(int i, int j, int k) {
        int count(0);
        //rows
        if (data[index(i,j)][k] != 0 ) {
            for (int z(1); z < 17; z++) {
                if ( data[index(i,z)][k] == 0 ) {
                    count++;
                }
            }
            if (count == 15) {
                data[index(i,j)][0] = data[index(i,j)][k];
                for (int x(1); x < 17; x++) {
                    data[index(i,j)][x] = 0;
                }
            }
        }
        updateSquare(i, j);
        //columns
        if (data[index(i,j)][k] != 0 ) {
            count = 0;
            for (int z(1); z < 17; z++) {
                if ( data[index(z,j)][k] == 0 ) {
                    count++;
                }
            }
            if (count == 15) {
                data[index(i,j)][0] = data[index(i,j)][k];
                for (int x(1); x < 17; x++) {
                    data[index(i,j)][x] = 0;
                }
            }
        }
        updateSquare(i, j);
        //squares
        if (data[index(i,j)][k] != 0 ) {
            count = 0;
            int x, y;
            if (i < 5) {x = 1; }
            else if (i > 4 && i < 9 ) {x = 5; }
            else if (i > 8 && i < 13) {x = 9; }
            else if (i > 12) {x = 13;}
            else {cout << "Error setting x in 'checkSquare'"; exit(1); }
            if (j < 5) {y = 1; }
            else if (j > 4 && j < 9 ) {y = 5; }
            else if (j > 8 && j < 13) {y = 9; }
            else if (j > 12) {y = 13; }
            else {cout << "Error setting y in 'checkSquare'"; exit(1); }
            for (int l(x); l < x + 4; l++) {
                for (int m(y); m < y + 4; m++ ){
                    if (data[index(l,m)][k] == 0 ) {
                        count++;
                    }
                }
            }
            if (count == 15) {
                data[index(i,j)][0] = data[index(i,j)][k];
                for (int x(1); x < 17; x++) {
                    data[index(i,j)][x] = 0;
                }
            }
        }
        updateSquare(i, j);
        
    }
    void checkRowsWithSquare (int row) {
        //for each column, if all the missing squares for it lie within one square, remove them from possibles of rest of square
        //column
        int i = row;
        int m1(0), m2(0), m3(0), m4(0);
        int mmin(1), mmax(16);
        int count(0);
        int temp[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        for (int j(1); j < 17; j++ ) {
            if ( data[index(i,j)][0] != 0 ) {
                temp[data[index(i,j)][0]] = 0;
            }
            else {
                if (count == 0) { mmin = m1 = j; }
                else if (count == 1) { mmax = m2 = j; }
                else if (count == 2) { mmax = m3 = j; }
                else if (count == 3) { mmax = m4 = j; }
                count++;
            }
        }
        if (count <= 4 ) {
            
            //check if in same square
            if ( (mmin >= 1 && mmax <= 4) || (mmin > 4 && mmax <= 8) || (mmin > 8 && mmax <=12) || (mmin > 12 && mmax <= 16 ) ) {
                // are in the same square --- remove data from possibles of rest of square
                int x, y;
                if (i < 5) {x = 1; }
                else if (i > 4 && i < 9 ) {x = 5; }
                else if (i > 8 && i < 13) {x = 9; }
                else if (i > 12) {x = 13;}
                else {cout << "Error setting x in 'checkSquare'"; exit(1); }
                if (mmax < 5) {y = 1; }
                else if (mmin > 4 && mmax < 9 ) {y = 5; }
                else if (mmin > 8 && mmax < 13) {y = 9; }
                else if (mmin > 12) {y = 13; }
                else {cout << "Error setting y in 'checkSquare'"; exit(1); }
                
                for (int q(x); q < x+4; q++) {
                    for (int r(y); r < y+4; r++) {
                        if (data[index(q,r)][0] == 0 ) {
                            if (q != i) {
                                for (int k(1); k < 17 ; k ++ ) {
                                    if (data[index(i,m1)][k] != 0 ) {
                                        data[index(q,r)][k] = 0;
                                        updateSquare(q,r);
                                    }
                                }
                                if (m2 != 0) {
                                    for (int k(1); k < 17 ; k ++ ) {
                                        if (data[index(i,m2)][k] != 0 ) {
                                            data[index(q,r)][k] = 0;
                                            updateSquare(q,r);
                                        }
                                    }
                                }
                                if (m3 != 0) {
                                    for (int k(1); k < 17 ; k ++ ) {
                                        if (data[index(i,m3)][k] != 0 ) {
                                            data[index(q,r)][k] = 0;
                                            updateSquare(q,r);
                                        }
                                    }
                                }
                                if (m4 != 0) {
                                    for (int k(1); k < 17 ; k ++ ) {
                                        if (data[index(i,m4)][k] != 0 ) {
                                            data[index(q,r)][k] = 0;
                                            updateSquare(q,r);
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
    void checkColumnsWithSquare (int col) {
        //for each column, if all the missing squares for it lie within one square, remove them from possibles of rest of square
        //column
        int j = col;
        int l1(0), l2(0), l3(0), l4(0);
        int lmin(1), lmax(16);
        int count(0);
        int temp[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        for (int i(1); i < 17; i++ ) {
            if ( data[index(i,j)][0] != 0 ) {
                temp[data[index(i,j)][0]] = 0;
            }
            else {
                if (count == 0) { lmin = l1 = i; }
                else if (count == 1) { lmax = l2 = i; }
                else if (count == 2) { lmax = l3 = i; }
                else if (count == 3) { lmax = l4 = i; }
                count++;
            }
        }
        if (count <= 4 ) {
            
            //check if in same square
            if ( (lmin >= 1 && lmax <= 4) || (lmin >= 5 && lmax <= 8) || (lmin >= 9 && lmax <=12) || (lmin >= 13 && lmax <= 16) ) {
                // are in the same square --- remove data from possibles of rest of square
                int x, y;
                if (lmax < 5) {x = 1; }
                else if (lmin > 4 && lmax < 9 ) {x = 5; }
                else if (lmin > 8 && lmax < 13) {x = 9; }
                else if (lmin > 12) {x = 13;}
                else {cout << "Error setting x in 'checkSquare'"; exit(1); }
                if (j < 5) {y = 1; }
                else if (j > 4 && j < 9 ) {y = 5; }
                else if (j > 8 && j < 13) {y = 9; }
                else if (j > 12) {y = 13; }
                else {cout << "Error setting y in 'checkSquare'"; exit(1); }
                
                for (int q(x); q < x+4; q++) {
                    for (int r(y); r < y+4; r++) {
                        if (data[index(q,r)][0] == 0 ) {
                            if (r != j) {
                                if (l1 != 0) {
                                    for (int k(1); k < 17 ; k ++ ) {
                                        if (data[index(l1,j)][k] != 0 ) {
                                            data[index(q,r)][k] = 0;
                                            updateSquare(q,r);
                                        }
                                    }
                                }
                                if (l2 != 0) {
                                    for (int k(1); k < 17 ; k ++ ) {
                                        if (data[index(l2,j)][k] != 0 ) {
                                            data[index(q,r)][k] = 0;
                                            updateSquare(q,r);
                                        }
                                    }
                                }
                                if (l3 != 0) {
                                    for (int k(1); k < 17 ; k ++ ) {
                                        if (data[index(l3,j)][k] != 0 ) {
                                            data[index(q,r)][k] = 0;
                                            updateSquare(q,r);
                                        }
                                    }
                                }
                                if (l4 != 0) {
                                    for (int k(1); k < 17 ; k ++ ) {
                                        if (data[index(l4,j)][k] != 0 ) {
                                            data[index(q,r)][k] = 0;
                                            updateSquare(q,r);
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
    
    
    void updateSquare(int i, int j) {
        int check(0);
        int s(0);
        for (int k(1); k < 17; k++) {
            if (data[index(i,j)][k] == 0 ) {
                check++;
            }
            else {
                s = k;
            }
        }
        if (check == 15 ) { // only one possibility
            //fill in square
            cout <<  "Element " << index(i,j) << " assigned " << endl ;
            data[index(i,j)][0] = data[index(i,j)][s];
            data[index(i,j)][s] = 0;
        }
        
    }
    // Validate completed matrix
    bool valid() {
        
        //rows
        bool val = true;
        for (int i(1); i < 17; i++) {
            int fill[9];
            for (int a(1); a < 17; a++) {
                fill[a] = a;
            }
            
            for (int j(1); j < 17; j++) {
                fill[data[index(i,j)][0]] = 0;
            }
            for (int p(1); p < 17; p++) {
                if ( fill[p] != 0 ) {
                    val = false;
                }
            }
        }
        
        //columns
        for (int i(1); i < 17; i++) {
            int fill[16];
            for (int a(1); a < 17; a++) {
                fill[a] = a;
            }
            
            for (int j(1); j < 17; j++) {
                fill[data[index(j,i)][0]] = 0;
            }
            for (int p(1); p < 17; p++) {
                if ( fill[p] != 0 ) {
                    val = false;
                }
            }
        }
        //squares
        for (int i(0); i < 4; i++) {
            for (int j(0); j < 4; j++) {
                int fill[16];
                for (int a(1); a < 17; a++) {
                    fill[a] = a;
                }
                for (int a(1 + i*4); a < 5 + i*4; a++ ) {
                    for (int b(1 + j*4); b < 5 + j*4; b++) {
                        fill[data[index(a,b)][0]] = 0;
                    }
                }
                for (int p(1); p < 17; p++) {
                    if ( fill[p] != 0 ) {
                        val = false;
                    }
                }
            }
        }
        return val;
    }
    
    
};




// Member functions defined outside class
// Overload insertion to output stream for matrices
ostream & operator<<(ostream &os, sudoku &grid) {
    cout << endl;
    for (int i(1); i < grid.getrows()+1; i++) {
        for (int j(1); j < grid.getcols()+1; j++) {
            if ( j == 5 || j == 9 || j == 13) {cout << " ";}
            cout << grid(i,j) << " ";
        }
        if ( i == 4 || i == 8 || i ==12 ) { cout << endl;}
        cout << endl;
    }
    cout << endl;
    return os;
}










int main() {
    
    sudoku g(16,16);
    sudoku check(16,16);
    sudoku initial(16,16);
    
    
    fstream myfile;
    myfile.open("sudoku.txt");
    if (!myfile.is_open()) {
        cout << "Error opening file!" << endl;
        exit(1);
    }
    
    cout << "Filling in Su Doku grid..." << endl;
    for (int i(1); i < g.getrows() + 1; i++) {
        for (int j(1); j < g.getcols() + 1; j++ ) {
            myfile >> g(i,j);
        }
    }
    //set checking martix
    check = g;
    initial = g;
    
    cout << endl << endl << "Su Doku Grid to solve:" << endl << endl << g;
    
    bool solver_finished(g.finished());
    int loop(0); //counter
    int doubleCheck(0);
    do
        
    {
        bool same(true);
        for (int i(0); i < 256; i++ ) {
            for (int k(0); k < 17; k++) {
                if (check.data[i][k] != g.data[i][k] ) {
                    same = false;
                }
            }
        }
        
        
        // has sudoku changed in last run?
        if (same == true && loop != 0) {
            if (doubleCheck == 0 || doubleCheck == 1 || doubleCheck == 2 ) {
                doubleCheck++;
               // for (int pt(1); pt < 17; pt++) {
                //    g.checkColumnsWithSquare(pt);
                //    g.checkRowsWithSquare(pt);
               // }
            }
            else if (doubleCheck == 3 ){
                cout << endl << endl << "Can not make any more progress!" << endl << "After " << loop << " iterations" << endl << endl << "Initial input was:" << initial << endl << endl << "Final output of programme is:" << endl << g;
                exit(1);
            }
        }
        else {
            doubleCheck = 0;
        }
        loop ++;
        check = g;
        
        // Search for complete squares
        for (int i(1); i < g.getrows() + 1; i++ ) {
            for (int j(1); j < g.getcols() + 1; j++ ) {
                
                if ( g(i,j) != 0 ) { //square is complete!
                    
                    //set all possible values to zero
                    for (int k(1); k<17 ;k++ ) {
                        g(i,j,k) = 0;
                    }
                }
                
                else { // square needs to be completed!!
                    g.checkRow(i, j);
                    g.updateSquare(i, j);
                    g.checkColumn(i, j);
                    g.updateSquare(i, j);
                    g.checkSquare(i, j);
                    g.updateSquare(i, j);
                    for (int k(1); k < 17; k++) {
                        g.checkPossibles(i,j,k);
                    }
                    g.updateSquare(i,j);
                    
                    /*
                    //See how many have 2 possibilities...
                    int howManyPos(0);
                    for ( int k(1); k < 17; k++ ) {
                        if (g.data[g.index(i,j)][k] != 0 ) { howManyPos++; }
                    }
                    if ( howManyPos == 2) {
                        cout << "(i,j) = (" << i << "," << j << "):" << endl;
                        for ( int k(1); k < 17; k++ ) {
                            if (g.data[g.index(i,j)][k] != 0 ) { cout << g.data[g.index(i,j)][k] << endl; }
                        }
                    }
                     */
                    
                    
                }
            }
        }
        solver_finished = g.finished();
        cout << endl << endl << "for loop = " << loop  << endl << "g:" << g;
        
    } while (solver_finished == false);
    
    
    if (solver_finished) {
        cout << "####################" << endl;
        cout << "SuDoku is completed!" << endl << "####################" << endl;
        cout << endl << endl << "After " << loop << " iterations." << endl <<"####################" << endl <<"Completed sudoku is:" << endl << "####################" << endl << endl << g << "####################" << endl;
    }
    
    cout << "Testing validation..." << endl << "####################" << endl;
    if (g.valid() == true) {
        cout << "  Sudoku is valid!" << endl << "####################" << endl << endl;
    }
    else {
        cout << endl << "WARNING : Sudoku is NOT valid!" << endl << endl;
    }
    
    
    return 0;
}