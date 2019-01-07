#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure contact information
typedef struct contactInfo{

    char fname[15];
    char lname[15];
    char contactNo[3][15];
    char emailID[20];
}contact;

//Structure AVLNode
typedef struct avlNode{
    contact data;
    struct avlNode * right;
    struct avlNode * left;
    int height;
}node;

//Structure Queue
typedef struct queue{
    node * data;
    struct queue* next;
}queueNode;

    //Defined Functions
    node * insert(node *,contact , int*);
    node * insert_node(contact);
    int getHeight(node * );
    int maxInt(int  , int );
    int minInt(int  , int );
    int isBalanced(node *);
    int getCase(node * );
    node * rotationLeft(node * );
    node * rotationRight(node * );
    void preOrder(node * );
    void inOrder(node * );

    ////////////////////////////////////
    queueNode * searchAVL(node * , char []);
    queueNode * getQueue(queueNode * ,node *, char[]);
    queueNode * insertQueue(node *, queueNode *);
    int printQueue(queueNode *);
    void printNode(contact);

    ////////////////////////////////////
    node *deleteAVL(node *, node*);
    node * deleteRotation(node * );
    node * find_succ(node *root);

    ///////////////////////////////////
    contact getContact();
    int hashFunction(char*);
    contact editContact(contact );
    void fileWrite(char [] ,node * []);
    node* deQueue(queueNode** );

    //Main function
