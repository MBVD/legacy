#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#define TO_STR(x) #x
#define BUF_MAX 256

enum Type {
	NUM, UNARY, BINARY
};

typedef struct node {
	enum Type type;
	char* op;
	double value;
  char* variable;
	struct node** branches;
} node;

char* readline() {
	int n = BUF_MAX, i = 0, c;
	char* buf = (char*) malloc(n * sizeof(char));
	while((c = getchar()) != '\n') {
		if (i == n - 1) {
			n *= 2;
			buf = (char*) realloc(buf, n * sizeof(char));
		}
		buf[i++] = c;
	}
	buf[i] = '\0';
	return buf;
}

char** split(char* buf, char delimeter) {
	int n = strlen(buf), k = 0;
	char** words = (char**) malloc(n * sizeof(char*));
	for (int i = 0; buf[i] != '\0';) {
		while (buf[i] == delimeter) i++;
		int j = 0;	
		
		while (buf[i + j] != delimeter && buf[i + j] != '\0' && buf[i + j] != '+' && buf[i + j] != '-' && buf[i + j] != '*' && buf[i + j] != '/' && buf[i + j] != '(' && buf[i + j] != ')' && buf[i + j] != '^') j++;

		if (j > 0) {
			words[k] = (char*) malloc((j + 1) * sizeof(char));
			strncpy(words[k], &buf[i], j);
			words[k][j] = '\0';
			i += j;
			k++;
		}

		if (buf[i] == '+' || buf[i] == '-' || buf[i] == '*' || buf[i] == '/' || buf[i] == '(' || buf[i] == ')' || buf[i] == '^') {
			words[k] = (char*) malloc(2 * sizeof(char));
			strncpy(words[k], &buf[i], 1);
			words[k][1] = '\0';
			k++;
			i++;
		}

	}
	words[k] = NULL;
	return words;
}

node* create_node(enum Type type, double value, const char* op, char* var, node* first, node* second) {
	node* temp = (node*) malloc(sizeof(node));
	temp -> type = type;
	temp -> value = value;
  temp -> op = op;
  temp -> variable = var;
	if (type == NUM) {
		temp -> branches = NULL;
	} else if (type == UNARY) {
		temp -> branches = (node**) malloc(sizeof(node*));
		temp -> branches[0] = first;
	} else {
		temp -> branches = (node**) malloc(2 * sizeof(node*));
		temp -> branches[0] = first;
		temp -> branches[1] = second;
	}
	return temp;
}

node* parse_sum_expr(char**, int*);

node* parse_unary_expr(char** words, int* i) {
  if (!strcmp(words[*i], "log")){
    char* op = words[(*i)++];
    return create_node(UNARY, 0, op, NULL, parse_unary_expr(words, i), NULL);
  } else if (!strcmp(words[*i], "cos")){
    char* op = words[(*i)++];
    return create_node(UNARY, 0, op, NULL, parse_unary_expr(words, i), NULL);
  } else if (!strcmp(words[*i], "sin")){
    char* op = words[(*i)++];
    return create_node(UNARY, 0, op, NULL, parse_unary_expr(words, i), NULL);
  }else if (!strcmp(words[*i], "+") || !strcmp(words[*i], "-")) {
		char* op = words[(*i)++];
		printf("created binary + node \n");
		return create_node(UNARY, 0, op, NULL, parse_unary_expr(words, i), NULL);// create_node(type, char op, char variable, node* first, node* second)
	} else if (!strcmp(words[*i], "(")) {
		(*i)++;
		node* expr = parse_sum_expr(words, i);
		if (!strcmp(words[*i], ")")) {
			(*i)++;
			return expr;
		} else {
			printf("Missing )\n");
			exit(0);
		}
	} else {
    int j = 0;
		while(words[*i][j]!='\0'){
			if (!isdigit(words[*i][j++])){// words[i][j] < 0 || words[i][j] > 9
				printf("created num x node \n");
				return create_node(NUM, 0, '\0', words[(*i)++], NULL, NULL); 
			}
		}   
		double value = atof(words[(*i)++]);
		printf("created num d node \n");
		return create_node(NUM, value, '\0', NULL, NULL, NULL);
	}
}

