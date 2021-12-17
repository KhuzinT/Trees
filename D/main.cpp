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

    class Node {
    public:
        T elem;
        T promise;
    };

    std::vector<Node> tree_;

    void Push(const size_t& root) {
        size_t child = root << 1;
        if (child < tree_.size()) {
            tree_[child].promise += tree_[root].promise;
        }
        if (child + 1 < tree_.size()) {
            tree_[child + 1].promise += tree_[root].promise;
        }
        tree_[root].elem += tree_[root].promise;
        tree_[root].promise = kZero<T>;
    }

    void Build(const size_t& root, const size_t& left, const size_t& right, std::vector<T>& array) {
        if (left == right) {
            tree_[root].elem = array[left];
            tree_[root].promise = kZero<T>;
            return;
        }

        size_t mid = (left + right) / 2;
        size_t child = root << 1;
        Build(child, left, mid, array);
        Build(child + 1, mid + 1, right, array);
        tree_[root].elem = std::max(tree_[child].elem, tree_[child + 1].elem);
        tree_[root].promise = kZero<T>;
    }

    T Get(const size_t& root, const size_t& left, const size_t& right, const size_t& s_left, const size_t& s_right) {
        if (right < s_left || s_right < left) {
            return kZero<T>;
        }
        if (s_left <= left && right <= s_right) {
            Push(root);
            return tree_[root].elem;
        }
        Push(root);

        size_t mid = (left + right) / 2;
        size_t child = root << 1;

        size_t min_pos = std::min(s_right, mid);
        size_t max_pos = std::max(s_left, mid + 1);

        if (s_left <= min_pos && max_pos <= s_right) {
            T first_gcd = Get(child, left, mid, s_left, min_pos);
            T second_gcd = Get(child + 1, mid + 1, right, max_pos, s_right);
            return std::max(first_gcd, second_gcd);
        }
        if (s_left <= min_pos) {
            return Get(child, left, mid, s_left, min_pos);
        }
        if (max_pos <= s_right) {
            return Get(child + 1, mid + 1, right, max_pos, s_right);
        }

        return kZero<T>;
    }


    void Update(const size_t& root, const size_t& left, const size_t& right, const size_t& s_left, const size_t& s_right, const T& value) {
        if (s_left <= left && right <= s_right) {
            tree_[root].promise += value;
            return;
        }
        Push(root);

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

        T first_elem = tree_[child].elem + tree_[child].promise;
        T second_elem = tree_[child + 1].elem + tree_[child + 1].promise;
        tree_[root].elem = std::max(first_elem, second_elem);
    }

public:
    explicit SegmentTree(const size_t& quantity) {
        tree_.resize(4 * quantity, {kZero<T>, kZero<T>});
    }

    void BuildTree(std::vector<T>& array) {
        Build(1, 0, array.size() - 1, array);
    }

    [[nodiscard]] T Medals(const size_t& quantity, const size_t& left, const size_t& right) {
        return Get(1, 0, quantity - 1, left - 1, right - 1);
    }

    void UpdateSegment(const size_t& quantity, const size_t& left, const size_t& right, const T& value) {
        Update(1, 0, quantity - 1, left - 1, right - 1, value);
    }

    [[nodiscard]] std::vector<T> GetTreeElem() const {
        std::vector<T> values;
        for (size_t i = 0; i < tree_.size(); ++i) {
            values.push_back(tree_[i].elem);
        }
        return values;
    }

    void PrintElem() {
        for (size_t i = 0; i < tree_.size(); ++i) {
            std::cout << tree_[i].elem << ' ';
        }
        std::cout << std::endl;
    }

    void PrintPromise() {
        for (size_t i = 0; i < tree_.size(); ++i) {
            std::cout << tree_[i].promise << ' ';
        }
        std::cout << std::endl;
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

    //*****************************************************************************

    int q_requests = 0;
    std::cin >> q_requests;

    std::vector<int64_t> answer;
    char symbol = 0;
    for (size_t i = 0; i < q_requests; ++i) {
        std::cin >> symbol;
        if (symbol == 'm') {
            size_t left = 0, right = 0;
            std::cin >> left >> right;

            int64_t medals = army.Medals(q_soldiers, left, right);
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