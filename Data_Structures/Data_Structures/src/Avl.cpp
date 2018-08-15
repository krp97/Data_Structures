#include "stdafx.h"
#include "..\include\Avl.h"

Avl::Avl()
	: root_{ nullptr }
{}

Avl::Avl(Avl_Node& root)
	: root_{ &root }
{}

Avl::~Avl()
{
	while (root_)
		remove_node(root_, root_->value_);
}

void Avl::insert(const int value)
{
	add_node(root_, nullptr, value);
}

void Avl::remove(const int value)
{
	remove_node(root_, value);
}

void Avl::inorder(Avl_Node * node, std::string & output)
{
	if (node)
	{
		inorder(node->left_, output);
		output += ", " + node->to_string();
		inorder(node->right_, output);

		if (output.at(0) == ',')
			output = output.substr(1);
	}

}

void Avl::preorder(Avl_Node * node, std::string & output)
{
	if (node)
	{
		output += ", " + node->to_string();
		preorder(node->left_, output);
		preorder(node->right_, output);

		if (output.at(0) == ',')
			output = output.substr(1);
	}
}

void Avl::postorder(Avl_Node * node, std::string & output)
{
	if (node)
	{
		postorder(node->left_, output);
		postorder(node->right_, output);
		output += ", " + node->to_string();

		if (output.at(0) == ',')
			output = output.substr(1);
	}
}

Avl_Node* Avl::remove_node(Avl_Node * node, const int value)
{
	if (node == nullptr)
		return nullptr;

	if (node->value_ == value)
		return remove_case(node);

	else if (value > node->value_)
		node->right_ = remove_node(node->right_, value);

	else if (value < node->value_)
		node->left_ = remove_node(node->left_, value);

	update_height(node);
	return retrace(node, value);
}

Avl_Node* Avl::remove_case(Avl_Node * node)
{
	Avl_Node* tmp;
	if (node->left_ && node->right_)
		tmp = remove_with_children(node);
	else if (node->left_ || node->right_)
		tmp = remove_with_single_child(node);
	else
		tmp = remove_leaf(node);
	return tmp;
}

Avl_Node* Avl::remove_leaf(Avl_Node * node_to_remove)
{
	bool root_test = root_ == node_to_remove;

	delete node_to_remove;

	if (root_test)
		root_ = nullptr;
	return nullptr;
}

Avl_Node* Avl::remove_with_single_child(Avl_Node * node)
{
	auto child{ node->right_ ? node->right_ : node->left_ };

	*node = *child;
	delete child;
	return node;
}

Avl_Node* Avl::remove_with_children(Avl_Node * node)
{
	auto successor = find_successor(node);

	node->value_ = successor->value_;

	auto right_succ_child = successor->right_;
	if (right_succ_child)
	{
		*successor = *right_succ_child;
		delete right_succ_child;
	}
	else
	{
		auto succ_parent{ successor->up_ };
		if (succ_parent->value_ > successor->value_)
			succ_parent->left_ = nullptr;
		else
			succ_parent->right_ = nullptr;
		delete successor;
	}
	return node;
}

Avl_Node * Avl::find_successor(Avl_Node * node)
{
	if (node->right_)
		return find_min(node->right_);
	else
	{
		auto parent{ node->up_ };
		auto tmp_node{ node };

		while (parent)
		{
			if (parent->left_ == tmp_node)
				return parent;

			tmp_node = parent;
			parent = parent->up_;
		}
		return parent;
	}
}

Avl_Node * Avl::find_min(Avl_Node * node)
{
	auto tmp{ node };

	while (tmp->left_)
		tmp = tmp->left_;
	return tmp;
}

Avl_Node* Avl::add_node(Avl_Node * leaf, Avl_Node* leaf_parent, const int value)
{
	if (leaf == nullptr)
	{
		leaf = new Avl_Node(value, 0, leaf_parent, nullptr, nullptr);
		if (root_ == nullptr)
			root_ = leaf;
		return leaf;
	}
	else if (value > leaf->value_)
		leaf->right_ = add_node(leaf->right_, leaf, value);
	else if (value < leaf->value_)
		leaf->left_ = add_node(leaf->left_, leaf, value);
	
	update_height(leaf);

	return retrace(leaf, value);
}

int Avl::max(const int value1, const int value2) const
{
	return value1 > value2 ? value1 : value2;
}

int Avl::get_height(Avl_Node const* node) const
{
	if (node == nullptr)
		return -1;
	else
		return node->bal_factor_;
}

void Avl::update_height(Avl_Node * node)
{
	node->bal_factor_ = max(get_height(node->left_), get_height(node->right_)) + 1;
}

Avl_Node* Avl::retrace(Avl_Node* leaf, const int value)
{
	auto right_height{ get_height(leaf->right_)};
	auto left_height{ get_height(leaf->left_)};

	if (right_height - left_height >= 2)
		return fix_right(leaf, value);

	else if (left_height - right_height >= 2)
		return fix_left(leaf, value);
	return leaf;
}

Avl_Node* Avl::fix_right(Avl_Node * node, const int value)
{
	Avl_Node* tmp;
	

	if (value > node->right_->value_)
		tmp = rot_left(node);
	else
		tmp = rot_right_left(node);

	auto parent{ tmp->up_ };
	reattach_parent(parent, tmp);
	update_height(node);
	return tmp;
}

Avl_Node* Avl::fix_left(Avl_Node * node, const int value)
{
	Avl_Node* tmp;
	

 	if (value < node->left_->value_)
		tmp = rot_right(node);
	else
		tmp = rot_left_right(node);

	auto parent{ tmp->up_ };
	update_height(node);
	reattach_parent(parent, tmp);
	return tmp;
}

void Avl::reattach_parent(Avl_Node * parent, Avl_Node * child)
{	
	if (parent)
	{
		if (child->value_ > parent->value_)
			parent->right_ = child;
		else
			parent->left_ = child;
	}
}

Avl_Node * Avl::rot_left(Avl_Node* node)
{
	auto right_child{ node->right_ };

	if (node->up_)
		right_child->up_ = node->up_;
	else
	{
		root_ = right_child;
		right_child->up_ = nullptr;
	}
	node->up_ = right_child;
	node->right_ = right_child->left_;
	
	if(right_child->left_)
		right_child->left_->up_ = node;

	right_child->left_ = node;

	return right_child;
}

Avl_Node * Avl::rot_right_left(Avl_Node* node)
{
	auto right_child{ node->right_ };
	node->right_ = rot_right(right_child);
	update_height(node->right_);
	update_height(node->right_->right_);
	return rot_left(node);
}

Avl_Node * Avl::rot_right(Avl_Node* node)
{
	auto left_child{ node->left_ };

	if (node->up_)
		left_child->up_ = node->up_;
	else
	{
		root_ = left_child;
		left_child->up_ = nullptr;
	}
	node->up_ = left_child;

	node->left_ = left_child->right_;
	if(left_child->right_)
		left_child->right_->up_ = node;

	left_child->right_ = node;
	
	return left_child;
}

Avl_Node * Avl::rot_left_right(Avl_Node* node)
{
	auto left_child{ node->left_ };
	node->left_ = rot_left(left_child);
	update_height(node->left_);
	update_height(node->left_->left_);
	return rot_right(node);
}

std::string Avl::to_string()
{
	return std::string();
}