node* parse_mult_expr(char** words, int* i) {
	printf("call parse_unary_expr \n");
	node* left = parse_unary_expr(words, i);
	while (words[*i] != NULL && (!strcmp(words[*i], "*") || !strcmp(words[*i], "/") || !strcmp(words[*i], "^"))) {
		char* op = words[(*i)++];
		printf("call parse_mult_expr \n");
		node* right = parse_mult_expr(words, i);
		left = create_node(BINARY, 0, op, NULL ,left, right);
	}
	return left;
}

node* parse_sum_expr(char** words, int* i) {
	printf("call parse_mult_expr \n");
	node* left = parse_mult_expr(words, i);
	while (words[*i] != NULL && (!strcmp(words[*i], "+") || !strcmp(words[*i], "-"))) {
		char* op = words[(*i)++];
		printf("call parse_sum_expr \n");
		node* right = parse_sum_expr(words, i);
		left = create_node(BINARY, 0, op, NULL, left, right);
		printf("created binary + node \n");
	}
	return left;
}

node* parse(char** words) {
	int i = 0;
	return parse_sum_expr(words, &i);
}


void print_tree(node* root) {
	if (root -> type == BINARY) {
		print_tree(root -> branches[0]);
		printf("%s", root -> op);
		print_tree(root -> branches[1]);
	} else if (root -> type == UNARY) {
		printf("%s", root -> op);
		print_tree(root -> branches[0]);
	} else {
    if (root->variable != NULL){
      printf("%s", root -> variable);
    }else{
      printf("%g", root -> value);
    }
	}
}

double op_tree(node* root, double var) {
	if (root == NULL){
		return 0;
	}
	if (root -> type == BINARY) {
		double tmp1 = op_tree(root->branches[0], var);
		double tmp2 = op_tree(root->branches[1], var);
		if (!strcmp(root -> op, "+")){
			return tmp1 + tmp2;
		}
		if (!strcmp(root -> op, "-")){
			return tmp1 - tmp2;
		}
		if (!strcmp(root -> op, "*")){
			return tmp1 * tmp2;
		}
		if (!strcmp(root -> op, "/")){
			return tmp1 / tmp2;
		}
    if (!strcmp(root -> op, "^")){
			return pow(tmp1, tmp2);
		}
	} else if (root -> type == UNARY) {
		double tmp = op_tree(root->branches[0], var);
    if (!strcmp(root->op, "log")){
      return log(tmp);
    }
    if (!strcmp(root->op, "cos")){
      return cos(tmp);
    }
    if (!strcmp(root->op, "sin")){
      return sin(tmp);
    }
		if (!strcmp(root -> op, "+")){
			return tmp;
		}
		if (!strcmp(root -> op, "-")){
			return -tmp;
		}
	} else {
    if (root -> variable != NULL){
      return var;
    }
		return root -> value; 
	}
}

char* merge(const char* str1, const char* str2){
	int n = BUF_MAX;
	char* tmp = malloc(n * sizeof(char));
	strcpy(tmp, str1);
	int i = 0, j = strlen(str1);
	while(str2[i] != '\0'){
		if (j == n-1){
			n*=2;
			tmp = realloc(tmp, n* sizeof(char));
		}
		tmp[j++] = str2[i++];
	}
  tmp[j] = '\0';
	return tmp;
}

char* merge_arr(const** arr1){
	int i = 0;
	char* ans;
	while(arr1[i]!=NULL){
		ans = merge(ans, arr1[i]);
		i++;
	}
	return ans;
}

