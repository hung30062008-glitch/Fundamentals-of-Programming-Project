#include "config.h"

bool _turn;
int _command;
int _X, _Y;
_point _a[boardsize][boardsize];

bool _sound = true;
int _volume = 80;
bool _block2ends = true;
int _keyUp = 'W', _keyDown = 'S', _keyLeft = 'A', _keyRight = 'D', _keyEnter = 13;

vector<CellPos> winCells;

void resetdata() {
    for (int i = 0; i < boardsize; i++)
        for (int j = 0; j < boardsize; j++) {
            _a[i][j].x = 4 * j + left + 2;
            _a[i][j].y = 2 * i + top + 1;
            _a[i][j].c = 0;
        }
    _turn = 1;
    _command = -1;
    _X = _a[5][5].x;
    _Y = _a[5][5].y;
    winCells.clear();
}

int testboard() {
    winCells.clear();
    int d_c[4] = { 1, 0, 1, -1 };
    int d_r[4] = { 0, 1, 1, 1 };
    int checkedcell = 0;
    for (int i = 0;i < boardsize;i++) {
        for (int j = 0;j < boardsize;j++) {
            int player = _a[i][j].c;
            if (player == 0)
                continue;
            checkedcell++;
            for (int k = 0; k < 4; k++) {
                int wincondition = 1;
                for (int h = 1; h <= 4; h++) {
                    int r = i + h * d_r[k];
                    int c = j + h * d_c[k];
                    if (r >= 0 && r < boardsize && c >= 0 && c < boardsize && _a[r][c].c == player)
                        wincondition++;
                    else break;
                }
                if (wincondition == 5) {
                    if (_block2ends) {
                        int head_r = i - d_r[k];
                        int head_c = j - d_c[k];
                        int tail_r = i + 5 * d_r[k];
                        int tail_c = j + 5 * d_c[k];
                        bool headBlocked = (head_r >= 0 && head_r < boardsize && head_c >= 0 && head_c < boardsize && _a[head_r][head_c].c == -player);
                        bool tailBlocked = (tail_r >= 0 && tail_r < boardsize && tail_c >= 0 && tail_c < boardsize && _a[tail_r][tail_c].c == -player);
                        if (headBlocked && tailBlocked)
                            continue;
                    }
                    for (int h = 0; h < 5; h++) {
                        winCells.push_back({ i + h * d_r[k], j + h * d_c[k] });
                    }
                    return player;
                }
            }
        }
    }
    if (checkedcell == 144)
        return 0;
    return 2;

}

int checkboard(int pX, int pY) {
    for (int i = 0;i < boardsize;i++)
        for (int j = 0;j < boardsize;j++) {
            if (_a[i][j].x == pX && _a[i][j].y == pY && _a[i][j].c == 0) {
                if (_turn)
                    _a[i][j].c = 1;
                else
                    _a[i][j].c = -1;
                return _a[i][j].c;
            }
        }
    return 0;
}

bool saveGameData(const string& filename) {
    ofstream outFile(filename.c_str());
    if (!outFile.is_open()) return false;
    outFile << _turn << "\n";
    for (int i = 0; i < boardsize; i++) {
        for (int j = 0; j < boardsize; j++) {
            outFile << _a[i][j].c << " ";
        }
        outFile << "\n";
    }
    outFile.close();
    return true;
}

bool loadGameData(const string& filename) {
    ifstream inFile(filename.c_str());
    if (!inFile.is_open()) return false;
    inFile >> _turn;
    for (int i = 0; i < boardsize; i++) {
        for (int j = 0; j < boardsize; j++) {
            inFile >> _a[i][j].c;
            _a[i][j].x = 4 * j + left + 2;
            _a[i][j].y = 2 * i + top + 1;
        }
    }
    inFile.close();
    _X = _a[5][5].x;
    _Y = _a[5][5].y;
    winCells.clear();
    return true;
}