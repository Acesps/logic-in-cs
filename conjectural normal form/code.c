// C program for expression tree


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

 /*------------------------------------Global declarations------------------------------------------*/
int top=-1;                                             // variable to store top element of the stack
#define SIZE 500                                        // SIZE = 500 as the character array length
char s[SIZE];                                           // character array to implement stack for infix to prefix conversion
char infix_exp[SIZE],prefix_exp[SIZE];                  // character arrays to store infix and prefix expressions
char ch,elem;                                           // utility character variables
int atom[26];                                           // integer array to store values of the atom elements


/* Implementing Stack Functionality */
// =======================================================================================================
 // Function for PUSH operation 
void push(char elem)
{                      
    s[++top]=elem;
}
// Function for POP operation 
char pop()
{                      
    return(s[top--]);
}
// ========================================================================================================


/*-----------------------Function to REVERSE a string using pointers--------------------------------*/
char *strrev(char *str)
{
      char *p1, *p2;
      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}



/*-----------------------Function for OPERATOR PRECEDENCE of Natural Deduction-------------------------*/
int priority(char elem)
{                 
    switch(elem)
    {
    case '#': return 0;
    case ')': return 1;    
    case '>': return 2;  
    case '+':
    case '*': return 3;
    case '~': return 4;
    }
    return 0;
}


/*--------------------------------------------NODE------------------------------------------------------*/
// A struct DEFINING an expression tree node 
struct node
{
    char character;                                         // store the character from character array of the infix/prefix expressions
    struct node* left;                                      // pointer to the left subtree of the node
    struct node* right;                                     // pointer to the right subtree of the node
    struct node* parent;                                    // pointer to the parent of the node
    int value ;                                             // integer variable to store node value during evaluation
};

// A utility function to CREATE a new node of type struct node
struct node* new_Node(int v)
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->left = temp->right = NULL;
    temp->character = v;
    temp->value= -1;                                       // default value of node is -1
    return temp;
};

struct node* HEAD ;
int initial_size=0, cnf_size=0;


/*----------------------------Functions to RETURN TYPE of 'c' in infix or prefix expressions-----------*/

// is an Operator
int isOperator(char c)
{
    if (c == '~' || c == '+' || c == '*' || c == '>' )
        return 1;
    return 0;
}

// is the Negation Operator
int isNegation(char c)
{
    if(c == '~')
        return 1;
    return 0;
}

// is an Atom Element
int isAtom(char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    return 0;
}

/* ----------------TASK-1::Function to generate PREFIX expression FROM the given INFIX expression-----------------*/
void infix_to_prefix()
{                         
    int i=0,k=0;
    while( (ch=infix_exp[i++]) != '\0')
    {
        if(ch!=' ')
        {
            if( ch == ')') push(ch);
            else if(isalnum(ch)) prefix_exp[k++]=ch;
            else if( ch == '(')
            {
                 while( s[top] != ')')
                      prefix_exp[k++]=pop();
                 elem=pop();                                                                                   // Remove 
            }
            else
            {       //  ch is an operator 
                 while( priority(s[top]) >= priority(ch) )
                      prefix_exp[k++]=pop();
                 push(ch);
            }
        }
    }
    while( s[top] != '#')                                                                   // pop from stack till empty 
        prefix_exp[k++]=pop();
    prefix_exp[k]='\0';                                                               // make prefix_exp as valid string 
    strrev(prefix_exp);
    strrev(infix_exp);
    //printf("\n\n    Given Infix Expression    : %s  \n Generated Prefix Expression  : %s\n",infix_exp,prefix_exp);
}


/* ----------------TASK-2::Function to GENERATE BINARY PARSE TREE for the prefix expression-----------------*/
struct node* constructTree_from_prefix(char* prefix, int* index)
{
    if(prefix[*index] == '\0')
    return NULL;
    struct node* node=NULL;
    if(isOperator(prefix[*index]))
        {
            node = new_Node(prefix[*index]);
            (*index)++;

            // recursive call to constructTree_from_prefix()
            if(prefix[*index] != '\0')
            {

              // checking if the operator is '~'
              // if operator is '~' left subtree is initialized as NULL
                if(node->character == '~')
                    {
                      node->left = NULL;
                    }
                else
                    {
                      node->left = constructTree_from_prefix(prefix, index);
                      node->left->parent = node;
                    }

                node->right = constructTree_from_prefix(prefix, index);
                node->right->parent = node;
            }
        }
    else
        {
            node = new_Node(prefix[*index]);
            (*index)++;
        }
    return node;
}


