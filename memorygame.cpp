#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

const int GRID_SIZE = 4; // 4x4 grid

struct Card {
    int value;
    bool isFlipped;
    bool isMatched;
};


void clearScreen() {
    // Try ANSI codes first
    cout << "\033[2J\033[1;1H";
    cout.flush();
    
    //Fallback: also try system clear
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


void printBoard(const vector<Card>& cards) {
    cout << "\n     ";
    for (int col = 0; col < GRID_SIZE; col++) {
        cout << col + 1 << "   ";
    }
    cout << "\n";

    for (int row = 0; row < GRID_SIZE; row++) {
        cout << " " << row + 1 << "  ";
        for (int col = 0; col < GRID_SIZE; col++) {
            int index = row * GRID_SIZE + col;

            if (cards[index].isMatched) {
                cout << "[" << cards[index].value << "] ";
            }
            else if (cards[index].isFlipped) {
                cout << " " << cards[index].value << "  ";
            }
            else {
                cout << " *  ";
            }
        }
        cout << "\n";
    }
}

vector<Card> setupGame() {
    vector<Card> cards;
    int totalPairs = GRID_SIZE * GRID_SIZE / 2;

    for (int i = 1; i <= totalPairs; i++) {
        cards.push_back({ i, false, false });
        cards.push_back({ i, false, false });
    }

    // Simple shuffle using rand()
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < cards.size(); i++) {
        int randomIndex = rand() % cards.size();
        swap(cards[i], cards[randomIndex]);
    }

    return cards;
}

void waitForEnter() {
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

bool getValidInput(int& row, int& col) {
    cout << "Enter row and column (example: 1 2): ";

    if (!(cin >> row >> col)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter numbers only!\n";
        return false;
    }

    if (row < 1 || row > GRID_SIZE || col < 1 || col > GRID_SIZE) {
        cout << "Please enter numbers between 1 and " << GRID_SIZE << "!\n";
        return false;
    }

    return true;
}

int main() {
    cout << "=== MEMORY MATCHING GAME ===\n";
    cout << "Find all the matching pairs!\n";
    waitForEnter();

    vector<Card> cards = setupGame();
    int moves = 0;
    int pairsFound = 0;
    int totalPairs = GRID_SIZE * GRID_SIZE / 2;

    while (pairsFound < totalPairs) {
        clearScreen();
        cout << "=== MEMORY MATCHING GAME ===\n";
        printBoard(cards);
        cout << "\nMoves: " << moves << " | Pairs: " << pairsFound << "/" << totalPairs << "\n";

        // First card
        int row1, col1, index1;
        while (true) {
            if (!getValidInput(row1, col1)) continue;

            index1 = (row1 - 1) * GRID_SIZE + (col1 - 1);

            if (cards[index1].isMatched) {
                cout << "Already matched! Try another card.\n";
                continue;
            }
            break;
        }

        cards[index1].isFlipped = true;
        clearScreen();
        cout << "=== MEMORY MATCHING GAME ===\n";
        printBoard(cards);

        // Second card
        int row2, col2, index2;
        while (true) {
            cout << "Select second card:\n";
            if (!getValidInput(row2, col2)) continue;

            index2 = (row2 - 1) * GRID_SIZE + (col2 - 1);

            if (cards[index2].isMatched) {
                cout << "Already matched! Try another card.\n";
                continue;
            }
            if (cards[index2].isFlipped) {
                cout << "Already flipped! Try another card.\n";
                continue;
            }
            break;
        }

        cards[index2].isFlipped = true;
        moves++;

        clearScreen();
        cout << "=== MEMORY MATCHING GAME ===\n";
        printBoard(cards);

        // Check match
        if (cards[index1].value == cards[index2].value) {
            cout << "\nMATCH FOUND!\n";
            cards[index1].isMatched = true;
            cards[index2].isMatched = true;
            pairsFound++;
        }
        else {
            cout << "\nNo match. Try again!\n";
            cards[index1].isFlipped = false;
            cards[index2].isFlipped = false;
        }

        if (pairsFound < totalPairs) {
            waitForEnter();
        }
    }

    clearScreen();
    cout << "CONGRATULATIONS!\n";
    printBoard(cards);
    cout << "\nYou won in " << moves << " moves!\n";
    cout << "Game Over!\n";
    waitForEnter();

    return 0;
}


