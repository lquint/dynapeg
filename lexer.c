#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union Node Node;
typedef struct Exploration Exploration;


enum opcode {
    String,
    And,
    Or,
    Star,
    Plus,
    Class,
    Question_mark,
    Exclamation_mark,
    Dot
};
struct Exploration
{
    int position;
    char *text;
};

Exploration *mkExploration(char *text)
{
    Exploration *exploration = calloc(1,sizeof(Exploration*)); //sizeof du type pointeur
    exploration->position= 0;
    exploration->text= text;
    return exploration;
}

void advance(Exploration *exploration, int add)
{
    exploration->position+= add;
}

int atEnd(Exploration *exploration) {
    return exploration->text[exploration->position] == 0;
    }

void setPosition(Exploration *exploration, int position)
{
    exploration->position= position;
}

int getPosition(Exploration *exploration) {
    return exploration->position;
}

int currentChar(Exploration *exploration)
{
    return exploration->text[exploration->position];
}

char *currentText(Exploration *exploration)
{
    return exploration->text + exploration->position;
}

struct String           { enum opcode type; char *stringValue; int len;};
struct Class            { enum opcode type; char *stringValue; int len;};
struct And              { enum opcode type; Node *children[2];};
struct Or               { enum opcode type; Node *children[2];};
struct Star             { enum opcode type; Node *children[1];};
struct Plus             { enum opcode type; Node *children[1];};
struct Question_mark    { enum opcode type; Node *children[1];};
struct Exclamation_mark { enum opcode type; Node *children[1];};
struct Dot              { enum opcode type;};
union Node
{
    enum opcode type;
	struct String String;
    struct Class Class;
    struct And And;
    struct Star Star;
    struct Plus Plus;
    struct Or Or;
    struct Question_mark Question_mark;
    struct Exclamation_mark Exclamation_mark;
    struct Dot Dot;
};



Node *mkNode(size_t size,enum opcode type)
{
    Node *node= calloc(1, size);
    node->type= type;
    return node;
}
#define new(type)			mkNode(sizeof(struct type),type)

Node *mkString(char *value)
{
    Node *node= new(String);
    node->String.stringValue= value;
    node->String.len=strlen(value);
    return node;
}

Node *mkAnd(Node *node1, Node *node2)
{
    Node *node= new(And);
    node->And.children[0]= node1;
    node->And.children[1]= node2;
    return node;
}


Node *mkStar(Node *child)
{
    Node *node= new(Star);
    node->Star.children[0]= child;
    return node;
}

Node *mkPlus(Node *child)
{
    Node *node= new(Plus);
    node->Plus.children[0]= child;
    return node;
}

Node *mkOr(Node *node1, Node *node2)
{
    Node *node= new(Or);
    node->Or.children[0]= node1;
    node->Or.children[1]= node2;
    return node;
}

Node *mkClass(char* str)
{
    Node *node= new(Class);
    node->Class.stringValue= str;
    node->Class.len=strlen(str);
    return node;
}

Node *mkQuestionMark(Node *child)
{
    Node *node= new(Question_mark);
    node->Question_mark.children[0]= child;
    return node;
}

Node *mkExclamationMark(Node *child)
{
    Node *node= new(Exclamation_mark);
    node->Exclamation_mark.children[0]= child;
    return node;
}



Node *mkDot()
{
    static Node *node= 0;
    if (node==0){
        node=new(Dot);
    }
    return node;
}

Node *_checktype(Node *object, enum opcode type)
{
    if (object->type == type) return object;
    fprintf(stderr, "\naccesing type %i as if it were a %i\n", object->type, type);
    exit(1);
    return 0;
}

#define get(object, type, member)	(_checktype(object, type)->type.member)

const char* getTypeName(enum opcode type) 
{
   switch (type) 
   {
      case Star: return "Star";
      case Exclamation_mark: return "NOT";
      case Dot: return "Dot";
      case Question_mark: return "QMARK";
      case Plus : return "Plus";
      case Class : return "Class";
      default : printf("unexpected use");return 0;
      abort();
   }
}