/* -----------TASK-3::Function to do INORDER TRAVERSAL in the expression tree to generate infix expression----------*/
void inorder(struct node *t)
{
    
    if(t)
    {
        if(!isAtom(t->character))
            printf("(");                                                                  // opening parentheses added
        inorder(t->left);        
        printf("%c", t->character);                                                       // print the node character
        inorder(t->right);
        if(!isAtom(t->character))
           printf(")");                                                                   // closing parentheses added
    }
    
}

char ar_cnf[1000];
int k = 0;

/*---------------------------------Generate a CNF array by inorder traversal----------------------------------------*/
void generate_array_cnf(struct node* t)
{

    if(t)
    {
        if(!isAtom(t->character))
           ar_cnf[k++]='(';                                                                  // opening parentheses added
        inorder(t->left);        
        ar_cnf[k++]=t->character;                                                           // print the node character
        inorder(t->right);
        if(!isAtom(t->character))
           ar_cnf[k++]=')';                                                                  // closing parentheses added
    }

}

/*---------------------------------Inserting negation at right child----------------------------------------*/

void insert_neg_right(struct node* t)
{
    struct node* temp = (struct node *)malloc( sizeof ( struct node ));
    temp->character = '~';
    temp->left = NULL;
    temp->parent = t;
    temp->right = t->right;
    t->right = temp;
    temp->right->parent=temp;
}

/*---------------------------------Inserting negation at left child----------------------------------------*/
void insert_neg_left(struct node *t)
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->character = '~';
    temp->left = NULL;
    temp->parent = t;
    temp->right = t->right;
    t->right = temp;
    temp->right->parent = temp;
}

/*---------------------------------Function to convert pasre tree to IMPL_FREE form------------------------*/
void impl_free(struct node *t)
{

    if(t == NULL) return ;
    if(t->character == '>')
    {
      t->character = '+' ;
      insert_neg_left(t);
    }
    impl_free(t->left);
    impl_free(t->right);
   
}


/*---------------------------------Function to convert pasre tree to NNF form------------------------*/
void nnf(struct node* t)
{
    if(t == NULL) return ;
    if(t->character == '~')
    {
        if(t->right->character == '~')
        {
                t->right->right->parent = t->parent;
                if(t == t->parent->right)
                {
                    t->parent->right = t->right->right;
                }
                else
                {
                    t->parent->left = t->right->right;
                }
            if(t->parent->character == '$')
              {
                HEAD = t->right;
              }
            t = t->right->right;
            nnf(t);
        }
        else if(t->right->character == '|')
        {
            insert_neg_left(t->right);
            insert_neg_right(t->right);
            t->right->parent = t->parent;
            if(t == t->parent->right)
            {
                t->parent->right = t->right;
            }
            else
            {
                t->parent->left = t->right;
            }
            if(t->parent->character == '$')
            {
               HEAD = t->right;
             }
            t = t->right;
           
            nnf(t->left);
            nnf(t->right);

            t->character = '*';
            
        }
        
        else if(t->right->character == '*')
        {
            insert_neg_left(t->right);
            insert_neg_right(t->right);
            t->right->parent = t->parent;
            if(t == t->parent->right)
            {
                t->parent->right = t->right;
            }
            else
            {
                t->parent->left = t->right;
            }
            if(t->parent->character == '$')
            {
              HEAD =  t->right;
            }
            t = t->right;
           
            nnf(t->left);
            nnf(t->right);

            t->character = '|';
        }
    }
    else 
    {
        nnf(t->left);
        nnf(t->right);
    }
}


