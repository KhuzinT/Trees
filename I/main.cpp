//
//I didn't use smart pointers, since TL happens to them
//

#include <iostream>

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
        Node* left = nullptr;
        Node* right = nullptr;

        T size = kOne<T>;
        T value = kZero<T>;
        T result = kZero<T>;
        T priority = kZero<T>;

        Node(const T& v, const T& p) {
            value = v;
            result = v;
            priority = p;
        }

    };

    Node* root_ = nullptr;

    //*********************************************************************************

    void UpdateNode(Node* &root) {
        if (root == nullptr) {
            return;
        }

        Node* left = root->left;
        Node* right = root->right;

        if (left == nullptr && right == nullptr) {
            root->result = root->value;
            root->size = 1;
            return;
        }
        if (left == nullptr) {
            root->result = std::min(root->value, right->result);
            root->size = right->size + 1;
            return;
        }
        if (right == nullptr) {
            root->result = std::min(root->value, left->result);
            root->size = left->size + 1;
            return;
        }

        root->result = std::min(root->value, std::min(left->result, right->result));
        root->size = left->size + right->size + 1;
    }

    void Merge(Node* &answer, Node* Left, Node* Right) {
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

    void Split(const T& value, Node* root, Node* &Left, Node* &Right) {
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


    Node* Insert(const T& key, const T& priority, Node* root, const uint64_t& position) {
        Node* left = nullptr;
        Node* right = nullptr;

        Split(position, root, left, right);

        Node* object = new Node(key, priority);

        Merge(left, left, object);
        Merge(root, left, right);

        return root;
    }

    T Get(Node* &root, const uint64_t& left, const uint64_t& right) {
        if (root == nullptr) {
            return kZero<T>;
        }
        Node* RLeft = nullptr;
        Node* RRight = nullptr;

        Split(right, root, RLeft, RRight);

        Node* LLeft = nullptr;
        Node* LRight = nullptr;
        Split(left - 1, RLeft, LLeft, LRight);

        T answer = root->result;
        if (LRight != nullptr) {
            answer = LRight->result;
        }

        Merge(root, LRight, RRight);
        Merge(root, LLeft, root);

        return answer;
    }

    void Free(Node* root) {
        if (root == nullptr) {
            return;
        }
        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            return;
        }
        Free(root->left);
        Free(root->right);

        delete root;
    }

    void Print(Node* root) {
        if (root == nullptr) {
            return;
        }
        Print(root->left);
        std::cout << root->value << ' ';
        Print(root->right);
    }

    //*********************************************************************************
public:
    void Add(const T& key, const T& priority, const uint64_t& position) {
        root_ = Insert(key, priority, root_, position);
    }

    ~Treap() {
        Free(root_);
    }

    T GetMin(const uint64_t& left, const uint64_t& right) {
        return Get(root_, left, right);
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

    uint64_t q_requests = 0;
    std::cin >> q_requests;

    //*********************************************************************************

    Treap<int64_t> tree;

    char symbol = 0;
    for (uint64_t i = 0; i < q_requests; ++i) {
        const uint64_t module = 1000 * 1000 * 1000 * 1L;
        std::cin >> symbol;

        if (symbol == '+') {
            uint64_t position = 0;
            int64_t value = 0;
            std::cin >> position >> value;
            tree.Add(value, rand() % module, position);
        }
        if (symbol == '?') {
            uint64_t left = 0, right = 0;
            std::cin >> left >> right;
            std::cout << tree.GetMin(left, right) << std::endl;
        }
    }

    //*********************************************************************************
}

//*********************************************************************************