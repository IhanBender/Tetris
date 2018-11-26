#ifndef __PIECES_H_INCLUDED_
#define __PIECES_H_INCLUDED_

#include <iostream>
#include <vector>
#include <stdlib.h> 

struct Piece {
    float x;
    float y;
    unsigned int state;
    char name;
};

std::vector<int> I0 = {0,0, 1,0, 2, 0, 3, 0};
std::vector<int> I1 = {1,1, 1,0, 1,-1, 1,-2};
std::vector <std::vector<int>> I = {I0, I1};

std::vector<int> O0 = {0,0, 1,0, 0,-1, 1,-1};
std::vector <std::vector<int>> O = {O0};

std::vector<int> T0 = {0,0, 1,0, 2,0, 1,-1};
std::vector<int> T1 = {0,0, 1,1, 1,0, 1,-1};
std::vector<int> T2 = {0,0, 1,0, 2,0, 1,1};
std::vector<int> T3 = {2,0, 1,1, 1,0, 1,-1};
std::vector <std::vector<int>> T = {T0, T1, T2, T3};

std::vector<int> S0 = {0,0, 1,0, -1,-1, 0,-1};
std::vector<int> S1 = {0,0, 0,1, 1, 0, 1,-1};
std::vector<std::vector<int>> S = {S0, S1};

std::vector<int> Z0 = {0, 0, 1,0, 1,-1, 2,-1};
std::vector<int> Z1 = {0,-1, 0,0, 1, 0, 1, 1};
std::vector<std::vector<int>> Z = {Z0, Z1};

std::vector<int> J0 = {0,0, 1,0, 2,0, 2,-1};
std::vector<int> J1 = {1, 1, 1,0, 1,-1, 0,-1};
std::vector<int> J2 = {0, 1, 0,0, 1, 0, 2, 0};
std::vector<int> J3 = {1,-1, 1,0, 1, 1, 2, 1};
std::vector<std::vector<int>> J = {J0, J1, J2, J3};

std::vector<int> L0 = {0,-1, 0,0, 1, 0, 2, 0};
std::vector<int> L1 = {0, 1, 1,1, 1, 0, 1,-1};
std::vector<int> L2 = {0, 0, 1,0, 2, 0, 2, 1};
std::vector<int> L3 = {1, 1, 1,0, 1,-1, 2,-1};
std::vector<std::vector<int>> L = {L0, L1, L2, L3};