char* extract (node* root){
	if (root == NULL){
		return 0;
	}
	if (root -> type == BINARY) {
		char* tmp1 = extract(root->branches[0]);
		char* tmp2 = extract(root->branches[1]);
		return merge(merge(tmp1, root->op) , tmp2);
	} else if (root -> type == UNARY) {
		char* tmp = extract(root->branches[0]);
		if (!strcmp(root -> op, "+")){
			return tmp;
		}
		if (!strcmp(root -> op, "-")){
			return merge("-", tmp);
		}
		return merge(merge(merge(root->op, "("), tmp), ")");
	} else {
    if (root -> variable != NULL){
      return root -> variable;
    }
		char* s1 = malloc(BUF_MAX * sizeof(char));
		sprintf(s1, "%g", root->value);
		return s1; 
	}
} 

char* d_tree(node* root){
	if (root -> type == BINARY){
		char* tmp1 = d_tree(root -> branches[0]);
		char* tmp2 = d_tree(root -> branches[1]);
		char* ans = "";
		if (!strcmp(root -> op, "+")){
			ans = merge(tmp1, "+");
			ans = merge(ans, tmp2);
		}
		if (!strcmp(root -> op, "-")){
			ans = merge(tmp1, "+");
			ans = merge(ans, tmp2);
		}
		if (!strcmp(root -> op, "*")){ // f'*g + g'*f
			char* h1_ans = merge(tmp1, "*"); // f' *
			h1_ans = merge(h1_ans, extract(root->branches[1])); // g(x) - надо вытащить f' * g
			char* h2_ans = merge("+", merge(merge(tmp2, "*"), extract(root->branches[0]))); // + g' * f 
			ans = merge(h1_ans, h2_ans);
			free(h1_ans); free(h2_ans);
		}
		if (!strcmp(root -> op, "/")){ // (f'*g - g'*f)/g^2
			char* h1_ans = merge("(", merge(tmp1, "*")); // (f' *
			h1_ans = merge(h1_ans, extract(root->branches[1])); // g(x) - надо вытащить f' * g
			char* h2_ans = merge(merge("-", merge(merge(tmp2, "*"), extract(root->branches[0]))), ")"); // - g' * f)
			char* pow_ans = merge(merge("(", extract(root->branches[1])), ")^2"); // (g)^2 
			ans = merge(merge(merge(h1_ans, h2_ans), "/"), pow_ans); // (f'g - g'f)/(g)^2
			free(h1_ans); free(h2_ans); free(pow_ans);
		}
		if (!strcmp(root -> op, "^")){ // (f'*g - g'*f)/g^2
			char* h1_ans = merge(merge(merge("e^(", extract(root->branches[1])), "*ln("), merge(extract(root->branches[0]), ")*(")); // e^(g*ln(f))*( merge_arr(["e^", ])
			char* h2_ans = merge(merge(merge(merge(tmp2, "*ln("), extract(root->branches[0])), ")+"), merge(merge(merge(merge(extract(root->branches[1]), "*"),  tmp1), "/"), merge(extract(root->branches[0]), ")"))); // g' * ln(f)+ g * f'
			ans = merge(merge(h1_ans, "*"), h2_ans);
		}
		return ans;
	} else if (root -> type == UNARY) {
		char* tmp = d_tree(root->branches[0]);
    if (!strcmp(root->op, "log")){
      return merge(merge(tmp, "/"), extract(root->branches[0])); // f'/f * f'
    }
    if (!strcmp(root->op, "cos")){
      return merge(merge("-sin(", extract(root->branches[0])), merge(")*", tmp));// -sin(f) * f'
    }
    if (!strcmp(root->op, "sin")){
      return merge(merge("cos(", extract(root->branches[0])), merge(")*", tmp)); // cos(f) * f'
    }
		if (!strcmp(root -> op, "+")){
			return tmp;
		}
		if (!strcmp(root -> op, "-")){
			return merge("-", tmp);
		}
	} else {
    if (root -> variable != NULL){
      return "1";
    }else{
			return "0";
		} 
	}	
}

