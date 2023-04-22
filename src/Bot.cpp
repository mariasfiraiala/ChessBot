#include "Bot.h"

#include <bits/stdc++.h>

const std::string Bot::BOT_NAME = "MyBot"; /* Edit this, escaped characters are forbidden */

GameConfig::GameConfig(PlaySidePiece currentConfig[TABLE_SIZE + 1][TABLE_SIZE + 1]) {
  for (int i = 1; i <= TABLE_SIZE; i++) {
    for (int j = 1; j <= TABLE_SIZE; j++) {
      this->config[i][j] = currentConfig[i][j]; 
    }
  }
}

Bot::Bot() { /* Initialize custom fields here */
  setGameBoard();
  kingCheck = NO_CHECK;
  previousKingCheck = NO_CHECK;
  shortCastle = CAN_CASTLE;
  longCastle = CAN_CASTLE;
  threeRepeatedConfigs = false;
}

void Bot::setGameBoard() {
  for (int i = 1; i <= TABLE_SIZE; i++) {
    for (int j = 1; j <= TABLE_SIZE; j++) {
      gameBoard[i][j] = NO_PIECE;
    }
  }

  gameBoard[1][1] = WHITE_ROOK;
  gameBoard[1][2] = WHITE_KNIGHT;
  gameBoard[1][3] = WHITE_BISHOP;
  gameBoard[1][4] = WHITE_QUEEN;
  gameBoard[1][5] = WHITE_KING;
  gameBoard[1][6] = WHITE_BISHOP;
  gameBoard[1][7] = WHITE_KNIGHT;
  gameBoard[1][8] = WHITE_ROOK;

  for (int i = 1; i <= TABLE_SIZE; i++) {
    gameBoard[2][i] = WHITE_PAWN;
    gameBoard[7][i] = BLACK_PAWN;
  }

  gameBoard[8][1] = BLACK_ROOK;
  gameBoard[8][2] = BLACK_KNIGHT;
  gameBoard[8][3] = BLACK_BISHOP;
  gameBoard[8][4] = BLACK_QUEEN;
  gameBoard[8][5] = BLACK_KING;
  gameBoard[8][6] = BLACK_BISHOP;
  gameBoard[8][7] = BLACK_KNIGHT;
  gameBoard[8][8] = BLACK_ROOK;
}

void getCoordinates(std::string position, int &x, int &y) {
  y = position[0] - 'a' + 1;
  x = position[1];
}

void Bot::findKing(PlaySidePiece king, int &x, int &y) {
  for (int i = 1; i <= TABLE_SIZE; i++) {
    for (int j = 1; j <= TABLE_SIZE; j++) {
      if (gameBoard[i][j] == king) {
        x = i;
        y = j;
        return;
      }
    }
  }
}

bool Bot::checkPawnOnBoard(int &x, int &y) {
  if (gameBoard[x][y] == BLACK_PAWN || gameBoard[x][y] == WHITE_PAWN ||
      gameBoard[x][y] == BLACK_EN_PAS || gameBoard[x][y] == WHITE_EN_PAS) {
        return true;
  }
  
  return false;
}

bool Bot::checkEnPassant(PlaySide sideToMove, int &x_start, int &y_start, int &x_end, int &y_end) {
  if (sideToMove == BLACK) {
    /* Black en passant from 7th to 5th row */
    if (x_start == 7 && x_end == 5 && gameBoard[x_start][y_start] == BLACK_PAWN) {
      gameBoard[x_end][y_end] = BLACK_EN_PAS;
      return true;
    }
  } else {
    /* White en passant from 2nd to 4th row */
    if (x_start == 2 && x_end == 4 && gameBoard[x_start][y_start] == WHITE_PAWN) {
      gameBoard[x_end][y_end] = WHITE_EN_PAS;
      return true;
    }
  }
  
  return false;
}

