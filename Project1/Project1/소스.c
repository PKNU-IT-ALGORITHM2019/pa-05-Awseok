#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4013)
#define wordsize 30
#define des 1200

// 18536번째 'T was -> 'Twas
typedef struct dict
{
	char word_name[wordsize];
	char word_text[des];

	struct dict* left;
	struct dict* right;
	struct dict* parent;
} Dict;

void open();
Dict* getData();
void check_(char* name_);
void find_word(Dict* Dic, char* word);
void run(Dict* Dic);
void add_node(Dict* Dic);
void swap(Dict* z, Dict* y);
Dict* TREE_SUCCESSOR(Dict* x);
Dict* TREE_MINIMUM(Dict* x);
void delete_word(Dict* Dic, char* word);
void deleteall(Dict* Dic);

int size, del_i, del_cnt, val, val2;
Dict *del_p[20];
FILE* f;

int main()
{
	Dict* start = NULL;
	open();
	start = getData();
	run(start);

	return 0;
}

void open()
{
	fopen_s(&f, "shuffled_dict.txt", "r");

	if (f == NULL)
	{
		puts("파일열기 실패");
		open();
	}
}

Dict* getData()
{
	char buffer[des], *token = NULL, *context = NULL, cur_[wordsize], Dic_[wordsize];
	Dict* start = NULL, *current = NULL;
	
	while (fgets(buffer, sizeof(buffer), f) != NULL)
	{
		size++;
		Dict* Dic = (Dict*)malloc(sizeof(Dict));

		token = strtok_s(buffer, " ", &context);
		strcpy_s(Dic->word_name, wordsize, token);
		token = strtok_s(NULL, "\n", &context);
		if (token != NULL)
			strcpy_s(Dic->word_text, des, token);
		else
			strcpy_s(Dic->word_text, des, " \n");
		Dic->left = NULL;
		Dic->right = NULL;
		Dic->parent = NULL;

		if (start == NULL)
			start = current = Dic;

		else
		{
			while (current->word_name != NULL)
			{
				strcpy_s(cur_, wordsize, current->word_name);
				strcpy_s(Dic_, wordsize, Dic->word_name);
				check_(cur_);
				check_(Dic_);
				if (_strcmpi(cur_, Dic_) < 0)
				{
					if (current->right != NULL)
						current = current->right;
					else
					{
						current->right = Dic;
						Dic->parent = current;
						break;
					}
				}
				else
				{
					if (current->left != NULL)
						current = current->left;
					else
					{
						current->left = Dic;
						Dic->parent = current;
						break;
					}
				}
			}
			current = start;
		}

	}
	return start;
}

void check_(char *name_)
{
	for (int i = 0; i < wordsize; i++)
		if (name_[i] == '-' || name_[i] == '\\' || name_[i] == '\'')
		{
			for (int j = i; j < wordsize - 1; j++)
				name_[j] = name_[j + 1];
		}
}

void run(Dict *Dic)
{
	char op[7 + wordsize] = { 0 }, *token = NULL, *context = NULL;

	do
	{
		printf("\n$ ");
		gets(op);
		token = strtok_s(op, " ", &context);

		if (!_strcmpi(token, "size"))
		{
			printf("size = %d\n", size);
		}
		else if (!_strcmpi(token, "find"))
		{
			val = 0;
			token = strtok_s(NULL, " ", &context);
			find_word(Dic, token);
		}
		else if (!_strcmpi(token, "add"))
		{
			add_node(Dic);
		}
		else if (!_strcmpi(token, "delete"))
		{
			val2 = 0;
			token = strtok_s(NULL, " ", &context);
			delete_word(Dic, token);
		}
		else if (!_strcmpi(token, "deleteall"))
		{
			deleteall(Dic);
		}
		else
		{
			puts("다시 입력해주세요");
		}
	} while (strcmp(token, "exit"));
	
}

