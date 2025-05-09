#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define BRIGHT_GREEN   "\x1b[92m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define BRIGHT_BLUE    "\x1b[94m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
// 1. Contacts System

// ----- GLOBAL VARIABLES -----
short unsigned nr_contacts = 0, nr_blocked = 0;
struct contact {
    char phone[25]; // phone number of contact
    char label[126]; // label/name of contact
    char company[126]; // company related to contact
    char email[126]; // email address of contact
    char additional[256]; // additional info about contact, such as a short description or bio
} all_contacts[256];
char blocked_contacts[256][25];
// ----------------------------

// The header of the program
void header() { // Creates the header for the program
    printf("-------------------------------\n");
    printf("    "BRIGHT_BLUE"Phone book"RESET"\n");
    printf(""BLUE"1."RESET" View Contacts\n");
    printf(""BLUE"2."RESET" Add New Contact\n");
    printf(""BLUE"3."RESET" Edit Contact\n");
    printf(""BLUE"4."RESET" Remove Existing Contact\n");
    printf(""BLUE"5."RESET" Search By Contact Field\n");
    printf(""BLUE"6."RESET" Block A Phone Number\n");
    printf(""BLUE"7."RESET" View Blocked Phone Numbers\n");
    printf(""BLUE"8."RESET" Unblock Number\n");
    printf(""BLUE"9."RESET" Exit program\n");
    printf("-------------------------------\n");
    printf("Select an option "BLUE"(1-9)"RESET": ");
}

// Fills the contact field with the information in "string"
void fill_field(unsigned int contact_field, int contact, char string[256]) {
    switch (contact_field) {
        case 0: {
            strcpy(all_contacts[contact].phone, string);
            break;
        }
        case 1: {
            strcpy(all_contacts[contact].label, string);
            break;
        }
        case 2: {
            strcpy(all_contacts[contact].company, string);
            break;
        }
        case 3: {
            strcpy(all_contacts[contact].email, string);
            break;
        }
        case 4: {
            string[strlen(string) - 1] = '\0'; // Gets rid of /n at the end
            strcpy(all_contacts[contact].additional, string);
            break;
        }
        default: {
            printf("I don't think it's possible to get here.\n");
        }
    }
}

// Loads contacts into the programs memory
void load_contacts() {
    FILE *f = fopen("C:/Users/themi/CLionProjects/Programming_Project/c-project-menu-Marcu-Mihai-Vlad/c-project-menu-Marcu-Mihai-Vlad/contacts.csv", "r");
    // Change the filename as needed. Remember! It requires the entire file path!
    if(f == NULL) {
        printf("Error reading contacts!\n");
        exit(1);
    }
    char line[1024];
    int contact = 0;
    while(fgets(line, 1024, f)) {
        nr_contacts++;
        char *info = strtok(line, ",");
        int field = 0;
        while(info != NULL) {
            if(strcmp(info, "empty") != 0) { // checks to see if the field is empty, if empty it jumps over the field.
                fill_field(field, contact, info);
            }
            else fill_field(field, contact, "empty");
            field++;
            info = strtok(NULL, ",");
        }
        contact++;
    }
}

// Loads blocked phone numbers into memory
void load_blocked() {
    FILE *f = fopen("C:/Users/themi/CLionProjects/Programming_Project/c-project-menu-Marcu-Mihai-Vlad/c-project-menu-Marcu-Mihai-Vlad/blocked.csv","r");
    // Change the filename as needed. Remember! It requires the entire file path!
    if(f == NULL) {
        printf("Error reading blocked phone numbers!\n");
        exit(1);
    }
    char line[1024];
    while(fgets(line, 1024, f)) {
        char *number = strtok(line, ",");
        int pos = 0;
        while(number != NULL) {
            strcpy(blocked_contacts[pos], number);
            nr_blocked++;
            pos++;
            number = strtok(NULL, ",");
        }
    }
}

