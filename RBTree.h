#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <exception>

enum COLOR { RED, BLACK };

struct Node
{
	Node(const std::string key, const int& count) : key_(key), count_(count) {}

	COLOR color_ = RED;
	std::string key_;
	int count_ = 1;
	Node* left_ = nullptr, * right_ = nullptr, * parent_ = nullptr;
};


class RBTree
{
public:

	RBTree() : root_(nullptr) {}

	~RBTree() {
		clearRBT(root_);
	}

	void insertElem(const std::string& key, const int& count) {

		Node* newNode = new Node(key, count);
		Node* parent = nullptr;
		Node* current = root_;

		while (current != nullptr) {
			parent = current;

			if (key < current->key_)
				current = current->left_;
			else if (key > current->key_)
				current = current->right_;
			else {

				current->count_ = count;
				delete newNode;
				return;

			}
		}

		newNode->parent_ = parent;

		if (parent == nullptr)
			root_ = newNode;
		else if (key < parent->key_)
			parent->left_ = newNode;
		else
			parent->right_ = newNode;

		insertBalance(newNode);
	}

	Node* searchElem(const std::string& key) const {

		Node* current = root_;

		while (current != nullptr) {
			if (key < current->key_)
				current = current->left_;
			else if (key > current->key_)
				current = current->right_;
			else
				return current;
		}

		return nullptr;
	}

	void deleteElem(const std::string& key) {

		Node* node = searchElem(key);

		if (node == nullptr)
			return;

		Node* toDelete = node;

		if (node->left_ == nullptr)
			transplant(node, node->right_);
		else if (node->right_ == nullptr)
			transplant(node, node->left_);
		else {
			Node* successor = minimum(node->right_);

			toDelete = successor;
			node->key_ = successor->key_;
			node->count_ = successor->count_;

			transplant(successor, successor->right_);
		}

		if (toDelete->color_ == BLACK)
			deleteBalance(toDelete);

		delete toDelete;
	}

	void printMostFrequentWords(int numWords) const {
		std::vector<Node*> nodes;
		inorderTraversal(root_, nodes);

		std::sort(nodes.begin(), nodes.end(), [](const Node* a, const Node* b) {
			return a->count_ > b->count_;
			});

		if (numWords > nodes.size()) {

			throw invalidAmount();

		}
		std::cout << numWords << " most frequent words:" << std::endl;

		for (int i = 0; i < numWords; ++i) {
			std::cout << nodes[i]->key_ << " (count: " << nodes[i]->count_ << ")" << std::endl;
		}

	}

	class invalidAmount :public std::exception {
	public:

		invalidAmount() {

			reason_ = "Error: Invalid amount of elements";

		}

		const char* what() const noexcept override {

			return reason_.c_str();

		}
	private:

		std::string reason_;
	};

private:

	Node* root_ = nullptr;

	void insertBalance(Node* node) {

		while (node != root_ && node->parent_->color_ == RED) {

			if (node->parent_ == node->parent_->parent_->left_) {
				Node* uncle = node->parent_->parent_->right_;

				if (uncle != nullptr && uncle->color_ == RED) {

					node->parent_->color_ = BLACK;
					uncle->color_ = BLACK;
					node->parent_->parent_->color_ = RED;
					node = node->parent_->parent_;

				}
				else {
					if (node == node->parent_->right_) {
						node = node->parent_;
						rotateLeft(node);
					}

					node->parent_->color_ = BLACK;
					node->parent_->parent_->color_ = RED;
					rotateRight(node->parent_->parent_);
				}
			}
			else {
				Node* uncle = node->parent_->parent_->left_;

				if (uncle != nullptr && uncle->color_ == RED) {
					node->parent_->color_ = BLACK;
					uncle->color_ = BLACK;
					node->parent_->parent_->color_ = RED;
					node = node->parent_->parent_;
				}
				else {
					if (node == node->parent_->left_) {
						node = node->parent_;
						rotateRight(node);
					}

					node->parent_->color_ = BLACK;
					node->parent_->parent_->color_ = RED;
					rotateLeft(node->parent_->parent_);
				}
			}
		}

		root_->color_ = BLACK;
	}

