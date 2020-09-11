#ifndef LIST_H
#define LIST_H
struct Node {
	Node(int val)
		: next(nullptr), val(val)
	{}
	Node* next;
	int val;
};

class List
{
public:
	List();
	~List();
	void Insert(int val);
	void Delete();
	int Get(int idx);

private:
	Node* ptr;
};
#endif
