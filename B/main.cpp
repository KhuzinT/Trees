#include <iostream>
#include <vector>
#include <numeric>

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

    void Build(const size_t& root, const size_t& left, const size_t& right, std::vector<T>& array) {
        if (left == right) {
            tree_[root] = array[left];
            return;
        }
        size_t mid = (left + right) / 2;
        size_t child = root << 1;
        Build(child, left, mid, array);
        Build(child + 1, mid + 1, right, array);
        tree_[root] = std::gcd(tree_[child], tree_[child + 1]);
    }

    T Get(const size_t& root, const size_t& left, const size_t& right, const size_t& s_left, const size_t& s_right) const {
        if (s_left == left && s_right == right) {
            return tree_[root];
        }
        size_t mid = (left + right) / 2;
        size_t child = root << 1;

        auto answer = kOne<T>;

        size_t min_pos = std::min(s_right, mid);
        size_t max_pos = std::max(s_left, mid + 1);
        if (s_left <= min_pos && max_pos <= s_right) {
            T first_gcd = Get(child, left, mid, s_left, min_pos);
            T second_gcd = Get(child + 1, mid + 1, right, max_pos, s_right);
            answer = std::gcd<T>(first_gcd, second_gcd);
        } else if (s_left <= min_pos) {
            answer = Get(child, left, mid, s_left, min_pos);
        } else if (max_pos <= s_right) {
            answer = Get(child + 1, mid + 1, right, max_pos, s_right);
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
        tree_[root] = std::gcd(tree_[child], tree_[child + 1]);
    }

public:
    explicit SegmentTree(const size_t& quantity) {
        tree_.resize(4 * quantity, kZero<T>);
    }

    void BuildTree(std::vector<T>& array) {
        Build(1, 0, array.size() - 1, array);
    }

    T GcdSegment(const size_t& quantity, const size_t& left, const size_t& right) const {
        return Get(1, 0, quantity - 1, left - 1, right - 1);
    }

    void UpdateValue(const size_t& quantity, const size_t& position, const T& value) {
        Update(1, 0, quantity - 1, position - 1, value);
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
        if (symbol == 's') {
            size_t left = 0, right = 0;
            std::cin >> left >> right;

            int64_t sum = army.GcdSegment(q_soldiers, left, right);
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