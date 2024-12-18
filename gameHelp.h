#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

// The StopWatch class is a simple timer utility, allowing for start, stop, 
// pause, resume, and reset functionalities. It uses SFML's Clock and Time classes.
class StopWatch {
    // Private member variables:
    Clock theGameClock;   // Internal clock for tracking elapsed time.
    Time theGameTime;     // Accumulated time (updated when paused).
    bool paused = false; // Indicates whether the stopwatch is currently paused.

public:
    // Constructor: Initializes the stopwatch with zero time and starts the clock.
    StopWatch() {
        theGameTime = Time::Zero;    // Sets the elapsed time to zero.
        theGameClock.restart();      // Starts the clock from zero.
    }

    // Starts or resumes the stopwatch. If paused, restarts the internal clock.
    void start() {
        if (paused) { 
            theGameClock.restart(); // Resumes timing from the last pause.
            paused = false; 
        }
    }

    // Pauses the stopwatch and updates the total elapsed time.
    void stop() {
        if (!paused) { 
            theGameTime += theGameClock.getElapsedTime(); // Add current elapsed time.
            paused = true; 
        }
    }

    // Resets the stopwatch to zero and starts it again.
    void restart() {
        theGameTime = Time::Zero;    // Resets the accumulated time.
        theGameClock.restart();      // Starts the clock from zero.
    }

    // Returns the total elapsed time. 
    // Includes the clock's time if running, or just the accumulated time if paused.
    Time getElapsedTime() {
        return paused ? theGameTime : theGameTime + theGameClock.getElapsedTime();
    }

    // Returns whether the stopwatch is currently paused.
    bool isPaused() {
        return paused;
    }
};

// The Tile class represents a single tile in a Minesweeper game.
// Each tile has textures, state flags (e.g., is it revealed or flagged?), 
// and a list of pointers to neighboring tiles.
class Tile {
    // Private member variables:
    Texture tileHidden;    // Texture for a hidden (default) tile.
    Texture revealedTileTexture;  // Texture for a revealed tile.
    Texture mineTexture;          // Texture for a tile containing a mine.
    Texture textureWithFlag;          // Texture for a flagged tile.
    Texture textureOne;     // Texture for a tile showing the number 1.
    Texture textureTwo;     // Texture for a tile showing the number 2.
    Texture textureThree;   // Texture for a tile showing the number 3.
    Texture textureFour;    // Texture for a tile showing the number 4.
    Texture textureFive;    // Texture for a tile showing the number 5.
    Texture textureSix;     // Texture for a tile showing the number 6.
    Texture textureSeven;   // Texture for a tile showing the number 7.
    Texture textureEight;   // Texture for a tile showing the number 8.

public:
    Sprite sprite;  // Sprite representing the tile.
    vector<Tile*> vectorOfNeighborTilePointers; // Pointers to up to 8 neighboring tiles.

    bool tile_mine;     // Indicates if the tile contains a mine.
    bool tile_flagged;  // Indicates if the tile is flagged.
    bool tile_enabled;  // Indicates if the tile is interactable.
    bool tile_revealed; // Indicates if the tile has been revealed.
    int nearbyMines; // Number of mines in the neighboring tiles.

    // Parameterized constructor: Initializes a tile with its coordinates.
    Tile(int xCoordinate, int yCoordinate);

    // Draws the tile on the game window based on its state and game conditions.
    void draw(RenderWindow& window, bool is_debugMode, bool is_paused, bool leaderBoard, bool loser, bool winner);

    // Toggles the flagged state of the tile.
    void toggleFlag();
};

// The Board struct represents the Minesweeper game board.
// It contains all tiles and manages their interactions and game states.
struct Board {
    // Variables:
    int rows;   // Number of rows in the board.
    int columns; // Number of columns in the board.
    int tiles;   // Total number of tiles.
    int placeFlagging; // Number of flags available for placement.
    int mineCount;    // Total number of mines on the board.
    vector<vector<Tile*>*> boardPointer2D; // 2D vector holding all tile pointers.
    bool is_debugMode; // Indicates if debug mode is active (e.g., reveals all mines).
    bool is_paused;    // Indicates if the game is paused.
    bool leaderBoard;  // Indicates if leaderboard mode is active.
    bool loser;        // Indicates if the game is lost.
    bool winner;       // Indicates if the game is won.

    // Methods:
    Board(); // Default constructor: Initializes the board with default settings.

    // Draws or updates the board on the game window.
    void draw(RenderWindow& window);

    // Toggles debug mode on or off.
    void toggleDebugMode();

    // Toggles pause mode on or off.
    void togglePauseMode();

    // Toggles leaderboard display on or off.
    void toggleOfLB();

    // Disables all tiles, preventing interactions.
    void disableTiles();

    // Enables all tiles, allowing interactions.
    void enableAllTiles();

    // Reveals a tile and triggers any associated actions.
    void reveal(RenderWindow& window, Tile* tile);

    // Checks if the player has won the game.
    bool checkIfWinner();

    // Clears the board (e.g., resets all tiles).
    void clear();
};
