#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

const int TILE_SIZE = 75;
const int BOARD_SIZE = 8;
const int WINDOW_SIZE = TILE_SIZE * BOARD_SIZE;

enum PieceType { EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum Player { NONE, PLAYER1, PLAYER2 };

class Piece {
public:
    PieceType type;
    Player player;
    bool hasMoved;

    Piece(PieceType t = EMPTY, Player p = NONE) : type(t), player(p), hasMoved(false) {}

    bool isValidMove(int startX, int startY, int endX, int endY, const std::vector<std::vector<Piece>>& board) {
        if (type == EMPTY) return false;
        // Basic movement logic for each piece (can be expanded further for each piece)
        if (type == PAWN) {
            // Pawn movement (2 squares at the start, 1 square after)
            return true;
        }
        if (type == KNIGHT) {
            // Knight moves in an L shape
            return true;
        }
        if (type == BISHOP) {
            // Bishop moves diagonally
            return true;
        }
        if (type == ROOK) {
            // Rook moves in straight lines
            return true;
        }
        if (type == QUEEN) {
            // Queen moves both diagonally and in straight lines
            return true;
        }
        if (type == KING) {
            // King moves 1 square in any direction
            return true;
        }
        return false;
    }
};

class ChessGame {
private:
    sf::RenderWindow window;
    std::vector<std::vector<Piece>> board;
    sf::RectangleShape tile;
    sf::CircleShape pieceShape;
    Piece selectedPiece;
    sf::Vector2i selectedTile;
    Player currentPlayer;
    bool playerVsAI;
    int maxDepth;

public:
    ChessGame(bool vsAI = false) : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Chess Game"), playerVsAI(vsAI), currentPlayer(PLAYER1), maxDepth(3) {
        board.resize(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE));
        setupBoard();
        tile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        pieceShape.setRadius(TILE_SIZE / 3);
        pieceShape.setFillColor(sf::Color::Red); // Temporary color for pieces
    }

    void setupBoard() {
        // Setup the pieces for both players (simplified for now)
        for (int i = 0; i < BOARD_SIZE; ++i) {
            board[1][i] = Piece(PAWN, PLAYER1);
            board[6][i] = Piece(PAWN, PLAYER2);
        }
        board[0][0] = Piece(ROOK, PLAYER1); board[0][7] = Piece(ROOK, PLAYER1);
        board[0][1] = Piece(KNIGHT, PLAYER1); board[0][6] = Piece(KNIGHT, PLAYER1);
        board[0][2] = Piece(BISHOP, PLAYER1); board[0][5] = Piece(BISHOP, PLAYER1);
        board[0][3] = Piece(QUEEN, PLAYER1); board[0][4] = Piece(KING, PLAYER1);

        board[7][0] = Piece(ROOK, PLAYER2); board[7][7] = Piece(ROOK, PLAYER2);
        board[7][1] = Piece(KNIGHT, PLAYER2); board[7][6] = Piece(KNIGHT, PLAYER2);
        board[7][2] = Piece(BISHOP, PLAYER2); board[7][5] = Piece(BISHOP, PLAYER2);
        board[7][3] = Piece(QUEEN, PLAYER2); board[7][4] = Piece(KING, PLAYER2);
    }

    void drawBoard() {
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                tile.setFillColor(((row + col) % 2 == 0) ? sf::Color(255, 206, 158) : sf::Color(124, 77, 33));
                tile.setPosition(col * TILE_SIZE, row * TILE_SIZE);
                window.draw(tile);

                // Draw the pieces
                if (board[row][col].type != EMPTY) {
                    pieceShape.setPosition(col * TILE_SIZE + TILE_SIZE / 6, row * TILE_SIZE + TILE_SIZE / 6);
                    pieceShape.setFillColor(board[row][col].player == PLAYER1 ? sf::Color::White : sf::Color::Black);
                    window.draw(pieceShape);
                }
            }
        }
    }

    void handleInput() {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int x = mousePos.x / TILE_SIZE;
        int y = mousePos.y / TILE_SIZE;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (selectedTile.x == -1 && selectedTile.y == -1) {
                if (board[y][x].player == currentPlayer) {
                    selectedTile = { y, x };
                }
            }
            else {
                if (board[y][x].type == EMPTY || board[y][x].player != currentPlayer) {
                    if (board[selectedTile.y][selectedTile.x].isValidMove(selectedTile.x, selectedTile.y, x, y, board)) {
                        board[y][x] = board[selectedTile.y][selectedTile.x];
                        board[selectedTile.y][selectedTile.x] = Piece(EMPTY, NONE);
                        selectedTile = { -1, -1 };
                        currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
                    }
                }
            }
        }
    }

    void aiMove() {
        std::cout << "AI making a move!" << std::endl;
        // Implement the AI's Minimax algorithm here to make an intelligent move
        // For now, let's pick a random move
        std::pair<int, int> aiMove = { rand() % BOARD_SIZE, rand() % BOARD_SIZE };
        int startX = rand() % BOARD_SIZE;
        int startY = rand() % BOARD_SIZE;
        if (board[startY][startX].player == PLAYER2) {
            board[aiMove.first][aiMove.second] = board[startY][startX];
            board[startY][startX] = Piece(EMPTY, NONE);
        }
        currentPlayer = PLAYER1;
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
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
    std::cout << "Enter 1 to play against AI, 0 for Player vs Player: ";
    std::cin >> vsAI;

    ChessGame game(vsAI);
    game.run();

    return 0;
}
