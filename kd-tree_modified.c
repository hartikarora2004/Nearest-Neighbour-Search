#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <float.h>

// Global variable for dimension of KD-tree
int k;

// Structure to represent a node in KD-tree
typedef struct Node {
    int* data;             // Data stored as per dimensions in an array
    struct Node* left;     // Pointer to left child
    struct Node* right;    // Pointer to right child
} node;

// Function to create or add a new node in the tree
node* insert_node(node* root, int* point, int depth) {
    // Allocate memory dynamically for a new node
    node* newnode = (node*)malloc(sizeof(node));
    newnode->data = (int*)malloc(k * sizeof(int));

    // Copy the data point to the node
    for (int i = 0; i < k; i++) {
        newnode->data[i] = point[i];
    }

    // If tree is empty, the new node becomes the root
    if (root == NULL) {
        root = newnode;
        newnode->left = NULL;
        newnode->right = NULL;
    } else {
        // Calculate the current dimension of comparison
        int disc = depth % k;

        // Compare the newly inserted node with the root node
        if (point[disc] < root->data[disc]) {
            // Insert node in the left subtree
            root->left = insert_node(root->left, point, depth + 1);
        } else {
            // Insert node in the right subtree
            root->right = insert_node(root->right, point, depth + 1);
        }
    }
    return root;
}

// Structure to represent a point
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

// Function to calculate the nearest neighbor in the KD-tree
node* findNearestNeighbor(node* root, point* searchpoint, int depth, node* best, double* bestDistance) {
    // Base case: if tree is empty
    if (root == NULL) {
        return best;
    }

    // Calculate the current dimension of comparison
    int disc = depth % k;
    node* next_Branch;
    node* opp_Branch;

    // Determine which branch to search next
    if (searchpoint->data[disc] < root->data[disc]) {
        next_Branch = root->left;
        opp_Branch = root->right;
    } else {
        next_Branch = root->right;
        opp_Branch = root->left;
    }

    // Recursively search the next branch
    best = findNearestNeighbor(next_Branch, searchpoint, depth + 1, best, bestDistance);

    // Check if the current node is closer
    double currentDistance = euclideanDistance(root->data, searchpoint->data);
    if (currentDistance < *bestDistance && currentDistance != 0) {
        *bestDistance = currentDistance;
        best = root;
    }

    // Check the other side of the splitting plane if necessary
    if ((*bestDistance) >= fabs(searchpoint->data[disc] - root->data[disc])) {
        best = findNearestNeighbor(opp_Branch, searchpoint, depth + 1, best, bestDistance);
    }

    return best;
}

// Function to print the KD-tree in inorder traversal
void print_tree(node* tree) {
    if (tree) {
        print_tree(tree->left);
        printf("(");
        for (int i = 0; i < k - 1; i++) {
            printf("%d ", tree->data[i]);
        }
        printf("%d", tree->data[k - 1]);
        printf(") ");
        print_tree(tree->right);
    }
}

// Function to check if two points are equal
bool arepointsEqual(int* point1, int* point2) {
    for (int i = 0; i < k; i++) {
        if (point1[i] != point2[i]) {
            return false;
        }
    }
    return true;
}

// Function to search for a node in the KD-tree
bool search_node(node* root, int* searchpoint, int depth) {
    // Base case: if tree is empty
    if (root == NULL) {
        return false;
    }

    // Calculate the current dimension of comparison
    int disc = depth % k;

    // Check if the current node matches the search point
    if (arepointsEqual(root->data, searchpoint)) {
        return true;
    }

    // Recursively search in the appropriate subtree
    if (searchpoint[disc] < root->data[disc]) {
        return search_node(root->left, searchpoint, depth + 1);
    } else {
        return search_node(root->right, searchpoint, depth + 1);
    }
}

int main() {
    node* root = NULL;
    int n;
    printf("Enter the number of points: ");
    scanf("%d", &n);
    printf("Enter the number of dimensions (k): ");
    scanf("%d", &k);

    // Allocate memory for the array of points
    int** points = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        points[i] = (int*)malloc(k * sizeof(int));
    }

    // Scan the values of points
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            scanf("%d", &points[i][j]);
        }
    }

    // Insert the points into the KD-tree
    for (int i = 0; i < n; i++) {
        root = insert_node(root, points[i], 0);
    }

    // Print the inorder traversal of the KD-tree
    printf("The inorder traversal of KD-tree is: \n");
    print_tree(root);

    // Define a search point
    point searchpoint;
    searchpoint.data = (int*)malloc(k * sizeof(int));
    printf("\nEnter search point: ");
    for (int i = 0; i < k; i++) {
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

    // Free the allocated memory for point arrays
    for (int i = 0; i < n; i++) {
        free(points[i]);
    }
    free(points);
    free(searchpoint.data);

    return 0;
}
