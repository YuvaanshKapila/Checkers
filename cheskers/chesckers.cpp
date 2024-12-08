#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

const int BOARD_SIZE = 8;
const int TILE_SIZE = 75;
const int WINDOW_SIZE = BOARD_SIZE * TILE_SIZE;

enum Piece { EMPTY, PLAYER1, PLAYER2, KING1, KING2 };

class CheckersGame {
private:
    sf::RenderWindow window;
    sf::RectangleShape tile;
    std::vector<std::vector<Piece>> board;
    sf::CircleShape player1Piece, player2Piece, kingPiece;
    Piece currentPlayer;
    bool playerVsAI;
    int maxDepth = 5; // The depth the AI looks ahead for moves

    struct Move {
        int startX, startY, endX, endY;
        bool isCapture = false;
    };

public:
    CheckersGame(bool vsAI = false) : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Checkers Game"), playerVsAI(vsAI) {
        board.resize(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE, EMPTY));

        // Initialize the pieces on the board
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                if ((row + col) % 2 == 1) {  // Only place pieces on dark squares
                    if (row < 3) board[row][col] = PLAYER1;  // Player 1's pieces
                    else if (row > 4) board[row][col] = PLAYER2;  // Player 2's pieces
                }
            }
        }

        tile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));

        player1Piece.setRadius(TILE_SIZE / 3);
        player1Piece.setFillColor(sf::Color::Red);

        player2Piece.setRadius(TILE_SIZE / 3);
        player2Piece.setFillColor(sf::Color::Black);

        kingPiece.setRadius(TILE_SIZE / 3);
        kingPiece.setFillColor(sf::Color::Yellow);

        currentPlayer = PLAYER1;
    }

    void drawBoard() {
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                if ((row + col) % 2 == 0) {
                    tile.setFillColor(sf::Color(255, 206, 158));  // Light squares
                }
                else {
                    tile.setFillColor(sf::Color(124, 77, 33));  // Dark squares
                }
                tile.setPosition(col * TILE_SIZE, row * TILE_SIZE);
                window.draw(tile);

                // Draw pieces (Player1, Player2, King1, King2)
                if (board[row][col] == PLAYER1 || board[row][col] == KING1) {
                    sf::CircleShape piece = (board[row][col] == PLAYER1) ? player1Piece : kingPiece;
                    piece.setPosition(col * TILE_SIZE + TILE_SIZE / 6, row * TILE_SIZE + TILE_SIZE / 6);
                    window.draw(piece);
                }
                else if (board[row][col] == PLAYER2 || board[row][col] == KING2) {
                    sf::CircleShape piece = (board[row][col] == PLAYER2) ? player2Piece : kingPiece;
                    piece.setPosition(col * TILE_SIZE + TILE_SIZE / 6, row * TILE_SIZE + TILE_SIZE / 6);
                    window.draw(piece);
                }
            }
        }
    }

    void handleInput() {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int x = mousePos.x / TILE_SIZE;
        int y = mousePos.y / TILE_SIZE;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (board[y][x] == currentPlayer) {
                // Logic to select piece and move
                std::cout << "Piece selected at (" << x << ", " << y << ")\n";
                // TODO: Implement piece moving logic and update board state
            }
        }
    }

    bool isValidMove(Move move) {
        if (std::abs(move.startX - move.endX) != std::abs(move.startY - move.endY)) {
            return false;
        }

        if (board[move.endY][move.endX] != EMPTY) {
            return false;
        }

        if (std::abs(move.startX - move.endX) == 2) {  // Capture condition
            int midX = (move.startX + move.endX) / 2;
            int midY = (move.startY + move.endY) / 2;
            if (board[midY][midX] != (currentPlayer == PLAYER1 ? PLAYER2 : PLAYER1)) {
                return false;
            }
        }

        return true;
    }

    void makeMove(Move move) {
        board[move.endY][move.endX] = currentPlayer;
        board[move.startY][move.startX] = EMPTY;

        // Capture opponent piece
        if (move.isCapture) {
            int midX = (move.startX + move.endX) / 2;
            int midY = (move.startY + move.endY) / 2;
            board[midY][midX] = EMPTY;
        }

        // Kinging the piece
        if ((currentPlayer == PLAYER1 && move.endY == 0) || (currentPlayer == PLAYER2 && move.endY == BOARD_SIZE - 1)) {
            board[move.endY][move.endX] = (currentPlayer == PLAYER1) ? KING1 : KING2;
        }

        // Switch player
        currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
    }

    int evaluateBoard() {
        int score = 0;
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                if (board[row][col] == PLAYER1) score += 1;
                else if (board[row][col] == KING1) score += 3;
                else if (board[row][col] == PLAYER2) score -= 1;
                else if (board[row][col] == KING2) score -= 3;
            }
        }
        return score;
    }

    int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta) {
        if (depth == 0) return evaluateBoard();

        std::vector<Move> validMoves;
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                if (board[row][col] == (isMaximizingPlayer ? PLAYER2 : PLAYER1)) {
                    // Add logic for valid moves for both players
                    // You can implement more advanced capturing here
                }
            }
        }

        int bestScore = isMaximizingPlayer ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

        for (auto& move : validMoves) {
            makeMove(move);
            int score = minimax(depth - 1, !isMaximizingPlayer, alpha, beta);
            bestScore = isMaximizingPlayer ? std::max(bestScore, score) : std::min(bestScore, score);

            // Alpha-Beta Pruning
            if (isMaximizingPlayer) {
                alpha = std::max(alpha, score);
            }
            else {
                beta = std::min(beta, score);
            }

            if (beta <= alpha) break;
        }

        return bestScore;
    }

    void aiMove() {
        // Implement the AI move here, use minimax algorithm to choose the best move
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            handleInput();

            if (playerVsAI && currentPlayer == PLAYER2) {
                aiMove();
            }

            window.clear();
            drawBoard();
            window.display();
        }
    }
};

int main() {
    srand(time(0));

    bool vsAI;
    std::cout << "Select Game Mode (1 for Player vs Player, 2 for Player vs AI): ";
    std::cin >> vsAI;

    CheckersGame game(vsAI == 2);
    game.run();

    return 0;
}
