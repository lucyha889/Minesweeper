#include <iostream>
#include <fstream>  
#include <string>
#include <cctype>   // For character case validation in the welcome screen
#include <sstream>  
#include <SFML/Graphics.hpp>  // For graphical interface rendering
#include "gameHelp.h"
using namespace std;  
using namespace sf;   // Simplifies usage of SFML library components

// Adjusts the position of text to center it within a virtual text box
void getTheTextRect(Text &text, float xcoord, float ycoord) {
    FloatRect rectOfText = text.getLocalBounds();
    text.setOrigin(rectOfText.left + rectOfText.width / 2.0f, rectOfText.top + rectOfText.height / 2.0f);
    text.setPosition(Vector2f(xcoord, ycoord));
}

// Creates and configures a Text object with specified properties
Text setTheTextObj(const string& textString, Font& font, short size, Color color, float xcoord, float ycoord) {
    Text text(textString, font, size);
    text.setFillColor(color);
    getTheTextRect(text, xcoord, ycoord);
    return text;
}

// Struct to store leaderboard information for players
struct Player {
    short secondsTime;  // Player's game time in seconds
    string name;      // Player's name
};

int main(){
    
    Board gameBrd; // Initialize the game board (may take some time to load, causing a delay before the window appears)

    // Define dimensions for the Welcome and Game window
    int widthOfWindow = gameBrd.columns * 32; // Width of the main window, based on the number of columns
    int heightOfWindow = gameBrd.rows * 32 + 100; // Height of the main window, including extra space for UI elements

    // Define dimensions for the Leaderboard window
    int widthOfLB = gameBrd.columns * 16; // Width of the leaderboard window, scaled down from main window
    int heightOfLB = gameBrd.rows * 16 + 50; // Height of the leaderboard window, with additional space for text

    // Load font information from a file
    Font font;
    fstream fontFile("files/font.ttf");
    if (!font.loadFromFile("files/font.ttf")) {
        cout << "Failed to load font file!" << endl; // Output error message if the font file cannot be loaded
    }

    // Variables to store user input and render their name
    String inputTheUser; // Stores the player's name as entered in the welcome screen
    Text inputTheName; // Renderable Text object for displaying the player's name

    // Create the welcome window
    RenderWindow startingWindow(VideoMode(widthOfWindow, heightOfWindow), "Minesweeper", sf::Style::Close);
    while (startingWindow.isOpen()) { // Runs the loop until the close button (red "X") is pressed
        Event event;

        // Create and style the welcome message text
        Text textWel = setTheTextObj("WELCOME TO MINESWEEPER!", font, 24, Color::White, widthOfWindow / 2.0f, (heightOfWindow / 2.0f) - 150);
        textWel.setStyle(Text::Bold | Text::Underlined);

        // Create and style the input prompt text
        Text promptOfText = setTheTextObj("Enter your name:", font, 20, Color::White, widthOfWindow / 2.0f, (heightOfWindow / 2.0f) - 75);
        promptOfText.setStyle(Text::Bold);

        // Process user input and events in the welcome window
        while (startingWindow.pollEvent(event)) { // Continuously checks for events such as typing or button presses
            if (event.type == Event::Closed) {
                // Close the window and exit the application if the close button is clicked
                startingWindow.close();
                return 0;
            } else if (event.type == Event::TextEntered) {
                // Handle user typing: restrict input to alphabetic characters and limit to 10 letters
                if (isalpha(event.text.unicode) && inputTheUser.getSize() < 10) {
                    if (inputTheUser.getSize() == 0) {
                        // Capitalize the first letter of the name
                        char temp = static_cast<char>(event.text.unicode); // Convert Unicode to char
                        temp = toupper(temp);
                        inputTheUser += temp; // Append the character to the input string
                    } else {
                        // Make subsequent letters lowercase
                        char temp = static_cast<char>(event.text.unicode); // Convert Unicode to char
                        temp = tolower(temp);
                        inputTheUser += temp; // Append the character to the input string
                    }

                    // Update the Text object with the current input, adding a cursor ("|") at the end
                    inputTheName = setTheTextObj(inputTheUser + "|", font, 18, Color::Yellow, widthOfWindow / 2.0f, (heightOfWindow / 2.0f) - 45);
                    inputTheName.setStyle(Text::Bold);
                }
            } else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && inputTheUser.getSize() != 0) {
                // Close the welcome window if the "Enter" key is pressed and input is not empty
                startingWindow.close();
            } else if (event.type == Event::KeyPressed && event.key.code == Keyboard::BackSpace && inputTheUser.getSize() > 0) {
                // Remove the last character from the name if "Backspace" is pressed and input is not empty
                inputTheUser.erase(inputTheUser.getSize() - 1);
                inputTheName = setTheTextObj(inputTheUser + "|", font, 18, Color::Yellow, widthOfWindow / 2.0f, (heightOfWindow / 2.0f) - 45);
                inputTheName.setStyle(Text::Bold); // Update the Text object to reflect the new input
            }
        }

        // Update and render the welcome window every frame
        startingWindow.clear(Color::Blue); // Set the background color to blue
        startingWindow.draw(textWel); // Draw the welcome message
        startingWindow.draw(promptOfText); // Draw the input prompt
        startingWindow.draw(inputTheName); // Draw the current user input with the cursor
        startingWindow.display(); // Display the updated content on the screen
    }