void find_word(Dict *Dic, char *word)
{
	if (Dic == NULL)
		return;
	if (_strcmpi(Dic->word_name, word) == 0)
	{
		if (Dic->word_text != NULL)
		{
			if(val == 0)
				printf("des: %s\n", Dic->word_text);
			del_p[del_i++] = Dic;
			find_word(Dic->right, word);
		}
	}
	if (Dic->left == NULL && Dic->right == NULL)
		return;
	if (_strcmpi(Dic->word_name, word) < 0)
	{
		if (Dic->right == NULL)
			puts("Not found");
		else
			find_word(Dic->right, word);
	}
	else
	{
		if (Dic->left == NULL)
			puts("Not found");
		else
			find_word(Dic->left, word);
	}
}

void add_node(Dict* Dic)
{
	char buf2[wordsize + des], buf3[des], add_node_word[wordsize], x_word[wordsize], y_word[wordsize];
	Dict* x = Dic, * y = NULL;
	Dict* add_node = (Dict*)malloc(sizeof(Dict));

	puts("\nword: ");
	gets(add_node->word_name);
	puts("class: ");
	gets(buf2);
	puts("meaning: ");
	gets(buf3);
	if(buf2 == NULL)
		strcpy_s(add_node->word_text, des, buf3);
	if(buf3 == NULL)
		strcpy_s(add_node->word_text, des, buf2);
	strcat_s(buf2, wordsize + des, " ");
	strcat_s(buf2, des, buf3);
	strcpy_s(add_node->word_text, des, buf2);
	add_node->parent = NULL;
	add_node->left = NULL;
	add_node->right = NULL;


	while (x != NULL)
	{
		strcpy_s(add_node_word, wordsize, add_node->word_name);
		check_(add_node_word);
		strcpy_s(x_word, wordsize, x->word_name);
		check_(x_word);
		y = x;
		if(_strcmpi(add_node_word, x_word) < 0)
			x = x->left;
		else
			x = x->right;
	}
	add_node->parent = y;
	strcpy_s(add_node_word, wordsize, add_node->word_name);
	check_(add_node_word);

	if (y == NULL)
		Dic = add_node;
	else
	{
		strcpy_s(y_word, wordsize, y->word_name);
		check_(y_word);
		if (_strcmpi(add_node_word, y_word) < 0)
			y->left = add_node;
		else
			y->right = add_node;
	}
}

void delete_word(Dict *Dic, char* word)
{
	Dict* x, *y, *z;
	val = 1;
	find_word(Dic, word);
	if (del_p[0] == NULL)
		puts("Not found");

	else
	{
		for (int i = 0; i < 20; i++)
		{
			z = del_p[i];
			if (z->left == NULL || z->right == NULL)
				y = z;
			else
				y = TREE_SUCCESSOR(z);
			if (y->left != NULL)
				x = y->left;
			else
				x = y->right;
			if (x != NULL)
				x->parent = y->parent;
			if (y->parent == NULL)
				Dic = x;
			else if (y == y->parent->left)
				y->parent->left = x;
			else
				y->parent->right = x;
			if (y != z)
				swap(z, y);
			if(val2 == 0)
				puts("Deleted successfully");
		}
	}
}

void swap(Dict* z, Dict* y)
{
	Dict* temp;
	temp = y;
	y = z;
	z = y;
}

Dict* TREE_SUCCESSOR(Dict* x)
{
	Dict* y;
	if (x->right != NULL)
		return TREE_MINIMUM(x->right);
	y = x->parent;
	while (y != NULL && x == y->right)
	{
		x = y;
		y = y->parent;
	}
	return y;
}

Dict* TREE_MINIMUM(Dict* x)
{
	while (x->left != NULL)
		x = x->left;
	return x;
}

void deleteall(Dict *Dic)
{
	char buffer2[wordsize];
	FILE* f2;
	fopen_s(&f2, "to_be_deleted_words.txt", "r");
	while (fgets(buffer2, sizeof(buffer2), f2))
	{
		val2 = 1;
		delete_word(Dic, buffer2);
		del_cnt++;
	}
	printf("%d words were deleted successfully", del_cnt);
}
