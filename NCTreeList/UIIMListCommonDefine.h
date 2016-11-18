#ifndef UILISTCOMMONDEFINE_6FD08412_6B76J_491B_9FF9_DB59ADFF15B9_H__
#define UILISTCOMMONDEFINE_6FD08412_6B76J_491B_9FF9_DB59ADFF15B9_H__

#include <math.h>
#include <functional>
#include <algorithm>
/******************************************************************************/

struct NodeData
{
	int level_;
	bool folder_;
	bool child_visible_;
	bool has_child_;
	CDuiString text_;
	int sId;
	CListContainerElementUI* list_elment_;
};

double CalculateDelay(double state);

/**
* The class <code>Node</code>
*
*/
class Node
{
public:
	Node();
	explicit Node(NodeData t);
	Node(NodeData t, Node* parent);
	~Node();
	NodeData& data();
	int num_children() const;
	Node* child(int i);
	Node* parent();
	bool folder() const;
	bool has_children() const;
	void add_child(Node* child);
	void remove_child(Node* child);
	Node* get_last_child();
	void sort_children(std::function<bool(Node *&x, Node *&y)> &compare);
private:
	void set_parent(Node* parent);

private:
	typedef std::vector <Node*>	Children;

	Children	children_;
	Node*		parent_;

	NodeData    data_;
};
/******************************************************************************/
#endif// UILISTCOMMONDEFINE_6FD08412_6B76J_491B_9FF9_DB59ADFF15B9_H__