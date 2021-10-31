#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum op { String, Query, Star, Plus, Or, And, Class, Dot, Exc, Et, Id} ;


typedef union Node Node;
typedef struct Symbol Symbol;
typedef struct Array Array;
typedef struct SymbolTable SymbolTable;
typedef struct NodeCount NodeCount;
struct Array {
    Node **elements;
    int length;
};

struct String  { enum op type;  char *string; int len; };
struct Query   { enum op type;  Node *children[1]; };
struct Star    { enum op type;  Node *children[1]; };
struct Plus    { enum op type;  Node *children[1]; };
struct Or      { enum op type;  Node *children[2]; };
struct And     { enum op type;  Node *children[2]; };
struct Class   { enum op type;  char *array; };
struct Dot     { enum op type; };
struct Exc     { enum op type;  Node *children[1]; };
struct Et      { enum op type;  Node *children[1]; };
struct Id      { enum op type; Symbol *symbol; };

union Node {
    enum op type;
    struct String String;
    struct Query Query;
    struct Star Star;
    struct Plus Plus;
    struct Or Or;
    struct And And;
    struct Class Class;
    struct Dot Dot;
    struct Exc Exc;
    struct Et Et;
    struct Id Id;
};

struct NodeCount{
    int String;
    int Query;
    int Star;
    int Plus;
    int Or;
    int And;
    int Class;
    int Dot;
    int Exc;
    int Et;
    int Id; 
};


#define new(type)			mkNode(sizeof(struct type),type)

Node *mkNode(size_t size,enum op type)
{
    Node *node= calloc(1, size);
    node->type= type;
    return node;
}

Node *mkString(char *s)
{
    Node *node= new(String);
    node->String.string= strdup(s);
    node->String.len=strlen(s);
    return node;
}

Node *mkId(Symbol *s){

    Node *node= new(Id);
    node->Id.symbol=s;
    return node;
}



Node *mkQuery(Node *n)
{
    Node *node= new(Query);
    node->Query.children[0]= n;
    return node;
}

Node *mkOr(Node *node1, Node *node2)
{
    Node *node= new(Or);
    node->Or.children[0]= node1;
    node->Or.children[1]= node2;
    return node;
}

Node *mkAnd(Node *node1, Node *node2)
{
    Node *node= new(And);
    node->And.children[0]= node1;
    node->And.children[1]= node2;
    return node;
}

Node *mkStar(Node *n)
{
    Node *node= new(Star);
    node->Star.children[0]= n;
    return node;
}

Node *mkClass(char* str)
{
    Node *node= new(Class);
    node->Class.array= str;
    return node;
}

Node *mkPlus(Node *n)
{
    Node *node= new(Plus);
    node->Plus.children[0]= n;
    return node;
}

Node *mkDot()
{
    Node *node= new(Dot);
    return node;
}

Node *mkExc(Node *n)
{
    Node *node= new(Exc);
    node->Exc.children[0]= n;
    return node;
}

Node *mkEt(Node *n)
{
    Node *node= new(Et);
    node->Et.children[0]= n;
    return node;
}


NodeCount nodeCount={
    .String=0,
    .Query=0,
    .Star=0,
    .Plus=0,
    .Or=0,
    .And=0,
    .Class=0,
    .Dot=0,
    .Exc=0,
    .Et=0,
    .Id=0 
};

struct Symbol{
  char* name;
  Node *rule;
};

struct SymbolTable {
    Symbol **elements;
    int length;
};

Node *_checktype(Node *object, enum op type)
{
    if (object->type == type) return object;
    fprintf(stderr, "\naccesing type %i as if it were a %i\n", object->type, type);
    exit(1);
    return 0;
}

#define get(object, type, member)	(_checktype(object, type)->type.member)

