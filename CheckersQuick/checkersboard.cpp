#include "checkersboard.h"
#include <iostream>

CheckersBoard::CheckersBoard(QObject *parent) : QObject(parent)
{
    _rows = _columns = 8;
    m_black_pieces = m_white_pieces =0;
    setupBoard();
}

void CheckersBoard::setupBoard()
{//Place all pieces in starting positions
 // 0,0 is top left corner
 // Blacks are on top, whites are at the bottom
    m_black_pieces = m_white_pieces =0;
    for(int r = 0; r<_rows; ++r)
    {
        for(int c = 0; c<_columns; ++c)
        {
            if(r < 3 && (r+c)%2)//Black pieces on top
            {
                m_board[r][c] = BLACK_PAWN;
                m_black_pieces++;
            }
            else if(r >= (_rows-3) && (r+c)%2) // White pieces on the bottom
            {
                m_board[r][c] = WHITE_PAWN;
                m_white_pieces++;
            }
            else
                m_board[r][c] = EMPTY_SQUARE;
        }
    }
}

//Return value of the squre at row,column position
//Note: method does not check if the board exists or if the row and column are within board size
int CheckersBoard::square(int row, int column)
{
    if( row >= _rows|| row < 0 || column >= columns() || column < 0)
        return NOT_ON_BOARD;
    return m_board[row][column];
}

void CheckersBoard::movePiece(int srow, int scol, int trow, int tcol)
{
    int piece = m_board[srow][scol];
    m_board[srow][scol] = EMPTY_SQUARE;
    m_board[trow][tcol] = piece;
    emit pieceMoved(srow,scol,trow,tcol);
}

void CheckersBoard::capturePiece(int row, int col)
{
    int piece = m_board[row][col];
    if(piece ==  WHITE_PAWN || piece == WHITE_QUEEN)
        m_white_pieces--;
    else if(piece ==  BLACK_PAWN || piece == BLACK_QUEEN)
        m_black_pieces--;
    m_board[row][col] = EMPTY_SQUARE;
    emit pieceCaptured(row,col);
}

void CheckersBoard::promote(int row, int col, int promoteTo)
{
    m_board[row][col] = promoteTo;
    emit piecePromoted(row,col, promoteTo);
}

