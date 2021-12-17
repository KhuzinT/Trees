#include <iostream>
#include <vector>

//**********************************************************************************

template <typename T>
T kZero = 0;

template <typename T>
T kOne = 1;

//**********************************************************************************

template <typename T>
class SegmentTree {
private:

    std::vector<T> tree_;

    void Build(const size_t& root, const size_t& left, const size_t& right, std::vector<T>& soldiers) {
        if (left == right) {
            tree_[root] = soldiers[left];
            return;
        }
        size_t mid = (left + right) / 2;
        size_t child = root << 1;
        Build(child, left, mid, soldiers);
        Build(child + 1, mid + 1, right, soldiers);
    }

    T Get(const size_t& root, const size_t& left, const size_t& right, const size_t& position) const {
        if (left == right) {
            return tree_[root];
        }
        size_t mid = (left + right) / 2;
        size_t child = root << 1;
        if (position > mid) {
            return Get(child + 1, mid + 1, right, position) + tree_[root];
        }
        return Get(child, left, mid, position) + tree_[root];
    }

    void Update(const size_t& root, const size_t& left, const size_t& right, const size_t& s_left, const size_t& s_right, const T& value) {
        if (left == s_left && right == s_right) {
            tree_[root] += value;
            return;
        }
        size_t mid = (left + right) / 2;
        size_t child = root << 1;

        size_t min_pos = std::min(s_right, mid);
        if (s_left <= min_pos) {
            Update(child, left, mid, s_left, min_pos, value);
        }
        size_t max_pos = std::max(s_left, mid + 1);
        if (max_pos <= s_right) {
            Update(child + 1, mid + 1, right, max_pos, s_right, value);
        }
    }

public:
    explicit SegmentTree(const size_t& quantity) {
        tree_.resize(4 * quantity, kZero<T>);
    }

    void BuildTree(std::vector<T>& array) {
        Build(1, 0, array.size() - 1, array);
    }

    [[nodiscard]] T GetValue(const size_t& quantity, const size_t& position) const {
        return Get(1, 0, quantity - 1, position - 1);
    }

    void UpdateSegment(const size_t& quantity, const size_t& left, const size_t& right, const T& value) {
        Update(1, 0, quantity - 1, left - 1, right - 1, value);
    }

    [[nodiscard]] std::vector<T> GetTree() const {
        return tree_;
    }
};

//**********************************************************************************

int main() {
    //******************************************************************************

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    size_t q_soldiers = 0;
    std::cin >> q_soldiers;

    std::vector<int64_t> soldiers;
    for (size_t i = 0; i < q_soldiers; ++i) {
        int64_t soldier = 0;
        std::cin >> soldier;
        soldiers.push_back(soldier);
    }

    SegmentTree<int64_t> army(q_soldiers);
    army.BuildTree(soldiers);

    /*auto tree = army.GetTree();
    for (auto& element : tree) {
        std::cout << element << ' ';
    }*/

    //*****************************************************************************

    int q_requests = 0;
    std::cin >> q_requests;

    std::vector<int64_t> answer;
    char symbol = 0;
    for (size_t i = 0; i < q_requests; ++i) {
        std::cin >> symbol;
        if (symbol == 'g') {
            size_t position;
            std::cin >> position;

            int64_t medals = army.GetValue(q_soldiers, position);
            answer.push_back(medals);
        }
        if (symbol == 'a') {
            size_t left = 0, right = 0;
            int64_t value = 0;
            std::cin >> left >> right >> value;

            army.UpdateSegment(q_soldiers, left, right, value);
        }
    }

    //*****************************************************************************

    for (auto& element : answer) {
        std::cout << element << std::endl;
    }
    return 0;

    //******************************************************************************
}

//**********************************************************************************