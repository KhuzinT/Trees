#include <iostream>
#include <vector>

//**********************************************************************************

template <typename T>
T kZero = 0;

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
        tree_[root] = tree_[child] + tree_[child + 1];
    }

    T Get(const size_t& root, const size_t& left, const size_t& right, const size_t& s_left, const size_t& s_right) {
        if (s_left == left && s_right == right) {
            return tree_[root];
        }
        size_t mid = (left + right) / 2;
        size_t child = root << 1;

        auto answer = kZero<T>;

        size_t min = std::min(s_right, mid);
        if (s_left <= min) {
            answer += Get(child, left, mid, s_left, min);
        }

        size_t max = std::max(s_left, mid + 1);
        if (max <= s_right) {
            answer += Get(child + 1, mid + 1, right, max, s_right);
        }

        return answer;
    }

    void Update(const size_t& root, const size_t& left, const size_t& right, const size_t& position, const T& value) {
        if (left == right) {
            tree_[root] = value;
            return;
        }
        size_t mid = (left + right) / 2;
        size_t child = root << 1;
        if (position <= mid) {
            Update(child, left, mid, position, value);
        } else {
            Update(child + 1, mid + 1, right, position, value);
        }
        tree_[root] = tree_[child] + tree_[child + 1];
    }

public:
    explicit SegmentTree(const size_t& quantity) {
        tree_.resize(4 * quantity, kZero<T>);
    }

    void BuildTree(std::vector<T>& array) {
        Build(1, 0, array.size() - 1, array);
    }

    T SumSegment(const size_t& quantity, const size_t& left, const size_t& right) {
        return Get(1, 0, quantity - 1, left - 1, right - 1);
    }

    void UpdateValue(const size_t& quantity, const size_t& position, const T& value) {
        Update(1, 0, quantity - 1, position - 1, value);
    }
};

//**********************************************************************************

int main() {
    //******************************************************************************

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

    //*****************************************************************************

    int q_requests = 0;
    std::cin >> q_requests;

    std::vector<int64_t> answer;
    char symbol = 0;
    for (size_t i = 0; i < q_requests; ++i) {
        std::cin >> symbol;
        if (symbol == 's') {
            size_t left = 0, right = 0;
            std::cin >> left >> right;

            int64_t sum = army.SumSegment(q_soldiers, left, right);
            answer.push_back(sum);
        }
        if (symbol == 'u') {
            size_t position = 0;
            int64_t value = 0;
            std::cin >> position >> value;

            army.UpdateValue(q_soldiers, position, value);
        }
    }

    //*****************************************************************************

    for (auto& element : answer) {
        std::cout << element << ' ';
    }
    return 0;

    //******************************************************************************
}

//**********************************************************************************