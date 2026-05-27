#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "utils.h"
#include <time.h>


 
static int counter=0; // It has two functions: count the number of nodes of the trie, asked in the summary, and it's going to be the Node's id.

struct Node{
    char* bitString;
    unsigned int bit_position;
    unsigned int port; // 0 no port, 1 port
    int outport_interface; // The outport interface from the routing table
    int counter; // Id of the node
    struct Node *left;
    struct Node *right;
};

// Transform the prefix into binary strings
char *prefix_to_binary_string(uint32_t prefix, int prefixLength) {
    char *binaryString = (char *)malloc((prefixLength + 1) * sizeof(char));
    if (binaryString == NULL) {
        fprintf(stderr, "Error de asignación de memoria.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < prefixLength; i++) {
		if (prefix & (1 << (31 - i))) {
		    binaryString[i] = '1';
		} else {
		    binaryString[i] = '0';
		}
	}
    binaryString[prefixLength] = '\0'; // Null-terminator for the chain
    return binaryString;
}


struct Node* createNode(int index, char* prefix, int port, int counter, int outInterface) {
    // Allocate memory for the new node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    // Assign values to the new node
    newNode->bit_position = index;
    newNode->bitString = strdup(prefix);
    newNode->port = port;
    newNode->outport_interface = outInterface;
    newNode->counter = counter;
    newNode->left = NULL;
    newNode->right = NULL;
    
    return newNode;
}

struct Node* add(struct Node* root, char* prefix, int index, int outInterface) {
    int port;
    // Basic step of recursion
    if (root == NULL || (index  == strlen(prefix))) {
        // The 3rd attribute gives a 1 if we have completed the prefix string
        if (index  == strlen(prefix)) { // Establish if we have a port
            port = 1;;
        } else {
            port = 0;
    }
        if(root == NULL){ // if the node does not exist, we create it
            counter++;
            root = createNode(index, prefix, port, counter, outInterface);
        }else{ 
            root->port = port;
            root->outport_interface = outInterface;
        }
    }
    
    if(index == strlen(prefix)){ 
        return root;
    }
    
    char bit = prefix[index]; 
   
    // Recursive step. Add as many nodes as we need to reach the prefix we want to add
    if (bit == '0') {
        root->left = add(root->left, prefix, index + 1, outInterface);
    } else {
        root->right = add(root->right, prefix, index + 1, outInterface);
    }
        
    return root;
}

// Free the memory allocated for the nodes
void FreeNode(struct Node* node) {
    if (node == NULL)
        return;

    // Recursively free memory for left and right subtrees
    FreeNode(node->left);
    FreeNode(node->right);

    // Free memory allocated for bitString
    free(node->bitString);

    // Free memory allocated for the node itself
    free(node);
}

// This function prints the trie, telling the id, outport interface and right and left child of each node. We have been using it when we were developing the program.
void print_trie(struct Node* root) {
    printf("Node %d", root->counter);

    // Print port status
    if (root->port == 0){
        printf(", No port");
    }
    else{
        printf(", Port");
        printf(", interfaz %d", root->outport_interface);
    }
    if (root->left != NULL)
        printf(", Left %d", root->left->counter);
    else
        printf(", Left NULL");

    if (root->right != NULL)
        printf(", Right %d\n", root->right->counter);
    else
        printf(", Right NULL\n");

    // Recursively print the left and right subtrees
    if (root->left != NULL)
        print_trie(root->left);
    if (root->right != NULL)
        print_trie(root->right);
}




static int nodes_comp = 0;

void compress(struct Node** rootPtr) {
    struct Node* root = *rootPtr;

    // End of compression. 
    if (root == NULL || (root->left == NULL && root->right == NULL))
        return;

	// Recursive step
    if ((root->port == 0) && root->counter != 0 ) { // The node is not a port nor the root, necessary to compress
        // Recursion right part
        if (root->right != NULL && root->left == NULL) { //  There's right child but not left one, we can compress
            struct Node* aux = root; // We use auxiliary variables to be able to connect the nodes when we compress one and to free the memory
            *rootPtr = root->right;
            aux->right = NULL;
            free(aux->bitString);
            free(aux);
            nodes_comp++;
            compress(rootPtr); 
            return; // Return after freeing the memory
        }
        if (root->left != NULL && root->right == NULL) { // there's left child but not right one, we can compress
            struct Node* aux = root;
            *rootPtr = root->left;
            aux->left = NULL;
            free(aux->bitString);
            free(aux);
            nodes_comp++;
            compress(rootPtr); 
            return; // Return after freeing the memory
        }
    }

    if (((root->port == 1) && root->counter != 0)){ // the node is a port, take into account the case where it has a descendant that has 2 children, how to compress that descendant.
        if(root->left != NULL){ // Left part recursion
            compress(&(root->left));
            if((root->left->port)== 0 && (root->right == NULL)){
                struct Node* aux = root->left; // Auxiliary variables for the memory and to link the children of a node that is being compressed with the grandparent.
                struct Node* tempLeft = root->left->left;
                struct Node* tempRight = root->left->right;

                aux->left->left = NULL;
                aux->left->right = NULL;

                free(aux->bitString);
                free(aux);
                nodes_comp++;

                root->left = tempLeft;
                root->right = tempRight;
                return; // Return after freeing the memory
            }
        }
    }
// Same as previous case but for the right part of the trie
    if (((root->port == 1) && root->counter != 0)){
        if(root->right != NULL){ 
            compress(&(root->right));
            if((root->right->port)== 0 && (root->left == NULL)){
                struct Node* aux = root->right;

                struct Node* tempLeft = root->right->left;
                struct Node* tempRight = root->right->right;
                aux->right->left = NULL;
                aux->right->right = NULL;

                free(aux->bitString);
                free(aux);
                nodes_comp++;

                root->left = tempLeft;
                root->right = tempRight;
                return; // Return after freeing the memory
            }
        }
    }
	
	// Recursion to traverse the whole trie
    if (root->left != NULL) { 
        compress(&(root->left));
    }
    if (root->right != NULL) {
        compress(&(root->right));
    }
}



static int accessed_nodes = 0; // it counts all the accesed nodes taking into account the root (that we do not want to count it) but it does not count the last one, so it is equivalent.

int search(struct Node *root, char *address, int *last_outport_interface) {

    int outport_interface;

    unsigned int index = root->bit_position; // Position of the bit that we want to compare it

    // Basic case
    if (root->port == 1 && strncmp(address, root->bitString, index) == 0) { // the node is a port and the address coincides with its prefix 
        *last_outport_interface = root->outport_interface; // Save the last outport match
        outport_interface = *last_outport_interface;
    }

     //Initial value if there is not any match
    outport_interface = -1; 


    // Recursive case
    // decide what path we have to follow: left or right for the next bit
    if (address[index] == '0' && root->left != NULL) { //  left
        accessed_nodes++;
        *last_outport_interface = search(root->left, address, last_outport_interface); // update the outport interface for the case that we have found a better match

    } else if (address[index] == '1' && root->right != NULL) { // right
        accessed_nodes++;
        *last_outport_interface = search(root->right, address, last_outport_interface);
    }

    if (outport_interface == -1) {
        // If we have not found any coincidence, we save the last outport match
        outport_interface = *last_outport_interface;
    } else {
        // If we have found a new coincidence, update the last outport match
        *last_outport_interface = outport_interface;
    }

    return outport_interface;
}


int main(int argc, char *argv[]){

    //INITIALIZE AND ERROR READING FILE
    int initResult = initializeIO(argv[1], argv[2]);
    if (initResult != OK) {
        printIOExplanationError(initResult);
        return initResult;
    }

    char outputFileName[256];
    snprintf(outputFileName, sizeof(outputFileName), "%s.out", argv[2]);
    FILE* outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        fprintf(stderr, "Error: Cannot create output file.\n");
        freeIO();
        return -1;
    }

    // create root
    struct Node* root = createNode(0, "0", 0, counter, 0);
    

    uint32_t prefix;
    int prefixLength, outInterface, readResult;

	// Create the trie reading the routing table of the files 
    while ((readResult = readFIBLine(&prefix, &prefixLength, &outInterface)) == OK) {
    
        // Convert prefix to a binary string
         // 32 bits for the binary representation + 1 for null terminator
        char* binaryString = prefix_to_binary_string(prefix, prefixLength); 


        root = add(root, binaryString, 0, outInterface);
        free(binaryString); 
    }

	// Compress the trie
	compress(&root);
	
	// Variables for the statistics that will be shown on the output files
    int totalAccesses = 0;
    int totalTime = 0;
    int totalPacketProcessed = 0;
    double searchingTime;
    int totalNodes = counter - nodes_comp; // Number of nodes of the compressed trie
    

    uint32_t ipAddress;
    // Reading the file with IP addressed that we are going to assign to an outport following the routing table
    while ((readResult = readInputPacketFileLine(&ipAddress)) == OK) {

        int match_port = 0;

        char* binaryString_IP = prefix_to_binary_string(ipAddress, 32); 
        
        // Obtaining the time that the program needs to do the search
        struct timespec initialTime;
        struct timespec finalTime;
        
        clock_gettime(CLOCK_MONOTONIC_RAW, &initialTime);
        match_port = search(root, binaryString_IP, &match_port);
        free(binaryString_IP);
        clock_gettime(CLOCK_MONOTONIC_RAW, &finalTime);


        searchingTime = (finalTime.tv_sec - initialTime.tv_sec) * 1e9 + (finalTime.tv_nsec - initialTime.tv_nsec);
		// Total statistics
        totalAccesses +=  accessed_nodes;
        totalTime += searchingTime;
        totalPacketProcessed ++;
        
		// Print each address information (each search information)
        printOutputLine(ipAddress, match_port, &initialTime, &finalTime, &searchingTime, accessed_nodes);
    }

	// Average statistics
    double averageNodeAccesses = ((double)totalAccesses/totalPacketProcessed);
    double averagePacketProcessingTime = ((double)totalTime/totalPacketProcessed);
    
	// Print the summary of the statistics at the end of the search 
    printSummary(totalNodes, totalPacketProcessed, averageNodeAccesses, averagePacketProcessingTime);
        
    if (readResult != REACHED_EOF) {
        printIOExplanationError(readResult);
    }

    freeIO();
    FreeNode(root);



    return 0;

}


