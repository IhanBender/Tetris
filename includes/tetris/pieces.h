#ifndef __PIECES_H_INCLUDED_
#define __PIECES_H_INCLUDED_

#include <iostream>
#include <vector>
#include <stdlib.h> 

struct Piece {
    char name;
    unsigned int state;
    int x;
    int y;
};

std::vector<int> I0 = {0, 0, 0,-1, 0,-2, 0,-3};
std::vector<int> I1 = {0,-1, 1,-1, 2,-1, 3,-1};
std::vector <std::vector<int>> I = {I0, I1};

std::vector<int> O0 = {0,0, 1,0, -1,0, -1,-1};
std::vector <std::vector<int>> O = {O0};

std::vector<int> T0 = {0,0, 1,0, 2,0, 1,-1};
std::vector<int> T1 = {0,0, 1,1, 1,0, 1,-1};
std::vector<int> T2 = {0,0, 1,0, 2,0, 1,1};
std::vector<int> T3 = {2,0, 1,1, 1,0, 1,-1};
std::vector <std::vector<int>> T = {T0, T1, T2, T3};

std::vector<int> S0 = {1,0, 2,0, 0,-1, 1,-1};
std::vector<int> S1 = {0,0, 0,1, 1, 0, 1,-1};
std::vector<std::vector<int>> S = {S0, S1};

std::vector<int> Z0 = {0, 0, 1,0, 1,-1, 2,-1};
std::vector<int> Z1 = {0,-1, 0,0, 1, 0, 1, 1};
std::vector<std::vector<int>> Z = {Z0, Z1};

std::vector<int> J0 = {1, 1, 1,0, 1,-1, 0,-1};
std::vector<int> J1 = {0, 1, 0,0, 1, 0, 2, 0};
std::vector<int> J2 = {1,-1, 1,0, 1, 1, 2, 1};
std::vector<int> J3 = {0,0, 1,0, 2,0, 2,-1};
std::vector<std::vector<int>> J = {J0, J1, J2, J3};

std::vector<int> L0 = {1, 1, 1,0, 1,-1, 2,-1};
std::vector<int> L1 = {0,-1, 0,0, 1, 0, 2, 0};
std::vector<int> L2 = {0, 1, 1,1, 1, 0, 1,-1};
std::vector<int> L3 = {0, 0, 1,0, 2, 0, 2, 1};
std::vector<std::vector<int>> L = {L0, L1, L2, L3};


// Checks if piece collides with another piece already set
bool collides(Piece piece, bool ** map) {
    std::vector<int> coords = getPieceCoords(piece);

    for (int i = 0; i < coords.size; i += 2) {
        int x = coords[i];
        int y = coords[i+1];

        if(map[x][y]) 
            return true;
    }


    return false;

}



char generatePiece(){
    int v = rand() % 7;

    switch (v) {
        case 0: return 'i';
        case 1: return 'o';
        case 2: return 't';
        case 3: return 's';
        case 4: return 'z';
        case 5: return 'j';
        case 6: return 'l';
    }

    return 'i';
}

// Returns coordinates to plot pieces, based on their current main position
std::vector<int> getPieceCoords(Piece piece){

    int state = piece.state;
    char name = piece.name;

    if (name == 'i' || name == 'I')
        if (state >= 0 && state <= 2)
            return I[state];

    if (name == 'o' || name == 'O')
        if (state == 0)
            return O[0];

    if (name == 't' || name == 'T')
        if (state >= 0 && state <= 4)
            return T[state];

    if (name == 's' || name == 'S')
        if (state >= 0 && state <= 2)
            return S[state];

    if (name == 'z' || name == 'Z')
        if (state >= 0 && state <= 2)
            return Z[state];

    if (name == 'j' || name == 'J')
        if (state >= 0 && state <= 4)
            return J[state];

    if (name == 'l' || name == 'L')
        if (state >= 0 && state <= 4)
            return L[state];

    // Should never reach this line
    std::vector<int> r = {};
    return r;   
}



#endif // __PIECES_H__ 

	
