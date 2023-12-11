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
    struct Gate_record *next;
} Gate_record;

void addSpaceToGateNames(Gate_record *head) {
    Gate_record *current = head;

    while (current != NULL) {
        strcat(current->GateName, " ");
        current = current->next;
    }
}

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


void updateGateRecord(Gate_record *head, const char *targetName, int newLevel, int newOutput, int newNumber) {
    Gate_record *current = head;

    while (current != NULL) {
        if (strcmp(current->GateName, targetName) == 0) {

            current->Level = newLevel;
            current->output = newOutput;
            current->Number = newNumber;
            return; // Exit the function once the update is done
        }
        current = current->next;
    }

    printf("Gate_record with name '%s' not found.\n", targetName);
}

void read_words(FILE *f) {
    char x[1024];
    char last[1024] = "";       // Initialize last to an empty string
    char secondtL[1024] = "";  // Initialize secondToLast to an empty string
    char output[1024];
    Gate_record *gateRecords = NULL;  // Initialize the linked list to NULL
    Gate_record *tail = NULL;  // Keep track of the tail of the linked list

    while (fscanf(f, " %1023s", x) == 1) {
        //printf("Current word: %s\n", x);
        //printf("Previous word: %s\n", last);
        //printf("Second-to-last word: %s\n", secondToLast);

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

        //updateGateRecord(gateRecords, output, -1, 1, 0);      

        //Update last with the current word and secondtL with last
        strcpy(secondtL, last);
        strcpy(last, x);

    }
    printf("\nOutput: %s\n", output);
    updateGateRecord(gateRecords, output, -1, 1, 0);
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
                    printf("%s contains %s\n", outerc->inputs, innerc->GateName);
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



    printf("\nGate_records in the linked list:\n");
    Gate_record *current = gateRecords;
    while (current != NULL) {
        printf("GateName: %s, GateType: %d, Level: %d, Output: %s, Number: %d, Fanout: %s, Fanin: %s\n",
               current->GateName, current->GateType, current->Level, current->output ? "true" : "false", current->Number, current->outputs, current->inputs);

        current = current->next;
    }
}

int main() {
    FILE *file = fopen("s35.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    read_words(file);

    fclose(file);

    return 0;
}
