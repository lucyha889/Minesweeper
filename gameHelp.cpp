#include "gameHelp.h"
#include <random>
#include <fstream>

// Constructor for the Tile class
// Initializes the tile's coordinates, textures, and state
Tile::Tile(int xcoord, int ycoord) {
    // Load textures for various tile states
    tileHidden.loadFromFile("files/images/tile_hidden.png");
    revealedTileTexture.loadFromFile("files/images/tile_revealed.png");
    mineTexture.loadFromFile("files/images/mine.png");
    textureWithFlag.loadFromFile("files/images/flag.png");
    textureOne.loadFromFile("files/images/number_1.png");
    textureTwo.loadFromFile("files/images/number_2.png");
    textureThree.loadFromFile("files/images/number_3.png");
    textureFour.loadFromFile("files/images/number_4.png");
    textureFive.loadFromFile("files/images/number_5.png");
    textureSix.loadFromFile("files/images/number_6.png");
    textureSeven.loadFromFile("files/images/number_7.png");
    textureEight.loadFromFile("files/images/number_8.png");

    // Set default tile properties
    this->nearbyMines = 0; // Number of adjacent mines
    this->sprite.setPosition(xcoord * 32, ycoord * 32); // Position on the game board
    tile_flagged = false;  // Initially not flagged
    tile_mine = false;     // Initially not a mine
    tile_revealed = false; // Initially hidden
    tile_enabled = true;   // Initially active
}

// Render the tile based on its current state and game conditions
void Tile::draw(RenderWindow& window, bool is_debugMode, bool is_paused, bool leaderBoard, bool loser, bool winner) {
    if (is_debugMode && tile_mine) {
        // In debug mode, display mines even if hidden
        this->sprite.setTexture(tileHidden);
        window.draw(this->sprite);

        if (tile_flagged) {
            // Show flagged mines
            this->sprite.setTexture(textureWithFlag);
            window.draw(this->sprite);
        }

        this->sprite.setTexture(mineTexture);
        window.draw(this->sprite);
    } else if (is_debugMode && !tile_mine) {
        // In debug mode, display hidden or revealed non-mines
        if (!tile_revealed && !tile_flagged) {
            // Hidden and not flagged
            this->sprite.setTexture(tileHidden);
            window.draw(this->sprite);
        } else if (!tile_revealed && tile_flagged) {
            // Hidden but flagged
            this->sprite.setTexture(tileHidden);
            window.draw(this->sprite);

            this->sprite.setTexture(textureWithFlag);
            window.draw(this->sprite);
        } else if (tile_revealed && nearbyMines == 0) {
            // Revealed with no adjacent mines
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);
        } else {
            // Revealed with adjacent mines, display corresponding number texture
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            switch (nearbyMines) {
                case 1: this->sprite.setTexture(textureOne); break;
                case 2: this->sprite.setTexture(textureTwo); break;
                case 3: this->sprite.setTexture(textureThree); break;
                case 4: this->sprite.setTexture(textureFour); break;
                case 5: this->sprite.setTexture(textureFive); break;
                case 6: this->sprite.setTexture(textureSix); break;
                case 7: this->sprite.setTexture(textureSeven); break;
                case 8: this->sprite.setTexture(textureEight); break;
            }
            window.draw(this->sprite);
        }
    } else if (loser && tile_mine) {
        // Show all mines when the player loses
        this->sprite.setTexture(revealedTileTexture);
        window.draw(this->sprite);

        if (tile_flagged) {
            // Flagged mines
            this->sprite.setTexture(textureWithFlag);
            window.draw(this->sprite);
        }

        this->sprite.setTexture(mineTexture);
        window.draw(this->sprite);
    } else if (winner && tile_mine) {
        // Show all flagged mines when the player wins
        this->sprite.setTexture(tileHidden);
        window.draw(this->sprite);

        this->sprite.setTexture(textureWithFlag);
        window.draw(this->sprite);
    } else {
        // Handle other game states (general cases for hidden, revealed, flagged tiles)
        if (!tile_revealed && !tile_flagged && tile_enabled) {
            // Hidden and active tile
            this->sprite.setTexture(tileHidden);
            window.draw(this->sprite);
        } else if (!tile_revealed && tile_flagged) {
            // Hidden but flagged
            this->sprite.setTexture(tileHidden);
            window.draw(this->sprite);

            this->sprite.setTexture(textureWithFlag);
            window.draw(this->sprite);
        } else if (tile_revealed) {
            // Revealed tiles with surrounding mines
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);

            if (nearbyMines > 0 && !tile_mine) {
                switch (nearbyMines) {
                    case 1: this->sprite.setTexture(textureOne); break;
                    case 2: this->sprite.setTexture(textureTwo); break;
                    case 3: this->sprite.setTexture(textureThree); break;
                    case 4: this->sprite.setTexture(textureFour); break;
                    case 5: this->sprite.setTexture(textureFive); break;
                    case 6: this->sprite.setTexture(textureSix); break;
                    case 7: this->sprite.setTexture(textureSeven); break;
                    case 8: this->sprite.setTexture(textureEight); break;
                }
                window.draw(this->sprite);
            }
        }

        if (is_paused) {
            // Show revealed state when the game is paused
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);
        }

        if (leaderBoard && !winner) {
            // Show leaderboard view for non-winners
            this->sprite.setTexture(revealedTileTexture);
            window.draw(this->sprite);
        }
    }
}

