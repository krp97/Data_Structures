#include "Rbt_Node.h"
#include <string>

class Rbt
{
public:
	
	Rbt();

	Rbt(Rbt_Node* root);

	~Rbt();

	std::string preorder();
	
	std::string inorder();

	std::string postorder();


	void insert(int value);

private:

	Rbt_Node* root_;
	
	Rbt_Node* sentinel_;

	void preorder(Rbt_Node* node, std::string& output);

	void inorder(Rbt_Node* node, std::string& output);

	void postorder(Rbt_Node* node, std::string& output);


	Rbt_Node* add_node(Rbt_Node* node, const int value);

	void balance(Rbt_Node* node);

	Rbt_Node* get_uncle(Rbt_Node* node);

	void red_uncle_fix(Rbt_Node* node, Rbt_Node* uncle);

	void black_uncle_fix(Rbt_Node* node, Rbt_Node* uncle);

	void swap_colors(Rbt_Node* node1, Rbt_Node* node2);
	
	void reattach_parent(Rbt_Node* parent, Rbt_Node* child);


	void fix_right(Rbt_Node* node);

	Rbt_Node* rot_left(Rbt_Node* node);

	Rbt_Node* rot_right_left(Rbt_Node* node);


	void fix_left(Rbt_Node* node);

	Rbt_Node* rot_right(Rbt_Node* node);

	Rbt_Node* rot_left_right(Rbt_Node* node);
};