// Load and configure the "face" button, which indicates the game state (e.g., happy, win, or lose)
    Sprite spriteFaceSym;
    spriteFaceSym.setPosition((((gameBrd.columns) / 2) * 32) - 32, 32 * (gameBrd.rows + 0.5)); // Centered position at the bottom of the game grid
    Texture textureOfFace;
    textureOfFace.loadFromFile("files/images/face_happy.png"); // Initially set to a happy face
    spriteFaceSym.setTexture(textureOfFace);
    // No "enabled" flag is needed as this button is always active

// Load and configure the "debug" button for toggling debug mode
    Sprite spriteDebugSym;
    spriteDebugSym.setPosition(((gameBrd.columns) * 32) - 304, 32 * (gameBrd.rows + 0.5)); // Positioned to the left of the pause button
    Texture textureOfDebug;
    textureOfDebug.loadFromFile("files/images/debug.png");
    spriteDebugSym.setTexture(textureOfDebug);
    bool enabledDB = true; // Will be disabled during game over

// Load and configure the "pause" button for pausing the game
    Sprite spritePause;
    spritePause.setPosition(((gameBrd.columns) * 32) - 240, 32 * (gameBrd.rows + 0.5)); // Positioned to the right of the debug button
    Texture textPause;
    textPause.loadFromFile("files/images/pause.png");
    spritePause.setTexture(textPause);
    bool enabledPB = true; // Will be disabled during game over

// Load and configure the "leaderboard" button for displaying top scores
    Sprite spriteLB;
    spriteLB.setPosition(((gameBrd.columns) * 32) - 176, 32 * (gameBrd.rows + 0.5)); // Positioned to the right of the pause button
    Texture textLB;
    textLB.loadFromFile("files/images/leaderboard.png");
    spriteLB.setTexture(textLB);
    // No "enabled" flag is needed as this button is always active

// Load and configure digit sprites for displaying numbers and symbols (e.g., negative sign)
    Sprite spriteDigits[11];
    Texture textDigits;
    textDigits.loadFromFile("files/images/digits.png"); // Contains digits 0-9 and a negative sign
    for (unsigned int i = 0; i < 11; i++) {
        spriteDigits[i].setTexture(textDigits); // Assign the shared texture
        spriteDigits[i].setTextureRect(IntRect(i * 21, 0, 21, 32)); // Define the sub-rect for each digit or symbol
    }

// Initialize the game clock
    StopWatch clockOfGame; // Tracks the elapsed game time

// Create vectors for storing leaderboard information
    vector<Player> ScoreHighVector; // Stores the top five scores for display
    vector<Player> allHighFileVector; // Stores all scores from the leaderboard file

    int positionOfNewWinner = -1; // Tracks the position of the new high score, if applicable (-1 indicates no new high score)