node* d_tree_node(node* root){
	if (root -> type == BINARY){
		if (!strcmp(root -> op, "+")){
			node* left = d_tree_node(root->branches[0]);
			node* right = d_tree_node(root->branches[1]);
			node* ans = create_node(BINARY, 0, "+", NULL, left, right);
			return ans;
		}
	}else if (root->type != UNARY){
		if (root -> variable != NULL){
      return create_node(NUM, 0, NULL, "1", NULL, NULL);
    }
		return create_node(NUM, 0, NULL, "0", NULL, NULL); 
	}
}

node* perv_tree_node(node* root){
	if (root -> type == BINARY){
		if (!strcmp(root -> op, "+")){
			node* left = perv_tree_node(root->branches[0]);
			print_tree(left);
			printf("\n");
			node* right = perv_tree_node(root->branches[1]);
			print_tree(right);
			printf("\n");
			node* ans = create_node(BINARY, 0, root->op, NULL, left, right);
			return ans;
		}
		if (!strcmp(root -> op, "^")){// x ^ a -> x ^ a+1 / a+1
			node* tmp1 = create_node(NUM, 0, "\0", root->branches[0]->variable, NULL, NULL);
			node* tmp = create_node(NUM, root->branches[1]->value+1, "\0", NULL, NULL, NULL);
			node* left = create_node(BINARY, 0, "^", NULL, tmp1, tmp); // x ^ a+1
			node* right = create_node(NUM, root->branches[1]->value+1, "\0", NULL, NULL, NULL);
			node *ans = create_node(BINARY, 0, "/", NULL, left, right); // x ^ a+1 / a+1
			return ans;
		}
		if (!strcmp(root -> op, "*") || !strcmp(root -> op, "/")){
			char* variable;
			double value;
			node* left;
			if (root->branches[0]->variable != NULL){
				variable =  root->branches[0]->variable;
				value = root->branches[1]->value;
				left = perv_tree_node(root->branches[0]);
			}else{
				variable =  root->branches[1]->variable;
				value = root->branches[0]->value;
				left = perv_tree_node(root->branches[1]);
			}
			node* right = create_node(NUM, value, "\0", NULL, NULL, NULL);// value
			node* ans = create_node(BINARY, 0, root->op, NULL, left, right);// x^2/2/value
			return ans;
		}
	}else{
		if (root -> type == UNARY){
			node* left = perv_tree_node(root->branches[0]);
			return create_node(UNARY, 0, root->op, root->variable, left, NULL);
		}else{// num
			// если x, то нужно
			if (root -> variable != "\0" && root -> variable != NULL){// x^2 / 2
				node* tmp1 = create_node(NUM, 0, "\0", root -> variable, NULL, NULL); // x
				node* tmp = create_node(NUM, 2, "\0", NULL, NULL, NULL);
				node* left = create_node(BINARY, 0, "^", NULL, tmp1, tmp); // x ^ 2
				// printf("%s %c", left->branches[0]->variable, left->op);
				node* right = create_node(NUM, 2, "\0", NULL, NULL, NULL); 
				node* ans = create_node(BINARY, 0, "/", NULL, left, right); // x ^ 2 / 2
				return ans;
			} else{
				node* left = create_node(NUM, root->value, "\0", NULL, NULL, NULL);// a
				node* right = create_node(NUM, 0, "\0", "x", NULL, NULL);// x
				node* ans = create_node(BINARY, 0, "*", NULL, left, right);// a * x
				return ans;
			}
		}
	}
}

int main() {
	char* s = readline();
	char** words = split(s, ' ');
	node* tree = parse(words);
	print_tree(tree);
	printf("\n");
	node* ans = perv_tree_node(tree);
	print_tree(ans);
	// printf("%s", s1);
}