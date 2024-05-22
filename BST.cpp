#include "BST.h"

#include <iostream>
#include <cassert>
#include <utility>
#include <cstdint>
#include <string>
#include <stack>
#include <exception>
#include <stdexcept>


BinarySearchTree::BinarySearchTree(const BinarySearchTree &other)
    // конструктор копии
    : _size(other._size)
{
    if (other._root != nullptr)
    {
        // если у других есть корень, то создаем свой
        _root = new Node(*other._root);
    }
    else
    {
        // если у других нет корня, то у нас тоже не будет
        _root = nullptr;
    }
}

BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other)
    // переопределение оператора присваивания
{
    if (this != &other) // не равен ли объекту-субъекту и объекту-свершению
    {
        // обнуляем размер
        _size = other._size;

        if (other._root == nullptr) // если у других нет корня
        {
            // у нас тоже не будет
            _root = nullptr;
            return *this;
        }

        // создаем корень
        _root = new Node(other._root->keyValuePair.first, other._root->keyValuePair.second);

        // stack для рекурсии
        std::stack<std::pair<Node *, Node *>> stack;
        stack.push({other._root, _root});

        // рекурсия
        while (!stack.empty())
        {
            auto [otherNode, currentNode] = stack.top();
            stack.pop();

            
            if (otherNode->left != nullptr) // если у других есть левый сын
            {
                // создаем левый сын
                currentNode->left = new Node(otherNode->left->keyValuePair.first, otherNode->left->keyValuePair.second);
                // указываем на родителя
                currentNode->left->parent = currentNode;
                // добавляем в стек
                stack.push({otherNode->left, currentNode->left});
            }

            if (otherNode->right != nullptr) // если у других есть правый сын
            {
                // создаем правый сын
                currentNode->right = new Node(otherNode->right->keyValuePair.first, otherNode->right->keyValuePair.second);
                // указываем на родителя
                currentNode->right->parent = currentNode;
                // добавляем в стек
                stack.push({otherNode->right, currentNode->right});
            }
        }
    }
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept
    // конструктор перемещения
    : _root(other._root), _size(other._size)
{
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree &BinarySearchTree::operator=(BinarySearchTree &&other) noexcept
    // переопределение оператора перемещения
{
    if (this != &other) // не равен ли объекту-субъекту и объекту-свершению
    {
        // уничтожаем предыдущий объект
        this->~BinarySearchTree();
        // присваиваем поля
        _size = other._size;
        _root = other._root;
        other._root = nullptr;
        other._size = 0;
    }
    return *this;
}

BinarySearchTree::~BinarySearchTree()
    // деструктор
{
    delete this->_root;
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const
    // ищет в дереве
    // возвращает итератор на найденный элемент или конец
{
    Node *current = _root;

    while (current != nullptr)
    {
        if (key > current->keyValuePair.first &&
            !current->right
                 ->flag)
        {
            // если в правом поддереве есть элемент
            current = current->right;
        }
        else if (key <
                 current->keyValuePair.first)
        {
            // если в левом поддереве есть элемент
            current = current->left;
        }
        else if (key == current->keyValuePair.first)
        {
            // если нашли элемент
            return ConstIterator(current);
        }
        else
            // если не нашли элемент
            return cend();
    }
    return ConstIterator(nullptr);
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key)
{
    Node *current = _root;

    while (current != nullptr)
    {
        if (current->right && key > current->keyValuePair.first &&
            !current->right
                 ->flag)
        {
            // если в правом поддереве есть элемент
            current = current->right;
        }
        else if (current->left &&
                 key <
                     current->keyValuePair.first)

        {
            // если в левом поддереве есть элемент
            current = current->left;
        }
        else if (key == current->keyValuePair.first)
        {
            // если нашли элемент
            return Iterator(current);
        }
        else
            // если не нашли элемент
            return end();
    }
    return end();
}


size_t BinarySearchTree::size() const
{
    // возвращает количество элементов в дереве
    return _size;
}


BinarySearchTree::ConstIterator BinarySearchTree::min() const
{
    // возвращает итератор на минимальный элемент
    Node *node = _root;
    while (node->left != nullptr)
    {
        node = node->left;
    }
    return ConstIterator(node);
}


BinarySearchTree::ConstIterator BinarySearchTree::max() const
{
    // возвращает итератор на максимальный элемент
    Node *node = _root;
    while (node && node->right && !node->right->flag)
    {
        node = node->right;
    }
    return ConstIterator(node);
}


BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const
{
    // возвращает итератор на минимальный элемент, равный или больший, чем заданный
    Node *current = _root;
    Node *res = nullptr;
    Value min;
    int f = 1;

    while (current != nullptr)
    {
        if (key > current->keyValuePair.first &&
            !current->right
                 ->flag)
        {
            // если в правом поддереве есть элемент, больший, чем key
            current = current->right;
        }
        else if (key < current->keyValuePair.first)
        {
            // если в левом поддереве есть элемент
            current = current->left;
        }
        else
        {
            // если нашли элемент, равный key
            if (current->keyValuePair.second < min || f)
            {
                min = current->keyValuePair.second; 
                res = current;                      
                f = 0;                              
            }
            current = current->right; 
        }
    }
    return ConstIterator(res);
}


BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const
{
    // возвращает итератор на максимальный элемент, равный или меньший, чем заданный
    Node *current = _root; 
    Node *res = nullptr;
    Value max;
    int f = 1; 

    while (current != nullptr) 
    {
        if (key > current->keyValuePair.first &&
            !current->right
                 ->flag) 
        {
            // если в правом поддереве есть элемент, больший, чем key
            current = current->right;
        }
        else if (key <
                 current->keyValuePair.first) 

        {
            // если в левом поддереве есть элемент
            current = current->left;
        }
        else // если ключи равны
        {
            // если нашли элемент, равный key
            if (current->keyValuePair.second > max || f) // если значение текущего меньше max или max не проиницилизировано
            {
                max = current->keyValuePair.second; 
                res = current;                      
                f = 0;                              
            }
            current = current->right; 

        }
    }
    return ConstIterator(res);
}



// возвращает итератор на начало дерева
BinarySearchTree::Iterator BinarySearchTree::begin()
{
    Node *node = _root;
    if (node == nullptr)
    {
        // если дерево пустое
        return Iterator(nullptr);
    }
    // ищем левый нижний узел
    while (node->left != nullptr)
    {
        node = node->left;
    }
    return Iterator(node);
}


// возвращает итератор на конец дерева
BinarySearchTree::Iterator BinarySearchTree::end()
{
    Node *node = _root;
    if (node == nullptr)
    {
        // если дерево пустое
        return Iterator(nullptr);
    }
    // ищем правый верхний узел
    while (node->right != nullptr)
    {
        node = node->right;
    }
    return Iterator(node);
}


// возвращает константный итератор на начало дерева
BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
    Node *node = _root;
    if (node == nullptr)
    {
        // если дерево пустое
        return ConstIterator(nullptr);
    }
    // ищем левый нижний узел
    while (node->left != nullptr)
    {
        node = node->left;
    }
    return ConstIterator(node);
}


// возвращает константный итератор на конец дерева
BinarySearchTree::ConstIterator BinarySearchTree::cend() const
{
    Node *node = _root;
    if (node == nullptr)
    {
        // если дерево пустое
        return ConstIterator(nullptr);
    }
    // ищем правый верхний узел
    while (node->right != nullptr)
    {
        node = node->right;
    }
    return ConstIterator(node);
}

// конструктор узла дерева
BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right) : parent(parent), left(left), right(right)
{
    // заполняем поля узла
    keyValuePair = std::pair<Key, Value>(key, value);
}

// копировальный конструктор узла
BinarySearchTree::Node::Node(const Node &other)
{
    // копируем поля
    keyValuePair = other.keyValuePair;
    parent = other.parent;
    left = other.left;
    right = other.right;
}


// оператор сравнения узлов (равенство)
bool BinarySearchTree::Node::operator==(const Node &other) const
{
    // если поле keyValuePair, parent, left, right имеют одинаковые значения
    if (keyValuePair == other.keyValuePair && parent == other.parent && left == other.left && right == other.right)
    {
        // то узлы равны
        return true;
    }
    return false;
}



void BinarySearchTree::Node::output_node_tree() const
{
    // выводит информацию о текущем узле
    if (parent != nullptr)
    {
        // если родитель существует, то выводим его key
        std::cout << parent->keyValuePair.first << " ";
    }
    else
    {
        // если родителя нет, то выводим '#'
        std::cout << "#";
    }
    if (left != nullptr)
    {
        // если левый дочерний узел существует, то выводим его key
        std::cout << left->keyValuePair.first << " ";
    }
    else
    {
        // если левого дочернего узла нет, то выводим '#'
        std::cout << "#";
    }
    // выводим key текущего узла
    std::cout << keyValuePair.first << std::endl;
    if (right != nullptr)
    {
        // если правый дочерний узел существует, то выводим его key
        std::cout << right->keyValuePair.second << " ";
    }
    else
    {
        // если правого дочернего узла нет, то выводим '#'
        std::cout << "#";
    }
}

void BinarySearchTree::BinarySearchTree::output_tree()
{
    // выводит всю информацию о дереве
    if (_root != nullptr)
    {
        // если корень существует, то выводим его key
        std::cout << _root->keyValuePair.first << " ";
    }
    else
    {
        // если корня нет, то выводим '#'
        std::cout << "#";
    }
    if (_root->left != nullptr)
    {
        // если левый дочерний узел существует, то выводим его key
        std::cout << _root->left->keyValuePair.first << " ";
    }
    else
    {
        // если левого дочернего узла нет, то выводим '#'
        std::cout << "#";
    }
    // выводим key корня
    std::cout << _root->keyValuePair.first << std::endl;
    if (_root->right != nullptr)
    {
        // если правый дочерний узел существует, то выводим его key
        std::cout << _root->right->keyValuePair.second << " ";
    }
    else
    {
        // если правого дочернего узла нет, то выводим '#'
        std::cout << "#";
    }
}

BinarySearchTree::Iterator::Iterator(Node *node)
{
    // инициализация итератора
    _node = node;
}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*()
{
    // возвращает доступ к текущему узлу
    return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const
{
    // возвращает доступ к текущему узлу
    return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->()
{
    // возвращает указатель на текущий узел
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const
{
    // возвращает указатель на текущий узел
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++()
{
    // переход к следующему элементу
    if (_node == nullptr)
    {
        // если итератор уже итерируем, то возвращаем его
        return Iterator(nullptr);
    }
    if (_node->right != nullptr)
    {
        // если правый дочерний узел существует, то переходим в него
        _node = _node->right;
        while (_node->left != nullptr)
        {
            // если левый дочерний узел существует, то переходим в него
            _node = _node->left;
        }
    }
    else
    {
        // если правого дочернего узла нет, то ищем предка, который является левым для нашего узла
        while (_node->parent != nullptr && _node == _node->parent->right)
        {
            // переход к предку
            _node = _node->parent;
        }
        // переход к предку, если он существует
        _node = _node->parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int)
{
    // создаем временный объект, чтобы вернуть изменившийся итератор
    BinarySearchTree::Iterator *tmp = this;
    ++(*this);
    return *tmp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--()
{
    // если итератор уже в конечном состоянии, то возвращаем его
    if (_node == nullptr)
    {
        return Iterator(nullptr);
    }
    // если левый дочерний узел существует, то переходим в него
    if (_node->left != nullptr)
    {
        _node = _node->left;
        // ищем правый верхний узел
        while (_node->right != nullptr)
        {
            _node = _node->right;
        }
    }
    // если левого дочернего узла нет, то ищем предка, который является правым для нашего узла
    else
    {
        while (_node->parent != nullptr && _node == _node->parent->left)
        {
            // переход к предку
            _node = _node->parent;
        }
        // переход к предку, если он существует
        _node = _node->parent;
    }
    // возвращаем измененный итератор
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int)
{
    // создаем временный объект, чтобы вернуть изменившийся итератор
    BinarySearchTree::Iterator *tmp = this;
    --(*this);
    return *tmp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const
{
    // сравниваем итераторы, если указатели на узлы равны, то итераторы равны
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const
{
    // итераторы не равны, если указатели на узлы не равны
    return !(*this == other);
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node)
{
    // инициализация итератора
    _node = node;
}

const std::pair<Key, Value> &BinarySearchTree::ConstIterator::operator*() const
{
    // возвращает значение, хранящееся в текущем узле
    return (*_node).keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::ConstIterator::operator->() const
{
    // возвращает указатель на текущий узел
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++()
{
    // переход к следующему элементу
    if (_node->right != nullptr)
    {
        // если правый дочерний узел существует, то переходим в него
        _node = _node->right;
        // ищем левый верхний узел
        while (_node->left != nullptr)
        {
            _node = _node->left;
        }
    }
    // если правого дочернего узла нет, то ищем предка, который является левым для нашего узла
    else
    {
        while (_node->parent != nullptr && _node == _node->parent->right)
        {
            // переход к предку
            _node = _node->parent;
        }
        // переход к предку, если он существует
        _node = _node->parent;
    }
    // возвращаем измененный итератор
    return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int)
{
    // создаем временный итератор
    BinarySearchTree::ConstIterator t(*this);
    // иходим к следующему элементу
    ++(*this);
    // возвращаем временный итератор
    return t;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--()
{
    // если левый дочерний узел существует, то переходим в него
    if (_node->left != nullptr)
    {
        _node = _node->left;
        // ищем правый верхний узел
        while (_node->right != nullptr)
        {
            _node = _node->right;
        }
    }
    // если левого дочернего узла нет, то ищем предка, который является правым для нашего узла
    else
    {
        while (_node->parent != nullptr && _node == _node->parent->left)
        {
            // переход к предку
            _node = _node->parent;
        }
        // переход к предку, если он существует
        _node = _node->parent;
    }
    // возвращаем измененный итератор
    return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int)
{
    // создаем временный итератор
    BinarySearchTree::ConstIterator p = *this;
    // идем к предыдущему элементу
    --(*this);
    // возвращаем временный итератор
    return p;
}
bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const
{
    // сравниваем два итератора
    return _node == other._node;
}
bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const
{
    // сравниваем два итератора
    return !(*this == other);
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key)
{
    // находим диапазон значений, равных заданному ключу
    Iterator current = begin();
    int flag = 1;
    Iterator resBegin(nullptr);
    Iterator resEnd(nullptr);

    while (current != end())
    {
        // если текущий элемент равен заданному ключу, то
        if ((*current).first == key)
        {
            // если флаг равен 1, то сохраняем начало диапазона
            if (flag)
            {
                resBegin = current;
                flag = 0;
            }
            // идем на следующий элемент
            ++current;
            // если следующий элемент больше заданного ключа или
            // если мы достигли конца списка, то сохраняем конец диапазона
            if ((*current).first > key || current == end())
            {
                resEnd = current;
                break;
            }
        }
        // идем на следующий элемент
        ++current;
    }
    // возвращаем диапазон
    return std::pair<Iterator, Iterator>(resBegin, resEnd);
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const
{
    // находим диапазон значений, равных заданному ключу
    ConstIterator current = cbegin();
    int flag = 1;
    ConstIterator resBegin(nullptr);
    ConstIterator resEnd(nullptr);

    while (current != cend())
    {
        // если текущий элемент равен заданному ключу, то
        if ((*current).first == key)
        {
            // если флаг равен 1, то сохраняем начало диапазона
            if (flag)
            {
                resBegin = current;
                flag = 0;
            }
            // идем на следующий элемент
            ++current;
            // если следующий элемент больше заданного ключа или
            // если мы достигли конца списка, то сохраняем конец диапазона
            if ((*current).first > key || current == cend())
            {
                resEnd = current;
                break;
            }
        }
        // идем на следующий элемент
        ++current;
    }
    // возвращаем диапазон
    return std::pair<ConstIterator, ConstIterator>(resBegin, resEnd);
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value)
{
    // добавляем новый узел
    if (key < keyValuePair.first) 
    {
        // если левый дочерний узел существует, то рекурсивно добавляем в него
        if (left) 
        {
            left->insert(key, value);
        }
        // если левого дочернего узла нет, то создаем новый узел
        else
        {
            Node *node = new Node(key, value, this, nullptr, nullptr);
            this->left = node;
        }
    }
    // если новый ключ больше или равен ключу текущего узла, то рекурсивно
    // добавляем в правый дочерний узел
    if (key >= keyValuePair.first) 
    {
        if (right) 
        {
            right->insert(key, value);
        }
        else 
        {
            Node *node = new Node(key, value, this, nullptr, nullptr);
            this->right = node;
        }
    }
}

void BinarySearchTree::insert(const Key &key, const Value &value)
{
    // если дерево не пустое, то рекурсивно добавляем узел
    if (_root)
    {
        delete_nill();
        _root->insert(key, value);
    }
    // если дерево пустое, то создаем новый корневой узел
    else
    {
        _root = new Node(key, value);
    }
    insert_nill();
    // увеличиваем количество элементов в дереве
    _size++;
}


/**
 * @brief Элимирует узел с заданным ключом
 * @param node указатель на узел
 * @param key ключ, который надо найти
 * @return указатель на узел, если был найден
 */
BinarySearchTree::Node *BinarySearchTree::Node::erase_node(Node *node,
                                                           const Key &key)
{
    // если узел не существует, то возвращаем nullptr
    if (node == nullptr)
        return node;

    // если key меньше ключа текущего узла, то рекурсивно
    // ищем в левом поддереве
    if (key < node->keyValuePair.first) 
    {
        node->left = erase_node(node->left, key); 
        return node;
    }
    // если key больше ключа текущего узла, то рекурсивно
    // ищем в правом поддереве
    else if (key > node->keyValuePair.first) 
    {
        node->right = erase_node(node->right, key); 
        return node;
    }

    // если key равен ключу текущего узла, то
    // ищем заменителя
    if (node->left == nullptr)
    {
        // если правого дочернего узла нет, то
        // возвращаем текущий узел
        Node *temp = node->right;
        delete node;
        return temp;
    }
    else if (node->right == nullptr)
    {
        // если левого дочернего узла нет, то
        // возвращаем текущий узел
        Node *temp = node->left;
        delete node;
        return temp;
    }


    // найденный заменитель
    Node *temp = node->right; 
    Node *parent = node;      

    // ищем заменителя в правом поддереве
    while (temp->left != nullptr)
    {
        parent = temp;
        temp = temp->left; 
    }

    // заменяем key и value в текущем узле
    node->keyValuePair = temp->keyValuePair;

    // изменяем указатель на заменителя
    if (parent->left == temp)
        parent->left = temp->right;
    else
        parent->right = temp->right;

    // уничтожаем заменителя
    delete temp;
    return node;
}

/**
 * @brief Удаляет элемент с заданным ключом
 * @param key ключ, который надо удалить
 */
void BinarySearchTree::erase(const Key &key)
{
    // если дерево не пустое, то
    if (_root)
    {
        // вызываем рекурсивную функцию
        delete_nill();
        _root = _root->erase_node(_root, key);
        _size--;
        // вызываем функцию, которая восстанавливает nil
        insert_nill();
    }

    // ищем элемент
    Iterator f = find(key);
    // если элемент найден, то вызываем erase
    if (f != end())
    {
        erase(key);
    }
}

/**
 * @brief Удаляет nil
 */
void BinarySearchTree::delete_nill()
{
    Node *current = _root;

    // если nil не существует, то возвращаемся
    if (!current)
        return;

    // находим nil
    while (current->right)
    {
        current = current->right;
    }

    // если nil не является корневым узлом
    if (current != nullptr && !current->flag)
    {
        // ищем его родителя
        Node *parent = current->parent;

        // если nil является левым дочерним узлом
        if (parent->left == current)
            parent->left = nullptr;
        // если nil является правым дочерним узлом
        else
            parent->right = nullptr;

        // уничтожаем nil
        delete current;
    }
}

/**
 * @brief Восстанавливает nil
 */
void BinarySearchTree::insert_nill()
{
    // если дерево не пустое, то
    if (_root)
    {
        // создаем nil
        Node *nil = new Node(0, 0);
        nil->flag = true;

        Node *current = _root;

        // находим nil
        while (current->right)
        {
            current = current->right;
        }

        // если nil не является правым дочерним узлом,
        // то создаем его
        if (!current->flag)
        {
            current->right = nil;
            nil->parent = current;
        }
    }
}