// Load all high scores from the leaderboard file
    string storageOfLine; // Temporary storage for each line in the file
    fstream fileOfScoreHigh("files/leaderboard.txt", ios_base::in); // Open the leaderboard file for reading

    // Read and parse all lines in the leaderboard file
    while (getline(fileOfScoreHigh, storageOfLine)) {
        Player tempPlayer;
        stringstream ss(storageOfLine); // Use stringstream to parse line data

        string highscoreMinString; // Minutes part of the score
        getline(ss, highscoreMinString, ':'); // Extract minutes up to the colon
        int highScoreMin = stoi(highscoreMinString); // Convert to integer

        string highscoreSecString; // Seconds part of the score
        getline(ss, highscoreSecString, ','); // Extract seconds up to the comma
        int highScoreSec = stoi(highscoreSecString); // Convert to integer

        highScoreSec += (highScoreMin * 60); // Convert total time to seconds
        tempPlayer.secondsTime = highScoreSec; // Store the time in seconds
        getline(ss, tempPlayer.name); // Extract the player's name

        allHighFileVector.push_back(tempPlayer); // Add the player's data to the full leaderboard vector
    }
    fileOfScoreHigh.close(); // Close the file (good practice)

// Load the top 5 high scores for display
    fileOfScoreHigh.open("files/leaderboard.txt", ios_base::in); // Re-open the file for reading
    for (unsigned int i = 0; i < 5 && getline(fileOfScoreHigh, storageOfLine); i++) {
        Player tempPlayer;
        stringstream ss(storageOfLine); // Use stringstream to parse line data

        string highscoreMinString; // Minutes part of the score
        getline(ss, highscoreMinString, ':'); // Extract minutes up to the colon
        int highScoreMin = stoi(highscoreMinString); // Convert to integer

        string highscoreSecString; // Seconds part of the score
        getline(ss, highscoreSecString, ','); // Extract seconds up to the comma
        int highScoreSec = stoi(highscoreSecString); // Convert to integer

        highScoreSec += (highScoreMin * 60); // Convert total time to seconds
        tempPlayer.secondsTime = highScoreSec; // Store the time in seconds
        getline(ss, tempPlayer.name); // Extract the player's name

        ScoreHighVector.push_back(tempPlayer); // Add the player's data to the active leaderboard vector
    }
    fileOfScoreHigh.close(); // Close the file (good practice)