	void rotateLeft(Node* node) {

		Node* rightChild = node->right_;

		node->right_ = rightChild->left_;

		if (rightChild->left_ != nullptr)
			rightChild->left_->parent_ = node;

		rightChild->parent_ = node->parent_;

		if (node->parent_ == nullptr)
			root_ = rightChild;
		else if (node == node->parent_->left_)
			node->parent_->left_ = rightChild;
		else
			node->parent_->right_ = rightChild;

		rightChild->left_ = node;
		node->parent_ = rightChild;
	}

	void rotateRight(Node* node) {
		Node* leftChild = node->left_;
		node->left_ = leftChild->right_;

		if (leftChild->right_ != nullptr)
			leftChild->right_->parent_ = node;

		leftChild->parent_ = node->parent_;

		if (node->parent_ == nullptr)
			root_ = leftChild;
		else if (node == node->parent_->right_)
			node->parent_->right_ = leftChild;
		else
			node->parent_->left_ = leftChild;

		leftChild->right_ = node;
		node->parent_ = leftChild;
	}

	void transplant(Node* u, Node* v) {
		if (u->parent_ == nullptr)
			root_ = v;
		else if (u == u->parent_->left_)
			u->parent_->left_ = v;
		else
			u->parent_->right_ = v;

		if (v != nullptr)
			v->parent_ = u->parent_;
	}

	Node* minimum(Node* node) const {
		while (node->left_ != nullptr)
			node = node->left_;
		return node;
	}

	void deleteBalance(Node* node) {
		while (node != root_ && node->color_ == BLACK) {
			if (node == node->parent_->left_) {
				Node* brother = node->parent_->right_;

				if (brother->color_ == RED) {
					brother->color_ = BLACK;
					node->parent_->color_ = RED;
					rotateLeft(node->parent_);
					brother = node->parent_->right_;
				}

				if (brother->left_->color_ == BLACK && brother->right_->color_ == BLACK) {
					brother->color_ = RED;
					node = node->parent_;
				}
				else {
					if (brother->right_->color_ == BLACK) {
						brother->left_->color_ = BLACK;
						brother->color_ = RED;
						rotateRight(brother);
						brother = node->parent_->right_;
					}

					brother->color_ = node->parent_->color_;
					node->parent_->color_ = BLACK;
					brother->right_->color_ = BLACK;
					rotateLeft(node->parent_);
					node = root_;
				}
			}
			else {
				Node* brother = node->parent_->left_;

				if (brother->color_ == RED) {
					brother->color_ = BLACK;
					node->parent_->color_ = RED;
					rotateRight(node->parent_);
					brother = node->parent_->left_;
				}

				if (brother->right_->color_ == BLACK && brother->left_->color_ == BLACK) {
					brother->color_ = RED;
					node = node->parent_;
				}
				else {
					if (brother->left_->color_ == BLACK) {
						brother->right_->color_ = BLACK;
						brother->color_ = RED;
						rotateLeft(brother);
						brother = node->parent_->left_;
					}

					brother->color_ = node->parent_->color_;
					node->parent_->color_ = BLACK;
					brother->left_->color_ = BLACK;
					rotateRight(node->parent_);
					node = root_;
				}
			}
		}

		node->color_ = BLACK;
	}

	void inorderTraversal(Node* node, std::vector<Node*>& nodes) const {
		if (node == nullptr)
			return;

		inorderTraversal(node->left_, nodes);
		nodes.push_back(node);
		inorderTraversal(node->right_, nodes);
	}

	void clearRBT(Node* node) {
		if (node != nullptr) {

			clearRBT(node->left_);
			clearRBT(node->right_);
			delete node;

		}
	}


};


