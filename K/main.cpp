#include <iostream>
#include <memory>
#include <vector>

//*********************************************************************************

template <typename T>

T kZero = 0;

template <typename T>

T kOne = 1;

//*********************************************************************************

template <typename T>
class Treap {
public:
    //*********************************************************************************

    struct Node {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;

        T size = kOne<T>;
        T value = kZero<T>;
        T result = kZero<T>;
        T priority = kZero<T>;

        Node(const T& v, const T& p) {
            value = v;
            result = v;
            priority = p;

            left = nullptr;
            right = nullptr;
        }

    };

    std::shared_ptr<Node> odd_ = nullptr;
    std::shared_ptr<Node> even_ = nullptr;

    //*********************************************************************************

    T SizeNode(std::shared_ptr<Node> root) {
        if (root == nullptr) {
            return kZero<T>;
        }
        return root->size;
    }

    T ResultNode(std::shared_ptr<Node> root) {
        if (root == nullptr) {
            return kZero<T>;
        }
        return root->result;
    }

    void UpdateNode(std::shared_ptr<Node> root) {
        if (root == nullptr) {
            return;
        }

        std::shared_ptr<Node> left = root->left;
        std::shared_ptr<Node> right = root->right;

        root->result = root->value + ResultNode(left) + ResultNode(right);
        root->size = 1 + SizeNode(left) + SizeNode(right);
    }

    void Merge(std::shared_ptr<Node> &answer, std::shared_ptr<Node> Left, std::shared_ptr<Node> Right) {
        if (Left == nullptr) {
            answer = Right;
            return;
        }
        if (Right == nullptr) {
            answer = Left;
            return;
        }

        if (Left->priority > Right->priority) {
            Merge(Left->right, Left->right, Right);
            answer = Left;
            UpdateNode(answer);
            return;
        }

        Merge(Right->left, Left, Right->left);
        answer = Right;
        UpdateNode(answer);
    }

    void Split(const T& value, std::shared_ptr<Node> root, std::shared_ptr<Node> &Left, std::shared_ptr<Node> &Right) {
        if (root == nullptr) {
            Left = nullptr;
            Right = nullptr;
            return;
        }

        T key = kZero<T>;

        T size = kZero<T>;
        if (root->left) {
            size += root->left->size;
        }
        key += size;

        if (value <= key) {
            Split(value, root->left, Left, root->left);
            Right = root;
            UpdateNode(root);
            return;
        }

        Split(value - size - 1, root->right, root->right, Right);
        Left = root;
        UpdateNode(root);
    }

    std::shared_ptr<Node> Insert(const T& key, const T& priority, std::shared_ptr<Node> &root) {
        //std::shared_ptr<Node> object = new Node(key, priority);
        auto object = std::make_shared<Node>(key, priority);

        Merge(root, root, object);
        return root;
    }

    void Reverse(std::shared_ptr<Node> &odd, std::shared_ptr<Node> &even, const int64_t& left, const int64_t& right) {
        auto a = (left + 1) / 2;
        auto b = (right + 1) / 2 - 1;

        std::shared_ptr<Node> oddL = nullptr;
        std::shared_ptr<Node> oddR = nullptr;
        Split(a, odd, oddL, oddR);

        std::shared_ptr<Node> tmp1 = nullptr;
        Split((right / 2) - a + 1, oddR, oddR, tmp1);

        std::shared_ptr<Node> evenL = nullptr;
        std::shared_ptr<Node> evenR = nullptr;
        Split((left / 2), even, evenL, evenR);

        std::shared_ptr<Node> tmp2 = nullptr;
        Split(b - (left / 2) + 1, evenR, evenR, tmp2);

        Merge(evenR, oddL, evenR);
        Merge(odd, evenR, tmp1);
        Merge(oddR, evenL, oddR);
        Merge(even, oddR, tmp2);
    }

    T Get(std::shared_ptr<Node> odd, std::shared_ptr<Node> even, const int64_t& left, const int64_t& right) {
        if (left > right) {
            return kZero<T>;
        }
        auto a = (left + 1) / 2;
        auto b = (right + 1) / 2 - 1;

        std::shared_ptr<Node> oddL = nullptr;
        std::shared_ptr<Node> oddR = nullptr;
        Split(a, odd, oddL, oddR);

        std::shared_ptr<Node> tmp1 = nullptr;
        Split((right / 2) - a + 1, oddR, oddR, tmp1);

        std::shared_ptr<Node> evenL = nullptr;
        std::shared_ptr<Node> evenR = nullptr;
        Split(left / 2, even, evenL, evenR);

        std::shared_ptr<Node> tmp2 = nullptr;
        Split(b - (left / 2) + 1, evenR, evenR, tmp2);

        T answer = kZero<T>;
        if (oddR != nullptr) {
            answer += oddR->result;
        }
        if (evenR != nullptr) {
            answer += evenR->result;
        }

        Merge(oddR, oddL, oddR);
        Merge(odd, oddR, tmp1);
        Merge(evenR, evenL, evenR);
        Merge(even, evenR, tmp2);

        return answer;

    }

    //*********************************************************************************
public:
    void Add(const T& key, const int64_t& position) {
        const int64_t module = 1000 * 1000 * 1000 * 1L;
        const T& priority = rand() % module;
        if (position % 2 == 1) {
            odd_ = Insert(key, priority, odd_);
        }
        if (position % 2 == 0) {
            even_ = Insert(key, priority, even_);
        }
    }

    T GetSum(const int64_t& left, const int64_t& right) {
        return Get(odd_, even_, left - 1, right - 1);
    }

    void ReverseSegment(const int64_t& left, const int64_t& right) {
        Reverse(odd_, even_, left - 1, right - 1);
    }

    //*********************************************************************************
};

//*********************************************************************************

int main() {
    //*********************************************************************************

    srand(time(nullptr));
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int64_t quantity = 0;
    int64_t q_requests = 0;

    int64_t swapper_index = 1;

    do {
        Treap<int64_t> tree;
        std::cin >> quantity >> q_requests;
        if (quantity == 0 && q_requests == 0) {
            break;
        }
        for (int64_t i = 1; i <= quantity; ++i) {
            int64_t value = 0;
            std::cin >> value;
            tree.Add(value, i);
        }

        char symbol = 0;
        std::vector<int64_t> answer;
        for (int64_t i = 0; i < q_requests; ++i) {
            std::cin >> symbol;

            if (symbol == '1') {
                int64_t left = 0, right = 0;
                std::cin >> left >> right;
                tree.ReverseSegment(left, right);
            }
            if (symbol == '2') {
                int64_t left = 0, right = 0;
                std::cin >> left >> right;
                answer.push_back(tree.GetSum(left, right));
            }
        }

        std::cout << "Swapper " << swapper_index << ":" << std::endl;
        for (auto &element : answer) {
            std::cout << element << std::endl;
        }
        answer.clear();

        ++swapper_index;

    } while (quantity > 0 && q_requests > 0);

    //*********************************************************************************
}

//*********************************************************************************
