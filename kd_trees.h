#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<stdbool.h>

#define k 2

struct KDtree 
{
	int data[k];
	struct KDtree *left;
	struct KDtree *right;
};

typedef struct KDtree node;

node *insert_node(node *root , int point[] , int depth)
{
	if (root == NULL)
	{

		node *Newnode;
       		Newnode = (node*)malloc(sizeof(node));
		for(int i = 0 ; i < k ; i++)
		{
			Newnode->data[i] = point[i];
		}
		Newnode->left = NULL;
		Newnode->right = NULL;
	}	
	else
	{
		int disc = depth%k;

		if (point[disc] < (root->data[disc]))
		{
			root->left  = insert_node(root->left, point, depth + 1);
		}

    		else
		{
        		root->right = insert_node(root->right, point, depth + 1);
		}
	}
		return root;
}
node *insert(node *root , int point[])
{
	return insert_node(root , point , 0);
}

void inorder(node *root, int depth) 
{
    if (root == NULL) return;

    int cd = depth % k;
    inorder(root->left, depth + 1);
    printf("(%d , %d)", root->data[0] ,root->data[1]);
    inorder(root->right, depth + 1);
}



int main()
{
	node *root = NULL;
	int n;
    	printf("Enter the number of points: ");
    	scanf("%d", &n);
    	int points[n][k];

    	for (int i = 0; i < n; ++i)
       	{
        	scanf("%d %d", &points[i][0], &points[i][1]);
        	root = insert(root, points[i]);
    	}
		inorder(root , 0);

	return 0;
}
