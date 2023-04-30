# Proiect PA - Crazy House Chess Engine

## Compile Instructions

```console
maria@spectrum:~/proiect-pa-2023-exploding-pengwins.git$ cd src/
```

### `xboard` Integration

```console
maria@spectrum:~/proiect-pa-2023-exploding-pengwins.git/src$ xboard -fcp "make run" -debug
```

### Build

```console
maria@spectrum:~/proiect-pa-2023-exploding-pengwins.git/src$ make build
```

### Run

```console
maria@spectrum:~/proiect-pa-2023-exploding-pengwins.git/src$ make run
```

### Clean

```console
maria@spectrum:~/proiect-pa-2023-exploding-pengwins.git/src$ make clean
```

## Structure

```console
.
├── CONTRIBUTING.md
├── cs
│   └── check_cpp.py
├── README.md
└── src
    ├── Bot.cpp
    ├── Bot.h
    ├── Main.cpp
    ├── Makefile
    ├── Move.cpp
    ├── Move.h
    ├── MoveNode.cpp
    ├── MoveNode.h
    ├── Piece.h
    ├── PlaySide.h
```

* `Bot.cpp`, `Bot.h`:

   &rarr; the code necessary for configuring the bot, such as the `checkRepeatedConfigs()` method which gets whether the current configuration repetead itself 3 times.

* `Main.cpp`:

   &rarr; the code for the xboard communication

* `Move.cpp`, `Move.h`:

   &rarr; the skel which was used for actions such as dropping pieces in (Crazy House Chess) and promotions.

* `MoveNode.cpp`, `MoveNode.h`:

   &rarr; special class, a wrapper over `Move` with auxiliary functionalities

   &rarr; the code that generates all possible moves for all pieces in such a manner that the king doesn't get instantly attacked (eg `knightMoves()`, `bishopMoves()`)

   &rarr; the methods used for checking whether a piece attacks the king, or is in a special case (eg `checkRookAttack()`, `checkSpecialCases()`)

   &rarr; the methods that deal with Crazy House rules, such as `switchSide()`, which gets all captured pieces, changes their color and transforms the ones that were promoted, back into pawns

## Algorithmic Approach

The class that is of extreme interest is `MoveNode`.
It contains the full logic behind the bot and the table, consisting of 4 major groups of methods:

1. king safety checkers

   1. `findKing()`, `checkKingSafety()`

   1. `checkPawnAttack()`, `checkRookAttack()`, `checkKnightAttack()`, `checkBishopAttack()`, `checkQueenAttack()`, `checkKingAttack()`.
   These methods are all used in the wraper function, `checkKingSafety()`.

1. special cases

   1. `checkSpecialCases()`, `revertEnPassantBoard()`.
   The first method checks whether the enemy captured an en-passant pawn, made an en-passant move or performed a castling.
   The second method transforms all en-passant marked pawns back to normal.

1. move generators

   1. `whitePawnMoves()`, `blackPawnMoves()`, `rookMoves()`, `knightMoves()`, `bishopMoves()`, `queenMoves()`, `kingMoves()`.
   These methods are all used to append to the `possibleMoves` array (MoveNode.h:55) all the legal moves that can be achieved for the current board and the current pieces.
   This means, deciding the captured pieces when needed (used for the Crazy Chess feature), moving where there is no piece of our own or where there is a piece we capture, updating the board and checking special cases such as castle for rook or en-passant for pawn. 

   1. `crazyHouseMoves()` used for dropping captured pieces on the board, except when the piece is a pawn, which cannot be placed on the first or last row.

1. helpers

   1. `checkPawnOnBoard()`, `removeCapturedPiece()`, `switchSide`, `setInitialBoard`,`updateBoard()`

   1. `calculateAllNextMoves()` applies all the move generators and stores the final vector of MoveNodes.
   This vector will be used in `calculateNextMove()` (Bot.cpp:65), a method that randomly chooses a move from the array, sends it to the xboard engine and changes the current context based off of it.
   
   >**Note**: All moves from the `possibleMoves` array are guaranteed to keep the king safe, by additional checks introduced in the move generators methods.

## Bibliography

[Chess Programming](https://www.chessprogramming.org/Main_Page#Basics)

## Members and Their Work

Alexandru Mihai, _Zeus, the mastermind behind the project_

   &rarr; debugging
   
   &rarr; refactoring
   
   &rarr; en-passant
   
   &rarr; castling
   
   &rarr; corner chess cases

Delia Constantinescu, _protector from the wrath of the mastermind_

   &rarr; debugging
   
   &rarr; record all possible moves for pawns, knight, king

Ecaterina Mincă, _the code breaker, tested the bot like hellllll_

   &rarr; debugging
   
   &rarr; record all possible moves for pawns, knight, king

Maria Sfîrăială, _the git girl that took a vacation_

   &rarr; README
   
   &rarr; coding style
   
   &rarr; place Crazy House pieces
   
   &rarr; generate next move (integrate Cati, Delia, Maria work)   
