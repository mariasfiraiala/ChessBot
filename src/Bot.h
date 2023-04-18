#ifndef BOT_H
#define BOT_H
#include <bits/stdc++.h>

#include "Move.h"
#include "PlaySide.h"

#define TABLE_SIZE 8
#define KNIGHT_MOVES 8

enum PlaySidePiece {
  WHITE_PAWN = 1, WHITE_ROOK = 2,
  WHITE_BISHOP = 3, WHITE_KNIGHT = 4,
  WHITE_QUEEN = 5, WHITE_KING = 6,
  WHITE_EN_PAS = 7,
  BLACK_PAWN = -1, BLACK_ROOK = -2,
  BLACK_BISHOP = -3, BLACK_KNIGHT = -4,
  BLACK_QUEEN = -5, BLACK_KING = -6,
  BLACK_EN_PAS = 7,
  NO_PIECE = 0
};

enum Check {
  NO_CHECK = 0,
  IN_CHECK = 1
};

enum Castling {
  NO_CASTLE = 0,
  CAN_CASTLE = 1
};

class GameConfig {
  public:
  
  PlaySidePiece config[TABLE_SIZE + 1][TABLE_SIZE + 1];

  GameConfig(PlaySidePiece currentConfig[TABLE_SIZE + 1][TABLE_SIZE + 1]);
};

class Bot {
 private:
  static const std::string BOT_NAME;

 public:
  /* Declare custom fields below */

  /**
   * Game board with index starting from 1 for easier movement
   * management (rows - 1 to 8, columns - A to H)
   * White King - E1
   * Black King - E8
   */
  PlaySidePiece gameBoard[TABLE_SIZE + 1][TABLE_SIZE + 1];
  
  int counterMoves; /* Moves without capture or pawn movement */

  Check kingCheck; /* King is in check */
  Check previousKingCheck; /* King was once in check */
  Castling shortCastle; /* Short castling available */
  Castling longCastle; /* Long castling available */
  bool threeRepeatedConfigs; /* Checking if three configs repeated */

  std::vector<PlaySidePiece> myCapturedPieces;
  std::vector<PlaySidePiece> enemyCapturesPieces;

  /* Past table configs */
  std::vector<GameConfig> pastConfigs;

  /* Declare custom fields above */
  Bot();

  /**
   * Record move (either by enemy in normal mode, or by either side
   * in force mode) in custom structures
   * @param move received move
   * @param sideToMode side to move
   */
  void recordMove(Move* move, PlaySide sideToMove);

  /**
   * Calculates next move, in response to enemyMove
   * @param enemyMove the enemy's last move
   *                  null if this is the opening move, or previous
   *                  move has been recorded in force mode
   * @return your move
   */
  Move* calculateNextMove();

  /**
   * Sets up the initial board with the all pieces in the initial
   * positions.
   */
  void setGameBoard();

  /* find the coordinates of a king on board */
  void findKing(PlaySidePiece king, int &x, int &y);

  /* check if there is a pawn at the specified coordinates */
  bool checkPawnOnBoard(int &x, int &y);

  /* check if a pawn is in en passant after move */
  bool checkEnPassant(PlaySide sideToMove, int &x_start, int &y_start, int &x_end, int &y_end);

  /* check if a pawn attacks the king */
  bool checkPawnAttack(PlaySide side, PlaySidePiece pawn, int &x, int &y);

  /* check if a rook attacks a king */
  bool checkRookAttack(PlaySidePiece rook, int &x, int &y);

  /* check if a knight attacks a king */
  bool checkKnightAttack(PlaySidePiece knight, int &x, int &y);

  /* check if a bishop attacks a king */
  bool checkBishopAttack(PlaySidePiece bishop, int &x, int &y);

  /* check if a queen attacks a king */
  bool checkQueenAttack(PlaySidePiece queen, int &x, int &y);

  /* check if a king attack a king */
  bool checkKingAttack(PlaySidePiece king, int &x, int &y);

  /* check if the king is in check currently */
  bool checkKingSafety(PlaySide sideToMove);

  /* select specific player side piece from a generic piece */
  PlaySidePiece selectPiece(Piece piece, PlaySide side);
  
  /* convert to regular piece an en passant pawn at the specified coords */
  PlaySidePiece convertRegularPiece(int &x, int &y);

    /* copy the current config in the pastConfigs vector */
  void copyCurrentConfig();

  /* check for repeated configs in the pastConfigs vector */
  bool checkRepeatedConfigs();

  static std::string getBotName();
};
#endif