void println(Node *node, int indent)
{
    for(int i=0;i<indent;i++){
          printf("----");

    }
    indent++;
    switch (node->type) {			
      case String:	printf("%.*s", get(node, String, len), get(node, String, stringValue));	return;
      case And: {
          printf("AND\n");
          println(get(node,And,children[0]),indent);printf("\n");
          println(get(node,And,children[1]),indent);printf("\n");return;
      }
      case Or: {
          printf("OR\n");
          println(get(node,Or,children[0]),indent);printf("\n");
          println(get(node,Or,children[1]),indent);printf("\n");return;
      }
      case Dot:{
          printf("DOT\n");return;
      }
      case Class:{
          printf("Class : Char in [%.*s]\n",get(node, Class, len),get(node, Class, stringValue));return;
      }
      case Plus:{
          printf("Plus\n");
          println(get(node,Plus,children[0]),indent);printf("\n");return;
      }
      case Question_mark:{
          printf("Question_mark\n");
          println(get(node,Question_mark,children[0]),indent);printf("\n");return;
      }
      case Exclamation_mark:{
          printf("Exclamation_mark\n");
          println(get(node,Exclamation_mark,children[0]),indent);printf("\n");return;
      }
      case Star:{
          printf("Star\n");
          println(get(node,Star,children[0]),indent);printf("\n");return;
      }
        
    }
    printf("Unexpected type %i",node->type);
}



int execute(Node *node, Exploration *in)
{
    switch (node->type) {
      case String: {
      if (strncmp(currentText(in), get(node,String,stringValue), get(node,String,len))) {
        return 0;
      }
      advance(in, get(node,String,len));
      return 1;
      }
      case And: {
      int pos= getPosition(in);
      if (!execute(get(node,And,children[0]), in)) //si il y a eu une erreur
      {
          return 0;
      } //si ça s'est bien passé
      if (!execute(get(node,And,children[1]), in)) {
          setPosition(in, pos);
          return 0;
      }
      return 1;
      }
      case Or: {
      if (execute(get(node,Or,children[0]), in)) {
          return 1;
      }
      return execute(get(node,Or,children[1]), in);
      }
      case Star: {
      while (execute(get(node,Star,children[0]), in));
      return 1;
      }
      case Plus: {
      if (!execute(get(node,Plus,children[0]), in)) {
        return 0;
      }
      while (execute(get(node,Plus,children[0]), in));
      return 1;
      }
      case Class: {
      if (!currentChar(in)) {
        return 0;
      }
      if (strchr(get(node,Class,stringValue), currentChar(in))) {
        advance(in, 1);
        return 1;
      }
      return 0;
      }
      case Question_mark: {
      execute(get(node,Question_mark,children[0]), in);
      return 1;
      }
      case Exclamation_mark: {
      int pos= getPosition(in);
      if (!execute(get(node,Exclamation_mark,children[0]), in)) {
        return 1;
      }
      setPosition(in, pos);
      return 0;
      }
      case Dot: {
      if (atEnd(in)) {
        return 0;
      }
      advance(in, 1);
      return 1;
      }
    }
    printf("this cannot happen\n");
    abort();
}


int main(int argc, char **argv)
{
    Exploration *in= mkExploration("aabbcc");

    /*Node *program=
	mkString("aabbcc");
    Node *program=
	mkAnd(mkString("aab"),
	      mkString("bcc"));*/

    /*Node *program=
	mkAnd(mkString("aa"),
	      mkAnd(mkString("bb"),
		    mkString("cc")));*/

    /*Node *program=
            mkOr(mkPlus(mkString("a")),       // "a"*"b"*"c"*
                mkAnd(mkStar(mkString("b")),
                mkStar(mkString("c"))));*/


    Node *program= mkOr(mkString("don't match pls"),mkStar(mkDot()));
    Node *program3=mkAnd(mkAnd(mkPlus(mkString("hello world")),mkExclamationMark(mkString("how are u world !?"))),mkStar(mkClass("abcde")));

    if (!execute(program, in) || !atEnd(in))
    {
        printf("no match, current position : %i\n", getPosition(in));
    } else
    {
        printf("match, current position : %i\n", getPosition(in));
    }// 0 => no match, 1 => match
    println(program3,0);


    return 0;
}