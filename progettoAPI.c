#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
char buffer[MAX_LENGTH];

typedef struct Car{
    unsigned int range, amount;
    struct Car *left;
    struct Car *right;
} Car;

typedef struct Station{
    unsigned int distance;
    Car* cars;
    struct Station* left;
    struct Station* right;
} Station;
Station* stationsTreeRoot = NULL;

Station* removeStation(Station* root, int distance);
void insertStation(Station **root, Station *newStation);

//Returns 1 if line starts with pattern, false otherwise.
int startsWith(char* line, char* pattern){
    return strncmp(line, pattern, strlen(pattern)) == 0;
}

/**********   binary tree stuff    **********/
void insertStation(Station **root, Station *newStation){
    if (*root == NULL){
        *root = newStation;
        printf("NULL\n");
    }else {
        Station* curr = *root;
        Station* parent = NULL;

        while (curr != NULL) {
            parent = curr;
            if (newStation -> distance < curr->distance) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }

        if (newStation -> distance < parent->distance) {
            parent->left = newStation;
        } else {
            parent->right = newStation;
        }
    }
}
Station* findMinStation(Station* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}
Station* removeStation(Station* root, int distance){
    if (root == NULL) {
        return root;  // Tree is empty or value not found
    }

    if (distance < root->distance) {
        root->left = removeStation(root->left, distance);
    } else if (distance > root->distance) {
        root->right = removeStation(root->right, distance);
    } else {
        printf("root 3: %d", root -> distance);
        // Node to be deleted is found
        // Case 1: Node has no children (leaf node)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
        }
        // Case 2: Node has one child
        else if (root->left == NULL) {
            Station* temp = root;
            root = root->right;
            free(temp);
        } else if (root->right == NULL) {
            Station* temp = root;
            root = root->left;
            free(temp);
        }
        // Case 3: Node has two children
        else {
            Station* temp = findMinStation(root->right);
            root->distance = temp->distance;
            root->right = removeStation(root->right, temp->distance);
        }
    }
    if (root -> distance == stationsTreeRoot -> distance) return NULL; // hot fix
    return root;
}
Station* findStation(Station* root, int distance) {
    if (root == NULL || root->distance == distance) {
        return root;
    }

    if (distance < root->distance) {
        return findStation(root->left, distance);
    } else {
        return findStation(root->right, distance);
    }
}

void addCarToStation(Car** carsRoot, int range){
    if (*carsRoot == NULL){
        *carsRoot = (Car*) malloc(sizeof(Car));
        (*carsRoot) -> range = range;
    }else {
        Car* curr = *carsRoot;
        Car* parent = NULL;

        while (curr != NULL) {
            parent = curr;
            if (range < curr->range) {
                curr = curr->left;
            } else if (range == curr -> range){     // if exact range already present, increase count and return
                curr -> amount++;
                return;
            } else {
                curr = curr->right;
            }
        }

        if (range < parent->range) {
            parent -> left = (Car*) malloc(sizeof(Car));
            parent -> left -> range = range;
        } else {
            parent -> right = (Car*) malloc(sizeof(Car));
            parent -> right -> range = range;
        }
    }
}
Car* findCar(Car* cars, int range){
     if (cars == NULL || cars->range == range) {
        return cars;
    }

    if (range < cars->range) {
        return findCar(cars->left, range);
    } else {
        return findCar(cars->right, range);
    }
}
Car* findMinCar(Car* cars){
    while (cars->left != NULL) {
        cars = cars->left;
    }
    return cars;
}
Car* removeCar(Car* root, int range){
    if (root == NULL) {
        return root;  // Tree is empty or value not found
    }

    if (range < root->range) {
        root->left = removeCar(root->left, range);
    } else if (range > root->range) {
        root->right = removeCar(root->right, range);
    } else {
        // Node to be deleted is found

        // Case 1: Node has no children (leaf node)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
        }
        // Case 2: Node has one child
        else if (root->left == NULL) {
            Car* temp = root;
            root = root->right;
            free(temp);
        } else if (root->right == NULL) {
            Car* temp = root;
            root = root->left;
            free(temp);
        }
        // Case 3: Node has two children
        else {
            Car* temp = findMinCar(root->right);
            root->range = temp->range;
            root->right = removeCar(root->right, temp->range);
        }
    }

    return root;
}
/**********   end binary tree stuff    **********/

//Reads aggiungi-stazione command. Adds station to Stations tree if not present.
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
    if (findStation(stationsTreeRoot, km) != NULL){
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
    int range;
    Car* carsRoot = (Car*) malloc(sizeof(Car));
    for (i = 0; i < numOfCars; i++){
        sscanf(string, "%d", &range);
        addCarToStation(&carsRoot, range);
        while (*string != ' ') string++;       // move to next int
        string++;                              // move *string from ' ' -> '5'
    }

    Station* newStation = (Station*) malloc(sizeof(Station));
    newStation -> distance = km;
    newStation -> cars = carsRoot;
    newStation -> left = NULL;
    newStation -> right = NULL;
    insertStation(&stationsTreeRoot, newStation);
    printf("aggiunta\n");
}
void demStation(){
    int distance;
    sscanf(buffer, "demolisci-stazione %d", &distance);
    Station* s = removeStation(stationsTreeRoot, distance);
    if (s == NULL)
        printf("non demolita\n");
    else printf("demolita\n");
}
void addCar(){
    int distance, range;
    sscanf(buffer, "aggiungi-auto %d %d", &distance, &range);
    printf("Range: %d, distance: %d", range, distance);
    Station* station = findStation(stationsTreeRoot, range);
    if (station != NULL){
        addCarToStation(&(station -> cars), range);
        printf("aggiunta\n");
    } else
        printf("non aggiunta\n");
}
void demCar(){
    int distance, range;
    sscanf(buffer, "rottama-auto %d %d", &distance, &range);
    Station* s = findStation(stationsTreeRoot, distance);
    Car* car;
    if (s != NULL){
        if (car != NULL){
            addCarToStation(&(s -> cars), range);
            printf("rottamata\n");
        }
    } else{
        printf("non rottamata\n");
    }
}
void planRoute(){
    printf("plan route\n");
}

int _print_t(Station *tree, int is_left, int offset, int depth, char s[20][255])
{
    char b[20];
    int width = 5;

    if (!tree) return 0;

    sprintf(b, "(%03d)", tree->distance);

    int left  = _print_t(tree->left,  1, offset,                depth + 1, s);
    int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);

#ifdef COMPACT
    for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
#endif

    return left + width + right;
}

void print_t(Station *tree)
{
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    _print_t(tree, 0, 0, 0, s);

    for (int i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}
int main() {    
    // Read a line from standard input
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (startsWith(buffer, "aggiungi-stazione")) {
            addStation();
            print_t(stationsTreeRoot);
        }
        if (startsWith(buffer, "demolisci-stazione")){
         demStation();
         print_t(stationsTreeRoot);
        }   
        if (startsWith(buffer, "aggiungi-auto")) addCar();
        if (startsWith(buffer, "rottama-auto")) demCar();
        if (startsWith(buffer, "pianifica-percorso")) planRoute();
    }
    return 0;
}