int main() {

    //Declaration Section
    char filename[15];
    FILE *fin = NULL;
    contact val;
    char ch;
    node* hasArray[26];
    char str[15];
    node * head = NULL;
    int j;
    for(j=0;j<26 ; j++)
        hasArray[j] = NULL;

    j=1;
    int*check= &j;

    //Execution Section
    printf("\nEnter the filename : ");
    scanf("%s",filename);

    do{
        fin = fopen(filename,"rb");

        if(fin == NULL){
            printf("\nFile isn't exist!!!");
            printf("\nDo you want to create new file(y/n) ?");
            scanf("%s",&ch);

            if(ch == 'y'){//Will create a new file if it is not exist
                    printf("\nEnter new filename : ");
                    scanf("%s",filename);
                    break;
            }
            else{//Will ask for another new file if it is not exist
                printf("\nEnter another filename : ");
                scanf("%s",filename);
                fin = fopen(filename, "rb");
                continue;
            }
        }
        else{
            fread(&val , sizeof(val) ,1 ,fin);//Read the file

            while(feof(fin)==0){
                //printNode(val);
                hasArray[hashFunction(val.fname)] = insert(hasArray[hashFunction(val.fname)] , val ,check);
                fread(&val , sizeof(val) ,1 ,fin);
            }
            fclose(fin);
            break;
        }
    }while(fin == NULL);

    int n;
    contact c;
    while(1){

        printf("\n\nMenu");
        printf("\n1. Insert a new Contact");
        printf("\n2. Search Contacts");
        printf("\n3. Edit a contact");
        printf("\n4. Delete a contact");
        printf("\n5. View all Contacts");
        printf("\n0. Save and Exit");
        printf("\n\n\tChoice : ");
        scanf("%d",&n);

        switch(n){

            case 1 : //Insert Contact
                c = getContact();

                do{
                    n = hashFunction(c.fname);
                    node * head = hasArray[n];
                    head = insert(head,c,check);
                    hasArray[n] = head;

                    if(*check == 0){
                        printf("\nContact already exist with same name !!!");
                        printf("\nDo you want to modify contact name(y/n) ?");
                        scanf("%s",&ch);

                        if(ch == 'y'){
                            printf("\nEnter the first name : ");
                            scanf("%s",c.fname);
                            printf("\nEnter the last name : ");
                            scanf("%s",c.lname);
                            //hasArray[hashFunction(c.fname)] = insert(hasArray[hashFunction(c.fname)] , c ,check);
                            continue;
                        }
                        else{
                            printf("\nInsert unsuccessful...");
                            break;
                        }
                    }
                    else{
                        printf("\nInsert Successful...");
                    }
                }while(*check == 0);

                break;

            case 2 :  //Search Contact
                printf("\nEnter the first name of contact which you want to search : ");
                scanf("%s",str);
                queueNode* que = searchAVL(hasArray[hashFunction(str)] , str);
                printQueue(que);
                break;

            case 3 ://Edit Contact
                printf("\nEnter the first name of contact which you want to edit : ");
                scanf("%s",str);
                que = NULL;
                que = searchAVL(hasArray[hashFunction(str)] , str);
                c.fname[0] = '\0';
                if(que == NULL){
                    printf("\nThere is nothing to Edit");
                }
                else{

                    int i = printQueue(que);
                    if(i == 1){
                        c = (que->data)->data;
                        hasArray[hashFunction(str)] = deleteAVL(hasArray[hashFunction(str)],que->data);
                    }
                    else{
                        printf("\nSelect a contact to Edit : ");
                        scanf("%d" , &n);
                        while(n>1){
                            que = que->next;
                            n--;
                        }
                        if(n>0){
                            c = (que->data)->data;
                            hasArray[hashFunction(str)] = deleteAVL(hasArray[hashFunction(str)],que->data);
                        }
                    }
                    if(c.fname[0] != '\0'){
                        c = editContact(c);
                        do{
                            hasArray[hashFunction(c.fname)] = insert(hasArray[hashFunction(c.fname)] , c ,check);
                            if(*check == 0){
                                printf("\nContact already exist with same name !!!");
                                printf("\nDo you want to modify contact name(y/n) ?");
                                scanf("%s",ch);

                                if(ch == 'y'){
                                    printf("\nEnter the first name : ");
                                    scanf("%s",&c.fname);
                                    printf("\nEnter the last name : ");
                                    scanf("%s",&c.lname);
                                    hasArray[hashFunction(c.fname)] = insert(hasArray[hashFunction(c.fname)] , c ,check);
                                }
                                else{
                                    break;
                                }
                            }
                            else{
                                break;
                            }
                        }while(*check == 0);
                    }
                }

                break;

            case 4 : //Delete contact
                printf("\nEnter the first name of contact which you want to delete : ");
                scanf("%s",str);
                que = NULL;
                que = searchAVL(hasArray[hashFunction(str)] , str);
                if(que != NULL){
                    int i = printQueue(que);
                    if(i == 1)
                        hasArray[hashFunction(str)] = deleteAVL(hasArray[hashFunction(str)],que->data);
                    else{
                        printf("\nSelect a contact to delete : ");
                        scanf("%d" , &n);
                        while(n>1){
                            que = que->next;
                            n--;
                        }
                        if(n>0){
                            hasArray[hashFunction(str)] = deleteAVL(hasArray[hashFunction(str)],que->data);
                            printf("\nDelete Successful");
                        }
                        else{
                            printf("Oops...Record not Deleted...");
                        }
                    }
                }
                else {
                    printf("\nThere is nothing to be delete.!!!");
                }
                break;

            case 5 : //View List
                printf("\nContacts : ");
                int i =0;
                while(i < 26){
                    inOrder(hasArray[i]);
                    i++;
                }
                break;

            case 0 : //File Storing and Exit
                fileWrite(filename,hasArray);
                printf("\nBye Bye...");
                return;

        }
    }
}

int hashFunction(char* fname){//Hash function
    return (fname[0] - 'a');
}

////Return the height of the AVL tree
int getHeight(node * part){
    if(part == NULL)
        return -1;

    return part->height;
}

////Returns maximum of both Integer
int maxInt(int i , int j){
    return (i>j) ? i : j;
}
////Returns minimum of both Integer
int minInt(int i , int j){
    return (i<j) ? i : j;
}

////Check if the tree at part node is balanced or not if balanced then return 1 otherwise 0
int isBalanced(node * part){

    int i = getHeight(part->right)-getHeight(part->left);
    if(i>1 || i<-1)
        return 0;
    else
        return 1;
}

////Returns the Integer which suggest which rotation we need to do to make height balanced
int getCase(node * head){

    node * rchild = head->right;
    node * lchild = head->left;
    if(getHeight(head->right) - getHeight(head->left)>1){

        if(getHeight(rchild->left) > getHeight(rchild->right))
            return 3;//RL
        else return 4;//RR
    }
    else{
        if(getHeight(lchild->left) < getHeight(lchild->right))
            return 2;//LR
        else return 1;//LL
    }

}

