#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 10000
char buffer[MAX_LENGTH];
int stationsCounter = 0;
int removedStation = -1;
int removedCar = -1;
int ind;

typedef struct ResultIndex{
    int stationIndex, resultIndex;
} ResultIndex;

typedef struct StationAndRange {
    int distance;
    int range;
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
            if (parent -> distance == root -> distance) //first cycle of while loop: go right (take right branch)
                parent = root -> right;
            else                                        //afterwards, go left (find min)
                parent = parent -> left;
        }
        root -> distance = curr->distance;
        root -> cars = curr -> cars;
        if (parent == root)     // if parent and curr dont move, root -> right is the min of the right branch
            parent -> right = curr -> right;
        else{
            parent -> left = curr -> right;
        }
        free(curr);
    }
    return root;
}
Station* findStation(Station* root, int distance) {
    if (root == NULL)
        return root;
    if (root -> distance == distance)
        return root;

    if (distance < root->distance) {
        return findStation(root->left, distance);
    } else {
        return findStation(root->right, distance);
    }
}

void addCarToCars(Car** carsRoot, Car* car){
    if (*carsRoot == NULL){
        *carsRoot = car;
    }else {
        Car* curr = *carsRoot;
        Car* parent = NULL;
        while (curr != NULL) {
            //printf("CURR: %d", curr -> range);
            parent = curr;
            if (car -> range < curr -> range) {
                curr = curr->left;
            } else if (car ->range > curr -> range){
                curr = curr->right;
            } else{     // if exact range already present, increase count and return
                curr -> amount++;
                free(car);
                return;
            }
        }

        if (car ->range < parent->range) {
            parent -> left = car;
            parent -> left -> range = car -> range;
            parent -> left ->  amount = 1;
        } else {
            parent -> right = car;
            parent -> right -> range = car -> range;
            parent -> right -> amount = 1;
        }
    }
}
Car* findCar(Car* cars, int range){
    if (cars == NULL) {
        return cars;
    }
    if (cars -> range == range){
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
    if (root == NULL) return 0;
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
        removedCar = 1;
        if (root -> amount > 1){    //if already more than 1 present, just reduce amount
            root -> amount--;
            return root;
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
            if (parent -> range == root -> range) //first cycle of while loop: go right (take right branch)
                parent = root -> right;
            else                                        //afterwards, go left (find min)
                parent = parent -> left;
        }
        root -> range = curr -> range;
        root -> amount = curr -> amount;
        if (parent == root)     // if parent and curr dont move, root -> right is the min of the right branch
            root -> right = curr -> right;
        else{
            parent -> left = curr -> right;
        }
        free(curr);
    }
    return root;
}
/**********   end binary tree stuff    **********/

/**********      pinafica percorso stuff     **************/
void printAllNodes(Station* root){
    if(root) {
        //store the left subtree
        printAllNodes(root->left);

        //store current root value
        printf("%d, ", root -> distance);
        
        //store the right subtree
        printAllNodes(root->right);
    }
}
void inorderTraversal(Station* root, StationAndRange* arrayBST, int start, int finish) {
    if(root) {
        //store the left subtree
        inorderTraversal(root->left, arrayBST, start, finish);

        //store current root value
        int distance = root -> distance;
        if (start < finish){
            if (distance >= start && distance < finish) { // if within start-finish range
                arrayBST[ind].distance = distance;
                arrayBST[ind].range = findMaxCar(root->cars);
                ind++;
            }
        }
        else{
            if (distance >= finish && distance < start) { // if within start-finish range
                arrayBST[ind].distance = distance;
                arrayBST[ind].range = findMaxCar(root->cars);
                ind++;
            }
        }
        //store the right subtree
        inorderTraversal(root->right, arrayBST, start, finish);
    }
}
StationAndRange* treeToArray(int start, int finish){
    //printf("stationsCounter is %d\n", stationsCounter);
    StationAndRange *arrayBST = (StationAndRange*) malloc((stationsCounter) * sizeof(StationAndRange)); // segfault is here
    inorderTraversal(stationsTreeRoot, arrayBST, start, finish);
    return arrayBST;
}
void insertionSort(StationAndRange* array){       //sorts the array by distance + range
    for (int i = 1; i < ind; i++){
        StationAndRange curr = array[i];
        int j = i - 1;
        while (j >= 0 && array[j].distance + array[j].range > curr.distance + curr.range){
            array[j + 1] = array[j];    //swapping to right place
            j--;
        }
        array[j + 1] = curr;
    }
}