// Saves contacts into a file after changes
void save_contacts() {
    FILE *f = fopen("C:/Users/themi/CLionProjects/Programming_Project/c-project-menu-Marcu-Mihai-Vlad/c-project-menu-Marcu-Mihai-Vlad/contacts.csv", "w");
    // Change the filename as needed. Remember! It requires the entire file path!
    if(f == NULL) {
        printf("Error saving contacts!\n");
        return;
    }
    for(int i = 0; i < nr_contacts; i++) {
        fprintf(f, "%s,%s,%s,%s,%s\n", all_contacts[i].phone, all_contacts[i].label, all_contacts[i].company, all_contacts[i].email, all_contacts[i].additional);
    }
    fclose(f);
}

// Saves blocked numbers into a file after changes
void save_blocked() {
    FILE *f = fopen("C:/Users/themi/CLionProjects/Programming_Project/c-project-menu-Marcu-Mihai-Vlad/c-project-menu-Marcu-Mihai-Vlad/blocked.csv","w");
    // Change the filename as needed. Remember! It requires the entire file path!
    if(f == NULL) {
        printf("Error saving blocked phone numbers!\n");
        return;
    }
    for(int i = 0; i < nr_blocked; i++) {
        if(i != 0) fprintf(f, ",");
        fprintf(f, "%s", blocked_contacts[i]);
    }
}

// Prints a list of all contacts with all relevant information
void print_contact_info() {
    for(int i = 0; i < nr_contacts; i++) {
        printf(""BLUE"%d."RESET" "CYAN"Label:"RESET" %s, "CYAN"Phone Number:"RESET" %s", i + 1, all_contacts[i].label, all_contacts[i].phone);
        if(strlen(all_contacts[i].company) > 0 && strcmp(all_contacts[i].company, "empty") != 0) {
            printf(","CYAN" Company:"RESET" %s", all_contacts[i].company);
        }
        if(strlen(all_contacts[i].email) > 0 && strcmp(all_contacts[i].email, "empty") != 0) {
            printf(","CYAN" Email:"RESET" %s", all_contacts[i].email);
        }
        if(strlen(all_contacts[i].additional) > 0 && strcmp(all_contacts[i].additional, "empty") != 0) {
            printf(","CYAN" Additional Info:"RESET" %s", all_contacts[i].additional);
        }
        printf("\n");
    }
}

// ----- Program Functions -----

// 1. Showing list of all contacts
void view_contacts() {
    // show all contacts
    system("cls"); // clears the command prompt
    printf("-------------------------------\n");
    if(nr_contacts > 0) {
        printf("    "BRIGHT_BLUE"All contacts:"RESET"\n");
        print_contact_info();
    }
    else {
        printf("No contact found.\n");
    }
    printf("-------------------------------\n");
    // -------------------------------------

    // leave confirmation
    printf("Press enter to return.\n");
    while(fgetc(stdin) != '\n'){}
}

