// x = 0 (mod 3)
// y = 1 (mod 3)
// z = 2 (mod 3)

#include <iostream>
#include <vector>
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

    T SizeUpdate(std::shared_ptr<Node> root) {
        if (root == nullptr) {
            return kZero<T>;
        }
        return root->size;
    }

    void UpdateNode(std::shared_ptr<Node> &root) {
        if (root == nullptr) {
            return;
        }
        root->size = 1 + SizeUpdate(root->left) + SizeUpdate(root->right);
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


    std::shared_ptr<Node> Insert(const T& key, const T& priority, std::shared_ptr<Node> root, const uint64_t& position) {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;

        Split(position, root, left, right);

        //std::shared_ptr<Node> object = new Node(key, priority);
        auto object = std::make_shared<Node>(key, priority);

        Merge(left, left, object);
        Merge(root, left, right);

        return root;
    }

    void Begin(std::shared_ptr<Node> root, const uint64_t& old, const uint64_t& fresh) {
        std::shared_ptr<Node> Left = nullptr;
        std::shared_ptr<Node> Right = nullptr;

        Split(old, root, Left, Right);

        std::shared_ptr<Node> tmp = nullptr;
        Split(old - 1, Left, Left, tmp);

        Merge(root, Left, Right);

        Split(fresh - 1, root, Left, Right);

        Merge(Right, tmp, Right);
        Merge(root, Left, Right);
    }

    T FindIn(std::shared_ptr<Node> root, uint64_t& position) {

        std::shared_ptr<Node> Left = nullptr;
        std::shared_ptr<Node> Right = nullptr;

        Split(position, root, Left, Right);

        std::shared_ptr<Node> tmp = nullptr;
        Split(position - 1, Left, Left, tmp);

        T answer = tmp->value;

        Merge(Left, Left, tmp);
        Merge(root, Left, Right);

        return answer;
    }

    void Print(std::shared_ptr<Node> root) {
        if (root == nullptr) {
            return;
        }
        Print(root->left);
        if (root->value % 3 == 0) {
            std::cout << "x";
        }
        if (root->value % 3 == 1) {
            std::cout << "y";
        }
        if (root->value % 3 == 2) {
            std::cout << "z";
        }
        Print(root->right);
    }

    //*********************************************************************************
public:
    void Add(const T& key, const uint64_t& position) {
        const int64_t module = 1000 * 1000 * 1000 * 1L;
        T priority = rand() % module;
        root_ = Insert(key, priority, root_, position);
    }

    void ToBegin(const uint64_t& old, const uint64_t& fresh) {
        Begin(root_, old, fresh);
    }

    T Find(uint64_t& position) {
        return FindIn(root_, position);
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

    uint64_t q_requests = 0;
    std::cin >> q_requests;

    //*********************************************************************************

    Treap<int64_t> tree;

    char symbol = 0;

    uint64_t x_plus = 0;
    uint64_t y_plus = 0;
    uint64_t z_plus = 0;
    for (uint64_t i = 0; i < quantity; ++i) {
        std::cin >> symbol;
        if (symbol == 'x') {
            tree.Add(x_plus, i);
            x_plus += 3;
        }
        if (symbol == 'y') {
            tree.Add(1 + y_plus, i);
            y_plus += 3;
        }
        if (symbol == 'z') {
            tree.Add(2 + z_plus, i);
            z_plus += 3;
        }

    }

    //tree.PrintTree();

    //*********************************************************************************

    std::vector<int64_t> answer;
    for (uint64_t i = 0; i < q_requests; ++i) {
        std::cin >> symbol;

        if (symbol == 'a') {
            uint64_t old = 0;
            uint64_t fresh = 0;

            std::cin >> old >> fresh;
            tree.ToBegin(old, fresh);

            //tree.PrintTree();
        }
        if (symbol == 'q') {
            uint64_t position = 0;
            std::cin >> position;

            auto value = tree.Find(position);
            answer.push_back(value);
        }
    }

    //*********************************************************************************

    for (auto &elem : answer) {
        if (elem % 3 == 0) {
            std::cout << "x" << std::endl;
        }
        if (elem % 3 == 1) {
            std::cout << "y" << std::endl;
        }
        if (elem % 3 == 2) {
            std::cout << "z" << std::endl;
        }
    }

    //*********************************************************************************
}

//*********************************************************************************

