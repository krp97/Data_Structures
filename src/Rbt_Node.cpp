#include "../include/Rbt_Node.h"
#include <string>


Rbt_Node::Rbt_Node()
	: value_{ 0 }, color_{ 'B' }, up_{ this }, left_{ this }, right_{ this }
{
}

Rbt_Node::Rbt_Node(int value, char color, Rbt_Node * up, Rbt_Node * left, Rbt_Node * right)
	: value_{ value }, color_{ color }, up_{ up }, left_{ left }, right_{ right }
{

}

Rbt_Node& Rbt_Node::operator=(Rbt_Node & rhs)
{
	value_ = rhs.value_;
	left_ = rhs.left_;
	color_ = rhs.color_;
	right_ = rhs.right_;

	return *this;
}


std::string Rbt_Node::to_string()
{
	return std::string(std::to_string(value_) + ":" + color_);
}

Rbt_Node::~Rbt_Node()
{

}