// 2. Add a new contact
void add_contact() {
    char phone_number[25], label[126], company[126], email[126], additional[256];
    system("cls");  //clears the command prompt
    printf("-------------------------------\n");
    printf("    "BLUE"Adding A New Contact:"RESET"\n");
    // Adding a Phone Number (required)
    while(1) {
        char ok = 1;
        printf("Insert phone number "BLUE"(must be a valid phone number and is required)"RESET": ");
        fgets(phone_number, sizeof(phone_number), stdin);
        if(phone_number[0] == '\n')   ok = 2; // Checks if field is empty
        else {
            // check to see if phone number is, y'know, comprised of numbers. Or of characters found in phone numbers
            for(int i = 0; i < strlen(phone_number) - 1 ; i++) {
                if(!strchr("0123456789 +()-.", phone_number[i])) {
                    ok = 0;
                    break;
                }
            }
        }
        if(ok != 1) {
            if(ok == 0) printf("Phone number contains invalid characters!\n");
            else printf("This field is required!\n"); // aka ok == 2
        }
        else {
            phone_number[strlen(phone_number) - 1] = '\0'; // removing the \n at the end
            break;
        }
    }
    // -------------------------------------

    // Blocked number check
    for(int i = 0; i < nr_blocked; i++) {
        if(strcmp(phone_number, blocked_contacts[i]) == 0) {
            printf("Phone number is blocked, continue anyway? "BLUE"(y/n)"RESET"\n");
            char answer = getchar();
            while(fgetc(stdin) != '\n') {} // removes unwanted characters in console
            if(answer != 'y') {
                return;
            }
        }
    }
    // -------------------------------------

    // Adding a Contact Label (required)
    while(1) {
        printf("Insert label "BLUE"(required)"RESET": ");
        fgets(label, sizeof(label), stdin);
        if(label[0] == '\n') { // Checks if field is empty
            printf("This field is required!\n");
        }
        else {
            label[strlen(label) - 1] = '\0'; // removing the \n at the end
            break;
        }
    }
    // -------------------------------------

    // Adding a Company
    printf("Insert company "BLUE"(press enter to skip)"RESET": ");
    fgets(company, sizeof(company), stdin);
    company[strlen(company) - 1] = '\0'; // removing the \n at the end
    if(strlen(company) == 0) strcpy(company, "empty");
    // -------------------------------------

    // Adding an Email
    printf("Insert email "BLUE"(press enter to skip)"RESET": ");
    fgets(email, sizeof(email), stdin);
    email[strlen(email) - 1] = '\0'; // removing the \n at the end
    if(strlen(email) == 0) strcpy(email, "empty");
    // -------------------------------------

    // Adding Additional Information
    printf("Additional information "BLUE"(press enter to skip)"RESET": ");
    fgets(additional, sizeof(additional), stdin);
    additional[strlen(additional) - 1] = '\0'; // removing the \n at the end
    if(strlen(additional) == 0) strcpy(additional, "empty");
    // -------------------------------------

    // Confirmation
    system("cls");  //clears the command prompt
    printf("-------------------------------\n");
    printf("    "BRIGHT_BLUE"Confirmation:"RESET"\n");
    printf(""CYAN"Label:"RESET" %s", label);
    printf("\n"CYAN"Phone Number:"RESET" %s", phone_number);
    printf("\n"CYAN"Company:"RESET" %s", company);
    printf("\n"CYAN"Email:"RESET" %s", email);
    printf("\n"CYAN"Additional info:"RESET" %s", additional);
    printf("\n-------------------------------");
    printf("\nAdd contact? "BLUE"(y/n)"RESET":\n");
    char c = getchar();
    while(1) {
        if(c == 'y') {
            while(fgetc(stdin) != '\n'){} // getting rid of unnecessary characters in the console

            // copying to global all_contacts
            strcpy(all_contacts[nr_contacts].label, label);
            strcpy(all_contacts[nr_contacts].phone, phone_number);
            strcpy(all_contacts[nr_contacts].company, company);
            strcpy(all_contacts[nr_contacts].email, email);
            strcpy(all_contacts[nr_contacts].additional, additional);
            // -------------------------------------

            nr_contacts ++; // increasing the number of contacts
            printf("Contact added! Press enter to return.\n");
            while(fgetc(stdin) != '\n'){} // in case of keyboard spam before pressing enter
            break;
        }
        return;
    }
    // -------------------------------------

    // Sorting all contacts alphabetically by label
    for(int i = 0; i < nr_contacts; i++) {
        for(int j = 1; j < nr_contacts; j++) {
            for(int k = 0; k < strlen(all_contacts[i].label); k++) { // works similarly to strcmp, totally didn't forget about it and wrote my own version
                if(all_contacts[j].label[k] < all_contacts[j - 1].label[k]) {
                    struct contact temp = all_contacts[j];
                    all_contacts[j] = all_contacts[j - 1];
                    all_contacts[j - 1] = temp;
                    break;
                }
                if(all_contacts[j].label[k] > all_contacts[j - 1].label[k]) break;
            }
        }
    }
    // -------------------------------------
}

