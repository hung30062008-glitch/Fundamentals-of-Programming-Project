#include "config.h"

// ==========================================
// ĐỊNH NGHĨA CÁC BIẾN TOÀN CỤC (MODEL)
// ==========================================
bool _turn;         // true: Lượt của Người chơi 1 (X); false: Lượt của Người chơi 2 (O)
int _command;       // Lưu mã phím điều khiển cuối cùng nhận được từ bàn phím
int _X, _Y;         // Tọa độ con trỏ hiện tại trên màn hình console (tương ứng vị trí ô cờ)
_point _a[boardsize][boardsize]; // Mảng 2 chiều 12x12 lưu trạng thái từng ô trên bàn cờ

bool _sound = true;       // Trạng thái âm thanh (mặc định bật)
int _volume = 50;         // Mức âm lượng (mặc định 50%)
bool _block2ends = true;  // Luật chặn 2 đầu (mặc định bật: 5 con bị chặn 2 đầu thì chưa thắng)
int _keyup = 'W', _keydown = 'S', _keyleft = 'A', _keyright = 'D', _keyenter = 13; // Phím điều hướng và chọn

vector<CellPos> winCells; // Danh sách lưu 5 ô cờ tạo nên hàng thắng để thực hiện hiệu ứng nhấp nháy

/**
 * @brief Khởi tạo hoặc đặt lại toàn bộ dữ liệu bàn cờ về trạng thái ban đầu.
 * - Tính toán tọa độ hiển thị x, y trên console cho từng ô cờ dựa theo kích thước boardsize, lề trái (left), lề trên (top).
 * - Đặt giá trị quân cờ c = 0 (ô trống) cho tất cả các ô.
 * - Đặt lại lượt đi đầu tiên cho Người chơi 1 (_turn = true).
 * - Đặt vị trí con trỏ ban đầu ở ô trung tâm (5, 5).
 * - Xóa danh sách các ô thắng trước đó.
 */
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

/**
 * @brief Kiểm tra trạng thái ván cờ (thắng, thua, hòa, hoặc đang tiếp tục).
 * - Thuật toán duyệt qua từng ô trên bàn cờ. Nếu ô có quân (X hoặc O), tiến hành kiểm tra 4 hướng:
 *   1. Hướng ngang sang phải (d_r = 0, d_c = 1)
 *   2. Hướng dọc xuống dưới (d_r = 1, d_c = 0)
 *   3. Hướng chéo chính xuống phải (d_r = 1, d_c = 1)
 *   4. Hướng chéo phụ xuống trái (d_r = 1, d_c = -1)
 * - Nếu phát hiện chuỗi 5 ô liên tiếp cùng màu:
 *   + Nếu bật luật chặn 2 đầu (_block2ends == true): kiểm tra ô đầu và ô cuối của chuỗi 5 ô.
 *     Nếu CẢ HAI ĐẦU đều bị quân đối phương (-player) chặn thì bỏ qua (không tính thắng).
 *   + Nếu hợp lệ, lưu tọa độ 5 ô thắng vào vector winCells và trả về người chiến thắng.
 * - Nếu không có ai thắng và đã đánh kín bàn cờ (144 ô), trả về kết quả hòa.
 * 
 * @return int:
 *   1  : Người chơi 1 (X) thắng
 *  -1  : Người chơi 2 (O) thắng
 *   0  : Trận đấu hòa (đầy bàn cờ)
 *   2  : Trận đấu chưa kết thúc (tiếp tục chơi)
 */
int testboard() {
    winCells.clear();
    int d_c[4] = { 1, 0, 1, -1 };
    int d_r[4] = { 0, 1, 1, 1 };
    int checkedcell = 0;
    for (int i = 0; i < boardsize; i++) {
        for (int j = 0; j < boardsize; j++) {
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
                            continue; // Bị chặn cả 2 đầu -> không tính thắng theo luật
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
        return 0; // Đã đánh hết bàn cờ (12 * 12 = 144 ô) -> Hòa
    return 2;     // Trận đấu vẫn đang diễn ra bình thường
}

/**
 * @brief Kiểm tra và ghi nhận nước đánh cờ tại tọa độ console (pX, pY).
 * - Duyệt qua ma trận _a để tìm ô có tọa độ console khớp với (pX, pY).
 * - Nếu ô đó còn trống (c == 0):
 *   + Nếu đang là lượt _turn (lượt X): gán c = 1.
 *   + Nếu không (lượt O): gán c = -1.
 *   + Trả về giá trị quân vừa đánh (1 hoặc -1).
 * - Nếu ô đã có quân hoặc không hợp lệ: trả về 0.
 * 
 * @param pX Tọa độ X hiện tại của con trỏ trên console
 * @param pY Tọa độ Y hiện tại của con trỏ trên console
 * @return int: 1 nếu đánh X thành công, -1 nếu đánh O thành công, 0 nếu ô đã có quân
 */
int checkboard(int pX, int pY) {
    for (int i = 0; i < boardsize; i++)
        for (int j = 0; j < boardsize; j++) {
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

/**
 * @brief Lưu trạng thái ván cờ hiện tại ra file văn bản.
 * - Ghi lượt chơi kế tiếp (_turn).
 * - Ghi ma trận trạng thái quân cờ 12x12 (giá trị 0, 1, -1).
 * 
 * @param filename Tên đường dẫn file cần lưu (ví dụ: "save1.txt")
 * @return bool: true nếu lưu file thành công, false nếu không mở được file
 */
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

/**
 * @brief Đọc và nạp trạng thái ván cờ đã lưu từ file văn bản.
 * - Đọc lại lượt chơi (_turn).
 * - Đọc lại ma trận quân cờ và tính toán lại tọa độ console tương ứng cho từng ô.
 * - Đặt vị trí con trỏ về trung tâm bàn cờ và làm sạch danh sách winCells.
 * 
 * @param filename Tên đường dẫn file cần đọc dữ liệu
 * @return bool: true nếu đọc file thành công, false nếu file không tồn tại/lỗi mở file
 */
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