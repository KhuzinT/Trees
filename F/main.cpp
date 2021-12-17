#include <iostream>
#include <vector>
#include <memory>

//*********************************************************************************

template <typename T>

T kZero = 0;

//*********************************************************************************

template <typename T>
class Treap {
private:
    //*********************************************************************************

    struct Node {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;

        T key = kZero<T>;
        T priority = kZero<T>;
        T result = kZero<T>;

        Node(const T& k, const T& p) {
            key = k;
            priority = p;
            result = k;
        }
    };

    std::shared_ptr<Node> root_ = nullptr;

    //*********************************************************************************

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

        root->result = root->key + ResultNode(left) + ResultNode(right);
    }

    void Split(const T& key, std::shared_ptr<Node> &root, std::shared_ptr<Node> &Left, std::shared_ptr<Node> &Right) {
        if (root == nullptr) {
            return;
        }
        std::shared_ptr<Node> object = nullptr;

        if (key <= root->key) {
            if (root->left) {
                Split(key, root->left, Left, object);
            } else {
                Left = nullptr;
            }

            Right = root;

            Right->left = object;
            Right->right = root->right;
        }

        if (key > root->key) {
            if (root->right) {
                Split(key, root->right, object, Right);
            } else {
                Right = nullptr;
            }

            Left = root;

            Left->left = root->left;
            Left->right = object;
        }

        UpdateNode(root);
        UpdateNode(Left);
        UpdateNode(Right);
    }

    std::shared_ptr<Node> Merge(std::shared_ptr<Node> Left, std::shared_ptr<Node> Right) {
        if (Left == nullptr) {
            return Right;
        }
        if (Right == nullptr) {
            return Left;
        }

        std::shared_ptr<Node> answer = nullptr;
        std::shared_ptr<Node> new_tree = nullptr;

        if (Left->priority > Right->priority) {
            new_tree = Merge(Left->right, Right);
            Left->right = new_tree;
            answer = Left;
            UpdateNode(answer);
            return answer;
        }

        new_tree = Merge(Left, Right->left);
        Right->left = new_tree;
        answer = Right;
        UpdateNode(answer);
        return answer;
    }


    std::shared_ptr<Node> Find(const T& key, std::shared_ptr<Node> root) {
        if (root == nullptr) {
            return root;
        }
        if (root->key == key) {
            return root;
        }

        if (key < root->key) {
            return Find(key, root->left);
        } else {
            return Find(key, root->right);
        }
    }

    std::shared_ptr<Node> Insert(const T& key, const T& priority, std::shared_ptr<Node> root) {
        std::shared_ptr<Node> in = Find(key, root);
        if (in) {
            return root;
        }

        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        Split(key, root, left, right);

        //std::shared_ptr<Node> object = new Node(key, priority);
        auto object = std::make_shared<Node>(key, priority);
        return Merge(Merge(left, object), right);
    }

    T Get(std::shared_ptr<Node> root, const size_t& left, const size_t& right) {

        std::shared_ptr<Node> LLeft = nullptr;
        std::shared_ptr<Node> LRight = nullptr;
        Split(left, root, LLeft, LRight);


        std::shared_ptr<Node> RLeft = nullptr;
        std::shared_ptr<Node> RRight = nullptr;
        Split(right, LRight, RLeft, RRight);

        T answer = kZero<T>;
        if (RLeft != nullptr) {
            answer = RLeft->result;
        }

        LRight = Merge(RLeft, RRight);
        root = Merge(LLeft, LRight);

        return answer;
    }

    //*********************************************************************************
public:
    void Add(const T& key) {
        const int64_t module = 1000 * 1000 * 1000 * 1LL;
        T priority = rand() % module;
        root_ = Insert(key, priority, root_);
    }

    T GetSum(const size_t& left, const size_t& right) {
        return Get(root_, left, right + 1);
    }

    //*********************************************************************************
};

//*********************************************************************************

int main() {
    //*********************************************************************************

    srand(time(nullptr));
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t quantity = 0;
    std::cin >> quantity;

    bool is_previous = false;
    int64_t previous = 0;

    Treap<int64_t> tree;

    //*********************************************************************************

    std::vector<int64_t> answer;
    for (uint64_t i = 0; i < quantity; ++i) {
        char symbol;
        std::cin >> symbol;

        if (symbol == '+') {
            int64_t key = 0;
            std::cin >> key;

            const int64_t module = 1000 * 1000 * 1000 * 1LL;
            if (is_previous) {
                tree.Add((key + previous) % module);
                is_previous = false;
            } else {
                tree.Add(key);
            }
            continue;
        }

        if (symbol == '?') {
            uint64_t left = 0, right = 0;
            std::cin >> left >> right;

            previous = tree.GetSum(left, right);
            answer.push_back(previous);
            is_previous = true;
            continue;
        }
    }

    //*********************************************************************************

    for (auto& element : answer) {
        std::cout << element << std::endl;
    }

    return 0;

    //*********************************************************************************
}

//*********************************************************************************