bool Bot::checkPawnAttack(PlaySide side, PlaySidePiece pawn, int &x, int &y) {
  if (side == BLACK) {
    /* check for black pawns -- they attack on a lower x value */
    for (int i = 1; i <= TABLE_SIZE; i++) {
      for (int j = 1; j <= TABLE_SIZE; j++) {
        if (gameBoard[i][j] != pawn) {
          continue;
        }

        if (x == (i - 1) && (y == (j - 1) || y == (j + 1))) {
          return true;
        }
      }
    }
  } else {
    /* check for white pawns -- they attack on a higher x value */
    for (int i = 1; i <= TABLE_SIZE; i++) {
      for (int j = 1; j <= TABLE_SIZE; j++) {
        if (gameBoard[i][j] != pawn) {
          continue;
        }

        if (x == (i + 1) && (y == (j - 1) || y == (j + 1))) {
          return true;
        }
      }
    }
  }
  return false;
}

bool Bot::checkRookAttack(PlaySidePiece rook, int &x, int &y) {
  bool pieceBetween;
  
  for (int i = 1; i <= TABLE_SIZE; i++) {
    for (int j = 1; j <= TABLE_SIZE; j++) {
      if (gameBoard[i][j] != rook) {
        continue;
      }

      /* rook and king on the same row */
      if (x == i) {
        int start = std::min(y, j) + 1;
        int end = std::max(y, j);

        pieceBetween = false;

        for (; start < end; start++) {
          if (gameBoard[i][start] != NO_PIECE) {
            pieceBetween = true;
            break;
          }
        }

        if (!pieceBetween) {
          return true;
        }
      }

      /* rook and king on the same column */
      if (y == j) {
        int start = std::min(x, i) + 1;
        int end = std::max(x, i);

        pieceBetween = false;

        for (; start < end; start++) {
          if (gameBoard[start][j] != NO_PIECE) {
            pieceBetween = true;
            break;
          }
        }

        if (!pieceBetween) {
          return true;
        }
      }
    }
  }

  return false;
}

bool Bot::checkKnightAttack(PlaySidePiece knight, int &x, int &y) {
  int dx[KNIGHT_MOVES] = {2, 1, -1, -2, -2, -1, 1, 2};
  int dy[KNIGHT_MOVES] = {1, 2, 2, 1, -1, -2, -2, -1};

  for (int i = 1; i <= TABLE_SIZE; i++) {
    for (int j = 1; j <= TABLE_SIZE; j++) {
      if (gameBoard[i][j] != knight) {
        continue;
      }

      /* Knight attack implies 8 relative positions */
      for (int k = 0; k < KNIGHT_MOVES; k++) {
        if ((i + dx[k]) == x && (j + dy[k]) == y) {
          return true;
        }
      }
    }
  }

  return false;
}

bool Bot::checkBishopAttack(PlaySidePiece bishop, int &x, int &y) {
  for (int i = 1; i <= TABLE_SIZE; i++) {
    for (int j = 1; j <= TABLE_SIZE; j++) {
      if (gameBoard[i][j] != bishop) {
        continue;
      }

      /* check if the bishop attacks the king on 1 of the 4 diagonals */
      for (int k = 1; k < TABLE_SIZE; k++) {
        if ((i + k) <= TABLE_SIZE && (j + k) <= TABLE_SIZE) {
          if ((i + k) == x && (j + k) == y) {
            return true;
          }

          if (gameBoard[i + k][j + k] != NO_PIECE) {
            break;
          }
        } else {
          break;
        }
      }

      for (int k = 1; k < TABLE_SIZE; k++) {
        if ((i - k) > 0 && (j + k) <= TABLE_SIZE) {
          if ((i - k) == x && (j + k) == y) {
            return true;
          }

          if (gameBoard[i - k][j + k] != NO_PIECE) {
            break;
          }
        } else {
          break;
        }
      }

      for (int k = 1; k < TABLE_SIZE; k++) {
        if ((i - k) > 0 && (j - k) > 0) {
          if ((i - k) == x && (j - k) == y) {
            return true;
          }

          if (gameBoard[i - k][j - k] != NO_PIECE) {
            break;
          }
        } else {
          break;
        }
      }

      for (int k = 1; k < TABLE_SIZE; k++) {
        if ((i + k) <= TABLE_SIZE && (j - k) > 0) {
          if ((i + k) == x && (j - k) == y) {
            return true;
          }

          if (gameBoard[i + k][j - k] != NO_PIECE) {
            break;
          }
        } else {
          break;
        }
      }
    }
  }

  return false;
}

