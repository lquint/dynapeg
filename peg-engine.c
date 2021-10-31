#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *_checktype(Node *object, enum op type)
{
    if (object->type == type) return object;
    fprintf(stderr, "\naccesing type %i as if it were a %i\n", object->type, type);
    exit(1);
    return 0;
}

#define get(object, type, member)	(_checktype(object, type)->type.member)

void setBit(char *bits, int n)
{
    bits[n/8] |= (1 << n%8);
}

int testBit(char *bits, int n)
{
    return bits[n/8] & (1 << n%8);
}

int execute(Node *node, InputBuffer *in)
{
    switch (node->type) {
      case String: {
	  if (strncmp(currentText(in), get(node,String,string), get(node,String,len))) {
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
	  if (testBit(get(node,Class,array),currentChar(in))) {
          advance(in, 1);
	      return 1;
	  }
	  return 0;
      }
      case Query: {
	  execute(get(node,Query,children[0]), in);
	  return 1;
      }
      case Exc: {
	  int pos= getPosition(in);
	  if (!execute(get(node,Exc,children[0]), in)) {
	      return 1;
	  }
	  setPosition(in, pos);
	  return 0;
      }
      case Et: {
	  int pos= getPosition(in);
	  if (!execute(get(node,Et,children[0]), in)) {
	      return 0;
	  }
	  setPosition(in, pos);
	  return 1;
      }
      case Dot: {
	  if (atEnd(in)) {
	      return 0;
	  }
	  advance(in, 1);
	  return 1;
      }
      case Id: {
	  Symbol *symbol= get(node, Id, symbol);
	  if (0 == symbol->rule) { printf("undefined rule: %s\n", symbol->name); }
	  return execute(symbol->rule, in);
      }
    }
    printf("this cannot happen\n");
    abort();
}