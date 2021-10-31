enum op { String, Query, Star, Plus, Or, And, Class, Dot, Exc, Et, Id} ;


typedef union Node Node;
typedef struct Symbol Symbol;
typedef struct SymbolTable SymbolTable;
typedef struct NodeCount NodeCount;


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

struct Symbol{
  char* name;
  Node *rule;
  int number;
};