int getClosestStation(StationAndRange* arr, int top, int key){
    int low = 0, high = top - 1, result = -1;
    for (int i = 0; i <= high; i++)
        //printf("%d ", arr[i].distance);
    //printf("\n");
    while(low <= high){
            int mid = low + (high - low)/2;
            //printf("key: %d, high: %d, low: %d, m: %d\n", key, high, low, mid);
            if ((mid == 0 || arr[mid - 1].distance < key) && arr[mid].distance >= key){
                result = mid;
                break;
            }
            else if (arr[mid].distance < key){
                low = mid + 1;
            }
            else{
                high = mid - 1;
            }
    }
    //printf("result %d, ind: %d\n", result, ind);
    return result == -1 ? -1 : arr[result].distance;
}

/* Returns the last station before the finish station if a valid route exists, -1 otherwise */
int findNextStation(StationAndRange* arr, int start, int finish, ResultIndex* resultStations, int resultCounter){
    // printf("START: %d, %d. - FINISH: %d. - IND: %d - ARRAY IM WORKING WITH: \n", start, 6969, finish, ind);
    // for (int i = 0; i < ind; i++)
    //     if (arr[i].distance != 0) 
    //         printf("%d,%d: %d;  ", arr[i].distance, arr[i].range, arr[i].distance - arr[i].range);
    // printf("\n\n");

    int low = 0, high = ind - 1, result = -1;
    if (start < finish){
        // find first occurrence of finish, or first value above it. -1 if no value above finish is found
        while(low <= high){
            int mid = low + (high - low)/2;
            //printf("high: %d, low: %d, m: %d\n", high, low, mid);
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
        if (result == -1) return -1;

        int minStation = arr[result].distance;  //closest station to km0
        int newInd = result;
        result++;
        while (result < ind){
            //if (arr[result].distance > finish) break;    // went over the route range don't think this should ever occur
            if (arr[result].distance < minStation) minStation = arr[result].distance;                     // new min found
            result++;
        }
        //printf("MINDISTANCE: %d\n", maxDistance);
        ind = newInd;  // cut array of sums to result
        return minStation;
    } else {
        Station* station = findStation(stationsTreeRoot, start);
        int key = start - findMaxCar(station -> cars);    // key is furthest reachable station
        //printf("key: %d, arr[ind]: %d\n", key, findMaxCar(station -> cars));
        while(low <= high){
            int mid = low + (high - low)/2;
            //printf("high: %d, low: %d, m: %d\n", high, low, mid);
            if ((mid == 0 || arr[mid - 1].distance < key) && arr[mid].distance >= key){
                result = mid;
                break;
            }
            else if (arr[mid].distance < key){
                low = mid + 1;
            }
            else{
                high = mid - 1;
            }
        }
        // if no reachable station found, return -1
        if (result == -1) return -1;   
        resultStations[resultCounter].resultIndex = result;  
        // else pick the one among the reachable ones that allows to reach the furthest station
        // next step: if tie, select closest to km0

        StationAndRange minStation = arr[result];
        //printf("First reachable station: %d, %d\n", arr[result].distance, arr[result].range);
        int minNextStation = getClosestStation(arr, result, minStation.distance - minStation.range);
        //printf("Minnexstation: %d\n", minNextStation);
        ind--;
        while (ind >= result){
            int s = getClosestStation(arr, result, arr[ind].distance - arr[ind].range);
            if ((s != -1 && s <= minNextStation) || minNextStation == -1){
                minNextStation = s;
                minStation = arr[ind];
                resultStations[resultCounter].stationIndex = ind;
            }
            //printf("arr[ind]: %d, s: %d, minNextStation: %d, result: %d\n", arr[ind].distance, s, minNextStation, result);
            ind--;
        }
        ind++;
        return minStation.distance;
    }
}
/**********************************************************/
void addStation(){
    int km, numOfCars, range;
    Car* carsRoot = NULL;

    //read km, numOfCars
    sscanf(buffer, "aggiungi-stazione %d %d", &km, &numOfCars);
    //printf("aggiungi-stazione %d %d   \n", km, numOfCars);

    //if station already present, do nothing
    if (findStation(stationsTreeRoot, km) != NULL){
        printf("non aggiunta\n");
        return;
    }

    if (numOfCars != 0){    // add cars to station
        //find index of third space
        int count = 0, i = 0;
        while (count < 3){
            if (buffer[i] == ' ') count++;
            i++;
        }

        //read the car milages (in kms :P)
        char *string = (char*) malloc(numOfCars * sizeof(char) * 15);
        char *stringPointer = string;
        strcpy(string, buffer);
        string += i;    //set string pointer to the third int
        for (i = 0; i < numOfCars; i++){
            int shift = 0;
            if (sscanf(string, "%d%n", &range, &shift) != 1) {
                break;
            }
            //printf("Range: %d\n", range);
            Car* car = (Car*) malloc(sizeof(Car));
            car -> range = range;
            car -> amount = 1;
            car -> left = NULL;
            car -> right = NULL;
            addCarToCars(&carsRoot, car);
            string += shift;
        }
        free(stringPointer);
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
    stationsTreeRoot = removeStation(stationsTreeRoot, distance);
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
    if (station != NULL){
        Car* car = (Car*) malloc(sizeof(Car));
        car -> range = range;
        car -> amount = 1;
        car -> left = NULL;
        car -> right = NULL;
        addCarToCars(&(station -> cars), car);
        printf("aggiunta\n");
    } else
        printf("non aggiunta\n");
}
void demCar(){
    int distance, range;
    sscanf(buffer, "rottama-auto %d %d", &distance, &range);
    Station* s = findStation(stationsTreeRoot, distance);
    if (s != NULL){
        s -> cars = removeCar(s -> cars, range);
        if (removedCar != -1){
            printf("rottamata\n");
            removedCar = -1;
            return;
        }
    }
    printf("non rottamata\n");
}
void planRoute(){
    int start, finish, finnish, resultCounter = 0;
    ResultIndex *stations = (ResultIndex*) malloc(sizeof(ResultIndex) * (stationsCounter));
    ind = 0;

    sscanf(buffer, "pianifica-percorso %d %d", &start, &finish);
    finnish = start < finish ? finish : start;
    //printf("here    \n");
    StationAndRange* arr = treeToArray(start, finish);
    if (start <= finish){
        insertionSort(arr);    // sort by distance + range
        while (start < finish){
            finish = findNextStation(arr, start, finish, NULL, 0);
            if (finish == -1) {
                printf("nessun percorso\n");
                return;
            }
            stations[resultCounter].stationIndex = finish;
            resultCounter++;
        }
        for (int j = 0; j < resultCounter; j++)
            printf("%d ", stations[resultCounter - j - 1].stationIndex);
        printf("%d", finnish);
    } else {
        while (start > finish){
            start = findNextStation(arr, start, finish, stations, resultCounter);
            if (start == -1) {
                printf("nessun percorso\n");
                return;
            }
            resultCounter++;
        }
        stations[resultCounter].stationIndex = 0;
        for (int i = resultCounter - 1; i >= 0; i--){
            int j = stations[i].stationIndex; //arr index of result
            j--;
            //printf("First found station: %d, array[i+1]: %d\n", arr[j].distance, arr[stations[i+1].stationIndex].distance);
            while (j >= stations[i].resultIndex){
                //printf("arr[j]: %d, arr[j].key: %d, yes/no: %d\n", arr[j].distance, arr[j].distance - arr[j].range, arr[j].distance - arr[j].range <= arr[stations[i+1].stationIndex].distance);
                if (arr[j].distance - arr[j].range <= arr[stations[i+1].stationIndex].distance)
                    stations[i].stationIndex = j;
                j--;
            }
            // printf("new Res: %d", arr[stations[i].stationIndex].distance);
            // printf("\n");
        }
        printf("%d ", finnish);
        for (int j = 0; j < resultCounter; j++)
            printf("%d ", arr[stations[j].stationIndex].distance);
    }
    printf("\n");
    //deallocare array
    free(arr);
    free(stations);
}

int main() {
    // Read a line from standard input
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (startsWith(buffer, "aggiungi-stazione")) {
            addStation();
            //printAllNodes(stationsTreeRoot);
            //print_t(stationsTreeRoot);
        }
        if (startsWith(buffer, "demolisci-stazione")){
            //printAllNodes(stationsTreeRoot);
            //print_t(findStation(stationsTreeRoot, 4827));
            demStation();
            //print_t(stationsTreeRoot);
        }
        if (startsWith(buffer, "aggiungi-auto")) addCar();
        if (startsWith(buffer, "rottama-auto")) demCar();
        if (startsWith(buffer, "pianifica-percorso")) {
            //print_t(stationsTreeRoot);
            //printAllNodes(stationsTreeRoot);
            planRoute();
        }

    }
    return 0;
}