# PA Project - Crazy House Chess Engine

# 1st Stage

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
    └── PlaySide.h
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

# 2nd Stage

## Compile Instructions

```console
maria@spectrum:~/proiect-pa-2023-exploding-pengwins.git$ cd src/
```

### `xboard` Integration

#### `syockfish` - easy

```console
maria@spectrum:~/proiect-pa-2023-exploding-pengwins.git$ xboard -variant crazyhouse -fcp "./stockfish" -firstOptions "Skill Level=-20,Slow Mover=10,Use NNUE=false" -fn "Stockfish Easy" -scp "make run" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseSecond false 
```

#### `syockfish` - medium

```console
maria@spectrum:~/proiect-pa-2023-exploding-pengwins.git$ xboard -variant crazyhouse -fcp "./stockfish" -firstOptions "Skill Level=-10,Slow Mover=10,Use NNUE=false" -fn "Stockfish Medium" -scp "make run" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseSecond false 
```

#### `syockfish` - hard

```console
maria@spectrum:~/proiect-pa-2023-exploding-pengwins.git$ xboard -variant crazyhouse -fcp "./stockfish" -firstOptions "Skill Level=-5,Slow Mover=10,Use NNUE=false" -fn "Stockfish Hard" -scp "make run" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseSecond false 
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
    ├── BoardHelpers.cpp
    ├── BoardHelpers.h
    ├── Bot.cpp
    ├── Bot.h
    ├── Evaluate.cpp
    ├── Evaluate.h
    ├── Main.cpp
    ├── Makefile
    ├── Move.cpp
    ├── Move.h
    ├── MoveNode.cpp
    ├── MoveNode.h
    ├── Piece.h
    └── PlaySide.h
```

* `BoardHelpers.cpp`, `BoardHelpers.h`:

   &rarr; Moved the `BoardConfig()` class and the `PlaySidePiece()` enum from the previous stage here, for better control.

* `Bot.cpp`, `Bot.h`:

   &rarr; Alpha-beta pruning algorithms: one for early game, `alphaBetaEarly()` and one for late game, `alphaBetaLate()`.
   Based on which situation we find our bot to be in, we call the `evaluateEarly()` or `evaluateLate()` methods, to get the best results.

*  `Evaluate.cpp`, `Evaluate.h`:

   &rarr; The meat of the project: the 2 evaluate methods, plus some important helpers for them, such as `pawnControl()`, `bishopControl()` and `rookControl()`, used for scanarios such as controling the center of the table, for the pawns, or optimal positions for the other pieces.

## Algorithmic Approach

The main algorithms used for this stage are placed in the `Evaluate.cpp` source and they are the most important aspect when thinking of an efficient Chess player:

1. `checkFileIsolated()`:

1. `evaluateBasic()`, `evaluateEarly()`, `evaluateLate()`:

1. `pawnControl()`:

1. `bishopControl()`:

1. `rookControl()`:

1. `knightsEarly()`:

1. `checkKingAttacked()`:

1. `checkPawnShield()`:

## Bibliography

[MiniMax Lab](https://ocw.cs.pub.ro/courses/pa/laboratoare/laborator-05)

## Members and Their Work

Alexandru Mihai, _the "let Ofast do its magic" believer_

&rarr; evaluate functions (early and late game)

&rarr; Alpha-beta pruning algorithm

&rarr; euristics

Delia Constantinescu, _the "but depth=5 doesn't get blocked for me" person_

&rarr; evaluate functions (early and late game)

&rarr; Alpha-beta pruning algorithm

&rarr; euristics

Ecaterina Mincă, _the competition spy_

&rarr; Alpha-beta pruning algorithm

Maria Sfîrăială _the "let our bot be named Bot-ez" author_

&rarr; Alpha-beta pruning algorithm

&rarr; README

&rarr; testing with `stockfish` (levels -20 - 0)