//3. Editing an existing contact
void edit_contact() {
    system("cls");
    printf("------------------------------\n");
    printf("    "BRIGHT_BLUE"Editing a contact:"RESET"\n");
    print_contact_info();
    printf("-----------------------------\n");
    while(1) {
        int option = 0;
        printf("Which contact? "BLUE"(1-%d or 0 to return)"RESET"\n", nr_contacts);
        scanf("%d", &option);
        while(getchar() != '\n') {}
        if(option == 0) {
            return;
        }
        option = option - 1;
        if(option > nr_contacts) printf("Invalid option!\n");
        else {
            while(1) {
                short int field;
                system("cls");
                printf("-----------------------------\n");
                printf("1. "CYAN"Label:"RESET" %s  \n", all_contacts[option].label);
                printf("2. "CYAN"Phone Number:"RESET" %s  \n", all_contacts[option].phone);
                printf("3. "CYAN"Company:"RESET" %s  \n", all_contacts[option].company);
                printf("4. "CYAN"Email:"RESET" %s  \n", all_contacts[option].email);
                printf("5. "CYAN"Additional info:"RESET" %s  \n", all_contacts[option].additional);
                printf("-----------------------------\n");
                printf("Which field? "BLUE"(1-5 or 0 to return to main menu)"RESET":\n");
                scanf("%d", &field);
                while(fgetc(stdin) != '\n') {}
                if(field == 0) return;
                if (field > 5 || field < 1) printf("Invalid field!\n");
                else switch(field) {
                    case 1: {
                        while(1) {
                            char new_label[126];
                            printf(""CYAN"Old label:"RESET" %s \n", all_contacts[option].label);
                            printf(""CYAN"New label:"RESET" ");
                            fgets(new_label, sizeof(new_label), stdin);
                            if(strlen(new_label) == 0) printf("This field is required!\n");
                            else {
                                new_label[strlen(new_label) - 1] = '\0';
                                strcpy(all_contacts[option].label, new_label);
                                break;
                            }
                        }
                        break;
                    }
                    case 2: {
                        while(1) {
                            int ok = 1;
                            char new_phone[126];
                            printf(""CYAN"Old phone number:"RESET" %s \n", all_contacts[option].phone);
                            printf(""CYAN"New phone number (must be a valid phone number):"RESET" ");
                            fgets(new_phone, sizeof(new_phone), stdin);
                            if(strlen(new_phone) == 0) {
                                printf("This field is required!\n");
                                ok = 0;
                            }
                            else {
                                new_phone[strlen(new_phone) - 1] = '\0';
                                for(int i = 0; i < strlen(new_phone) - 1 ; i++) {
                                    if(!strchr("0123456789 +()-.", new_phone[i])) {
                                        ok = 0;
                                        break;
                                    }
                                }
                                strcpy(all_contacts[option].phone, new_phone);
                            }
                            if(ok == 1) break;
                        }
                        break;
                    }
                    case 3: {
                        char new_company[126];
                        printf(""CYAN"Old company:"RESET" %s \n", all_contacts[option].company);
                        printf(""CYAN"New company:"RESET" ");
                        fgets(new_company, sizeof(new_company), stdin);
                        new_company[strlen(new_company) - 1] = '\0';
                        if(strlen(new_company) == 0) strcpy(new_company, "empty");
                        strcpy(all_contacts[option].company, new_company);
                        break;
                    }
                    case 4: {
                        char new_email[126];
                        printf(""CYAN"Old email:"RESET" %s \n", all_contacts[option].email);
                        printf(""CYAN"New email:"RESET" ");
                        fgets(new_email, sizeof(new_email), stdin);
                        new_email[strlen(new_email) - 1] = '\0';
                        if(strlen(new_email) == 0) strcpy(new_email, "empty");
                        strcpy(all_contacts[option].email, new_email);
                        break;
                    }
                    case 5: {
                        char new_bio[256];
                        printf(""CYAN"Old Additional Info:"RESET" %s \n", all_contacts[option].email);
                        printf(""CYAN"New Additional Info:"RESET" ");
                        fgets(new_bio, sizeof(new_bio), stdin);
                        new_bio[strlen(new_bio) - 1] = '\0';
                        if(strlen(new_bio) == 0) strcpy(new_bio, "empty");
                        strcpy(all_contacts[option].additional, new_bio);
                        break;
                    }
                    default: {
                        printf("How did you even get here????\n");
                        break;
                    }
                }
                char edit;
                printf("Edit another field? "BLUE"(y/n)"RESET":\n");
                scanf("%c", &edit);
                while(getchar() != '\n') {}
                if(edit != 'y') break;

            }
        }
        printf("Contact edited! Press enter to return.");
        while(fgetc(stdin) != '\n') {}
        return;
    }
}