/*---------------------------------Function to check validity of CNF formula------------------------*/
int validity()
{
    int i;
    int len = sizeof(ar_cnf)/sizeof(ar_cnf[0]);
    int start[500], end[500], clauses=0;
    for(int i=0; i<500; i++)
    {
        start[i]=end[i]=-1;
    }
    start[0]=0;
    end[0]=len;
    for(i=0; i<len; i++)
    {
        if(i==0)
        {
            start[clauses]=i;
        }
        else
        {
            if(ar_cnf[i]=='&')
            {
                end[clauses]=i-1;
                clauses++;
                if(ar_cnf[i+1]=='(')
                    start[clauses]=i+1;
            }
        }
    }
    end[clauses]=len-1;
    int in_clause_index=0,neg=0,truth_val;
    while(1)
    {
        if(end[in_clause_index]==-1 && end[in_clause_index]==-1)
        {
            break;
        }
        int atom[200]={0};
        truth_val=0,neg=0;
        for(i=start[in_clause_index]; i<=end[in_clause_index]; i++)
        {
            if(ar_cnf[i]=='~')
            {
                neg=1;
            }
            else if(isAtom(ar_cnf[i]))
            {
                if(neg==1 && atom[ar_cnf[i]]==0)
                {
                    atom[ar_cnf[i]]=-1;
                }
                else if(neg==1 && atom[ar_cnf[i]]==1)
                {
                    truth_val=1;
                    break;
                }
                else if(neg==0 && atom[ar_cnf[i]]==0)
                {
                    atom[ar_cnf[i]]=1;
                }
                else if(neg==0 && atom[ar_cnf[i]]==-1)
                {
                    truth_val=1;
                    break;
                }
                neg=0;
            }
        }
        if(truth_val==0)
        {
            return 0;
        }
        in_clause_index++;
    }
    return 1;
}


/*---------------------------------Function to convert pasre tree to Distr form------------------------*/
struct node* distr(struct node *one,struct node *two)
{
    printf("%c %c\n",one->character,two->character);
    if(one->character=='*')
    {
        struct node* temp;
        temp = (struct node *)malloc( sizeof ( struct node ));        
        temp->character='*';
        temp->left=distr(one->left,two);
        temp->right=distr(one->right,two);
        temp->left->parent=temp;
        temp->right->parent=temp;
        return temp;
    }
    else if(two->character=='*')
    {
        struct node* temp;
        temp = (struct node *)malloc( sizeof ( struct node ));        
        temp->character='*';
        temp->left=distr(one,two->left);
        temp->right=distr(one,two->right);
        temp->left->parent=temp;
        temp->right->parent=temp;
        return temp;
    }
    else
    {
        struct node* temp;
        temp = (struct node *)malloc( sizeof ( struct node ));        
        temp->character='|';
        temp->left=one;
        temp->right=two;
        temp->left->parent=temp;
        temp->right->parent=temp;
        return temp;
    }
}

/*---------------------------------Function to convert pasre tree to CNF form------------------------*/
struct node* cnf(struct node *t)
{
    if(t->character=='|')
    {
        //printf("111\n");
        return distr(cnf(t->left),cnf(t->right));
    }
    if(t->character=='*')
    {
        //printf("1111\n");
        struct node* temp;
        temp = (struct node *)malloc( sizeof ( struct node ));
        
        temp->character = '*';
        temp->left=cnf(t->left);
        temp->right=cnf(t->right);
        //temp->left->parent=temp;
        //temp->right->parent=temp;
        return temp;
        
    }   
    return t;
}


int treeSize(struct node* t)
{
    if(t == NULL)
       return 0;
     else
      return (1 + treeSize(t->left) + treeSize(t->right));
}
/*---------------------------------Function to convert pasre tree to CNF form------------------------*/
void convert_to_cnf()
{
    int index=0;             
    HEAD = (struct node*)malloc(sizeof(struct node));
    HEAD->left = NULL;  
    HEAD->character = '$';
    struct node* main_root ;
    HEAD->right = main_root;
    main_root->parent = HEAD;
    main_root = constructTree_from_prefix(prefix_exp, &index); // created a root node to traverse the tree
    initial_size = treeSize(main_root);                            // initial size of the parse tree

    impl_free(main_root);                                       // implication free
    printf("\n\n Implication free expression by Inorder Traversal : ");
    inorder(main_root);            //passing the root node
    printf("\n\n");

    nnf(main_root);                                             // negation normal form
    printf("\n NNF expression by Inorder Traversal : ");
    inorder(main_root);            //passing the root node
    printf("\n\n"); 

    distr(main_root->left, main_root->right);                   // distr format
    cnf(main_root);                                             // final cnf form
    printf("\n CNF expression by Inorder Traversal : ");
    inorder(main_root);            //passing the root node
    printf("\n\n"); 

    cnf_size = treeSize(main_root);                               // size after converting to CNF

}

