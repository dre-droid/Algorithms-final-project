#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
char buffer[MAX_LENGTH];

typedef struct Station{
    unsigned int distance, *cars;
    struct Station* left;
    struct Station* right;
} Station;
Station* stationsTreeRoot = NULL;

Station* removeStation(Station* root, int distance);
void insertStation(Station **root, int distance, int* cars);
Station* findMin(Station* node);
int highwayContains(Station*, int distance);

//Returns 1 if line starts with pattern, false otherwise.
int startsWith(char* line, char* pattern){
    return strncmp(line, pattern, strlen(pattern)) == 0;
}

/**********   binary tree stuff    **********/
void insertStation(Station **root, int distance, int* cars){
    Station* newStation = (Station*) malloc(sizeof(Station));
    newStation -> distance = distance;
    newStation -> cars = cars;
    newStation -> left = NULL;
    newStation -> right = NULL;

    if (*root == NULL){
        *root = newStation;
        printf("NULL\n");
    }else {
        Station* curr = *root;
        Station* parent = NULL;

        while (curr != NULL) {
            parent = curr;
            if (distance < curr->distance) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }

        if (distance < parent->distance) {
            parent->left = newStation;
        } else {
            parent->right = newStation;
        }
    }
}
Station* findMin(Station* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}
Station* removeStation(Station* root, int distance){
    if (root == NULL) {
        return root;
    }

    if (distance < root->distance) {
        root->left = removeStation(root->left, distance);
    } else if (distance > root->distance) {
        root->right = removeStation(root->right, distance);
    } else {
        if (root->left == NULL) {
            Station* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Station* temp = root->left;
            free(root);
            return temp;
        }

        Station* temp = findMin(root->right);
        root->distance = temp->distance;
        root->right = removeStation(root->right, temp->distance);
    }
    return root;
}
int highwayContains(Station* root, int distance){
    if (root == NULL) {
        return 0;  // value not found, return 0
    }

    if (root->distance == distance) {
        return 1;  // value found, return 1
    } else if (distance < root->distance) {
        return highwayContains(root->left, distance);  // Search in the left subtree
    } else {
        return highwayContains(root->right, distance);  // Search in the right subtree
    }
}
void printTree(Station* root, int depth){
    while (root -> left != NULL) {
        root = root-> left;
        depth++;
    }
    char** lines = (char**)malloc(depth * sizeof(char*));
    for (int i = 0; i < depth; i++){
        lines[i] = root->distance;  // Assuming 'data' is a char* containing the line of the tree node
        root = root->right;  
    }

     for (int i = 0; i < depth; i++){
        printf("%s", lines[i]);
     }
}
/**********   end binary tree stuff    **********/

//Reads aggiungi-stazione command.
//Expected format: "aggiungi-stazione km numOfCars kmCar1 kmCar2 ... kmCarnumOfCars - 1", where 
//km, numOfCars and kmCarI are all space separated ints
void addStation(){
    int km, numOfCars, *carDistance;
    //copy buffer to modifyable string
    char* string = (char*)malloc(50 * sizeof(char));
    string = strcpy(string, buffer);

    //read km, numOfCars
    sscanf(string, "aggiungi-stazione %d %d", &km, &numOfCars);
    carDistance = (int*)malloc(numOfCars * sizeof(int));

    //if station already present, do nothing
    if (highwayContains(&stationsTreeRoot, km)) {
        printf("non aggiunta\n");
        return;
    }

    //find index of third space 
    int count = 0, i = 0;
    while (count < 2){
        if (string[i] == ' ') count++;
        i++;
    }

    //read the car milages (in kms :P)
    string += i;    //set string pointer to the third int
    for (i = 0; i < numOfCars; i++){
        sscanf(string, "%d", carDistance + i);
        while (*string != ' ') string++;       // move to next int
        string++;                              // move *string from ' ' -> '5'
    }

    printf("km: %d\n", km);
    for (i = 0; i < numOfCars; i++){
        printf("Car %d: distance = %d\n", i, carDistance[i]);
    }
    insertStation(&stationsTreeRoot, km ,carDistance);
    printf("aggiunta\n");
    printTree(stationsTreeRoot, 0);
}
void demStation(){
    //printf("non demolita");
    printf("demolita\n");
}
void addCar(){
    printf("aggiunta car\n");
}
void demCar(){
    printf("rottamata\n");
}
void planRoute(){
    printf("plantroute\n");
}

int main() {    
    // Read a line from standard input
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (startsWith(buffer, "aggiungi-stazione")) addStation();
        if (startsWith(buffer, "demolisci-stazione")) demStation();
        if (startsWith(buffer, "aggiungi-auto")) addCar();
        if (startsWith(buffer, "rottama-auto")) demCar();
        if (startsWith(buffer, "pianifica-percorso")) planRoute();
    }
    printTree(stationsTreeRoot, 0);
    return 0;
}