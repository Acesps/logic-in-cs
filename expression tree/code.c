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
    printf("\n\n    Given Infix Expression    : %s  \n Generated Prefix Expression  : %s\n",infix_exp,prefix_exp);
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
                    node->left = NULL;
                else
                    node->left = constructTree_from_prefix(prefix, index);
                node->right = constructTree_from_prefix(prefix, index);
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


/* ---------TASK-4::Function to find the HEIGHT of the generated BINARY PARSE TREE of the given expression------------*/
int maxDepth(struct node* root)
{
   if (root==NULL)
       return 0;
   else
   {
      // compute the depth of each subtree 
       int lDepth = maxDepth(root->left);
       int rDepth = maxDepth(root->right);

       // use the larger one 
       if (lDepth > rDepth)
           return(lDepth+1);
       else return(rDepth+1);
   }
}



/* ---------------------------TASK-5::Function to EVALUATE the BIANRY PARSE TREE------------------------------*/
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



/* ------------------Utility function to STORE VALUES for the atom elements from console---------------------------*/

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
        printf(" %c %c %d \n", i+97,equal,atom[i]);*/
}

/* ---------------------------Utility function to ASSIGN VALUES to the atom elements -----------------------------*/
void assign_values(struct node *root)
{
    if (isAtom(root->character))
     {
          root->value=atom[(int)((root->character)-97)];

          /* uncomment the line below to see individual atom element's value*/
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
}


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
    // input the infix expression
    input_infix_expression();

    // convert infix expression to prefix expression
    infix_to_prefix();

    // construct Binary Parse tree from prefix expression
    int index=0;
              // created a root node to traverse the tree
    struct node* main_root = constructTree_from_prefix(prefix_exp, &index);

    // infix expression by Inorder traversal of the tree
    printf("\n Infix Expression by Inorder Traversal : ");
    inorder(main_root);            //passing the root node
    printf("\n\n");

    // finding the height of the binary tree
    printf(" The height of the Binary Tree is : %d\n\n", maxDepth(main_root));
        //passing the root node

    
    // taking input for evaluation
    printf(" Press 1 to input values and evaluate .\n");
    printf(" Press 2 to exit.\n\n");
    int q;
    scanf("%d",&q);
    while(q==1)
    { 
        input_atom_value();
        assign_values(main_root);

        // evaluating the expression
        printf("\n\nThe truth value of the expression is : ");
        printf("%d\n\n",evaluate(main_root));

        printf(" Press 1 to input values and evaluate .\n");
        printf(" Press 2 to exit.\n\n");
        scanf("%d",&q);
    }
    return 0; 
}