// Toggles the flagged state of the tile
void Tile::toggleFlag() {
    this->tile_flagged = !this->tile_flagged; // Switch the flagged state (true to false or vice versa)
}

// Random number generation utilities
mt19937 random_mt; // Mersenne Twister random number generator

// Generates a random integer between 'min' and 'max' (inclusive)
int Random(int min, int max) {
    uniform_int_distribution<int> dist(min, max);
    return dist(random_mt);
}

// Board constructor: initializes the game board based on the config file
Board::Board() {
    // Open and read the configuration file
    fstream boardConfig("files/config.cfg");

    // Read the number of columns from the first line
    string columnInfoString;
    getline(boardConfig, columnInfoString);
    this->columns = stoi(columnInfoString);

    // Read the number of rows from the second line
    string rowInfoString;
    getline(boardConfig, rowInfoString);
    this->rows = stoi(rowInfoString);

    // Calculate the total number of tiles (rows × columns)
    this->tiles = this->rows * this->columns;

    // Read the number of mines from the third line
    string mineCountInfoString;
    getline(boardConfig, mineCountInfoString);
    this->mineCount = stoi(mineCountInfoString);

    // Initialize the remaining flags to place
    this->placeFlagging = mineCount;

    // Initialize game state flags
    this->is_debugMode = false;
    this->is_paused = false;
    this->leaderBoard = false;
    this->loser = false;
    this->winner = false;

    // Create and initialize a 2D vector of tiles
    for (unsigned i = 0; i < rows; i++) {
        vector<Tile*> *currRow = new vector<Tile*>;
        for (unsigned j = 0; j < columns; j++) {
            Tile *tempPointer = new Tile(j, i); // Create a new tile
            currRow->push_back(tempPointer);    // Add tile to the current row
        }
        boardPointer2D.push_back(currRow); // Add row to the board
    }

    // Randomly assign mines to tiles
    for (int i = 0; i < this->mineCount; i++) {
        int randomRow = Random(0, rows - 1);
        int randomColumn = Random(0, columns - 1);

        // Ensure the randomly chosen tile is not already a mine
        if (boardPointer2D.at(randomRow)->at(randomColumn)->tile_mine) {
            i--; // Retry if the tile is already a mine
        } else {
            boardPointer2D.at(randomRow)->at(randomColumn)->tile_mine = true;
        }
    }

    // Assign neighboring tiles and calculate surrounding mine counts
    for (int i = 0; i < boardPointer2D.size(); i++) {
        for (int j = 0; j < boardPointer2D.at(i)->size(); j++) {
            Tile *currentTile = boardPointer2D.at(i)->at(j);

            // Add neighboring tiles
            if (j - 1 >= 0) currentTile->vectorOfNeighborTilePointers.push_back(boardPointer2D.at(i)->at(j - 1)); // Left
            if ((i - 1 >= 0) && (j - 1 >= 0)) currentTile->vectorOfNeighborTilePointers.push_back(boardPointer2D.at(i - 1)->at(j - 1)); // Top-left
            if (i - 1 >= 0) currentTile->vectorOfNeighborTilePointers.push_back(boardPointer2D.at(i - 1)->at(j)); // Top
            if ((i - 1 >= 0) && (j + 1 < columns)) currentTile->vectorOfNeighborTilePointers.push_back(boardPointer2D.at(i - 1)->at(j + 1)); // Top-right
            if (j + 1 < columns) currentTile->vectorOfNeighborTilePointers.push_back(boardPointer2D.at(i)->at(j + 1)); // Right
            if ((i + 1 < rows) && (j + 1 < columns)) currentTile->vectorOfNeighborTilePointers.push_back(boardPointer2D.at(i + 1)->at(j + 1)); // Bottom-right
            if (i + 1 < rows) currentTile->vectorOfNeighborTilePointers.push_back(boardPointer2D.at(i + 1)->at(j)); // Bottom
            if ((i + 1 < rows) && (j - 1 >= 0)) currentTile->vectorOfNeighborTilePointers.push_back(boardPointer2D.at(i + 1)->at(j - 1)); // Bottom-left

            // Count surrounding mines (skip if the current tile is a mine)
            if (!currentTile->tile_mine) {
                for (Tile *neighbor : currentTile->vectorOfNeighborTilePointers) {
                    if (neighbor->tile_mine) currentTile->nearbyMines++;
                }
            }
        }
    }
}