bool Bot::checkQueenAttack(PlaySidePiece queen, int &x, int &y) {
  /* Queen is just a rook + bishop */
  return (checkRookAttack(queen, x, y) | checkBishopAttack(queen, x, y));
}

bool Bot::checkKingAttack(PlaySidePiece king, int &x, int &y) {
  for (int i = 1; i <= TABLE_SIZE; i++) {
    for (int j = 1; j <= TABLE_SIZE; j++) {
      if (gameBoard[i][j] != king) {
        continue;
      }

      /* Another king needs to be in one of the 
       * adjacent 9 fields on board
       */
      if (std::abs(x - i) <= 1 && std::abs(y - j) <= 1) {
        return true;
      }
    }
  }

  return false;
}

bool Bot::checkKingSafety(PlaySide sideToMove) {
  int x_king, y_king;
 
  if (sideToMove == BLACK) {
    /* check for white king's safety */
    findKing(WHITE_KING, x_king, y_king);
    return !(checkPawnAttack(sideToMove, BLACK_PAWN, x_king, y_king) |
        checkPawnAttack(sideToMove, BLACK_EN_PAS, x_king, y_king) |
        checkRookAttack(BLACK_ROOK, x_king, y_king) |
        checkKnightAttack(BLACK_KNIGHT, x_king, y_king) |
        checkBishopAttack(BLACK_BISHOP, x_king, y_king) |
        checkQueenAttack(BLACK_QUEEN, x_king, y_king) |
        checkKingAttack(BLACK_KING, x_king, y_king));
    
  } else {
    /* check for black king's safety */
    findKing(BLACK_KING, x_king, y_king);
    return !(checkPawnAttack(sideToMove, WHITE_PAWN, x_king, y_king) |
        checkPawnAttack(sideToMove, WHITE_EN_PAS, x_king, y_king) |
        checkRookAttack(WHITE_ROOK, x_king, y_king) |
        checkKnightAttack(WHITE_KNIGHT, x_king, y_king) |
        checkBishopAttack(WHITE_BISHOP, x_king, y_king) |
        checkQueenAttack(WHITE_QUEEN, x_king, y_king) |
        checkKingAttack(WHITE_KING, x_king, y_king));
  }

  return true;
}

PlaySidePiece Bot::selectPiece(Piece piece, PlaySide side) {
  switch (side) {
    case BLACK:
      switch (piece) {
        case PAWN: return BLACK_PAWN;
        case ROOK: return BLACK_ROOK;
        case KNIGHT: return BLACK_KNIGHT;
        case BISHOP: return BLACK_BISHOP;
        case QUEEN: return BLACK_QUEEN;
        default: return NO_PIECE;
      }
      break;
    
    case WHITE:
      switch (piece) {
        case PAWN: return WHITE_PAWN;
        case ROOK: return WHITE_ROOK;
        case KNIGHT: return WHITE_KNIGHT;
        case BISHOP: return WHITE_BISHOP;
        case QUEEN: return WHITE_QUEEN;
        default: return NO_PIECE;
      }

    default:
      return NO_PIECE;
  }
}

PlaySidePiece Bot::convertRegularPiece(int &x, int &y) {
  /* convert a pawn from en passant marked pawn to regular pawn after one move */
  if (gameBoard[x][y] == BLACK_EN_PAS) {
    return BLACK_PAWN;
  }

  if (gameBoard[x][y] == WHITE_EN_PAS) {
    return WHITE_PAWN;
  }

  /* otherwise just return the current piece */
  return gameBoard[x][y];
}

