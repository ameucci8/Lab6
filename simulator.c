#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

//Defining the structure of the Gates
typedef struct Gate_record {
    char GateName[1024];
    int GateType;
    int Level;
    int output;
    int Number;
    char outputs[1024];
    char inputs[1024];
    int out;
    int fanin[2];
    struct Gate_record *next;
} Gate_record;

//Used to count spaces in given word
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

//Used to add space at end of names for indexing error purposes
void addSpaceToGateNames(Gate_record *head) {
    Gate_record *current = head;

    while (current != NULL) {
        strcat(current->GateName, " ");
        current = current->next;
    }
}

//Used to replace commas with spaces of parse error purposes
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

//Code to add a Gate to the linked list
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

//Used to get the inputs and outputs of a particular gate
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

//Used to grab the output of a gate
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

//Used to edit parameters of a particular gate given its name
void updateGateRecord(Gate_record *head, const char *targetName, int newOutput) {
    Gate_record *current = head;

    while (current != NULL) {
        if (strcmp(current->GateName, targetName) == 0) {

            current->output = newOutput;
            return;
        }
        current = current->next;
    }

    printf("Gate_record with name '%s' not found.\n", targetName);
}

//////////////////////
// PARSE & SIMULATE //
/////////////////////
void read_words(FILE *f, FILE *inf) {
    char x[1024];
    char last[1024] = "";
    char secondtL[1024] = "";
    char output[1024];
    Gate_record *gateRecords = NULL;
    Gate_record *tail = NULL;
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
                gateRecords = tail;
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
                gateRecords = tail;
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
                gateRecords = tail;
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
                gateRecords = tail;
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
                gateRecords = tail;
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
                gateRecords = tail;
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
                gateRecords = tail;
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
                gateRecords = tail;
            }
        }
        if (strcmp(last, "output") == 0) {
            int x_length = strlen(x);
                if (x_length >= 2) {
                    x[x_length - 1] = '\0';
                }
            strcpy(output, x);
        }

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

    addSpaceToGateNames(gateRecords);
    replaceCommasWithSpaces(gateRecords);

    //Assign numbers to each gate for indexing purposes
    Gate_record *current = gateRecords;
    int number = 0;
    while (current != NULL) {
            current->Number = number;
            number++;
            current = current->next;
    }

    //Assign levels and use Gate.number to assign numeric values to fanin
    int c = 0;
    int it = 0;
    int fan = 0;
    Gate_record *outerc = gateRecords;
    Gate_record *innerc = gateRecords;
    while (outerc != NULL){
        fan = 0;
        innerc = gateRecords;
        while ( innerc != NULL){
            if(strstr(outerc->inputs,innerc->GateName) != NULL){
                outerc->fanin[fan] = innerc->Number;
                fan++;
            }
            if(strstr(outerc->inputs,innerc->GateName) != NULL && outerc->GateType != 1){
                if((innerc->Level + 1) > outerc->Level){

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

    //Print the list of Gate in the linked list
    printf("\nGate_records in the linked list:\n");
    current = gateRecords;
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

        }
        printf("GateName: %s, GateType: %d, Level: %d, Output: %s, Number: %d, Fanout: %s, Fanin: %s\n",
               current->GateName, current->GateType, current->Level, current->output ? "true" : "false", current->Number, current->outputs, current->inputs);
        current = current->next;
    }

    //Print the number of gates at each level
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

    //////////////
    //Simulation//
    /////////////
    int a;
    int b;
    int loc = 0;
    int outf[total];
    int ii;

    //Initialize array of outputs for each gate to undefined 
    for(ii = 0; ii < total; ii++) {
        outf[ii] = 4;
    }

    //For each word in input file... 
  while (fscanf(inf, " %1023s", x) == 1) {
    loc = 0;
    printf("Inputs: ");
    current = gateRecords;

    //Assign input to each input gate in circuit
    while (current != NULL) {
            if(current->GateType == 0){
                if (x[loc] == '0') {
                        outf[current->Number] = 0;
                        printf("%d", outf[current->Number]);

                } else {
                        outf[current->Number] = 1;
                        printf("%d", outf[current->Number]);

                }

                loc++;
            }
            current = current->next;
    }

    //For each level in the circuit, assign the output of gate at this 
    //current level using outputs of gates from previous level
    for(lvl=1; lvl <= highest_lvl; lvl++){
        outerc = gateRecords;

        while (outerc != NULL){
            if(outerc->Level == lvl){

                a = outf[outerc->fanin[0]];
                b = outf[outerc->fanin[1]];
                if(outerc->GateType == 3){

                    if(a == 4 && b == 4){
                        outf[outerc->Number] = 4;

                    }else if(a == 4){
                        outf[outerc->Number] = b ? 4 : 0;

                    }else if(b == 4){
                        outf[outerc->Number] = a ? 4 : 0;

                    }else{
                        outf[outerc->Number] = a && b;

                    }
                }else if(outerc->GateType == 2){

                    if(a == 4 && b == 4){
                        outf[outerc->Number] = 4;

                    }else if(a == 4){
                        outf[outerc->Number] = b ? 1 : 4;

                    }else if(b == 4){
                        outf[outerc->Number] = a ? 1 : 4;

                    }else{
                        outf[outerc->Number] = a || b;

                    }
                }else if(outerc->GateType == 5){

                    if(a == 4 && b == 4){
                        outf[outerc->Number] = 4;

                    }else if(a == 4){
                        outf[outerc->Number] = b ? 4 : 1;

                    }else if(b == 4){
                        outf[outerc->Number] = a ? 4 : 1;

                    }else{
                        outf[outerc->Number] = !(a && b);

                    }
                }else if(outerc->GateType == 4){

                    if(a == 4 && b == 4){
                        outf[outerc->Number] = 4;

                    }else if(a == 4){
                        outf[outerc->Number] = b ? 0 : 4;

                    }else if(b == 4){
                        outf[outerc->Number] = a ? 0 : 4;

                    }else{
                        outf[outerc->Number] = !(a || b);

                    }
                }else if(outerc->GateType == 6){

                    if(a == 4){
                        outf[outerc->Number] = 4;

                    }else{
                        outf[outerc->Number] = !a;

                    }
                }
            }
            outerc = outerc->next;
        }
    }

    //Print current state
    current = gateRecords;
    printf("\nState: ");
    while (current != NULL) {
            if(current->GateType == 1){
                printf("%d", outf[current->Number]);
            }
            current = current->next;
    }

    //Print output
    printf("\nOutput: ");
    current = gateRecords;
    while (current != NULL) {
            if(current->output == 1){
                printf("%d", outf[current->Number]);

            }
            current = current->next;
    }
    printf("\n");
    printf("\n");

    //Update the DFF from outputs of gates after end of clock cycle
    outerc = gateRecords;
    while (outerc != NULL){
        if(outerc->GateType == 1){
            outf[outerc->Number] = outf[outerc->fanin[0]];
        }

        outerc = outerc->next;
    }

  }

}

//MAIN//

int main() {
    FILE *file2 = fopen("s35.txt", "r");
    FILE *file1 = fopen("s27.txt", "r");
    FILE *input_file1 = fopen("s27_test.txt", "r");
    FILE *input_file2 = fopen("s35_test.txt", "r");

    if (file1 == NULL || file2 == NULL || input_file1 == NULL || input_file2 == NULL) {
        perror("Error opening file");
        return 1;
    }

    struct timeval start_time, end_time;
    double execution_time;
    gettimeofday(&start_time, NULL);

    read_words(file1, input_file1);
    //read_words(file2, input_file2);
    fclose(file1);

    gettimeofday(&end_time, NULL);
    execution_time = (end_time.tv_sec - start_time.tv_sec) +
                     (end_time.tv_usec - start_time.tv_usec) / 1e6;
    printf("Execution Time: %f seconds\n", execution_time);
    return 0;
}
