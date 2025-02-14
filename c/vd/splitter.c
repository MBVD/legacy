#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char specials[] = "+*?()";
#define SIZE 256

int isspecial(int c) {
    return strchr(specials, c) != NULL;
}

typedef enum {
    CHAR, CHAR_CLASS, REPETITION, TERM, FACTOR, CHAR_RANGE, CHAR_RANGE_1
} Type;

typedef struct Node {
    Type type;
    char c;
    char b;
    struct Node *left, *right;
} Node;

Node* createNode(Type type, char c, char b, Node* left, Node* right) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->type = type;
    node->c = c;
    node->b = b;
    node->left = left;
    node->right = right;    
}

Node* parseTerm(const char*, size_t*);
Node* parseCharRange(const char*, size_t*);

Node* parseBase(const char* buf, size_t* i) {
    Node* node;
    if (buf[*i] == '['){
      (*i)++;
      Node* result = parseCharRange(buf, i);
      if (buf[*i] != ']'){
        fprintf(stderr, "Expected closing ], encountered %c", buf[*i]);
        return NULL;
      }
      (*i)++;
      return result;
    }else if (buf[*i] == '(') {
        (*i)++;
        Node* result = parseTerm(buf, i);
        if (buf[*i] != ')') {
            fprintf(stderr, "Expected closing ), encountered %c", buf[*i]);
            return NULL;
        }
        (*i)++;
        return result;
    } else if (buf[*i] == '\\' && buf[*i + 1] != '\0') {
        switch (buf[*i + 1]) {
            case 'd': case 'w': case 's':
                node = createNode(CHAR_CLASS, buf[*i + 1], '\0', NULL, NULL);
                break;
            case '*': case '+': case '?': case '(': case ')':
                node = createNode(CHAR, buf[*i + 1], '\0', NULL, NULL);
                break;
            default:
                fprintf(stderr, "Invalid escaped character %c", buf[*i + 1]);
                return NULL;
        }
        *i += 2;
    } else if (!isspecial(buf[*i])) {
        node = createNode(CHAR, buf[*i], '\0', NULL, NULL);
        (*i)++;
    } else {
        fprintf(stderr, "Unexpected char %c", buf[*i]);
        return NULL;
    }
    return node;
}

Node* parseCharRange(const char* buf, size_t* i){
  Node* base = NULL;
  if (buf[*i] == ']'){
    return base;
  }
  if (buf[*i+1] == '-'){
    (*i)++; // buf[*i] == '-'
    Node* base = createNode(CHAR_RANGE_1, buf[*i-1], buf[*i+1], NULL, parseCharRange(buf, i));
  }
  else{
    (*i)++;
    Node* base =  createNode(CHAR_RANGE, buf[*i-1], '\0', NULL, parseCharRange(buf, i));
  }
  return base;
}

Node* parseRepetition(const char* buf, size_t* i) {
    Node* base = parseBase(buf,i);
    if (buf[*i] == '*' || buf[*i] == '+' || buf[*i] == '?') {
        base = createNode(REPETITION, buf[*i], '\0', base, NULL);
        (*i)++;
    }
    return base;
}

Node* parseFactor(const char* buf, size_t* i) {
    Node* left = parseRepetition(buf, i);
    if (buf[*i] != '|' && buf[*i] != '\0' && buf[*i] != ')' ) {
        Node* right = parseFactor(buf, i);
        left = createNode(FACTOR, '\0', '\0', left, right);  
    }
    return left;
}

Node* parseTerm(const char* buf, size_t* i) {
    Node* left = parseFactor(buf, i);
    if (buf[*i] == '|') {
        (*i)++;
        Node* right = parseTerm(buf, i);
        left = createNode(TERM, '|', '\0', left, right);
    }
    return left;
}

Node* parseRegEx(const char* buf) {
    size_t i = 0;
    return parseTerm(buf, &i);
}

int matchNode(Node*, const char*, size_t*);

int matchChar(Node* root, const char* text, size_t *i) {
    if (text[*i] == root->c) {
        (*i)++;
        return 1;
    }
    return 0;
}

int matchCharClass(Node* root, const char* text, size_t *i) {
    if (root->c == 'd') {
        if (isdigit(text[*i])) {
            (*i)++;
            return 1;
        }
        return 0;
    } else if (root->c == 'w') {
        if (isalpha(text[*i])) {
            (*i)++;
            return 1;
        }
        return 0;
    } else if (root->c == 's') {
        if (isspace(text[*i])) {
            (*i)++;
            return 1;
        }
        return 0;
    }
}

int matchRange(Node* root, const char* text, size_t* i){
  if (root->type == CHAR_RANGE){
    if (text[*i] == root->c || text[*i] == root->b){
      return 1;
    }
    (*i)++;
    return matchRange(root->right, text, i);
  }
  if (root->type == CHAR_RANGE_1){
    int n = root->b - root->c + 1;
    for (char k = root->c; k<=root->b; k++){
      if (text[*i] == k){
        return 1;
      }
    }
    (*i)++;
    return matchRange(root->right, text, i);
  }
  return 0;
}

int matchRepetition(Node* root, const char* text, size_t *i) {
    if (root->c == '+') {
        int flag = matchNode(root->left, text, i);
        if (!flag) return 0;
        while (matchNode(root->left, text, i));
        return 1;
    }
    if (root->c == '*'){
      while(matchNode(root->left, text, i));
      return 1;
    }
    if (root->c == '?'){
      matchNode(root->left, text, i);
      return 1;
    }
}

int matchTerm(Node* root, const char* text, size_t *i) {
    int left = matchNode(root->left, text, i);
    if (left) return 1;
    int right = matchNode(root->right, text, i);
    return right;
}

int matchFactor(Node* root, const char* text, size_t *i) {
    int left = matchNode(root->left, text, i);
    if (!left) return 0;
    int right = matchNode(root->right, text, i);
    return right;
}


int matchNode(Node* root, const char* text, size_t* i) {
  if (text[*i] == '\0') {
    return 0;
  }
  switch (root->type) {
    case CHAR:
      return matchChar(root, text, i);
    case CHAR_CLASS:
      return matchCharClass(root, text, i);
    case REPETITION:
      return matchRepetition(root, text, i);
    case TERM:
      return matchTerm(root, text, i);
    case FACTOR:
      return matchFactor(root, text, i);
    case CHAR_RANGE:
      return mathRange(root, text, i);
    case CHAR_RANGE_1:
      return mathRange(root, text, i);
  }
}

char* splitTextWithRegex(const char* text, Node* root) {  
  char ans[SIZE][SIZE];
  int p = 0;
  for (int i = 0; i < strlen(text); ++i) { 
    int j = i; 
    if (matchRegEx(root, &text[j])) { 
      while (text[j] != '\0' && matchRegEx(root, &text[j])) { 
        j++; 
      } 
      for (int k = 0; k<j-i+1; k++){
        ans[p][k] = text[k];
      }
      i = j - 1; 
      p++;
    } 
  } 
}

int matchRegEx(Node* root, const char* text) {
  size_t i = 0;
  return matchNode(root, text, &i);
}

int main() {
  const char regex[] = "\\d*\\+\\d+";
  const char text[] = "12-1";
  Node* root = parseRegEx(regex);
  printf("test regex {%s} for text {%s} - %d\n", regex, text, matchRegEx(root, text));
}