void Bot::copyCurrentConfig() {
  /* deep copy the current config using an auxiliary matrix */
  GameConfig currentGameBoard = GameConfig(gameBoard);

  pastConfigs.push_back(currentGameBoard);
}

bool Bot::checkRepeatedConfigs() {
  int counterRepeatedConfigs = 0;
  bool repeatedConfig;
  
  for (auto& pastConfig : pastConfigs) {
    repeatedConfig = true;
    for (int i = 1; i <= TABLE_SIZE && repeatedConfig; i++) {
      for (int j = 1; j <= TABLE_SIZE && repeatedConfig; j++) {
        if (pastConfig.config[i][j] != gameBoard[i][j]) {
          repeatedConfig = false;
        }
      }
    }

    if (repeatedConfig == true) {
      counterRepeatedConfigs++;
    }
  }

  if (counterRepeatedConfigs == 2) {
    return true;
  }

  return false;
}

void Bot::recordMove(Move* move, PlaySide sideToMove) {
    /* You might find it useful to also separately
     * record last move in another custom field */
  int x_start, y_start, x_end, y_end;
  getCoordinates(move->getSource().value(), x_start, y_start);
  getCoordinates(move->getDestination().value(), x_end, y_end);

  counterMoves++;

  if (checkPawnOnBoard(x_start, y_start)) {
    counterMoves = 0;
  }

  if (gameBoard[x_end][y_end] != NO_PIECE) {
    enemyCapturesPieces.push_back(convertRegularPiece(x_end, y_end));
    counterMoves = 0;
  }

  if (move->isNormal()) {
    if (!checkEnPassant(sideToMove, x_start, y_start, x_end, y_end)) {
      gameBoard[x_end][y_end] = convertRegularPiece(x_start, y_start);
    }

    gameBoard[x_start][y_start] = NO_PIECE;
  }

  if (move->isPromotion()) {
    gameBoard[x_end][y_end] = selectPiece(move->getReplacement().value(), sideToMove);
    gameBoard[x_start][y_start] = NO_PIECE;
  }

  if (move->isDropIn()) {
    gameBoard[x_end][y_end] = selectPiece(move->getReplacement().value(), sideToMove);
  }

  if (!checkKingSafety(sideToMove)) {
    previousKingCheck = IN_CHECK;
    kingCheck = IN_CHECK;
    shortCastle = NO_CASTLE;
    longCastle = NO_CASTLE;
  }

  if (checkRepeatedConfigs()) {
    threeRepeatedConfigs = true;
  }

  copyCurrentConfig();
}

std::string Bot::getPosition(int x, int y) {
  std::string position = "";
  position += 'a' + y - 1;
  position += '0' + x;

  return position;
}

std::vector<Move*> Bot::moveWhitePawn(int x, int y, std::vector<PlaySidePiece> &captured) {
  if (gameBoard[x][y] != WHITE_PAWN || gameBoard[x][y] != WHITE_EN_PAS) {
    return std::vector<Move*>();
  }

  std::vector<Move*> possibleMoves;

  /* become en passant */
  if (x == 2 && gameBoard[x + 2][y] == NO_PIECE) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 2, y)));
    convertRegularPiece(x, y);
  }

  if (x == 7 && gameBoard[x + 1][y] == NO_PIECE) {
    possibleMoves.push_back(Move::promote(getPosition(x, y), getPosition(x + 1, y), QUEEN));
  }

  if (gameBoard[x + 1][y] == NO_PIECE) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y)));
  }

  if (gameBoard[x + 1][y + 1] < NO_PIECE) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y + 1)));
    captured.push_back(gameBoard[x + 1][y + 1]);
  }

  if (gameBoard[x + 1][y - 1] < NO_PIECE) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y - 1)));
    captured.push_back(gameBoard[x + 1][y - 1]);
  }

  if (gameBoard[x][y - 1] == BLACK_EN_PAS) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y - 1)));
    captured.push_back(gameBoard[x][y - 1]);
    gameBoard[x][y - 1] = NO_PIECE;
  }

  if (gameBoard[x][y + 1] == BLACK_EN_PAS) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y + 1)));
    captured.push_back(gameBoard[x][y + 1]);
    gameBoard[x][y + 1] = NO_PIECE;
  }

  return possibleMoves;
}