////Rotate the tree at left
node * rotationLeft(node * head){

    node * temp = head;
    head = head->right;
    temp->right = head->left;
    head->left = temp;
    temp->height = maxInt(getHeight(temp->right) , getHeight(temp->left)) + 1;
    head->height = maxInt(getHeight(head->right) , getHeight(head->left)) + 1;
    //head->height++;

    return head;
}

////Rotate the tree at right
node * rotationRight(node * head){

    node * temp = head;
    head = head->left;
    temp->left = head->right;
    head->right = temp;
    temp->height = maxInt(getHeight(temp->right) , getHeight(temp->left)) + 1;
    head->height = maxInt(getHeight(head->right) , getHeight(head->left)) + 1;
    return head;
}

////Print in Pre-Order
void preOrder(node * root){
    if(root!=NULL){
        printNode(root->data);
        preOrder(root->left);
        preOrder(root->right);
    }
}

////Print in In-Order
void inOrder(node * root){
    if(root!=NULL){
        inOrder(root->left);
        printf("\n");
        printNode(root->data);
        inOrder(root->right);
    }
}


////Insert in Queue for Search in AVL Tree
queueNode * insertQueue(node * data , queueNode * head){

    queueNode * temp = NULL;
    if(head != NULL){

        for(temp = head ; temp->next != NULL ; temp = temp->next);

        temp->next = (queueNode *)malloc(sizeof(queueNode));
        temp = temp->next;
        temp->data = data;
        temp->next = NULL;
    }
    else {

        head = (queueNode *)malloc(sizeof(queueNode));
        head->data = data;
        head->next = NULL;
    }

    return head;
}

////Print Whole Queue using For loop
int printQueue(queueNode * head){

    queueNode * temp = NULL;
    int i=1;
    if(head != NULL){
        for(temp = head , i=1; temp!=NULL ; temp = temp->next ,i++){
            printf("\n%d.",i);
            printNode(temp->data->data);
        }
    }
    return i-1;
}

////Print the node or Contact Info
void printNode(contact c){
    printf("\n\tFirst name : %s ",c.fname);
    printf("\n\tLast name : %s ",c.lname);
    printf("\n\tContact number 1 : %s ",c.contactNo[0]);
        if(c.contactNo[1][0] != '\0'){
        printf("\n\tContact number 2 : %s",c.contactNo[1]);

        if(c.contactNo[2][0] != '\0')
            printf("\n\tContact number 3 : %s",c.contactNo[2]);
        }
    printf("\n\tEmail ID: %s",c.emailID);
}

//Compare the contact with first and last name
int compareContact(contact c1 , contact c2){

    int fcmp = strcmp(c1.fname , c2.fname);
    if(fcmp < 0){
        return -1;//C1 < C2
    }
    else if(fcmp > 0){
        return 1;//C1 > C2
    }
    else {//C1 = C2

        int lcmp = strcmp(c1.lname , c2.lname);
        if(lcmp < 0){//C1 < C2
            return -1;
        }
        else if(lcmp > 0){
            return 1;//C1 > C2
        }
        else return 0;//C1 = C2
    }
}

//Search the contact in AVL tree
queueNode * searchAVL(node * head , char n[]){
    if(head!=NULL){
        if(memcmp(n ,head->data.fname ,strlen(n)) < 0){
            return searchAVL(head->left , n);
        }
        else if(memcmp(n ,head->data.fname ,strlen(n)) > 0){
            return searchAVL(head->right , n);
        }
        else{
            queueNode * que = NULL;
            return getQueue(que ,head , n);
        }
    }
    else{
        printf("\nNo Result Found : ");
        return NULL;
    }
}

//Returns the linkedlist of required data
queueNode * getQueue(queueNode * que ,node * head , char n[]){

    if(head != NULL){

            if(memcmp(n ,head->data.fname ,strlen(n)) > 0){
                que = getQueue(que , head->right , n);
                return que;
            }
            else if(memcmp(n ,head->data.fname ,strlen(n)) < 0){
                que = getQueue(que ,head->left , n);
                return que;
            }
            else{
                que = getQueue(que ,head->left , n);
                if(memcmp(n ,head->data.fname ,strlen(n)) == 0)
                    que = insertQueue(head ,que);

                que = getQueue(que , head->right , n);
            }

    }
    return que;
}