void printVariableName(enum op type, NodeCount count, int increment){
    char number[50] ;
    char *dest=malloc(30);
    size_t n;
    switch(type){
        case String :
        if(increment==1)nodeCount.String++;
        number[0]=nodeCount.String+'0';
        n = strlen(number);
        strncpy(dest, "string", 6);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;
        case Query :
        if(increment==1)nodeCount.Query++;
        number[0]=nodeCount.Query+'0';
        n = strlen(number);
        strncpy(dest,"query",5);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;
        
        case Star:
        if(increment==1) nodeCount.Star++;
        number[0]=nodeCount.Star+'0';
        n = strlen(number);
        strncpy(dest,"star",4);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;

        case Plus :
        if(increment==1) nodeCount.Plus++;
        number[0]=nodeCount.Plus+'0';
        n = strlen(number);
        strncpy(dest,"plus",4);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;

        case Or :
        if(increment==1)   nodeCount.Or++;
        number[0]=nodeCount.Or+'0';
        n = strlen(number);
        strncpy(dest,"or",2);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;
        
        case And:
        if(increment==1)  nodeCount.And++;
        number[0]=nodeCount.And+'0';
        n = strlen(number);
        strncpy(dest,"and",3);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;

        case Class:
        if(increment==1)nodeCount.Class++;
        number[0]=nodeCount.Class+'0';
        n = strlen(number);
        strncpy(dest,"class",5);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;

        case Dot:
        if(increment==1)  nodeCount.Dot++;
        number[0]=nodeCount.Dot+'0';
        n = strlen(number);
        strncpy(dest,"dot",3);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;

        case Exc:
        if(increment==1)  nodeCount.Exc++;
        number[0]=nodeCount.Exc+'0';
        n = strlen(number);
        strncpy(dest,"exc",3);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;

        case Et:
        if(increment==1)   nodeCount.Et++;
        number[0]=nodeCount.Et+'0';
        n = strlen(number);
        strncpy(dest,"et",2);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;

        case Id:
        if(increment==1)   nodeCount.Id++;
        number[0]=nodeCount.Id+'0';
        n = strlen(number);
        strncpy(dest,"id",2);
        strncat(dest, number, n);
        printf("%s ",dest);
        break;
    }
}

void printCode(Node *node)
{
    printf("Node ");
    switch (node->type) {
      case String:
      printVariableName(node->type,nodeCount,1);
      printf("= { .String = { String, ");
	  printf("\"%s\"", node->String.string);
      printf(" , %i }}; \n",node->String.len);

	  return;
      case Query:
      printVariableName(node->type,nodeCount,1);
      printf("= { .Query = { Query, &");
      printVariableName(node->Query.children[0]->type,nodeCount,0);
	  printf(" }};\n");
	  return;
      case Star:
	  printVariableName(node->type,nodeCount,1);
      printf("= { .Star = { Star, &");
      printVariableName(node->Star.children[0]->type,nodeCount,0);
	  printf(" }};\n");
	  return;
      case Plus:
	  printVariableName(node->type,nodeCount,1);
      printf("= { .Plus = { Plus, &");
      printVariableName(node->Plus.children[0]->type,nodeCount,0);
	  printf(" }};\n");
	  return;
      case Or:
	  printVariableName(node->type,nodeCount,1);
      printf("= { .Or = { Or, &");
      printVariableName(node->Or.children[0]->type,nodeCount,0);
      printf(" , &");
      printVariableName(node->Or.children[1]->type,nodeCount,0);
	  printf(" }};\n");
	  return;
      case And:
	  printVariableName(node->type,nodeCount,1);
      printf("= { .And = { And, &");
      printVariableName(node->And.children[0]->type,nodeCount,0);
      printf(" , &");
      printVariableName(node->And.children[1]->type,nodeCount,0);
	  printf(" }};\n");
	  return;
      case Class:
	  printVariableName(node->type,nodeCount,1);
      printf("= { .Class = { Class, %s }};\n",node->Class.array);
	  return;
      case Dot:
	  printVariableName(node->type,nodeCount,1);
      printf("= { .Dot = { Dot}};\n");
	  return;
      case Exc:
	  printVariableName(node->type,nodeCount,1);
      printf("= { .Exc = { Exc, &");
      printVariableName(node->Exc.children[0]->type,nodeCount,0);
	  printf(" }};\n");
	  return;
      case Et:
	  printVariableName(node->type,nodeCount,1);
      printf("= { .Et = { Et, &");
      printVariableName(node->Et.children[0]->type,nodeCount,0);
	  printf(" }};\n");
	  return;
      case Id:
	  printVariableName(node->type,nodeCount,1);
      printf("= { .Id = { Id, symb");
	  printf(" }};\n");
	  return;
    }
    abort();
}

void println(Node *node)
{
    printCode(node);
    printf("\n");
}


int main(int arc, char **argv){
    Node *node1= mkString("ab");
    println(node1);
    Node *node2= mkQuery(node1);
    println(node2);
    Node *node3= mkString("bc");
    Node *node4= mkOr(node3,node2);
    printCode(node4);

}