std::vector<Move*> Bot::moveBlackPawn(int x, int y, std::vector<PlaySidePiece> &captured) {
  if (gameBoard[x][y] != BLACK_PAWN || gameBoard[x][y] != BLACK_EN_PAS) {
    return std::vector<Move*>();
  }

  std::vector<Move*> possibleMoves;

  /* become en passant */
  if (x == 7 && gameBoard[x - 2][y] == NO_PIECE) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 2, y)));
    convertRegularPiece(x, y);
  }

  if (x == 2 && gameBoard[x - 1][y] == NO_PIECE) {
    possibleMoves.push_back(Move::promote(getPosition(x, y), getPosition(x - 1, y), QUEEN));
  }

  if (gameBoard[x - 1][y] == NO_PIECE) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y)));
  }

  if (gameBoard[x - 1][y + 1] > NO_PIECE) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y + 1)));
    captured.push_back(gameBoard[x - 1][y + 1]);
  }

  if (gameBoard[x - 1][y - 1] > NO_PIECE) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y - 1)));
    captured.push_back(gameBoard[x - 1][y - 1]);
  }

  if (gameBoard[x][y - 1] == WHITE_EN_PAS) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y - 1)));
    captured.push_back(gameBoard[x][y - 1]);
    gameBoard[x][y - 1] = NO_PIECE;
  }

  if (gameBoard[x][y + 1] == WHITE_EN_PAS) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y + 1)));
    captured.push_back(gameBoard[x][y + 1]);
    gameBoard[x][y + 1] = NO_PIECE;
  }

  return possibleMoves;
}

std::vector<Move*> Bot::moveKnight(int x, int y, PlaySide side, std::vector<PlaySidePiece> &captured) {
  if ((side == WHITE && gameBoard[x][y] != WHITE_KNIGHT) || (side == BLACK && gameBoard[x][y] != BLACK_KNIGHT)) {
    return std::vector<Move*>();
  }

  std::vector<Move*> possibleMoves;

  if (x + 2 <= 8) {
    if (y + 1 <= 8 && gameBoard[x + 2][y + 1] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 2, y + 1)));

      if (gameBoard[x + 2][y + 1] < NO_PIECE) {
        captured.push_back(gameBoard[x + 2][y + 1]);
      }
    }

    if (y - 1 >= 1 && gameBoard[x + 2][y - 1] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 2, y - 1)));

      if (gameBoard[x + 2][y - 1] < NO_PIECE) {
        captured.push_back(gameBoard[x + 2][y - 1]);
      }
    }
  }

  if (x - 2 >= 1) {
    if (y + 1 <= 8 && gameBoard[x - 2][y + 1] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 2, y + 1)));

      if (gameBoard[x - 2][y + 1] < NO_PIECE) {
        captured.push_back(gameBoard[x - 2][y + 1]);
      }
    }

    if (y - 1 >= 1 && gameBoard[x - 2][y - 1] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 2, y - 1)));

      if (gameBoard[x - 2][y - 1] < NO_PIECE) {
        captured.push_back(gameBoard[x - 2][y - 1]);
      }
    }
  }

  return possibleMoves;
}

