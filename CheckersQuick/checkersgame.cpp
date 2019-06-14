#include "checkersgame.h"
#include <iostream>
#include <iomanip>

CheckersGame::CheckersGame(QObject *parent) : QObject(parent)
{
    _board = nullptr;
    m_active_player = CheckersGame::WHITE_PLAYER;
}

CheckersBoard *CheckersGame::board() const
{
    return _board;
}

void CheckersGame::initializeBoard()
{
    setBoard(new CheckersBoard(this));
}

void CheckersGame::setBoard(CheckersBoard *board)
{
    if(_board == board)
        return;
    if(_board)
        delete  _board;
    _board = board;
    emit boardChanged(board);
}

void CheckersGame::newGame()
{
    initializeBoard();
    setActivePlayer(WHITE_PLAYER);
}
// Calculate position for piece at given postion
void CheckersGame::calculateMovesForPiece(int row, int col)
{
    int square = _board->square(row,col);
    if(square == CheckersBoard::EMPTY_SQUARE || square == CheckersBoard::NOT_ON_BOARD)
        return;
    //Check if given piece belongs to active player
    char color = pieceColor(row,col);
    if( (color == WHITE && m_active_player == BLACK_PLAYER) ||
         (color == BLACK && m_active_player == WHITE_PLAYER) )
        return;
    QList<int> availablePos;
    calculateMovesForPiece(row, col, square, availablePos);
    emit availableMovesCalculated(row,col, availablePos);
}

char CheckersGame::pieceColor(int row, int col)
{
    int piece = _board->square(row,col);
    if(piece == CheckersBoard::WHITE_PAWN || piece == CheckersBoard::WHITE_QUEEN)
        return WHITE;
    return BLACK;
}

void CheckersGame::movePiece(int srow, int scol, int trow, int tcol)
{
    int source_sq = _board->square(srow,scol);
    int target_sq = _board->square(trow, tcol);
    int dr = trow - srow;
    int dc = tcol - scol;
    if(source_sq == CheckersBoard::EMPTY_SQUARE || source_sq == CheckersBoard::NOT_ON_BOARD || target_sq != CheckersBoard::EMPTY_SQUARE ||
            dr == 0 || dc == 0 /*only diagonal moves are allowed*/)
        return; // this is not a valid move
    bool up = dr < 0;
    bool right = dc > 0; // right direction is oriented when facing up
    for(int i = 1; i<=abs(dr); ++i)
    { //Check if any piece is captured with the move
        int row = -1; int col=-1;
        if(up && right)
        {
            row = srow-i;
            col = scol+i;
        }
        else if(!up && right)
        {
            row = srow+i;
            col = scol+i;
        }
        else if(up && !right)
        {
            row = srow-i;
            col = scol-i;
        }
        else if(!up && !right)
        {
            row = srow+i;
            col = scol-i;
        }
        int sq = _board->square(row, col);
        if(sq != CheckersBoard::EMPTY_SQUARE && sq != CheckersBoard::NOT_ON_BOARD)
            _board->capturePiece(row,col);
        if(_board->whitePiecesCount() <= 0)
            emit gameEnded(BLACK_PLAYER);
        else if(_board->blackPiecesCount() <= 0)
           emit gameEnded(WHITE_PLAYER);
    }
    _board->movePiece(srow, scol, trow, tcol);
    if(trow == 0 || trow == (_board->rows() - 1)) //  end row reached - promote pawn to queen
        promote(trow, tcol);
    //Other player turn
    if(_board->whitePiecesCount() > 0 && _board->blackPiecesCount() > 0) // Check end game condidtions before switching player
        setActivePlayer(!m_active_player);
}

void CheckersGame::promote(int row, int col)
{
    int piece = _board->square(row,col);
    if(piece == CheckersBoard::WHITE_PAWN)
        _board->promote(row, col, CheckersBoard::WHITE_QUEEN);
    else if(piece == CheckersBoard::BLACK_PAWN)
        _board->promote(row,col, CheckersBoard::BLACK_QUEEN);
}

void CheckersGame::setActivePlayer(bool player)
{
    if(player == m_active_player)
        return;
    m_active_player = player;
    emit activePlayerChanged(m_active_player);
}

