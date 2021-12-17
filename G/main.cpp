#include <iostream>
#include <memory>

//*********************************************************************************

template <typename T>

T kZero = 0;

template <typename T>

T kOne = 1;

//*********************************************************************************

template <typename T>
class Treap {
private:
    //*********************************************************************************

    struct Node {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;

        T size = kOne<T>;
        T value = kZero<T>;
        T priority = kZero<T>;

        Node(const T& v, const T& p) {
            value = v;
            priority = p;
        }

    };

    std::shared_ptr<Node> root_ = nullptr;

    //*********************************************************************************

    T SizeNode(std::shared_ptr<Node> root) {
        if (root == nullptr) {
            return kZero<T>;
        }
        return root->size;
    }

    void UpdateNode(std::shared_ptr<Node> &root) {
        if (root == nullptr) {
            return;
        }

        root->size = 1 + SizeNode(root->left) + SizeNode(root->right);
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


    std::shared_ptr<Node> Insert(const T& key, const T& priority, std::shared_ptr<Node> root) {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;

        Split(key, root, left, right);

        //std::shared_ptr<Node> object = new Node(key + 1, priority);
        auto object = std::make_shared<Node>(key + 1, priority);

        Merge(left, left, object);
        Merge(root, left, right);

        return root;
    }

    void Begin(std::shared_ptr<Node> root, const uint64_t& left, const uint64_t& right) {
        std::shared_ptr<Node> Left = nullptr;
        std::shared_ptr<Node> Right = nullptr;

        Split(right, root, Left, Right);

        std::shared_ptr<Node> tmp = nullptr;
        Split(left - 1, Left, tmp, Left);

        Merge(root, Left, tmp);
        Merge(root, root, Right);
    }

    void Print(std::shared_ptr<Node> root) {
        if (root == nullptr) {
            return;
        }
        Print(root->left);
        std::cout << root->value << ' ';
        Print(root->right);
    }

    //*********************************************************************************
public:
    void Add(const T& key) {
        const int64_t module = 1000 * 1000 * 1000 * 1L;
        T priority = rand() % module;
        root_ = Insert(key, priority, root_);
    }

    void ToBegin(const uint64_t& left, const uint64_t& right) {
        Begin(root_, left, right);
    }

    void PrintTree() {
        Print(root_);
        std::cout << std::endl;
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

    //*********************************************************************************

    Treap<int64_t> tree;

    for (uint64_t i = 0; i < quantity; ++i) {
        tree.Add(i);
    }

    //tree.PrintTree();

    //*********************************************************************************

    uint64_t q_requests = 0;
    std::cin >> q_requests;

    for (uint64_t i = 0; i < q_requests; ++i) {
        uint64_t left = 0, right = 0;
        std::cin >> left >> right;

        tree.ToBegin(left, right);
        //tree.PrintTree();
    }

    tree.PrintTree();

    //*********************************************************************************
}

//*********************************************************************************