char generatePiece(){
    srand(time(NULL));
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

Piece genCurrentPiece(char name) {
    Piece currentPiece;
    currentPiece.name = name;
    currentPiece.state = 0;
    currentPiece.x = 4;
    currentPiece.y = 20;

    return currentPiece;
}

Piece genNextPiece(){
    Piece nextPiece;
    nextPiece.name = generatePiece();
    nextPiece.state = 0;
    if (nextPiece.name == 'i'){ nextPiece.x = 12;    nextPiece.y = 9.7; }   
    if (nextPiece.name == 'o'){ nextPiece.x = 13;    nextPiece.y = 10;   }
    if (nextPiece.name == 't'){ nextPiece.x = 12.7;  nextPiece.y = 10;   }
    if (nextPiece.name == 's'){ nextPiece.x = 13.5;  nextPiece.y = 10;   }
    if (nextPiece.name == 'z'){ nextPiece.x = 12.5;  nextPiece.y = 10;   }
    if (nextPiece.name == 'j'){ nextPiece.x = 12.5;  nextPiece.y = 10;   }
    if (nextPiece.name == 'l'){ nextPiece.x = 12.5;  nextPiece.y = 10;   }

    return nextPiece;
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

// Checks if piece collides with another piece already set or the bottom
bool collides(Piece piece, bool ** map) {
    std::vector<int> coords = getPieceCoords(piece);

    for (int i = 0; i < (int)coords.size(); i += 2) {
        int x = coords[i];
        int y = coords[i+1];

        if(
            y + piece.y < 0    ||
            map[x + (int)piece.x][y + (int)piece.y]
        ) 
            return true;
    }

    return false;
}

bool canMoveDown(Piece piece, bool ** map) {
    piece.y -= 1.0;
    std::vector<int> newCoords = getPieceCoords(piece);

    for(int i = 0; i < newCoords.size(); i+=2) {
        if ((int)piece.y + newCoords[i+1] < 0 ||
            collides(piece, map)
        )
            return false;
    }

    return true;
}

bool canMoveRight(Piece piece,bool ** map) {
    piece.x += 1.0;
    std::vector<int> newCoords = getPieceCoords(piece);

    for(int i = 0; i < newCoords.size(); i+=2) {
        if ((int)piece.x + newCoords[i] < 0 || (int)piece.x + newCoords[i] > 9)
            return false;
    }

    if(collides(piece, map))
        return false;

    return true;
}

bool canMoveLeft(Piece piece,bool ** map) {
     piece.x -= 1.0;
    std::vector<int> newCoords = getPieceCoords(piece);

    for(int i = 0; i < newCoords.size(); i+=2) {
        if ((int)piece.x + newCoords[i] < 0 || (int)piece.x + newCoords[i] > 9)
            return false;
    }

    if(collides(piece, map))
        return false;

    return true;
}

unsigned int rotateRight(Piece piece,bool ** map){
    unsigned int state = piece.state;
    unsigned int newState;

    if (piece.name == 'o') {
        return state;
    }
    if (piece.name == 't' || piece.name == 'j' || piece.name == 'l' && state == 3){
        newState = 0;
    } 
    else if ((piece.name == 'i' || piece.name == 's' || piece.name == 'z') && state == 1 ){
        newState = 0;
    }
    else {
        newState = state + 1;
    }

    // Acts like if had changed state
    piece.state = newState;
    std::vector<int> newCoords = getPieceCoords(piece);
    for (int i = 0; i < newCoords.size(); i+=2) {
        if( 
            (int)piece.x + newCoords[i] < 0      ||
            (int)piece.x + newCoords[i] > 9      ||
            (int)piece.y + newCoords[i+1] < 0    ||
            map[(int)piece.x + newCoords[i]][(int)piece.y + newCoords[i+1]]) 
        {
            return state;
        }
    }

    return newState;
}

unsigned int rotateLeft(Piece piece, bool ** map){
    unsigned int state = piece.state;
    unsigned int newState;

    if (piece.name == 'o') {
        return state;
    }

    if (state == 0) {
        if      (piece.name == 't' || piece.name == 'j' || piece.name == 'l'){
            newState = 3;
        } 
        else if (piece.name == 'i' || piece.name == 's' || piece.name == 'z'){
            newState = 1;
        }
    }
    else {
        newState = state - 1;
    }

    piece.state = newState;
    std::vector<int> newCoords = getPieceCoords(piece);
    for (int i = 0; i < newCoords.size(); i+=2) {
        if(
            piece.x + newCoords[i] < 0      ||
            piece.x + newCoords[i] > 9      ||
            piece.y + newCoords[i+1] < 0    ||
            map[(int)piece.x + newCoords[i]][(int)piece.y + newCoords[i+1]]) 
        {
            return state;
        }
    }
    return newState;
}

void updateMap(Piece piece, bool ** map) {
    std::vector<int> coords = getPieceCoords(piece);
    for (int i = 0; i < coords.size(); i+=2){
        map[(int)piece.x + coords[i]][(int)piece.y + coords[i+1]] = true; 
    }
}

// Removes lines from map and returns number of lines removed
unsigned int removeFullLines(bool ** map) {
    unsigned int linesRemoved = 0;
    int firstLine = -1, i, j;
    bool mustRemove;
    // Checks for number of lines to remove
    for(i = 0; i < 20; i++){
        mustRemove = true;
        for(j = 0; j < 10 && mustRemove; j++){
            if(!map[j][i])
                mustRemove = false;
        }
        if(mustRemove) {
            ++linesRemoved;
            if (firstLine == -1){
                printf("FristLines\n");
                firstLine = i;
                printf("%d\n", firstLine);
            }
        }
    }
    // Remove lines
    if (firstLine != -1)
        for (i = 0; i < 10; i++) {
            for (j = firstLine; j < 20; j++) {
                map[i][j] = map[i][j+linesRemoved];
            }
        }

    return linesRemoved;
}

bool gameOver(bool ** map){
    for (int i = 3; i < 8; i++){
        if (map[i][19])
            return true;
    }
    return false;
}

#endif // __PIECES_H__ 

	