//Find squares available for given piece
void CheckersGame::calculateMovesForPiece(int row, int col, int piece,  QList<int> &availablePos)
{
    if(piece == CheckersBoard::EMPTY_SQUARE || piece == CheckersBoard::NOT_ON_BOARD ||
        _board->square(row,col) == CheckersBoard::NOT_ON_BOARD)
        return;
    bool queen = (piece == CheckersBoard::WHITE_QUEEN || piece == CheckersBoard::BLACK_QUEEN);

    //First check if any captures are available
    bool force_capture = capturesForPiece(row,col,piece,availablePos);
    if(force_capture)
        return; // Force capture move

    //Bottom right square
    if(piece == CheckersBoard::BLACK_PAWN || queen)
    {
        int bottom_right = _board->square(row+1, col -1);
        if(bottom_right == CheckersBoard::EMPTY_SQUARE){
            availablePos.push_back(row+1);
            availablePos.push_back(col-1);
        }
    }
    //Bottom left square
    if(piece == CheckersBoard::BLACK_PAWN || queen)
    {
        int bottom_left = _board->square(row+1, col+1);
        if(bottom_left == CheckersBoard::EMPTY_SQUARE){
            availablePos.push_back(row+1);
            availablePos.push_back(col+1);
        }
    }
    //Top right square
    if(piece == CheckersBoard::WHITE_PAWN || queen){
        int top_right = _board->square(row-1, col+1);
        if(top_right == CheckersBoard::EMPTY_SQUARE){
            availablePos.push_back(row-1);
            availablePos.push_back(col+1);
        }
    }
    //Top left square
    if(piece == CheckersBoard::WHITE_PAWN || queen){
        int top_left = _board->square(row-1, col-1);
        if(top_left == CheckersBoard::EMPTY_SQUARE){
            availablePos.push_back(row-1);
            availablePos.push_back(col-1);
        }
    }
}


//Check if given piece has any capture options, if it has true is returned
bool CheckersGame::capturesForPiece(int row, int col, int piece,  QList<int> &availablePos)
{
    if(piece == CheckersBoard::EMPTY_SQUARE || piece == CheckersBoard::NOT_ON_BOARD ||
        _board->square(row,col) == CheckersBoard::NOT_ON_BOARD)
        return false;
    bool queen = (piece == CheckersBoard::WHITE_QUEEN || piece == CheckersBoard::BLACK_QUEEN);
    bool force_capture = false;
    //Bottom right square
    if(piece == CheckersBoard::BLACK_PAWN || queen)
    {
        int bottom_right = _board->square(row+1, col -1);
        if(canCapture(piece, bottom_right) && _board->square(row+2, col-2) == CheckersBoard::EMPTY_SQUARE)
        {
            force_capture = true;
            availablePos.push_back(row+2);
            availablePos.push_back(col-2);
            //calculateMovesForPiece(row+2, col-2, piece, availablePos); // calculate postions available after capturing the piece
        }
    }
    //Bottom left square
    if(piece == CheckersBoard::BLACK_PAWN || queen)
    {
        int bottom_left = _board->square(row+1, col+1);
        if( canCapture(piece, bottom_left) && _board->square(row+2, col+2) == CheckersBoard::EMPTY_SQUARE)
        {
            force_capture = true;
            availablePos.push_back(row+2);
            availablePos.push_back(col+2);
            //availablePos.push_back(QPair<int,int>(row+2, col+2));
            //calculateMovesForPiece(row+2, col+2, piece, availablePos); // calculate postions available after capturing the piece
        }
    }
    //Top right square
    if(piece == CheckersBoard::WHITE_PAWN || queen){
        int top_right = _board->square(row-1, col+1);
        if(canCapture(piece, top_right) && _board->square(row-2, col+2) == CheckersBoard::EMPTY_SQUARE)
        {
            force_capture = true;
            availablePos.push_back(row-2);
            availablePos.push_back(col+2);
            //availablePos.push_back(QPair<int,int>(row-2, col+2));
            //calculateMovesForPiece(row-2, col+2, piece, availablePos); // calculate postions available after capturing the piece
        }
    }
    //Top left square
    if(piece == CheckersBoard::WHITE_PAWN || queen){
        int top_left = _board->square(row-1, col-1);
        if(canCapture(piece, top_left) && _board->square(row-2, col-2) == CheckersBoard::EMPTY_SQUARE)
        {
            force_capture = true;
            availablePos.push_back(row-2);
            availablePos.push_back(col-2);
//            availablePos.push_back(QPair<int,int>(row-2, col-2));
            //calculateMovesForPiece(row-2, col-2, piece, availablePos); // calculate postions available after capturing the piece
        }
    }
    return force_capture;
}

//Returns true if piece1 can capture piece2
bool CheckersGame::canCapture(int piece1, int piece2)
{
    if(piece1 == CheckersBoard::NOT_ON_BOARD ||
       piece2 == CheckersBoard::NOT_ON_BOARD)
        return false;
    // black pieces are encoded as positive number and white pieces are encoded as negative numbers
    // only if pieces have opossing colors the result of * will be negative
    if(piece1 * piece2 < 0)
        return true;
    return false;
}