// Renders the board and its tiles on the game window
void Board::draw(RenderWindow &window) {
    for (unsigned i = 0; i < boardPointer2D.size(); i++) {
        for (unsigned j = 0; j < boardPointer2D.at(i)->size(); j++) {
            boardPointer2D.at(i)->at(j)->draw(window, this->is_debugMode, this->is_paused, this->leaderBoard, this->loser, this->winner);
        }
    }
}

// Toggles the debug mode state
void Board::toggleDebugMode() {
    this->is_debugMode = !this->is_debugMode;
}

// Toggles the pause mode state
void Board::togglePauseMode() {
    this->is_paused = !this->is_paused;
}

// Toggles the leaderboard visibility state
void Board::toggleOfLB() {
    this->leaderBoard = !this->leaderBoard;
}

// Disables all tiles on the board
void Board::disableTiles() {
    for (unsigned i = 0; i < boardPointer2D.size(); i++) {
        for (unsigned j = 0; j < boardPointer2D.at(i)->size(); j++) {
            boardPointer2D.at(i)->at(j)->tile_enabled = false;
        }
    }
}

// Enables all tiles on the board
void Board::enableAllTiles() {
    for (unsigned i = 0; i < boardPointer2D.size(); i++) {
        for (unsigned j = 0; j < boardPointer2D.at(i)->size(); j++) {
            boardPointer2D.at(i)->at(j)->tile_enabled = true;
        }
    }
}

// Reveals a tile and its neighbors recursively if applicable
void Board::reveal(RenderWindow &window, Tile *tile) {
    if (tile->tile_flagged) return; // Do not reveal flagged tiles

    tile->tile_revealed = true;

    if (tile->nearbyMines == 0 && tile->tile_enabled) { // Recursive case for empty tiles
        for (Tile *neighbor : tile->vectorOfNeighborTilePointers) {
            if (!neighbor->tile_revealed && neighbor->tile_enabled) {
                reveal(window, neighbor);
            }
        }
    }
}

// Checks if the player has won the game
bool Board::checkIfWinner() {
    int revealedSafeTiles = 0;
    for (unsigned i = 0; i < boardPointer2D.size(); i++) {
        for (unsigned j = 0; j < boardPointer2D.at(i)->size(); j++) {
            Tile *tile = boardPointer2D.at(i)->at(j);
            if (!tile->tile_mine && tile->tile_revealed) {
                revealedSafeTiles++;
                if (revealedSafeTiles == tiles - mineCount) return true; // Win condition met
            }
        }
    }
    return false;
}

// Clears the board by deallocating memory (like a destructor)
void Board::clear() {
    for (unsigned i = 0; i < boardPointer2D.size(); i++) {
        vector<Tile *> *currRow = boardPointer2D.at(i);
        for (Tile *tile : *currRow) {
            delete tile;
        }
        delete currRow;
    }
}