std::vector<Move*> Bot::moveKing(int x, int y, PlaySide side, std::vector<PlaySidePiece> &captured) {
  if ((side == WHITE && gameBoard[x][y] != WHITE_KING) || (side == BLACK && gameBoard[x][y] != BLACK_KING)) {
    return std::vector<Move*>();
  }

  std::vector<Move*> possibleMoves;

  if (side == WHITE) {
    if (x + 1 <= 8 && gameBoard[x + 1][y] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y)));

      if (gameBoard[x + 1][y] < NO_PIECE) {
        captured.push_back(gameBoard[x + 1][y]);
      }
    }

    if (x - 1 >= 1 && gameBoard[x - 1][y] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y)));

      if (gameBoard[x - 1][y] < NO_PIECE) {
        captured.push_back(gameBoard[x - 1][y]);
      }
    }

    if (y + 1 <= 8 && gameBoard[x][y + 1] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x, y + 1)));

      if (gameBoard[x][y + 1] < NO_PIECE) {
        captured.push_back(gameBoard[x][y + 1]);
      }
    }

    if (y - 1 >= 1 && gameBoard[x][y - 1] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x, y - 1)));

      if (gameBoard[x][y - 1] < NO_PIECE) {
        captured.push_back(gameBoard[x][y - 1]);
      }
    }

    if (x + 1 <= 8 && y + 1 <= 8 && gameBoard[x + 1][y + 1] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y + 1)));

      if (gameBoard[x + 1][y + 1] < NO_PIECE) {
        captured.push_back(gameBoard[x + 1][y + 1]);
      }
    }

    if (x + 1 <= 8 && y - 1 >= 1 && gameBoard[x + 1][y - 1] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y - 1)));

      if (gameBoard[x + 1][y - 1] < NO_PIECE) {
        captured.push_back(gameBoard[x + 1][y - 1]);
      }
    }

    if (x - 1 >= 1 && y + 1 <= 8 && gameBoard[x - 1][y + 1] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y + 1)));

      if (gameBoard[x - 1][y + 1] < NO_PIECE) {
        captured.push_back(gameBoard[x - 1][y + 1]);
      }
    }

    if (x - 1 >= 1 && y - 1 >= 1 && gameBoard[x - 1][y - 1] <= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y - 1)));

      if (gameBoard[x - 1][y - 1] < NO_PIECE) {
        captured.push_back(gameBoard[x - 1][y - 1]);
      }
    }
  }

  if (side == BLACK) {
    if (x + 1 <= 8 && gameBoard[x + 1][y] >= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y)));

      if (gameBoard[x + 1][y] > NO_PIECE) {
        captured.push_back(gameBoard[x + 1][y]);
      }
    }

    if (x - 1 >= 1 && gameBoard[x - 1][y] >= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y)));

      if (gameBoard[x - 1][y] > NO_PIECE) {
        captured.push_back(gameBoard[x - 1][y]);
      }
    }

    if (y + 1 <= 8 && gameBoard[x][y + 1] >= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x, y + 1)));

      if (gameBoard[x][y + 1] > NO_PIECE) {
        captured.push_back(gameBoard[x][y + 1]);
      }
    }

    if (y - 1 >= 1 && gameBoard[x][y - 1] >= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x, y - 1)));

      if (gameBoard[x][y - 1] > NO_PIECE) {
        captured.push_back(gameBoard[x][y - 1]);
      }
    }

    if (x + 1 <= 8 && y + 1 <= 8 && gameBoard[x + 1][y + 1] >= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y + 1)));

      if (gameBoard[x + 1][y + 1] > NO_PIECE) {
        captured.push_back(gameBoard[x + 1][y + 1]);
      }
    }

    if (x + 1 <= 8 && y - 1 >= 1 && gameBoard[x + 1][y - 1] >= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x + 1, y - 1)));

      if (gameBoard[x + 1][y - 1] > NO_PIECE) {
        captured.push_back(gameBoard[x + 1][y - 1]);
      }
    }

    if (x - 1 >= 1 && y + 1 <= 8 && gameBoard[x - 1][y + 1] >= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y + 1)));

      if (gameBoard[x - 1][y + 1] > NO_PIECE) {
        captured.push_back(gameBoard[x - 1][y + 1]);
      }
    }

    if (x - 1 >= 1 && y - 1 >= 1 && gameBoard[x - 1][y - 1] >= NO_PIECE) {
      possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x - 1, y - 1)));

      if (gameBoard[x - 1][y - 1] > NO_PIECE) {
        captured.push_back(gameBoard[x - 1][y - 1]);
      }
    }
  }

  if (shortCastle == CAN_CASTLE) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x, y + 2)));
  }

  if (longCastle == CAN_CASTLE) {
    possibleMoves.push_back(Move::moveTo(getPosition(x, y), getPosition(x, y - 2)));
  }

  return possibleMoves;
}