// 4. Removes one contact
void remove_contact() {
    while(1) {
        system("cls");  //clears the command prompt
        printf("-------------------------------\n");
        printf("    "BRIGHT_BLUE"Removing A Contact:"RESET"\n");
        if(nr_contacts != 0) {
            print_contact_info(); // prints all contacts in a list
            printf("-------------------------------\n");
            printf("Which contact? "BLUE"(1-%d or 0 to return)"RESET":\n",nr_contacts);
            short unsigned choice;
            scanf("%d", &choice);
            while(fgetc(stdin) != '\n') {}  // removing any unwanted character from the console
            if(choice == 0) return;
            if(choice > nr_contacts) printf("Invalid contact!\n");
            else {
                printf("Do you want to remove %s from contacts? "BLUE"(y/n)"RESET":\n", all_contacts[choice - 1].label);
                unsigned char c = getchar();
                if(c == 'y') {
                    for(short unsigned i = choice - 1; i < nr_contacts - 1; i++) {
                        all_contacts[i] = all_contacts[i + 1];
                    }
                    nr_contacts --;
                    printf("Contact removed! Press enter to return.\n");
                    while(fgetc(stdin) != '\n') {}   // removing any unwanted characters from the console
                    break;
                }
            }
        }
        else {
            printf("-------------------------------\n");
            printf("No contacts to remove! Press enter to return.\n");
            while(fgetc(stdin) != '\n') {}
            return;
        }
    }
}

// 5. Search a contact by a selected field
void search_contact() {
    system("cls");
    printf("-------------------------------\n");
    printf("    "BRIGHT_BLUE"Searching Contacts:"RESET"\n");
    if(nr_contacts == 0) {
        printf("-------------------------------\n");
        printf("No contacts to search! Press enter to return.\n");
        while(fgetc(stdin) != '\n') {}
        return;
    }
    // Choosing Search Field
    short int choice = -1;
    while(choice == -1) {
        printf("Search by:\n");
        printf(""BLUE"1."RESET" "CYAN"Phone Number"RESET" \n"BLUE"2."RESET" "CYAN"Label"RESET" \n"BLUE"3."RESET" "CYAN"Company"RESET" \n"BLUE"4."RESET" "CYAN"Email"RESET"\n");
        printf("-------------------------------\n");
        printf("Choose field "BLUE"(1-4 or 0 to return)"RESET":\n");
        scanf("%d", &choice);
        while(fgetc(stdin) != '\n') {}
        if(choice == 0) return;
        if(choice > 4 || choice < 0) printf("Invalid Choice!\n");
    }
    // -------------------------------------

    // Input for searching
    char input[256], copy_input[256];
    printf("Search input: ");
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0'; // getting rid of end-line char
    strcpy(copy_input, input);
    strcpy(input, strlwr(input)); // making input all lower case
    if(strlen(input) == 0) {
        printf("-------------------------------\n");
        printf("Showing all contacts:\n"); //  if input is empty, it just shows all contacts
        print_contact_info();
        printf("-------------------------------\n");
        printf("Press enter to return:");
        while(fgetc(stdin) != '\n') {}
        return;
    }
    // -------------------------------------

    // Search Algorithm
    short int result = 0;
    char *temp;
    system("cls");
    printf("-------------------------------\n");
    printf("    "CYAN"Showing results for"RESET" '%s': \n", copy_input);
    for(int i = 0; i < nr_contacts; i++) { // going through all contacts
        char *contact_info;
        if(choice == 1) { // By Phone Number
            contact_info = malloc(sizeof(char) * (strlen(all_contacts[i].phone) + 1));
            temp = malloc(sizeof(char) * (strlen(all_contacts[i].label) + 1));
            strcpy(temp, all_contacts[i].phone);
            strcpy(contact_info, strlwr(temp));
            free(temp);
        }
        else if(choice == 2) { // By Label
            contact_info = malloc(sizeof(char) * (strlen(all_contacts[i].label) + 1));
            temp = malloc(sizeof(char) * (strlen(all_contacts[i].label) + 1));
            strcpy(temp, all_contacts[i].label);
            strcpy(contact_info, strlwr(temp));
            free(temp);
        }
        else if(choice == 3) { // By Company
            temp = malloc(sizeof(char) * (strlen(all_contacts[i].company) + 1));
            contact_info = malloc(sizeof(char) * (strlen(all_contacts[i].company) + 1));
            strcpy(temp, all_contacts[i].company);
            strcpy(contact_info, strlwr(temp));
            free(temp);
        }
        else if(choice == 4) { // By Email
            temp = malloc(sizeof(char) * (strlen(all_contacts[i].email) + 1));
            contact_info = malloc(sizeof(char) * (strlen(all_contacts[i].email) + 1));
            strcpy(temp, all_contacts[i].email);
            strcpy(contact_info, strlwr(temp));
            free(temp);
        }
        if(strstr(contact_info, input) != NULL) {
            result += 1;
            printf(""BLUE"%d."RESET" Label: %s, Phone Number: %s", result, all_contacts[i].label, all_contacts[i].phone);
            if(strlen(all_contacts[i].company) > 0) {
                printf(", Company: %s", all_contacts[i].company);
            }
            if(strlen(all_contacts[i].email) > 0) {
                printf(", Email: %s", all_contacts[i].email);
            }
            if(strlen(all_contacts[i].additional) > 0) {
                printf(", Additional Info: %s", all_contacts[i].additional);
            }
            printf("\n");
        }
        free(contact_info);
    }
    // -------------------------------------

    // Printing Result
    if(result == 0) { // No results found
        printf("-------------------------------\n");
        printf("No results found. Press enter to return.\n");
        while(fgetc(stdin) != '\n') {}
        return;
    }
    printf("-------------------------------\n");
    printf("Press enter to return.");
    while(fgetc(stdin) != '\n') {}
    // -------------------------------------
}

