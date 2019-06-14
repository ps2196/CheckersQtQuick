import QtQuick 2.4

Grid{
    id:board
    columns: 8
    rows: columns
    property int squareSize: 1
    scale: Math.min(parent.width / width, parent.height / height)
    anchors.fill:parent

    Repeater{
        model: board.rows * board.columns
        Rectangle{
            readonly property int col: index % board.columns
            readonly property int row: Math.floor(index/board.rows)
            width: board.squareSize
            height: board.squareSize
            color: (row+col)%2 ? "black":"white"
        }
    }
}
