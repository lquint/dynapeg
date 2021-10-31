#include <stdio.h>
#include <stdlib.h>
#include <math.h>



int digitValue(int c)
{
    switch (c) {
      case '0'...'9':	return c - '0';
      case 'A'...'Z':	return c - 'A' + 10;
      case 'a'...'z':	return c - 'a' + 10;
    }
    return 666;
}

int isRadix(int r, int c)
{
    return digitValue(c) < r;
}


int unescapedCharacter(char *cp, char **ep)
{
    int c= 0;
    c= *cp++;
    if ('\\' == c) {
	c= *cp++;
	switch (c) {
	  case 'n':	c= 10;    break;
	  case 'a':	c=  7;    break;
	  case 'b':	c=  8;    break;
	  case 'e':	c= 27;    break;
	  case 'f':	c= 12;    break;
	  case 'r':	c= 13;    break;
	  case 't':	c=  9;    break;
	  case 'v':	c=  1;    break;
	  case '\\':    c= '\\';  break;
	  case ']':     c= ']';   break;
	  case '-':     c= '-';   break;
	  case '0'...'7': {
	      c -= '0';		// c is now the value of the digit that represented it
	      if (isRadix(8, *cp)) c= c * 8 + *cp++ - '0';
	      if (isRadix(8, *cp)) c= c * 8 + *cp++ - '0';
	      break;
	  }
	  case 'x': {
	      char *op= cp;
	      c= 0;
	      if (isRadix(16, *cp)) {
		  c= c * 16 + digitValue(*cp++);
		  if (isRadix(16, *cp)) {
		      c= c * 16 + digitValue(*cp++);
		      break;
		  }
	      }
	      fprintf(stderr, "illegal hexadecimal escape: \\x%.2s\n", op);
	      break;
	  }
	  default:
	      fprintf(stderr, "unusual escape: \\%c\n", c);
	      break;
	}
    }
    if (ep) *ep= cp;
    return c;
}

char *unescapedString(char *ch){
	static char *s=0;
	static int size=0;
	static int position=0;
	position=0;
	while(*ch){
		int c;
		c = unescapedCharacter(ch,&ch);
		if(position>=size){
			size=position+16;
			s=realloc(s,size);
		}
		s[position++]=c;
	}
	if(position>=size){
			size=position+16;
			s=realloc(s,size);
		}
	s[position++]=0;
	return s;
}



char *classify(char *spec)
{
    char *class= calloc(1, 32);

    int negated= 0;
    if ('^' == *spec) ++negated, ++spec;

    int prevChar= 0;
    while (*spec) {
	if ('-' == *spec && prevChar && spec[1]) {
	    ++spec;
	    int thisChar= unescapedCharacter(spec, &spec);
	    if (thisChar < prevChar) {
		fprintf(stderr, "range is backwards: \\x%x-\\x%x\n", prevChar, thisChar);
		int tmp= prevChar;
		prevChar= thisChar;
		thisChar= tmp;
	    }
	    for (int i= prevChar;  i <= thisChar;  ++i)
		setBit(class, i);
	    prevChar= 0;
	    continue;
	}
	prevChar= *spec;
	setBit(class, unescapedCharacter(spec, &spec));
    }

    if (negated)
	for (int i= 0;  i < 32;  ++i)
	    class[i] ^= 255;   // invert all bits in the class

    return class;
}


#ifdef TESTING_CLASS_C
int main()
{
    char   *line=0;
    size_t  line_max=0;
    ssize_t line_len=0;
    char a='a';
    printf("%i\n",(int)a);
    printf("%s",classify("a"));
    while ((line_len= getline(&line,&line_max,stdin)) >= 0) {
        if (line_len>0 && line[line_len-1]=='\n') {
            line[line_len-1]=0;
        }
	char *class= classify(line);
	for (int i= 0;  i < 256;  ++i)
	    if (testBit(class, i))
		printf("%02x is set\n", i);
    }

    return 0;
}
#endif

