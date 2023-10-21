#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
int k ;
typedef struct Node
{
    int* data ;
    struct Node* left;
    struct Node* right;
} node;

node* insert_node(node* root, int* point, int depth)
{
    node* newnode;
    newnode = (node*)malloc(sizeof(node));
	newnode->data = (int*)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++)
    {
        newnode->data[i] = point[i];
    }
    if (root == NULL)
    {   root = newnode ;
        newnode->left = NULL;
        newnode->right = NULL;
    }
    else
    {
        int disc = depth % k;

        if (point[disc] < root->data[disc])
        {
            root->left = insert_node(root->left, point, depth + 1);
        }
        else
        {
            root->right = insert_node(root->right, point, depth + 1);
        }
    }
    return root;
}
int count= 0;
void print_tree(node * tree ) 
{ 	
	if (tree)
	{
		print_tree(tree->left);
		for (int i = 0; i < k; i++)
		{
			printf("%d ", tree->data[i]);
		}
		printf("\n");
		print_tree(tree->right);
	}
}

int main()
{
    node* root = NULL;
    int n;
    printf("Enter the number of points: ");
    scanf("%d", &n);
    printf("Enter the number of dimensions (k): ");
    scanf("%d", &k);

    // Allocate memory for the array of points
    int** point = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
    {
        point[i] = (int*)malloc(k * sizeof(int));
    }

    // Scan the values of points
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            scanf("%d", &point[i][j]);
        }
    }

    // Insert the points in the tree
    for (int i = 0; i < n; i++)
    {
        root = insert_node(root, point[i], 0);
    }

    // Don't forget to free the allocated memory for point arrays.
	print_tree(root);
	// print("%d",root->data[i]))
	for(int i = 0; i < k; i++)
	{
		printf("%d",root->data[i]);
	}
    for (int i = 0; i < n; i++)
    {
        free(point[i]);
    }
    free(point);

    return 0;
}