//Basic insert function to insert in AVL tree
node * insert(node * head , contact val , int * check){
    if(head == NULL){
    *check == 1;
    return insert_node(val);
}

    else if(compareContact(head->data , val) < 0){
        head->right = insert(head->right , val ,check);
    }

    else if(compareContact(head->data , val) > 0){
        head->left = insert(head->left , val ,check);
    }
    else{
        *check = 0;
        return head;
    }

    head->height = maxInt(getHeight(head->right) , getHeight(head->left)) + 1;

    if(isBalanced(head) == 1){
        return head;
    }
    else{
        switch(getCase(head)){
            case 1 :
                head = rotationRight(head);
                break;

            case 2 :
                head->left = rotationLeft(head->left);
                head = rotationRight(head);
                break;

            case 3 :
                head->right = rotationRight(head->right);
                head = rotationLeft(head);
                break;

            case 4 :
                head = rotationLeft(head);
        }
    }
    return head;

}

//Insert and memory allocate to a node
node * insert_node(contact c){

    node * temp = (node *)malloc(sizeof(node));
    temp->data = c;
    temp->left = NULL;
    temp->right = NULL;
    temp->height = 0;

    return temp;

}
//Gets the contact from the user
contact getContact(){

    char ch;

    contact temp;
    printf("\nFirst name :");
    scanf("%s",temp.fname);
    printf("\nLast name :");
    scanf("%s",temp.lname);
    printf("\nContact number 1:");
    scanf("%s",temp.contactNo[0]);
    printf("\nDo you want to insert another contact number? (y/n) : ");
    scanf("%s",&ch);
    if(ch == 'y'){
        printf("\nContact number 2:");
        scanf("%s",temp.contactNo[1]);

        printf("\nDo you want to insert another contact number? (y/n) : ");
        scanf("%s",&ch);
        if(ch == 'y'){
                printf("\nContact number 3:");
                scanf("%s",temp.contactNo[2]);
        }
        else
            temp.contactNo[2][0] = '\0';

    }
    else
    {
        temp.contactNo[1][0] = '\0';
        temp.contactNo[2][0] = '\0';
    }

    printf("\nEmail ID:");
    scanf("%s",temp.emailID);

    return temp;
}

//Delete function from
node * deleteAVL(node * root , node * nodeAddress)
{
    if(root!=NULL){
        if(root == nodeAddress){

            if(root->left == NULL && root->right == NULL){
                free(root);
                return NULL;
            }

            else if(root->left == NULL || root->right == NULL){

                if(root->left != NULL){
                    node * curr = root->left;
                    free(root);
                    return curr;
                }
                else{
                    node * curr = root->right;
                    free(root);
                    return curr;
                }
            }

            else{
                node * con = find_succ(root->right);
                root->data = con->data;
                root->right = deleteAVL(root->right , con);
                //return root;
            }
        }
        else{
            if(compareContact(root->data , nodeAddress->data) < 0){
                root->right = deleteAVL(root->right , nodeAddress);
                //return root;
            }
            else{
                root->left = deleteAVL(root->left , nodeAddress);
                //return root;
            }
        }
    }

    if(root == NULL){
        return root;
    }
    root->height = maxInt(getHeight(root->right) , getHeight(root->left)) + 1;

    if(isBalanced(root) == 1){
        return root;
    }
    else{
        switch(getCase(root)){
            case 1 :
                root = rotationRight(root);
                break;

            case 2 :
                root->left = rotationLeft(root->left);
                root = rotationRight(root);
                break;

            case 3 :
                root->right = rotationRight(root->right);
                root = rotationLeft(root);
                break;

            case 4 :
                root = rotationLeft(root);
                break;
        }
        return root;
    }
}

////Find the left-most element(Succesor) of the tree
node * find_succ(node *root)
{
    if(root->left != NULL)
        return find_succ(root->left);
    else
        return root;
}