std::vector<Move*> Bot::rook_moves(int x, int y, PlaySide side)
{
  std::vector<Move*> moves;
  bool turn = false;
  auto valid_move = turn ? [](char c){return c < 0;} : [](char c){ return c > 0;};
  for (int i = x + 1; i < TABLE_SIZE; ++i) { //right
    if (gameBoard[i][y] != NO_PIECE && valid_move(gameBoard[i][y])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, y)));
      break;
    } else if (valid_move(gameBoard[i][y])){
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, y)));
    }
  }

  for (int i = x - 1; i >=0; i--) { //left
    if (gameBoard[i][y] != NO_PIECE && valid_move(gameBoard[i][y])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, y)));
    } else if (valid_move(gameBoard[i][y])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, y)));
    }
  }

  for (int i = y - 1; i >=0 ; i--) { //down
    if (gameBoard[x][i] != NO_PIECE && valid_move(gameBoard[x][i])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(x, i)));
      break;
    } else if (valid_move(gameBoard[x][i])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(x, i)));
    }
  }

  for (int i = y + 1; y < TABLE_SIZE; i++) { //up
    if (gameBoard[x][i] != NO_PIECE && valid_move(gameBoard[x][i])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(x, i)));
      break;
    } else if (valid_move(gameBoard[x][i])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(x, i)));
    }
  }
  return moves;
}

std::vector<Move*> Bot::bishop_moves(int x, int y, PlaySide side)
{
  std::vector<Move*> moves;
  bool turn = false;

  auto valid_move = turn ? [](char c){return c < 0;} : [](char c){ return c > 0;};
  for (int i = x + 1, j = y + 1; i < TABLE_SIZE && j < TABLE_SIZE; i++, j++) {//up right
    if (gameBoard[i][j] != NO_PIECE && valid_move(gameBoard[i][j])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, j)));
      break;
    } else if (valid_move(gameBoard[i][j])){
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, j)));
    }
  }

  for (int i = x - 1, j = y + 1; i >= 0 && j < TABLE_SIZE; i--, j++) {//up left
    if (gameBoard[i][j] != NO_PIECE && valid_move(gameBoard[i][j])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, j)));
      break;
    } else if (valid_move(gameBoard[i][j])){
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, j)));
    }
  }

  for (int i = x + 1, j = y - 1; i < TABLE_SIZE && j >= 0; i++, j--) {//down right
    if (gameBoard[i][j] != NO_PIECE && valid_move(gameBoard[i][j])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, j)));
      break;
    } else if (valid_move(gameBoard[i][j])){
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, j)));
    }
  }

  for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {//down left
    if (gameBoard[i][j] != NO_PIECE && valid_move(gameBoard[i][j])) {
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, j)));
      break;
    } else if (valid_move(gameBoard[i][j])){
      moves.push_back(Move::moveTo(getPosition(x, y), getPosition(i, j)));
    }
  }
  return moves;
}

std::vector<Move*> Bot::queen_moves(int x, int y, PlaySide side)
{
  //std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> moves;
  auto all_moves = rook_moves(x, y, side);
  auto moves_bishop = bishop_moves(x, y, side);
  all_moves.insert(all_moves.end(), moves_bishop.begin(), moves_bishop.end());
  return all_moves;
}

Move* Bot::calculateNextMove() {
  /* Play move for the side the engine is playing (Hint: Main.getEngineSide())
   * Make sure to record your move in custom structures before returning.
   *
   * Return move that you are willing to submit
   * Move is to be constructed via one of the factory methods declared in Move.h */
  return Move::resign();
}

std::string Bot::getBotName() { return Bot::BOT_NAME; }