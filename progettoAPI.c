#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
char buffer[MAX_LENGTH];
int stationsCounter = 0;
int removedStation = -1;
int removedCar = -1;

typedef struct StationAndRange {
    unsigned int distance;
    unsigned int range;
} StationAndRange;

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
Station* removeStation(Station* root, int distance){
    if (root == NULL) {
        return root;  // Tree is empty or value not found
    }

    // branch left
    if (distance < root->distance) {
        root -> left = removeStation(root->left, distance);
    // branch right
    } else if (distance > root->distance) {
        root -> right = removeStation(root->right, distance);
    // found node to be removed
    } else {
        removedStation = root -> distance;
        //printf("root found: %d", root -> distance);
        // Case 1: Node has no children (leaf node)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // Case 2: Node has one child
        else if (root->left == NULL) {
            Station* temp = root -> right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Station* temp = root -> left;
            free(root);
            return temp;
        }
        // Case 3: Node has two children: replace root with smallest node in the right subtree, then remove that node
        Station *parent = root;
        Station *curr = root -> right;
        while (curr -> left != NULL){
            curr = curr -> left;
            if (parent -> distance == root -> distance)
                parent = parent -> right;
            else 
                parent = parent -> left;
        }
        root -> distance = curr->distance;
        root -> cars = curr -> cars;
        if (curr -> right != NULL){     // min in right branch can't have left children
            parent -> left = curr -> right;
            free(curr);
        }
        else {
            free(curr);
            parent -> left = NULL;
        }
    }
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
        (*carsRoot) -> amount = 1;
    }else {
        Car* curr = *carsRoot;
        Car* parent = NULL;

        while (curr != NULL) {
            parent = curr;
            if (range < curr -> range) {
                curr = curr->left;
            } else if (range > curr -> range){
                curr = curr->right;
            } else{     // if exact range already present, increase count and return
                curr -> amount++;
                return;
            }
        }

        if (range < parent->range) {
            parent -> left = (Car*) malloc(sizeof(Car));
            parent -> left -> range = range;
            parent -> left ->  amount = 1;
        } else {
            parent -> right = (Car*) malloc(sizeof(Car));
            parent -> right -> range = range;
            parent -> right -> amount = 1;
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
    while (cars->left != NULL) {
        cars = cars->left;
    }
    return cars;
}
int findMaxCar(Car* root){
    // assumes there is always at least one car in all stations
    while (root -> right != NULL)
        root = root -> right;
    return root -> range;
}
Car* removeCar(Car* root, int range){
    if (root == NULL) {
        return root;  // Tree is empty or value not found
    }

    // branch left
    if (range < root->range) {
        root -> left = removeCar(root->left, range);
    // branch right
    } else if (range > root->range) {
        root -> right = removeCar(root->right, range);
    // found node to be removed
    } else {
        removedCar = root -> range;
        if (root -> amount > 1){    //if already more than 1 present, just reduce amount
            root -> amount--;
            return NULL;
        }
        // Case 1: Node has no children (leaf node)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // Case 2: Node has one child
        else if (root->left == NULL) {
            Car* temp = root -> right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Car* temp = root -> left;
            free(root);
            return temp;
        }
        // Case 3: Node has two children: replace root with smallest node in the right subtree, then remove that node
        Car *parent = root;
        Car *curr = root -> right;
        while (curr -> left != NULL){
            curr = curr -> left;
            if (parent -> range == root -> range)
                parent = parent -> right;
            else 
                parent = parent -> left;
        }
        root -> range = curr->range;
        if (curr -> right != NULL){     // min in right branch can't have left children
            parent -> left = curr -> right;
            free(curr);
        }
        else {
            free(curr);
            parent -> left = NULL;
        }
    }
    return root;
}
/**********   end binary tree stuff    **********/

/**********      pinafica percorso stuff     **************/
void inorderTraversal(Station* root, StationAndRange* arrayBST, int* ind, int start, int finish) {
    if(root) {
        //store the left subtree
        inorderTraversal(root->left, arrayBST, ind, start, finish);

        //store current root value
        if (root -> distance >= start && root -> distance < finish || root -> distance > finish && root -> distance <= start){
            arrayBST[*ind].distance = root -> distance;
            arrayBST[*ind].range = findMaxCar(root -> cars);
            (*ind)++;
        }

        //store the right subtree
        inorderTraversal(root->right, arrayBST, ind, start, finish);
    }
}
StationAndRange* bstToArrayOfSums(Station* root, int start, int finish){
    int ind = 0;
    StationAndRange *arrayBST = (StationAndRange*) malloc((stationsCounter) * sizeof(StationAndRange));
    inorderTraversal(root, arrayBST, &ind, start, finish);
    return arrayBST;
}

void insertionSortLR(StationAndRange* array){
    for (int i = 1; i < stationsCounter; i++){
        StationAndRange curr = array[i];
        int j = i - 1;
        while (j >= 0 && array[j].distance + array[j].range > curr.distance + curr.range){
            array[j + 1] = array[j];    //swapping to right place
            j--;
        }
        array[j + 1] = curr;
    }
    for (int k = 0; k < stationsCounter; k++)
        printf("%d ", array[k].distance);
}
void insertionSortRL(StationAndRange* array){
    for (int i = 1; i < stationsCounter; i++){
        StationAndRange curr = array[i];
        int j = i - 1;
        while (j >= 0 && array[j].distance - array[j].range > curr.distance - curr.range){
            array[j + 1] = array[j];    //swapping to right place
            j--;
        }
        array[j + 1] = curr;
    }
    for (int k = 0; k < stationsCounter; k++)
        printf("%d ", array[k].distance);
}
/* Returns the last station before the finish station if a valid route exists, -1 otherwise */
int findNextStation(StationAndRange* arr, int start, int finish){
    //finds earliest station - with ref km0 - such that (maxRange of Cars* + station -> distance) > finish
    printf("STAR: %d. FINISH: %d. ARRAY IM WORKING WITH: \n", start, finish);
    for (int i = 0; i < stationsCounter; i++)
        printf("%d,%d: %d;  ", arr[i].distance, arr[i].range, - arr[i].distance + arr[i].range);
    printf("\n");

    int low = 0; int high = stationsCounter - 1, result = -1;
    printf("high: %d, low: %d\n", high, low);
    //if (arr[high].distance + arr[high].range < finish) return result;   //no viable station
    // find first occurance of finish, or first value above it
    if (start < finish){
        while(low <= high){
            int mid = low + (high - low)/2;
            printf("high: %d, low: %d, m: %d\n", high, low, mid);
            if ((mid == 0 || arr[mid - 1].distance + arr[mid - 1].range < finish) && arr[mid].distance + arr[mid].range >= finish){
                result = mid;
                break;
            }
            else if (arr[mid].distance + arr[mid].range < finish){
                low = mid + 1;
            }
            else{
                high = mid - 1;
            }
        }
    } else {
        while(low <= high){
                int mid = low + (high - low)/2;
                printf("high: %d, low: %d, m: %d\n", high, low, mid);
                if ((mid == 0 || - arr[mid - 1].distance + arr[mid - 1].range < start) && - arr[mid].distance + arr[mid].range >= -start){
                    result = mid;
                    break;
                }
                else if (- arr[mid].distance + arr[mid].range < -start){
                    low = mid + 1;
                }
                else{
                    high = mid - 1;
                }
            }
    }
    // find closest station to km0
    int minDistance = arr[result].distance;
    result++;
    while (result < stationsCounter){
        if (arr[result].distance < finish)
            if (arr[result].distance < minDistance) 
                minDistance = arr[result].distance;
        result++;
    }
    return minDistance;
}
/**********************************************************/
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
    stationsCounter++;
    printf("aggiunta\n");
}
void demStation(){
    int distance;
    sscanf(buffer, "demolisci-stazione %d", &distance);
    Station* s = removeStation(stationsTreeRoot, distance);
    stationsTreeRoot = s;
    if (removedStation == -1)
        printf("non demolita\n");
    else {
        stationsCounter--;
        printf("demolita\n");
    }
    removedStation = -1;
}
void addCar(){
    int distance, range;
    sscanf(buffer, "aggiungi-auto %d %d", &distance, &range);
    Station* station = findStation(stationsTreeRoot, distance);
    printf("Station: %d", station -> distance);
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
    if (s != NULL){
        removeCar(s -> cars, range);
        if (removedCar == -1)
            printf("non rottamata\n");
    } else{
        printf("rottamata\n");
        removedCar = -1;
    }
}
void planRoute(){
    int start, finish, finnish;
    StationAndRange* arr;
    sscanf(buffer, "pianifica-percorso %d %d", &start, &finish);
    finnish = finish; 

    int *stations = (int*) malloc(sizeof(int) * (stationsCounter));
    int i = 0;
    if (start < finish){
        while (start < finish){
            arr = bstToArrayOfSums(stationsTreeRoot, start, finish);
            insertionSortLR(arr);
            finish = findNextStation(arr, start, finish);
            if (finish == -1) {
                printf("nessun percorso\n");
                return;
            }
            stations[i] = finish;
            i++;
        }
    }
    else if (start > finish){
        while (start > finish){
            arr = bstToArrayOfSums(stationsTreeRoot, start, finish);
            insertionSortRL(arr);
            start = findNextStation(arr, finish, start);
            if (start == -1) {
                printf("nessun percorso\n");
                return;
            }
            stations[i] = start;
            i++;
        }
    }
    //deallocare array
    free(arr);
    printf("STAZIONI PERCORSE:\n");
    for (int j = 0; j < i; j++){
        printf("%d ", stations[i - j - 1]);
    }
    printf("%d ", finish);
    printf("\n");
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
            //print_t(stationsTreeRoot);
        }
        if (startsWith(buffer, "demolisci-stazione")){
            demStation();
            //print_t(stationsTreeRoot);
        }   
        if (startsWith(buffer, "aggiungi-auto")) addCar();
        if (startsWith(buffer, "rottama-auto")) demCar();
        if (startsWith(buffer, "pianifica-percorso")) {
            print_t(stationsTreeRoot);
            planRoute();
        }
    }
    return 0;
}