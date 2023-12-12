#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Gate_record {
    char GateName[1024];
    int GateType;
    int Level;
    int output;
    int Number;
    char outputs[1024];
    char inputs[1024];
    int out;
    struct Gate_record *next;
} Gate_record;

//count spaces in a word
int countSpacesInWord(const char *word) {
    int spaceCount = 0;
    int i;
    for (i = 0; word[i] != '\0'; ++i) {
        if (word[i] == ' ') {
            spaceCount++;
        }
    }
    return spaceCount;
}

//create additional space after each name
void addSpaceToGateNames(Gate_record *head) {
    Gate_record *current = head;

    while (current != NULL) {
        strcat(current->GateName, " ");
        current = current->next;
    }
}

//get rid of commas in structure
void replaceCommasWithSpaces(Gate_record *head) {
    Gate_record *current = head;

    while (current != NULL) {
        char *comma = strchr(current->inputs, ',');
        while (comma != NULL) {
            *comma = ' ';
            comma = strchr(comma + 1, ',');
        }
        strcat(current->inputs, " ");
        current = current->next;
    }
}
//code to add a gate to the linked list
Gate_record *addGateRecord(Gate_record *tail, const char *GateName, int GateType, int Level, const char *outputs, const char *inputs) {
    Gate_record *newNode = (Gate_record *)malloc(sizeof(Gate_record));
    if (newNode == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    strcpy(newNode->outputs, outputs);
    strcpy(newNode->inputs, inputs);
    strcpy(newNode->GateName, GateName);
    newNode->GateType = GateType;
    newNode->next = NULL;
    newNode->Level = Level;
    if (tail == NULL) {
        return newNode;
    } else {
        tail->next = newNode;
        return newNode;
    }
}

//code to get the outputs and inputs
void getOutAndIn(const char* input, char* first_element, int max_element_length, char* rest, int max_rest_length) {
    char* comma_position = strchr(input, ',');

    if (comma_position != NULL) {
        int first_element_length = comma_position - input;
        strncpy(first_element, input + 1, (first_element_length - 1 < max_element_length) ? first_element_length - 1 : max_element_length);
        first_element[(first_element_length - 1 < max_element_length) ? first_element_length - 1 : max_element_length] = '\0';

        strncpy(rest, comma_position + 1, (strlen(comma_position + 1) < max_rest_length) ? strlen(comma_position + 1) : max_rest_length);
        rest[(strlen(comma_position + 1) < max_rest_length) ? strlen(comma_position + 1) : max_rest_length] = '\0';

        int rest_length = strlen(rest);
        if (rest_length >= 2) {
            rest[rest_length - 2] = '\0';
        }
    } else {
        strncpy(first_element, "Error: No comma found in the input string.", max_element_length);
        strncpy(rest, input, (strlen(input) < max_rest_length) ? strlen(input) : max_rest_length);
        rest[(strlen(input) < max_rest_length) ? strlen(input) : max_rest_length] = '\0';
    }
}

//get output from paranetheses
void getOutput(const char* input, char* result, int max_length) {
    char* comma_position = strchr(input, ',');

    if (comma_position != NULL) {
        int first_element_length = comma_position - input;
        strncpy(result, input + 1, first_element_length - 1);
        result[first_element_length - 1] = '\0';
    } else {
        strncpy(result, "Error: No comma found in the input string.", max_length);
    }
}

//code to update a given gate 
void updateGateRecord(Gate_record *head, const char *targetName, int newOutput) {
    Gate_record *current = head;

    while (current != NULL) {
        if (strcmp(current->GateName, targetName) == 0) {

            //current->Level = newLevel;
            current->output = newOutput;
            //current->Number = newNumber;
            return; // Exit the function once the update is done
        }
        current = current->next;
    }

    printf("Gate_record with name '%s' not found.\n", targetName);
}

//main code
void read_words(FILE *f, FILE *inf) {
    char x[1024];
    char last[1024] = "";       // Initialize last to an empty string
    char secondtL[1024] = "";  // Initialize secondToLast to an empty string
    char output[1024];
    Gate_record *gateRecords = NULL;  // Initialize the linked list to NULL
    Gate_record *tail = NULL;  // Keep track of the tail of the linked list    
    
    while (fscanf(f, " %1023s", x) == 1) {

        if (strchr(x, '(') != NULL || strchr(x, ')') != NULL) {
            printf("");
        }

        if (strcmp(last, "input") == 0) {
            char GateName[1024];
            int x_length = strlen(x);
            x[x_length - 1] = '\0';
            strcpy(GateName, x);
            int GateType = 0;

            tail = addGateRecord(tail, GateName, GateType, 0, GateName, "None");
            if (gateRecords == NULL) {
                gateRecords = tail;  // Update the head of the linked list
            }
        }


        if (strcmp(secondtL, "dff") == 0) {
            char GateName[1024];
            strcpy(GateName, last);
            char outputs[1024];
            char inputs[1024];
            getOutAndIn(x, outputs, sizeof(outputs), inputs, sizeof(inputs));
            int GateType = 1;

            tail = addGateRecord(tail, outputs, GateType, 0, outputs, inputs);
            if (gateRecords == NULL) {
                gateRecords = tail;  // Update the head of the linked list
            }
        }

        if (strcmp(secondtL, "dff1") == 0) {
            char GateName[1024];
            strcpy(GateName, last);
            char outputs[1024];
            char inputs[1024];
            getOutAndIn(x, outputs, sizeof(outputs), inputs, sizeof(inputs));
            int GateType = 1;

            tail = addGateRecord(tail, outputs, GateType, 0, outputs, inputs);
            if (gateRecords == NULL) {
                gateRecords = tail;  // Update the head of the linked list
            }
        }

        if (strcmp(secondtL, "nor") == 0) {
            char GateName[1024];
            strcpy(GateName, last);
            char outputs[1024];
            char inputs[1024];
            getOutAndIn(x, outputs, sizeof(outputs), inputs, sizeof(inputs));
            int GateType = 4;

            tail = addGateRecord(tail, outputs, GateType, -1, outputs, inputs);
            if (gateRecords == NULL) {
                gateRecords = tail;  // Update the head of the linked list
            }
        }

        if (strcmp(secondtL, "not") == 0) {
            char GateName[1024];
            strcpy(GateName, last);
            char outputs[1024];
            char inputs[1024];
            getOutAndIn(x, outputs, sizeof(outputs), inputs, sizeof(inputs));
            int GateType = 6;

            tail = addGateRecord(tail, outputs, GateType, -1, outputs, inputs);
            if (gateRecords == NULL) {
                gateRecords = tail;  // Update the head of the linked list
            }
        }

        if (strcmp(secondtL, "and") == 0) {
            char GateName[1024];
            strcpy(GateName, last);
            char outputs[1024];
            char inputs[1024];
            getOutAndIn(x, outputs, sizeof(outputs), inputs, sizeof(inputs));
            int GateType = 3;

            tail = addGateRecord(tail, outputs, GateType, -1, outputs, inputs);
            if (gateRecords == NULL) {
                gateRecords = tail;  // Update the head of the linked list
            }
        }

        if (strcmp(secondtL, "nand") == 0) {
            char GateName[1024];
            strcpy(GateName, last);
            char outputs[1024];
            char inputs[1024];
            getOutAndIn(x, outputs, sizeof(outputs), inputs, sizeof(inputs));
            int GateType = 5;

            tail = addGateRecord(tail, outputs, GateType, -1, outputs, inputs);
            if (gateRecords == NULL) {
                gateRecords = tail;  // Update the head of the linked list
            }
        }

        if (strcmp(secondtL, "or") == 0) {
            char GateName[1024];
            strcpy(GateName, last);
            char outputs[1024];
            char inputs[1024];
            getOutAndIn(x, outputs, sizeof(outputs), inputs, sizeof(inputs));
            int GateType = 2;

            tail = addGateRecord(tail, outputs, GateType, -1, outputs, inputs);
            if (gateRecords == NULL) {
                gateRecords = tail;  // Update the head of the linked list
            }
        }


        if (strcmp(last, "output") == 0) {
            int x_length = strlen(x);
                if (x_length >= 2) {
                    x[x_length - 1] = '\0';
                }
            strcpy(output, x);
        }


        //Update last with the current word and secondtL with last
        strcpy(secondtL, last);
        strcpy(last, x);

    }

    fseek(f, 0, SEEK_SET);

    while (fscanf(f, " %1023s", x) == 1) {

        if (strcmp(last, "output") == 0) {
            int x_length = strlen(x);
            if (x_length >= 2) {
                x[x_length - 1] = '\0';
            }
            updateGateRecord(gateRecords, x, 1);
        }

        strcpy(secondtL, last);
        strcpy(last, x);
    }

    //printf("\nOutput: %s\n", output);
    //updateGateRecord(gateRecords, output, 1);
    addSpaceToGateNames(gateRecords);
    replaceCommasWithSpaces(gateRecords);

    //Assign level
    int c = 0;
    int it = 0;
    Gate_record *outerc = gateRecords;
    Gate_record *innerc = gateRecords;
    while (outerc != NULL){
        innerc = gateRecords;
        while ( innerc != NULL){
            if(strstr(outerc->inputs,innerc->GateName) != NULL && outerc->GateType != 1){
                if((innerc->Level + 1) > outerc->Level){
                    //printf("%s contains %s\n", outerc->inputs, innerc->GateName);
                    outerc->Level = innerc->Level + 1;
                    c = 1;
                }else{
                    outerc->Level = outerc->Level;
                }
            }
            innerc = innerc->next;
        }
        it++;
        if((outerc->next == NULL) && (c == 1)){
            outerc = gateRecords;
            c = 0;
        }else{
            outerc = outerc->next;
        }
    }


    //print the gate records
    printf("\nGate_records in the linked list:\n");
    Gate_record *current = gateRecords;
    int highest_lvl = 0;
    int spaces;
    int bruh = 0;
    while (current != NULL) {
        spaces = countSpacesInWord(current->inputs);
        if(spaces == 3){
            bruh = 1;
        }
        if(current->Level > highest_lvl){
                highest_lvl = current->Level;
        }
        if(current->Level == 2){
        //      printf("GateName: %s, GateType: %d, Level: %d, Output: %s, Number: %d, Fanout: %s, Fanin: %s\n",
        //         current->GateName, current->GateType, current->Level, current->output ? "true" : "false", current->Number, current->outputs, current->inputs);
        }
        printf("GateName: %s, GateType: %d, Level: %d, Output: %s, Fanout: %s, Fanin: %s\n",
               current->GateName, current->GateType, current->Level, current->output ? "true" : "false", current->outputs, current->inputs);

        current = current->next;
    }
    int lvl;
    int gn;
    int total;
    current = gateRecords;
    printf("\nNumber of Gates at each level:\n");
    for(lvl=0; lvl <= highest_lvl; lvl++){
        gn = 0;
        current = gateRecords;
        while (current != NULL) {
            if(current->Level == lvl){
                gn++;
                total++;
            }
            current = current->next;
        }
        printf("Level %d: %d gates\n", lvl, gn);
    }

    printf("\nTotal number of gates: %d\n", total);
    printf("\n");


    //SIMULATION\\

    int a;
    int b;
    int loc = 0;

    //Initialization of dffs to undefined (4)
    current = gateRecords;
    while (current != NULL) {
            if(current->GateType == 1){
                current->out = 4;
            }
            current = current->next;
    }


    //Inputs initialized at each new input
  while (fscanf(inf, " %1023s", x) == 1) {
    //printf("Current word: %s", x);
    //printf(" and current input: %d in decimal and %s in string\n", x[loc], x[loc]);
    loc = 0;
    printf("Inputs: ");
    current = gateRecords;
    while (current != NULL) {
            if(current->GateType == 0){
                if (x[loc] == '0') {
                        current->out = 0;
                        printf("%d", current->out);
                } else {
                        current->out = 1;
                        printf("%d", current->out);
                }

                //current->out = x[loc];
                //printf("%s", x[loc]);
                loc++;
            }
            current = current->next;
    }



    //Post-Initialization       
    for(lvl=1; lvl <= highest_lvl; lvl++){
        outerc = gateRecords;
        //printf("\nLEVEL %d:\n", lvl);
        while (outerc != NULL){
            if(outerc->Level == lvl){ //for each gate at this level
                innerc = gateRecords;
                a = -1;
                b = -1;
                    while ( innerc != NULL){
                        if(strstr(outerc->inputs,innerc->GateName) != NULL){  //if current gate is part of inputs of gate at this level
                            //printf("input located, input is %d from gate %s\n", innerc->out, innerc->GateName);
                            if(a == -1){
                                //printf("a found\n");
                                a = innerc->out;
                            }else{
                                b = innerc->out;
                            }
                        }
                        innerc = innerc->next;
                    }
                //printf("For gate %s the inputs are: %d and %d", outerc->GateName, a, b);
                if(outerc->GateType == 3){ //and
                    if(a == 5 && b == 5){
                        outerc->out = 5;
                    }else if(a == 4 && b == 4){
                        outerc->out = 4;
                    }else if((a==4 && b==5) || (a==5 && b==4)){
                        outerc->out = 0;
                    }else if(a == 5){
                        outerc->out = b ? 5 : 0;
                    }else if(b == 5){
                        outerc->out = a ? 5 : 0;
                    }else if(a == 4){
                        outerc->out = b ? 4 : 0;
                    }else if(b == 4){
                        outerc->out = a ? 4 : 0;
                    }else{
                        outerc->out = a && b;
                    }
                }else if(outerc->GateType == 2){ //or
                    if(a == 5 && b == 5){
                        outerc->out = 5;
                    }else if(a == 4 && b == 4){
                        outerc->out = 4;
                    }else if((a==4 && b==5) || (a==5 && b==4)){
                        outerc->out = 1;
                    }else if(a == 5){
                        outerc->out = b ? 1 : 5;
                    }else if(b == 5){
                        outerc->out = a ? 1 : 5;
                    }else if(a == 4){
                        outerc->out = b ? 1 : 4;
                    }else if(b == 4){
                        outerc->out = a ? 1 : 4;
                    }else{
                        outerc->out = a || b;
                    }
                }else if(outerc->GateType == 5){ //nand
                    if(a == 5 && b == 5){
                        outerc->out = 5;
                    }else if(a == 4 && b == 4){
                        outerc->out = 4;
                    }else if((a==4 && b==5) || (a==5 && b==4)){
                        outerc->out = 1;
                    }else if(a == 5){
                        outerc->out = b ? 4 : 1;
                    }else if(b == 5){
                        outerc->out = a ? 4 : 1;
                    }else if(a == 4){
                        outerc->out = b ? 5 : 1;
                    }else if(b == 4){
                        outerc->out = a ? 5 : 1;
                    }else{
                        outerc->out = !(a && b);
                    }
                }else if(outerc->GateType == 4){ //nor
                    if(a == 5 && b == 5){
                        outerc->out = 5;
                    }else if(a == 4 && b == 4){
                        outerc->out = 4;
                    }else if((a==4 && b==5) || (a==5 && b==4)){
                        outerc->out = 0;
                    }else if(a == 5){
                        outerc->out = b ? 0 : 4;
                    }else if(b == 5){
                        outerc->out = a ? 0 : 4;
                    }else if(a == 4){
                        outerc->out = b ? 0 : 5;
                    }else if(b == 4){
                        outerc->out = a ? 0 : 5;
                    }else{
                        outerc->out = !(a || b);
                    }
                }else if(outerc->GateType == 6){ //not
                    if(a == 4){
                        outerc->out = 5;
                    }else if(a == 5){
                        outerc->out = 4;
                    }else{
                        outerc->out = !a;
                    }
                }
                //printf(" and output is: %d\n", outerc->out);          
            }
            outerc = outerc->next;
        }
    }

    current = gateRecords;
    printf("\nState: ");
    while (current != NULL) {
            if(current->GateType == 1){
                printf("%d", current->out);
            }
            current = current->next;
    }
    printf("\nOutput: ");
    current = gateRecords;
    while (current != NULL) {
            if(current->output == 1){
                printf("%d", current->out);
            }
            current = current->next;
    }
    printf("\n");
    printf("\n");

    //DFF values updated for next run
    outerc = gateRecords;
    while (outerc != NULL){
        innerc = gateRecords;
        while ( innerc != NULL){
            if(strstr(outerc->inputs,innerc->GateName) != NULL && outerc->GateType == 1){
                outerc->out = innerc->out;
                //printf("\nDFF gate %s was assigned out %d from gate %s\n", outerc->GateName, innerc->out, innerc->GateName);
            }
            innerc = innerc->next;
        }
        outerc = outerc->next;

    }

  }
        //printf("\n%s\n", bruh ? "3 inputs cuh" : "Only 2's and 1's"); 
}

void simulate(){

}

int main() {
    //FILE *file = fopen("s35.txt", "r");
    FILE *file = fopen("s27.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    FILE *input_file = fopen("s27_test.txt", "r");
    //FILE *input_file = fopen("s35_test.txt", "r");

    read_words(file, input_file);

    fclose(file);

    return 0;
}