// 6. Block a phone number
void block_contact() {
    char phone_number[25];
    while(1) {
        system("cls");  //clears the command prompt
        printf("-------------------------------\n");
        printf("    "BRIGHT_BLUE"Blocking Phone Numbers:"RESET"\n");
        printf("Phone number "BLUE"(leave blank to return)"RESET": ");
        fgets(phone_number, 25, stdin);
        if(phone_number[0] == '\n') return;
        char  ok = 1;

        // Valid number check
        for(int i = 0; i < strlen(phone_number) - 1 ; i++) {
            if(!strchr("0123456789 +()-.", phone_number[i])) {
                ok = 0;
                break;
            }
        }
        // -------------------------------------

        phone_number[strlen(phone_number) - 1] = '\0'; // removing the \n at the end of the phone number

        // check to see if number is in contacts
        for(int i = 0; i < nr_contacts; i++) {
            if(strcmp(phone_number, all_contacts[i].phone) == 0) {
                printf("Number already in contacts under name %s, would you like to block and remove them?\n", all_contacts[i].label);
                char c = getchar();
                while(getchar() != '\n') {} // you will never guess what this does
                if(c == 'y') {
                    // blocking the number
                    strcpy(blocked_contacts[nr_blocked] , phone_number);
                    nr_blocked ++;

                    // removing the contact
                    for(short unsigned j = i; j < nr_contacts - 1; j++) {
                        all_contacts[j] = all_contacts[j + 1];
                    }
                    printf("-------------------------------\n");
                    printf("Contact blocked and removed! Press enter to return.\n");
                    while(fgetc(stdin) != '\n') {}  // you already should know what this does by now
                    nr_contacts --;
                    return;
                }
                else break;
            }
        }
        // -------------------------------------

        // If valid phone number, break, else printf
        if(ok == 1) break;
        printf("Invalid phone number!\n");
    }
    // -------------------------------------

    // Blocking confirmation
    printf("Block %s? "BLUE"(y/n)"RESET"\n", phone_number);
    char c = getchar();
    while(getchar() != '\n') {} // getting rid of any unwanted characters in the console
    if(c == 'y') {
        strcpy(blocked_contacts[nr_blocked] , phone_number);
        nr_blocked ++;
        printf("-------------------------------\n");
        printf("Contact blocked! Press enter to return.\n");
        while(getchar() != '\n') {}
    }
    // -------------------------------------
}

