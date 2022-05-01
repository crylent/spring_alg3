#pragma once

#include <functional>
#include <stdexcept>

#define BLACK false
#define RED true

template<class K, class V>
class TreeMap
{
private:
	struct Node {
		K key;
		V val;
		Node* left = nullptr;
		Node* right = nullptr;
		Node* parent = nullptr;
		bool color = BLACK;
	};

	Node* nil = new Node;
	Node* root = nil;
	size_t size = 0;

	void left_rotate(Node* x) {
		Node* y = x->right; // y is right subtree
		Node* p = x->parent;
		x->right = y->left; // node x becames parent for left subtree of y
		if (y->left != nil) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x == root) { // node x is root
			root = y;
		}
		else if (x == p->left) { // node x is left child
			p->left = y;
		}
		else { // node x is right child
			p->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	void right_rotate(Node* y) {
		Node* x = y->left; // x is left subtree
		Node* p = y->parent;
		y->left = x->right; // node y becames parent for right subtree of x
		if (x->right != nil) {
			x->right->parent = y;
		}
		x->parent = y->parent;
		if (y == root) { // node y is root
			root = x;
		}
		else if (y == p->right) { // node y is right child
			p->right = x;
		}
		else { // node y is left child
			p->left = x;
		}
		x->right = y;
		y->parent = x;
	}

	void move_node(Node* node, Node* new_place) {
		if (new_place->parent == nullptr) { // new_place is root
			root = node;
		}
		else if (new_place->parent->left == new_place) { // new_place is left child
			new_place->parent->left = node;
		}
		else { // new_place is right child
			new_place->parent->right = node;
		}
		node->parent = new_place->parent;
	}

	Node* find_node(K key) {
		Node* node = root;
		while (key != node->key) {
			if (key < node->key) {
				node = node->left;
			}
			else {
				node = node->right;
			}
			if (node == nil) { // no such key in the tree
				throw std::runtime_error("Key does not exists");
			}
		}
		return node;
	}

	void clear_recursive(Node* node) { // clear node and call function for its children
		if (node == nil) {
			return;
		}
		clear_recursive(node->left);
		clear_recursive(node->right);
		delete node;
	}

	void make_list_of_keys(Node* node, K* elems) { // make list of keys using breadth first search
		static int length = 0;
		if (node == nil) {
			return;
		}
		make_list_of_keys(node->left, elems);
		elems[length] = node->key;
		length++;
		make_list_of_keys(node->right, elems);
		if (length == size) {
			length = 0;
		}
	}

	void make_list_of_values(Node* node, V* elems) { // make list of values using breadth first search
		static int length = 0;
		if (node == nil) {
			return;
		}
		make_list_of_values(node->left, elems);
		elems[length] = node->val;
		length++;
		make_list_of_values(node->right, elems);
		if (length == size) {
			length = 0;
		}
	}

public:
	void insert(K key, V val) {
		Node* newNode = new Node;
		newNode->key = key;
		newNode->val = val;
		// INSERT
		if (root == nil) { // tree is empty
			root = newNode;
			root->color = BLACK;
			root->left = nil;
			root->right = nil;
			size++;
			return;
		}
		Node* node = root;
		while (true) {
			if (key == node->key) {
				throw std::runtime_error("Key already exists");
			}
			if (key > node->key) {
				if (node->right != nil) {
					node = node->right;
				}
				else {
					node->right = newNode;
					break;
				}
			}
			else {
				if (node->left != nil) {
					node = node->left;
				}
				else {
					node->left = newNode;
					break;
				}
			}
		}
		newNode->parent = node;
		newNode->left = nil;
		newNode->right = nil;
		newNode->color = RED;
		size++;

		// RECOVERY
		Node* p = newNode->parent; // parent
		Node* gp = p->parent; // grand parent
		while (p->color == RED) {
			p = newNode->parent;
			gp = p->parent;
			if (p == gp->left) {
				if (gp->right->color == RED) { // case I
					gp->left->color = BLACK;
					gp->right->color = BLACK;
					gp->color = RED;
					newNode = gp;
				}
				else {
					if (p->color == RED && newNode == p->right) { // case II
						newNode = p;
						left_rotate(newNode);
					}
					if (p->color == RED) { // case III
						p->color = BLACK;
						gp->color = RED;
						right_rotate(gp);
					}
				}
			}
			else {
				if (gp->left->color == RED) { // case I
					gp->left->color = BLACK;
					gp->right->color = BLACK;
					gp->color = RED;
					newNode = gp;
				}
				else {
					if (p->color == RED && newNode == p->left) { // case II
						newNode = p;
						right_rotate(newNode);
					}
					if (p->color == RED) { // case III
						p->color = BLACK;
						gp->color = RED;
						left_rotate(gp);
					}
				}
			}
		}
		root->color = BLACK;
	}

	V find(K key) {
		return find_node(key)->val;
	}

	K find(V value) { // reverse find (key by value)
		K* keys = get_keys();
		V* values = get_values();
		for (int i = 0; i < size; i++) {
			if (value == values[i]) return keys[i];
		}
		throw std::runtime_error("Value does not exists");
	}

	void remove(K key) {
		Node* nodeToDelete = find_node(key);
		// REMOVE
		Node* x;
		bool orig_color = nodeToDelete->color;
		if (nodeToDelete->left == nil) {
			x = nodeToDelete->right;
			move_node(x, nodeToDelete);
		}
		else if (nodeToDelete->right == nil) {
			x = nodeToDelete->left;
			move_node(x, nodeToDelete);
		}
		else {
			Node* y = nodeToDelete->right;
			while (y->left != nil) {
				y = y->left;
			}
			orig_color = y->color;
			x = y->right;
			if (y->parent == nodeToDelete) {
				x->parent = y;
			}
			else {
				move_node(x, y);
				y->right = nodeToDelete->right;
				y->right->parent = y;
			}
			move_node(y, nodeToDelete);
			y->left = nodeToDelete->left;
			y->left->parent = y;
			y->color = nodeToDelete->color;
		}
		delete nodeToDelete;
		size--;

		if (orig_color != BLACK) {
			return;
		}
		// RECOVERY
		Node* w;
		while (x != root && x->color == BLACK) {
			if (x->parent->left == x) { // x is left child
				w = x->parent->right;
				if (w->color == RED) { // case I
					w->color = BLACK;
					x->parent->color = RED;
					left_rotate(x->parent);
					w = x->parent->right;
				}
				if (w->left->color == BLACK && w->right->color == BLACK) { // case II
					w->color = RED;
					x = x->parent;
				}
				else {
					if (w->right->color == BLACK) { // case III
						w->left->color = BLACK;
						w->color = RED;
						right_rotate(w);
						w = x->parent->right;
					}
					w->color = x->parent->color; // case IV
					x->parent->color = BLACK;
					w->right->color = BLACK;
					left_rotate(x->parent);
					x = root;
				}
			}
			else { // x is right child
				w = x->parent->left;
				if (w->color == RED) { // case I
					w->color = BLACK;
					x->parent->color = RED;
					right_rotate(x->parent);
					w = x->parent->left;
				}
				if (w->left->color == BLACK && w->right->color == BLACK) { // case II
					w->color = RED;
					x = x->parent;
				}
				else {
					if (w->left->color == BLACK) { // case III
						w->right->color = BLACK;
						w->color = RED;
						left_rotate(w);
						w = x->parent->left;
					}
					w->color = x->parent->color; // case IV
					x->parent->color = BLACK;
					w->left->color = BLACK;
					right_rotate(x->parent);
					x = root;
				}
			}
			x->color = BLACK;
		}
	}

	void clear() {
		clear_recursive(root);
		root = nil;
		size = 0;
	}

	size_t get_size() {
		return size;
	}

	K* get_keys() {
		K* keys = new K[size];
		make_list_of_keys(root, keys);
		return keys;
	}

	V* get_values() {
		V* values = new V[size];
		make_list_of_values(root, values);
		return values;
	}

	friend std::ostream& operator<<(std::ostream& stream, TreeMap& map) {
		K* keys = map.get_keys();
		V* vals = map.get_values();
		stream << "{\n";
		for (int i = 0; i < map.get_size(); i++) {
			stream << "\t" << keys[i] << " -> " << vals[i] << "\n";
		}
		stream << "}\n";
		return stream;
	}

	void print() {
		std::cout << *this;
	}

	~TreeMap() {
		clear();
	}
};