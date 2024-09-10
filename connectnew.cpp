#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MaxRows = 14;
const int MaxCols = 14;
const int MaxPlayers = 2;
const char DefaultFill = '.';

char Board[MaxRows][MaxCols];

int Next_Move[MaxCols] = { 0 };

string PlayerNames[MaxPlayers];
char PlayerMarks[MaxPlayers] = { 'o', 'x' };

int TurningPlayer = 0;


int GameEndState = 0;

int Rows = 6;
int Columns = 7;
int ConnectLimit = 4;



int Menu() {
    int Choice;
    do {
        cout << "1. Play" << endl;
        cout << "2. Save Game" << endl;
        cout << "3. Load Game" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter Your Choice (1 - 4): " << endl;
        cin >> Choice;
    } while (Choice < 1 || Choice > 4);
    return Choice;
}


void initNewGame() {
    cout << "How Many Rows? ";
    cin >> Rows;
    cout << "How Many Cols? ";
    cin >> Columns;
    cout << "Connectivity Limit? ";
    cin >> ConnectLimit;
    cout << "First Player Name? ";
    cin >> PlayerNames[0];
    cout << "Second Player Name? ";
    cin >> PlayerNames[1];

    for (int r = 0; r < MaxRows; r++)
        for (int c = 0; c < MaxCols; c++)
            Board[r][c] = DefaultFill;
}

bool MakeMove(int Col) {
    int R = Next_Move[Col];
    if (R >= Rows) {
        cout << "Column is full. Choose another column." << endl;
        return false;
    }
    Board[R][Col] = PlayerMarks[TurningPlayer];
    TurningPlayer = (TurningPlayer + 1) % MaxPlayers;
    Next_Move[Col]++;
    return true;
}



bool GameisOn() {
    return GameEndState == 0;
}
bool CheckWin(int row, int col) {
    char currentPlayerMark = PlayerMarks[(TurningPlayer + 1) % MaxPlayers]; 

    // Check horizontally
    int count = 0;
    for (int i = std::max(0, col - (ConnectLimit - 1)); i <= std::min(Columns - 1, col + (ConnectLimit - 1)); i++) {
        if (Board[row][i] == currentPlayerMark) {
            count++;
            if (count == ConnectLimit) return true;
        }
        else {
            count = 0;
        }
    }

    // Check vertically
    count = 0;
    for (int i = std::max(0, row - (ConnectLimit - 1)); i <= std::min(Rows - 1, row + (ConnectLimit - 1)); i++) {
        if (Board[i][col] == currentPlayerMark) {
            count++;
            if (count == ConnectLimit) return true;
        }
        else {
            count = 0;
        }
    }

    // Check diagonally (top-left to bottom-right)
    count = 0;
    for (int i = -std::min(std::min(row, col), ConnectLimit - 1); i <= std::min(std::min(Rows - 1 - row, Columns - 1 - col), ConnectLimit - 1); i++) {
        if (Board[row + i][col + i] == currentPlayerMark) {
            count++;
            if (count == ConnectLimit) return true;
        }
        else {
            count = 0;
        }
    }

    // Check diagonally (bottom-left to top-right)
    count = 0;
    for (int i = -std::min(std::min(row, Columns - 1 - col), ConnectLimit - 1); i <= std::min(std::min(Rows - 1 - row, col), ConnectLimit - 1); i++) {
        if (Board[row + i][col - i] == currentPlayerMark) {
            count++;
            if (count == ConnectLimit) return true;
        }
        else {
            count = 0;
        }
    }

    return false;
}

bool IsValidMove(int col) {
    return col >= 0 && col < Columns && Next_Move[col] < Rows;
}


bool SaveGame(const string& FileName) {
    // SaveGame implementation
    ofstream outFile(FileName);

    if (!outFile.is_open()) {
        cout << "Unable to open the file for saving!" << endl;
        return false;
    }

    // Saving game data to the file
    outFile << Rows << " " << Columns << " " << ConnectLimit << endl;
    outFile << PlayerNames[0] << " " << PlayerNames[1] << endl;

    for (int r = 0; r < Rows; r++) {
        for (int c = 0; c < Columns; c++) {
            outFile << Board[r][c] << " ";
        }
        outFile << endl;
    }

    outFile << TurningPlayer << " " << GameEndState << endl;

    outFile.close();
    return true;
}

bool LoadSavedGame(const string& FileName) {
    // LoadSavedGame implementation
    ifstream inFile(FileName);

    if (!inFile.is_open()) {
        cout << "Unable to open the file for loading!" << endl;
        return false;
    }

    // Loading game data from the file
    inFile >> Rows >> Columns >> ConnectLimit;
    inFile >> PlayerNames[0] >> PlayerNames[1];

    for (int r = 0; r < Rows; r++) {
        for (int c = 0; c < Columns; c++) {
            inFile >> Board[r][c];
        }
    }

    inFile >> TurningPlayer >> GameEndState;

    inFile.close();
    return true;
}

void ShowGame() {
    for (int c = 0; c < Columns; c++)
        cout << "_________";
    cout << endl;
    for (int c = 0; c < Columns; c++)
        cout << "\t" << c;
    cout << endl;
    for (int c = 0; c < Columns; c++)
        cout << "_________";

    cout << endl;

    for (int r = Rows - 1; r >= 0; r--) {
        cout << "| " << r << " |\t";
        for (int c = 0; c < Columns; c++)
            cout << Board[r][c] << "\t";
        cout << endl << "|   |" << endl;
    }

    for (int c = 0; c < Columns; c++)
        cout << "_________";
    cout << endl;

    if (GameisOn())
        cout << endl << "\tPlayer:" << PlayerNames[TurningPlayer]
        << "("
        << PlayerMarks[TurningPlayer] << ")"
        << " Moving " << endl;
}

void Play() {
    int Move;
    do {
        ShowGame();
        cout << "\tENTER NEGATIVE NUMBER FOR THE MAIN MENU" << endl;
        cout << "\tPlease Enter Your Move ? (0 - " << Columns << "): ";
        cin >> Move;
        if (Move < 0)
            break;
        MakeMove(Move);

        // Check for winning condition after each move
        for (int r = 0; r < Rows; ++r) {
            for (int c = 0; c < Columns; ++c) {
                if (CheckWin(r, c)) {
                    ShowGame();
                    cout << "Player " << PlayerNames[(TurningPlayer + 1) % MaxPlayers] << " wins!" << endl;
                    GameEndState = TurningPlayer;
                    return;
                }
            }
        }
    } while (GameisOn());

    if (GameEndState == -1) {
        ShowGame();
        cout << "It's a draw!" << endl;
    }
}
int main() {
    int Choice;
    bool GameNotLoaded = true;
    string filename;

    for (int i = 0; i < MaxCols; i++)
        Next_Move[i] = 0;

    do {
        Choice = Menu();
        switch (Choice) {
        case 1:
            if (GameNotLoaded) {
                initNewGame();
                GameNotLoaded = false;
            }
            Play();
            break;
        case 2:
            // Implement Save Game
            cout << "Enter the filename to save the game: ";
            cin >> filename;
            if (SaveGame(filename)) {
                cout << "Game saved successfully!" << endl;
            }
            else {
                cout << "Failed to save the game!" << endl;
            }
            break;
        case 3:
            // Implement Load Game
            cout << "Enter the filename to load the game: ";
            cin >> filename;
            if (LoadSavedGame(filename)) {
                cout << "Game loaded successfully!" << endl;
                GameNotLoaded = false;
            }
            else {
                cout << "Failed to load the game!" << endl;
            }
            break;
        }
    } while (Choice != 4);
    return 0;
}