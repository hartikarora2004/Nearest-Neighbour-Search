#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<limits.h>
#include <math.h>
#include <float.h>
int k ; // dimension of K-D tree
typedef struct Node
{
    int* data ; // data stored according to dimension in array
    struct Node* left;
    struct Node* right;
} node;

// Function to create or add new node in the tree 
node* insert_node(node* root, int* point, int depth)
{
    node* newnode;
    newnode = (node*)malloc(sizeof(node));  // Allocate memory dynamically to new node
	newnode->data = (int*)malloc(k * sizeof(int)); // Dynamically allocate memory to data array according to dimension
    for (int i = 0; i < k; i++)
    {
        newnode->data[i] = point[i];
    }
    if (root == NULL)  // Tree is empty...
    {   root = newnode ; // first node inserted in the tree
        newnode->left = NULL;
        newnode->right = NULL;
    }
    else
    {
        int disc = depth % k; // calculate the current dimension of comparison

	// compare the newely inserted node with root node
        if (point[disc] < root->data[disc]) // If value of newnode is less than root node insert node in left subtree
        {
            root->left = insert_node(root->left, point, depth + 1);
        }
        else  // insert in right subtree
        {
            root->right = insert_node(root->right, point, depth + 1);
        }
    }
    return root;
}
typedef struct point {
    int* data;
} point;

// Function to calculate the Euclidean distance between two points
double euclideanDistance(int* point1, int* point2) {
    double distance = 0;
    for (int i = 0; i < k; i++) {
        double diff = point1[i] - point2[i];
        distance += diff * diff;
    }
    return sqrt(distance);
}
// function to calculate NearestNeighbor
node* findNearestNeighbor(node* root, point* searchpoint, int depth, node* best, double* bestDistance) {
    // Base Case
    if (root == NULL) 
        {
            return best;
        }
    // INTERMEDIATE/SMALL CALCULATION
    int disc = depth % k;
    node* next_Branch;
    node* opp_Branch;

    if (searchpoint->data[disc] < root->data[disc]) {
        next_Branch = root->left;
        opp_Branch = root->right;
    } else {
        next_Branch = root->right;
        opp_Branch = root->left;
    }

    // RECURSIVE CALLING
    best = findNearestNeighbor(next_Branch, searchpoint, depth + 1, best, bestDistance);

    // Check if the current node is closer
    double currentDistance = euclideanDistance(root->data, searchpoint->data);
    if (currentDistance < *bestDistance && currentDistance!=0) {
        *bestDistance = currentDistance;
        best = root;
    }

    // Check the other side of the splitting plane
    if ((*bestDistance) >= fabs(searchpoint->data[disc] - root->data[disc])) {
        best = findNearestNeighbor(opp_Branch, searchpoint, depth + 1, best, bestDistance);
    }

    return best;
}
int count= 0;
void print_tree(node * tree ) 
{ 
	if (tree)
	{
        
		print_tree(tree->left);
        printf("(");
		for (int i = 0; i < k-1; i++)
		{
			printf("%d ", tree->data[i]);
		}
        printf("%d", tree->data[k-1]);
        printf(") ");
		// printf("\n");
		print_tree(tree->right);
	}
}
bool arepointsEqual(int *point1, int *point2)
{
    for (int i = 0; i < k; i++)
    {
        if (point1[i] != point2[i])
        {
            return false;
        }
    }
    return true;
}

bool search_node(node *root, int *Searchpoint, int depth)
{
    // BASE CASE
    if (root == NULL)
    {
        return false;
    }

    // INTERMEDIATE/SMALL CALCULATION
    int disc = depth % k;

    if (arepointsEqual(root->data, Searchpoint))
    {
        return true;
    }

    // RECURSIVE CALLING
    if (Searchpoint[disc] < root->data[disc])
    {
        return search_node(root->left, Searchpoint, depth + 1);
    }
    else
    {
        return search_node(root->right, Searchpoint, depth + 1);
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
    int** points = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
    {
        points[i] = (int*)malloc(k * sizeof(int));
    }

    // Scan the values of points
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            scanf("%d", &points[i][j]);
        }
    }

    // Insert the points in the tree
    for (int i = 0; i < n; i++)
    {
        root = insert_node(root, points[i], 0);
    }

    // Don't forget to free the allocated memory for point arrays.
    printf("The inorder traversal of kd tree is: \n");
	print_tree(root);
    // Define a search point
    point searchpoint;
    searchpoint.data = (int*)malloc(k * sizeof(int));
        printf("\nEnter search point: ");

    for (int i = 0; i < k; i++) {
        // printf("Enter search point, dimension %d: ", i);
        scanf("%d", &searchpoint.data[i]);
    }

    // Initialize variables for the nearest neighbor search
    double bestDistance = DBL_MAX;
    node* nearest = findNearestNeighbor(root, &searchpoint, 0, NULL, &bestDistance);
    // Print the nearest neighbor
    printf("Nearest Neighbor: [");
    for (int i = 0; i < k; i++) {
        printf("%d", nearest->data[i]);
        if (i < k - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    for (int i = 0; i < n; i++)
    {
        free(points[i]);
    }
    free(points);

    return 0;
}