contact editContact(contact c){
    int n;
    char ch;

    printf("\n1. Edit First Name");
    printf("\n2. Edit Last Name");
    printf("\n3. Edit an existing contact");
    printf("\n4. Insert new contact number");
    printf("\n5. Edit an email ID");
    printf("\n\t Choice:");
    scanf("%d" ,&n);

    switch(n)
    {
        case 1:
            printf("\nCurrent first name : %s",c.fname);
            printf("\nEnter new first name : ");
            scanf("%s",c.fname);
            break;

        case 2:
            printf("\nCurrent last name : %s",c.lname);
            printf("\nEnter new last name : ");
            scanf("%s",c.lname);
            break;

        case 3:
            if(c.contactNo[1][0] == '\0' && c.contactNo[2][0] == '\0'){
                printf("\nCurrent contact number : %s",c.contactNo[0]);
                printf("\nEnter new contact number : ");
                scanf("%s",c.contactNo[0]);
            }
            else if(c.contactNo[2][0] == '\0'){
                printf("\nCurrent contact number 1: %s",c.contactNo[0]);
                printf("\nCurrent contact number 2: %s",c.contactNo[1]);
                printf("\nWhich contact number which you want to edit ? : ");
                scanf("%d",&n);

                if(n==1){
                    printf("\nEnter new contact number 1: ");
                    scanf("%s",c.contactNo[0]);
                }
                else if(n == 2)
                {
                    printf("\nEnter new contact number 2: ");
                    scanf("%s",c.contactNo[1]);
                }
                else
                    printf("\nWrong choice!!!");
            }
            else{
                printf("\nCurrent contact number 1: %s",c.contactNo[0]);
                printf("\nCurrent contact number 2: %s",c.contactNo[1]);
                printf("\nCurrent contact number 3: %s",c.contactNo[2]);

                printf("\nWhich contact number which you want to edit ? : ");
                scanf("%d",&n);

                if(n==1){
                    printf("\nEnter new contact number 1: ");
                    scanf("%s",c.contactNo[0]);
                }
                else if(n == 2)
                {
                    printf("\nEnter new contact number 2: ");
                    scanf("%s",c.contactNo[1]);
                }
                else if(n == 3)
                {
                    printf("\nEnter new contact number 3: ");
                    scanf("%s",c.contactNo[2]);
                }

                else
                    printf("\nWrong choice!!!");

            }
            break;

        case 4:
                if(c.contactNo[1][0] == '\0'){
                    printf("\nEnter new contact number 2: ");
                    scanf("%s",c.contactNo[1]);

                    printf("\nDo you want to add another contact number?(y/n) : ");
                    scanf("%s",&ch);

                    if(ch == 'y'){
                        printf("\nEnter new contact number 3: ");
                        scanf("%s",c.contactNo[2]);
                    }
                }

                else if( c.contactNo[2][0] == '\0'){
                    printf("\nEnter new contact number 3: ");
                    scanf("%s",c.contactNo[2]);
                }

                else{
                    printf("\nYou can't insert more than 3 numbers in one contact");
                }
                break;

        case 5:
            printf("\nCurrent email id : %s",c.emailID);
            printf("\nEnter new email id : ");
            scanf("%s",c.emailID);
            break;

        default:
            printf("\nWrong choice!!!");
            break;
    }
    return c;
}


void fileWrite(char filename[] ,node * hasArray[]){

    contact val;
    node * root;
    queueNode* que = NULL;
    node * temp;

    FILE *fout = fopen(filename , "wb");
    int i;
    for(i=0 ; i<26 ; i++){

        root = hasArray[i];

        if(root != NULL){
            que = insertQueue(root , que);

            while(que != NULL)
            {
                temp = deQueue(&que);
                val = temp->data;
                fwrite(&val,sizeof(contact),1,fout);

                if(temp->left!=NULL)
                    que = insertQueue(temp->left ,que);
                if(temp->right != NULL)
                    que = insertQueue(temp->right , que);
                free(temp);

            }
        }

    }
    fclose(fout);
}

node* deQueue(queueNode** que){

    node * temp = (*que)->data;
    *que = (*que)->next;

    return temp;
}