// Create the main game window
    RenderWindow gameWindow(VideoMode(widthOfWindow, heightOfWindow), "Minesweeper", sf::Style::Close);
    while (gameWindow.isOpen()) { // Run until the window is closed
        gameWindow.clear(Color::White); // Clear the window with a white background
        gameBrd.draw(gameWindow); // Render the game board

        // Calculate the current game time in minutes and seconds
        int minsInCurrentUser = clockOfGame.getElapsedTime().asSeconds() / 60;
        int secsInCurrentUser = ((int)clockOfGame.getElapsedTime().asSeconds()) % 60;

        // Display the mine count (if negative, show a negative sign)
        if (gameBrd.placeFlagging < 0) {
            spriteDigits[10].setPosition(12, (32 * (gameBrd.rows + 0.5)) + 16); // Display the negative sign
            gameWindow.draw(spriteDigits[10]);
        }

        // Display the mine count when positive
        if (gameBrd.placeFlagging >= 0) {
            int mineCountHundredsDigit = gameBrd.placeFlagging / 100; // Hundreds digit
            spriteDigits[mineCountHundredsDigit].setPosition(33, (32 * (gameBrd.rows + 0.5)) + 16);
            gameWindow.draw(spriteDigits[mineCountHundredsDigit]);

            int mineCountTensDigit = (gameBrd.placeFlagging % 100) / 10; // Tens digit
            spriteDigits[mineCountTensDigit].setPosition(54, (32 * (gameBrd.rows + 0.5)) + 16);
            gameWindow.draw(spriteDigits[mineCountTensDigit]);

            int mineCountOnesDigit = gameBrd.placeFlagging % 10; // Ones digit
            spriteDigits[mineCountOnesDigit].setPosition(75, (32 * (gameBrd.rows + 0.5)) + 16);
            gameWindow.draw(spriteDigits[mineCountOnesDigit]);
        } else if (gameBrd.placeFlagging < 0) { // Handle negative mine count
            gameBrd.placeFlagging = abs(gameBrd.placeFlagging); // Temporarily convert to positive for calculations

            int mineCountHundredsDigit = gameBrd.placeFlagging / 100; // Hundreds digit
            spriteDigits[mineCountHundredsDigit].setPosition(33, (32 * (gameBrd.rows + 0.5)) + 16);
            gameWindow.draw(spriteDigits[mineCountHundredsDigit]);

            int mineCountTensDigit = (gameBrd.placeFlagging % 100) / 10; // Tens digit
            spriteDigits[mineCountTensDigit].setPosition(54, (32 * (gameBrd.rows + 0.5)) + 16);
            gameWindow.draw(spriteDigits[mineCountTensDigit]);

            int mineCountOnesDigit = gameBrd.placeFlagging % 10; // Ones digit
            spriteDigits[mineCountOnesDigit].setPosition(75, (32 * (gameBrd.rows + 0.5)) + 16);
            gameWindow.draw(spriteDigits[mineCountOnesDigit]);

            gameBrd.placeFlagging *= -1; // Revert mine count back to negative
        }

        // Set the position of the timer digits on the bottom-right corner of the screen
        spriteDigits[minsInCurrentUser / 10].setPosition((gameBrd.columns * 32) - 97, (32 * (gameBrd.rows + 0.5)) + 16); // Left digit of minutes
        gameWindow.draw(spriteDigits[minsInCurrentUser / 10]); // Draw the left minute digit immediately

        spriteDigits[minsInCurrentUser % 10].setPosition((gameBrd.columns * 32) - 76, (32 * (gameBrd.rows + 0.5)) + 16); // Right digit of minutes
        gameWindow.draw(spriteDigits[minsInCurrentUser % 10]); // Draw the right minute digit immediately

        spriteDigits[secsInCurrentUser / 10].setPosition((gameBrd.columns * 32) - 54, (32 * (gameBrd.rows + 0.5)) + 16); // Left digit of seconds
        gameWindow.draw(spriteDigits[secsInCurrentUser / 10]); // Draw the left second digit immediately

        spriteDigits[secsInCurrentUser % 10].setPosition((gameBrd.columns * 32) - 33, (32 * (gameBrd.rows + 0.5)) + 16); // Right digit of seconds
        gameWindow.draw(spriteDigits[secsInCurrentUser % 10]); // Draw the right second digit immediately

        Event eventOfGame;

        // Capture the location of the mouse click within the game window
        auto clickPositionWindow = Mouse::getPosition(gameWindow); // Pixel coordinates of the click
        auto clickWindow = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow)); // Map click to game world coordinates

        // Render the UI components that need updating every frame
        gameWindow.draw(spriteFaceSym); // Draw the "face" button
        gameWindow.draw(spriteDebugSym); // Draw the "debug" button
        gameWindow.draw(spritePause); // Draw the "pause" button
        gameWindow.draw(spriteLB); // Draw the "leaderboard" button
        gameWindow.display(); // Display the updated content on the screen

        // Continuously check for events within the game window
        while (gameWindow.pollEvent(eventOfGame)) {

            // If the leaderboard is active, create and manage the leaderboard window
            if (gameBrd.leaderBoard) {
                RenderWindow windowLB(VideoMode(widthOfLB, heightOfLB), "Minesweeper", sf::Style::Close);
                while (windowLB.isOpen()) {

                    windowLB.clear(Color::Blue); // Set the leaderboard window background to blue
                    Text LBText = setTheTextObj("LEADERBOARD", font, 20, Color::White, widthOfLB / 2.0f, (heightOfLB / 2.0f) - 120);
                    LBText.setStyle(Text::Bold | Text::Underlined);

                    // Combine high scores into a formatted string for display
                    string combiningHighscoreText = ""; // Initialize empty string for the leaderboard
                    for (unsigned int i = 0; i < ScoreHighVector.size(); i++) { // Iterate through the top 5 scores
                        short tempUserTimeInSeconds = ScoreHighVector.at(i).secondsTime;
                        int tempUserMinInt = tempUserTimeInSeconds / 60;
                        string tempUserMinStr = (tempUserMinInt < 10 ? "0" : "") + to_string(tempUserMinInt); // Format minutes as two digits
                        int tempUserSecInt = tempUserTimeInSeconds % 60;
                        string tempUserSecStr = (tempUserSecInt < 10 ? "0" : "") + to_string(tempUserSecInt); // Format seconds as two digits
                        string tempUserTime = tempUserMinStr + ":" + tempUserSecStr;
                        string tempUser = ScoreHighVector.at(i).name;

                        // Add an asterisk to the new high score if applicable
                        if (positionOfNewWinner >= 0 && i == positionOfNewWinner) {
                            tempUser += "*";
                        }

                        // Format the leaderboard display
                        if (i == 0) {
                            combiningHighscoreText += to_string(i + 1) + "\t" + tempUserTime + "\t" + tempUser;
                        } else {
                            combiningHighscoreText += "\n\n" + to_string(i + 1) + "\t" + tempUserTime + "\t" + tempUser;
                        }
                    }

                    Text highscoreText = setTheTextObj(combiningHighscoreText, font, 18, Color::White, widthOfLB / 2.0f, (heightOfLB / 2.0f) + 20);
                    highscoreText.setStyle(Text::Bold);

                    Event eventLB;
                    while (windowLB.pollEvent(eventLB)) { // Continuously check for events in the leaderboard window
                        if (eventLB.type == Event::Closed) { // Close the leaderboard window if the red "X" is clicked
                            gameBrd.leaderBoard = false;
                            if (!gameBrd.is_paused && !gameBrd.loser && !gameBrd.winner) {
                                gameBrd.enableAllTiles(); // Re-enable game tile interactions
                                clockOfGame.start(); // Resume the game clock
                            }
                            windowLB.close();
                        }
                    }

                    // Update and display leaderboard content
                    windowLB.draw(LBText);
                    windowLB.draw(highscoreText);
                    windowLB.display();
                }
            }

            // Close the game window if the red "X" is clicked
            if (eventOfGame.type == Event::Closed) {
                gameBrd.clear();
                gameWindow.close();
                return 0;
            }

            // Handle mouse clicks when the leaderboard is not active
            if (eventOfGame.type == Event::MouseButtonPressed && !gameBrd.leaderBoard) {
                cout << "Mouse clicked at position (" << (clickPositionWindow.x / 32) << ", " << (clickPositionWindow.y / 32) << ")" << endl;

                if (eventOfGame.mouseButton.button == sf::Mouse::Left) { // Handle left mouse button clicks

                    // Check all tiles in the game board for a click
                    for (unsigned i = 0; i < gameBrd.boardPointer2D.size(); i++) {
                        for (unsigned j = 0; j < gameBrd.boardPointer2D.at(i)->size(); j++) {

                            // If a tile is clicked and enabled, determine if it's a mine or not
                            if (gameBrd.boardPointer2D.at(i)->at(j)->sprite.getGlobalBounds().contains(clickWindow) && gameBrd.boardPointer2D.at(i)->at(j)->tile_enabled) {

                                // Reveal the tile if it's not a mine, not flagged, and valid to interact
                                if (!gameBrd.is_paused && !gameBrd.is_debugMode && !gameBrd.leaderBoard && !gameBrd.boardPointer2D.at(i)->at(j)->tile_mine && !gameBrd.boardPointer2D.at(i)->at(j)->tile_flagged) {
                                    gameBrd.reveal(gameWindow, gameBrd.boardPointer2D.at(i)->at(j)); // Recursive reveal
                                }
                                // Handle mine click (loss scenario)
                                else if (gameBrd.boardPointer2D.at(i)->at(j)->tile_mine && !gameBrd.boardPointer2D.at(i)->at(j)->tile_flagged) {
                                    cout << "You Lost!" << endl;
                                    textureOfFace.loadFromFile("files/images/face_lose.png"); // Change face to "dead"
                                    clockOfGame.stop(); // Stop the game clock
                                    gameBrd.loser = true;
                                    gameBrd.disableTiles();
                                    enabledDB = false;
                                    enabledPB = false;
                                }
                            }
                        }
                    }

                    // Restart the game if the face button is clicked
                    if (spriteFaceSym.getGlobalBounds().contains(clickWindow)) {
                        cout << "RESTARTING" << endl;

                        if (gameBrd.loser) gameBrd.loser = false; // Reset loser state
                        if (gameBrd.winner) gameBrd.winner = false; // Reset winner state

                        Board newGameBoard; // Create a new game board
                        gameBrd.clear(); // Clear memory from the old board
                        gameBrd = newGameBoard; // Set the current board to the new one
                        textureOfFace.loadFromFile("files/images/face_happy.png"); // Reset face to "happy"
                        clockOfGame.restart(); // Restart the game clock
                        clockOfGame.start();
                        enabledPB = true;
                        enabledDB = true;
                    }

                    // Toggle debug mode if the debug button is clicked
                    if (spriteDebugSym.getGlobalBounds().contains(clickWindow) && enabledDB) {
                        cout << "Debug button pressed" << endl;
                        gameBrd.toggleDebugMode();
                        if (gameBrd.is_debugMode) gameBrd.disableTiles(); // Disable interactions in debug mode
                        else gameBrd.enableAllTiles();
                    }

                    // Toggle pause mode if the pause button is clicked
                    if (spritePause.getGlobalBounds().contains(clickWindow) && enabledPB) {
                        if (clockOfGame.isPaused()) clockOfGame.start();
                        else clockOfGame.stop();
                        cout << "Pause button pressed" << endl;
                        gameBrd.togglePauseMode();
                        if (gameBrd.is_paused) {
                            clockOfGame.stop();
                            gameBrd.disableTiles();
                            enabledDB = false;
                            textPause.loadFromFile("files/images/play.png"); // Change icon to "play"
                        } else {
                            gameBrd.enableAllTiles();
                            enabledDB = true;
                            clockOfGame.start();
                            textPause.loadFromFile("files/images/pause.png"); // Change icon to "pause"
                        }
                    }

                    // Open leaderboard if the leaderboard button is clicked
                    if (spriteLB.getGlobalBounds().contains(clickWindow)) {
                        cout << "Leaderboard button pressed" << endl;
                        clockOfGame.stop(); // Stop the game clock
                        gameBrd.disableTiles(); // Disable interactions
                        gameBrd.toggleOfLB();
                    }
                }

                // Handle right mouse button clicks (flagging tiles)
                else if (eventOfGame.mouseButton.button == sf::Mouse::Right) {

                    // Check all tiles in the game board for a click
                    for (unsigned i = 0; i < gameBrd.boardPointer2D.size(); i++) {
                        for (unsigned j = 0; j < gameBrd.boardPointer2D.at(i)->size(); j++) {

                            // Only allow flagging if the game is in a valid state
                            if (gameBrd.boardPointer2D.at(i)->at(j)->sprite.getGlobalBounds().contains(clickWindow) && !gameBrd.is_paused && !gameBrd.is_debugMode && !gameBrd.loser && !gameBrd.winner) {

                                // Place a flag if the tile is not already flagged or revealed
                                if (!gameBrd.boardPointer2D.at(i)->at(j)->tile_flagged && !gameBrd.boardPointer2D.at(i)->at(j)->tile_revealed) {
                                    gameBrd.placeFlagging--; // Decrement the mine counter
                                    gameBrd.boardPointer2D.at(i)->at(j)->toggleFlag();
                                }

                                // Remove a flag if it is already flagged and valid
                                else if (gameBrd.boardPointer2D.at(i)->at(j)->tile_flagged && !gameBrd.boardPointer2D.at(i)->at(j)->tile_revealed) {
                                    gameBrd.placeFlagging++; // Increment the mine counter
                                    gameBrd.boardPointer2D.at(i)->at(j)->toggleFlag();
                                }
                            }
                        }
                    }
                }
            }
        }


        // Check if the user has won the game
        if (gameBrd.checkIfWinner()) {
            clockOfGame.stop(); // Stop the game clock to record the most accurate time
            gameBrd.placeFlagging = 0; // Reset flags as per the game instructions

            if (!gameBrd.winner) { // Ensure the leaderboard window appears only once after winning
                gameBrd.leaderBoard = true; // Activate leaderboard display

                // Determine if the user's score qualifies for the leaderboard
                positionOfNewWinner = -1; // Default to no new high score (indicated by -1)

                // Fetch the fifth place from the active leaderboard and the last place in the complete leaderboard
                Player playerInFifth = ScoreHighVector.back();
                Player playerInLast = allHighFileVector.back();

                // Create a new Player object for the current winner
                Player tempPlayer;
                tempPlayer.name = inputTheUser; // Assign the user's name
                tempPlayer.secondsTime = secsInCurrentUser + (minsInCurrentUser * 60); // Convert elapsed time to total seconds

                // Check if the user's time is better than the fifth place on the active leaderboard
                if (tempPlayer.secondsTime < playerInFifth.secondsTime) {
                    ScoreHighVector.pop_back(); // Remove the slowest score from the active leaderboard

                    // Insert the new high score into the appropriate position (top 1-4)
                    for (auto iter = ScoreHighVector.begin(); iter != ScoreHighVector.end(); iter++) {
                        positionOfNewWinner++; // Increment the position for potential insertion
                        if (tempPlayer.secondsTime < iter->secondsTime) {
                            ScoreHighVector.insert(iter, tempPlayer); // Insert the new score
                            break;
                        }
                    }

                    // If the new high score was not inserted, add it as the fifth score
                    if (ScoreHighVector.size() < 5) {
                        positionOfNewWinner++; // Update the position for the fifth place
                        ScoreHighVector.push_back(tempPlayer); // Add the new score at the end
                    }
                }

                // Insert the new high score into the full leaderboard
                for (auto iter = allHighFileVector.begin(); iter != allHighFileVector.end(); iter++) {
                    if (tempPlayer.secondsTime < iter->secondsTime) {
                        allHighFileVector.insert(iter, tempPlayer); // Insert the new score
                        break;
                    }
                }

                // Add the new score to the end if it is the worst score so far
                if (tempPlayer.secondsTime >= playerInLast.secondsTime) {
                    allHighFileVector.push_back(tempPlayer);
                }

                // Update the leaderboard file with the latest scores
                fileOfScoreHigh.open("files/leaderboard.txt", std::ios::out | std::ios::trunc); // Open file for overwriting
                string tempFileMinsStr;
                string tempFileSecsStr;
                for (auto iter = allHighFileVector.begin(); iter != allHighFileVector.end(); iter++) {
                    short tempFileMins = (iter->secondsTime) / 60; // Calculate minutes
                    short tempFileSecs = (iter->secondsTime) % 60; // Calculate seconds

                    // Format minutes with leading zero if needed
                    if (tempFileMins >= 10) tempFileMinsStr = to_string(tempFileMins);
                    else tempFileMinsStr = "0" + to_string(tempFileMins);

                    // Format seconds with leading zero if needed
                    if (tempFileSecs >= 10) tempFileSecsStr = to_string(tempFileSecs);
                    else tempFileSecsStr = "0" + to_string(tempFileSecs);

                    // Write the formatted score to the file
                    fileOfScoreHigh << tempFileMinsStr + ":" + tempFileSecsStr + "," + iter->name << endl;
                }
                fileOfScoreHigh.close(); // Close the file
            }

            // Mark the game as won and update UI components
            gameBrd.winner = true; // Prevent multiple leaderboard pop-ups
            textureOfFace.loadFromFile("files/images/face_win.png"); // Display the winning face
            gameBrd.disableTiles(); // Disable further tile interactions
            enabledDB = false; // Disable debug button
            enabledPB = false; // Disable pause button
        }
    }
    return 0;

}