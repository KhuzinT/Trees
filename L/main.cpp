#include <iostream>
#include <vector>

//**********************************************************************************

typedef const uint64_t& ull_t;

template <typename T>
T kVeryBigValue = INT64_MAX;

//**********************************************************************************

template <typename T>
class SquareTreap {
private:
    typedef std::vector<std::vector<T>> matrix_t;

    matrix_t tree_;

    void BuildY(ull_t x_root, ull_t x_left, ull_t x_right, ull_t y_root, ull_t y_left, ull_t y_right, matrix_t& array) {
        if (y_left != y_right) {
            uint64_t y_mid = (y_left + y_right) / 2;
            uint64_t y_child = y_root << 1;
            BuildY(x_root, x_left, x_right, y_child, y_left, y_mid, array);
            BuildY(x_root, x_left, x_right, y_child + 1, y_mid + 1, y_right, array);
            tree_[x_root][y_root] = std::min(tree_[x_root][y_child], tree_[x_root][y_child + 1]);
            return;
        }
        if (x_left != x_right) {
            uint64_t x_child = x_root << 1;
            tree_[x_root][y_root] = std::min(tree_[x_child][y_root], tree_[x_child + 1][y_root]);
            return;
        }
        tree_[x_root][y_root] = array[x_left][y_left];
    }

    void BuildX(ull_t x_root, ull_t x_left, ull_t x_right, matrix_t& array) {
        if (x_left != x_right) {
            uint64_t x_mid = (x_left + x_right) / 2;
            uint64_t x_child = x_root << 1;
            BuildX(x_child, x_left, x_mid, array);
            BuildX(x_child + 1, x_mid + 1, x_right, array);
        }
        uint64_t M = array[0].size();
        BuildY(x_root, x_left, x_right, 1, 0, M - 1, array);
    }

    T GetY(ull_t x_root, ull_t y_root, ull_t y_left, ull_t y_right, ull_t y1, ull_t y2) {
        if (y_right < y1 || y2 < y_left || y1 > y2) {
            return kVeryBigValue<T>;
        }
        if (y1 <= y_left && y_right <= y2) {
            return tree_[x_root][y_root];
        }
        uint64_t y_mid = (y_left + y_right) / 2;
        uint64_t y_child = y_root << 1;
        auto f_result = GetY(x_root, y_child, y_left, y_mid, y1, std::min(y2, y_mid));
        auto s_result = GetY(x_root, y_child + 1, y_mid + 1, y_right, std::max(y1, y_mid + 1), y2);
        return std::min(f_result, s_result);
    }

    T GetX(ull_t x_root, ull_t x_left, ull_t x_right, ull_t x1, ull_t x2, ull_t y1, ull_t y2) {
        if (x_right < x1 || x2 < x_left || x1 > x2) {
            return kVeryBigValue<T>;
        }
        if (x1 <= x_left && x_right <= x2) {
            uint64_t M = tree_[0].size() / 4;
            return GetY(x_root, 1, 0, M - 1, y1, y2);
        }
        uint64_t x_mid = (x_left + x_right) / 2;
        uint64_t x_child = x_root << 1;
        auto f_result = GetX(x_child, x_left, x_mid, x1, std::min(x2, x_mid), y1, y2);
        auto s_result = GetX(x_child + 1, x_mid + 1, x_right, std::max(x1, x_mid + 1), x2, y1, y2);
        return std::min(f_result, s_result);
    }

public:
    explicit SquareTreap(ull_t N, ull_t M) {
        tree_.resize(4 * N);
        for (uint64_t i = 0; i < 4 * N; ++i) {
            tree_[i].resize(4 * M, kVeryBigValue<T>);
        }
    }

    void BuildTree(matrix_t& array) {
        BuildX(1, 0, array.size() - 1, array);
    }

    T GetMin(ull_t x1, ull_t y1, ull_t x2, ull_t y2) {
        uint64_t N = tree_.size() / 4;
        return GetX(1, 0, N - 1, x1 - 1, x2 - 1, y1 - 1, y2 - 1);
    }

    void PrintTree() {
        for (uint64_t i = 0; i < tree_.size(); ++i) {
            for (uint64_t j = 0; j < tree_[i].size(); ++j) {
                std::cout << tree_[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }
};

//**********************************************************************************

int main() {
    //**********************************************************************************

    typedef std::vector<std::vector<int64_t>> matrix_t;

    uint64_t N = 0, M = 0;
    std::cin >> N >> M;

    //**********************************************************************************

    matrix_t array;
    for (uint64_t i = 0; i < N; ++i) {
        std::vector<int64_t> elem;
        for (uint64_t j = 0; j < M; ++j) {
            int64_t value = 0;
            std::cin >> value;
            elem.push_back(value);
        }
        array.push_back(elem);
    }

    SquareTreap<int64_t> tree(N, M);

    tree.BuildTree(array);

    //tree.PrintTree();

    //**********************************************************************************

    uint64_t q_requests = 0;
    std::cin >> q_requests;
    for (uint64_t i = 0; i < q_requests; ++i) {
        uint64_t x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        std::cin >> x1 >> y1 >> x2 >> y2;
        int64_t result = tree.GetMin(x1, y1, x2, y2);
        std::cout << result << std::endl;
    }

    return 0;

    //**********************************************************************************
}