void valid_cnf()
{
    getchar();
    char cnf_exp[1000];
    printf("Enter the CNF expression to check for validity\n");
    scanf("%[^\n]s",cnf_exp);

    if(validity() == 1)
        printf("The above expression is a valid expression.\n");
    else
        printf("The above expression is not valid expression.\n");
}

/* ---------------------------TASK-5::Function to EVALUATE the BIANRY PARSE TREE------------------------------
int evaluate(struct node* root)
{
     if (root->value!=-1)                                                                     // base condition 
     {
          return root->value;                                                             
     }
     else
     {
          int left=-1,right=-1;                                                           // default node values

          // recursive call to evaluate() on left and right subtree
          // if operator is '~' left subtree does not exist
          if(root->left != NULL)                               
                left = evaluate(root->left);
          right = evaluate(root->right);

          // evaluations according to the node operator
          int val=-1;

          // 'or' operator
          if(root->character=='+')
          {
              if(left==1||right==1)
                  val = 1;
              else
                  val = 0;
          }

          // 'and' operator
          else if(root->character=='*')
          {
              if(left==1&&right==1)
                  val = 1;
              else
                  val = 0;
          }

          // 'implies' operator
          else if(root->character=='>')
          {
              if(left==1&&right==0)
                  val = 0;
              else
                  val = 1;
          }

          // 'negation' operator
          else if(root->character=='~')
          {
              val = 1-right;
          }
          root->value = val;

          // return the computed value
          return val;
     }
}



/* ------------------Utility function to STORE VALUES for the atom elements from console---------------------------

void input_atom_value()
{
    int i=0,n,val;
    char at,equal;
    printf("     Please input the number of atom elements : ");
    scanf("%d",&n);
    printf("\n");
    printf("     Please input in the following format\n          \tatom=value\n");
    printf("        example :: ");
    printf("a=0\n");
    printf("         \t   b=1\n");
    printf("  -------Atom elements have a default value of zero---------**\n");
    for(i=0;i<26;i++)
        atom[i]=0;    
    getchar();
    for(i=0;i<n;i++)
    {
        scanf("%c",&at);
        scanf("%c",&equal);
        scanf("%d",&val);
        getchar();
        //printf(" %c %c %d \n", at,equal,val);
        atom[(int)(at-97)]=val;
    }
    /*for(i=0;i<26;i++)
        printf(" %c %c %d \n", i+97,equal,atom[i]);
}

/* ---------------------------Utility function to ASSIGN VALUES to the atom elements -----------------------------
void assign_values(struct node *root)
{
    if (isAtom(root->character))
     {
          root->value=atom[(int)((root->character)-97)];

          // uncomment the line below to see individual atom element's value
          //printf("%c %d\n", (root->character),atom[(int)((root->character)-97)] );
     }
     else
     {
          // recursive call to assign_values() on left and right subtree
          // if operator is '~' left subtree does not exist
          if(root->left != NULL)
                assign_values(root->left);
          assign_values(root->right);
     }
}*/


/* -------------------------Utility function to INPUT the infix expression from console---------------------------*/
void input_infix_expression()
{
    printf("\n  Input the Infix Expression  : ");
    scanf("%[^\n]s",infix_exp);
    printf("%s",infix_exp);
    push('#');
    strrev(infix_exp);
}

/* -------------------------------------------program DRIVER function :: main() ----------------------------------*/
int main()
{
    printf(" A common cause of segmentation fault here would be not entering in the format specified in the Documentation/Assignment Instruction Sheet.\n");
    printf(" Please take care of it.");

    // input the infix expression
    /*input_infix_expression();

    // convert infix expression to prefix expression
    infix_to_prefix();    

    // convert to CNF
    convert_to_cnf();

    //comparing sizes
    printf("The size of the input expression was ");
    printf("%d\n", initial_size);
    printf("The size of the CNF expression is ");
    printf("%d\n", cnf_size);
*/
    printf("\n input 1 to check validity : ");
    int choice=0;
    scanf("%d",&choice);
    if(choice == 1)
      valid_cnf();
    return 0; 
}
