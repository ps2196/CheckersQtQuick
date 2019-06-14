#ifndef CHECKERSBOARD_H
#define CHECKERSBOARD_H

#include <QPointer>
#include <QObject>
#include <QList>


// Holds information on current board state, data source for drawing operations
class CheckersBoard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rows READ rows)
    Q_PROPERTY(int columns READ columns)
    Q_PROPERTY(int blackPiecesCount READ blackPiecesCount)
    Q_PROPERTY(int whitePiecesCount READ whitePiecesCount)
private:
   int _rows; int _columns; //  board size
   int m_black_pieces; int m_white_pieces;
   int m_board[8][8];

public:
    static const int EMPTY_SQUARE = 0; static const int NOT_ON_BOARD = 404;
    static const int BLACK_PAWN = 1; static const int BLACK_QUEEN = 2;
    static const int WHITE_PAWN = -1; static const int WHITE_QUEEN = -2;
    explicit CheckersBoard(QObject *parent = nullptr);
    int** board()const {return (int**)m_board;}
    void setupBoard();
    int square(int row, int column);
    void movePiece(int srow, int scol, int trow, int tcol);
    void capturePiece(int row, int col);
    void promote(int row, int col, int promoteTo);
signals:
    void pieceCaptured(int row, int col);
    void pieceMoved(int srow, int scol, int trow, int tcol);
    void piecePromoted(int row,int col, int promoteTo);
public slots:
    int rows(){return _rows;}
    int columns(){return _columns;}
    int blackPiecesCount() {return m_black_pieces;}
    int whitePiecesCount() {return m_white_pieces;}
};

#endif // CHECKERSBOARD_H