// 7. Shows all blocked numbers
void view_blocked() {
    system("cls");  //clears the command prompt
    printf("-------------------------------\n");
    printf("    "BRIGHT_BLUE"All Blocked Numbers:"RESET"\n");
    if(nr_blocked == 0) {
        printf("-------------------------------\n");
        printf("No phone numbers blocked! Press enter to return.\n");
        while(fgetc(stdin) != '\n') {}
        return;
    }
    for(int i = 0; i < nr_blocked; i++) {
        printf(""BLUE"%d."RESET" %s\n",i + 1, blocked_contacts[i]);
    }
    printf("-------------------------------\n");
    printf("Press enter to return.");
    while(fgetc(stdin) != '\n') {}
}

// 8. Unblock a blocked phone number
void unblock() {
    system("cls");
    printf("-------------------------------\n");
    printf("    "BRIGHT_BLUE"Unblocking:"RESET"\n");
    if(nr_blocked != 0) {
        while(1) {
            // Showing all blocked numbers
            printf("Blocked numbers:\n");
            for(int i = 0; i < nr_blocked; i++) {
                printf(""BLUE"%d."RESET" %s\n",i + 1, blocked_contacts[i]);
            }
            // -------------------------------------

            // Unblocking a number
            printf("-------------------------------\n");
            printf("Which number? "BLUE"(1-%d or 0 to return)"RESET"\n", nr_blocked);
            short unsigned choice;
            scanf("%d", &choice);
            while(fgetc(stdin) != '\n') {} // getting rid of any unwanted characters in the console
            if(choice == 0) return;
            if(choice > nr_blocked) printf("Invalid number!\n");
            else {
                printf("Are you sure you want to unblock %s? "BLUE"(y/n)"RESET"\n", blocked_contacts[choice - 1]);
                char c = getchar();
                while(getchar() != '\n') {}
                if(c == 'y') {
                    for(int i = choice - 1; i < nr_blocked; i++) {
                        strcpy(blocked_contacts[i], blocked_contacts[i + 1]);
                    }
                    nr_blocked--;
                    printf("Contact unblocked! Press enter to return.\n");
                    while(getchar() != '\n') {}
                    return;
                }
            }
            // -------------------------------------
        }
    }
    else {
        printf("-------------------------------\n");
        printf("No numbers to unblock!\n");
        while(fgetc(stdin) != '\n') {}
    }
    // -------------------------------------
}
// -----------------------------

// Main
int main(void) { // the program loop
    load_contacts();
    load_blocked();
    while(1) {
        system("cls");
        header();
        short int selection = 0; // required due to scanf fuckery
        scanf("%d", &selection);
        while(getchar() != '\n') {}
        switch (selection) {
            case 1: { // viewing all contacts
                view_contacts();
                break;
            }
            case 2: { // adding a new contact
                add_contact();
                save_contacts();
                break;
            }
            case 3: { // editing a contact
                edit_contact();
                save_contacts();
                break;
            }
            case 4: { // removing an existing contact
                remove_contact();
                save_contacts();
                break;
            }
            case 5: { // searching through contacts
                search_contact();
                break;
            }
            case 6: { // block a phone number
                block_contact();
                save_blocked();
                break;
            }
            case 7: { // view blocked
                view_blocked();
                break;
            }
            case 8: { // unblock a phone number
                unblock();
                save_blocked();
                break;
            }
            case 9: { // exiting the program
                printf("Exiting program");
                save_contacts();
                save_blocked(); // both of these are for redundancy I guess
                return 0;
            }
            default: { // invalid cases
                printf("Invalid selection!\n");
                break;
            }
        }
    }
}