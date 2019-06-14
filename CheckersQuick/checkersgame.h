#ifndef CHECKERSGAME_H
#define CHECKERSGAME_H
#include "QObject"
#include "checkersboard.h"

// Game controler, calculates game state and conditions, validates moves etc.
class CheckersGame : public QObject
{
    Q_OBJECT
    //typedef QPair<int,int> Pos;
    Q_PROPERTY(bool activePlayer READ activePlayer WRITE setActivePlayer)
    Q_PROPERTY(CheckersBoard* board READ board WRITE setBoard NOTIFY boardChanged)
    Q_PROPERTY(bool whitePlayer READ whitePlayer)
    Q_PROPERTY(bool blackPlayer READ blackPlayer)
private:
   CheckersBoard* _board;
   bool m_active_player;


public:
    typedef QPair<int,int> Pos;
    static const bool WHITE_PLAYER = true; static const bool BLACK_PLAYER = false;
    static const char BLACK = 'b'; static const char WHITE = 'w';
    explicit CheckersGame(QObject *parent = nullptr);
    CheckersBoard* board() const;
    bool whitePlayer() const {return WHITE_PLAYER;}
    bool blackPlayer() const {return BLACK_PLAYER;}

protected:
    void calculateMovesForPiece(int row, int col, int piece, QList<int> &availablePos);
    bool canCapture(int piece1, int piece2);
    bool capturesForPiece(int row, int col, int piece, QList<int> &availablePos);
    void promote(int,int);
    char pieceColor(int row, int col);
public slots:
    bool activePlayer() const { return m_active_player;}
    void setActivePlayer(bool player);
protected slots:
    virtual void initializeBoard();
    void setBoard(CheckersBoard *board);

signals:
    void boardChanged(CheckersBoard* board);
    void availableMovesCalculated(int row, int col, QList<int> availablePos);
    void gameEnded(bool winner);
    void activePlayerChanged(bool activePlayer);

public slots:
    virtual void newGame();
    void calculateMovesForPiece(int row, int col);
    void movePiece(int srow, int scol, int trow, int tcol);
};

#endif // CHECKERSGAME_H
