#ifndef STRINGTREE_H_INCLUDED
#define STRINGTREE_H_INCLUDED

#define CHECK() (*buffer == '(' || *buffer == ')')

#define LEFT_BRACKET()\
if (*buffer == '(')\
{\
    Node* newnode = (Node*)calloc(1, sizeof(Node));\
    if (!newnode) return NULL;\
\
    curr->left = newnode;\
    curr = CreateLeave(newnode, curr);\
}

#define RIGHT_BRACKET()\
while (*buffer == ')')\
{\
    curr = curr->parent;\
    if (curr == NULL) break;\
    if (curr->right != NULL) \
    {\
        buffer += 1;\
        continue;\
    }\
\
    Node* newnode = (Node*)calloc(1, sizeof(Node));\
    if (!newnode) return NULL;\
\
    curr->right = newnode;\
    curr = CreateLeave(newnode, curr);\
    break;\
}

#include <stdint.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct Node
{
    char* val;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int length;
} Node;

//char*
Node* GetStringTreePreorder(const char* filename);
Node* CreateStringNode(Node* parent, char* val);
Node* PreorderStringConvert(const char* buffer);
void string_tree_print(Node* tree, FILE* fstream);
void string_tree_destroy(Node* tree);
void PrintStringLeaves(Node* tree);
void TreePrintDot(Node* root, const char* filename);
void string_tree_print(Node* node, FILE* stream);
const char* sscan(Node* tree, const char* buffer);

//universal
char* GetBuffer(const char* filename);
Node* CreateLeave(Node* newnode, Node* parent);

Node* CreateLeave(Node* newnode, Node* parent)
{
    newnode->parent = parent;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

const char* sscan(Node* curr, const char* buffer)
{
    while (CHECK())
    {
        buffer += 1;
        if (*buffer == 0) buffer;
    }

    curr->val = (char*)calloc(1, sizeof(char));
    if (curr->val == NULL) return NULL;

    curr->length = 0;

    while(!CHECK())
    {
        curr->length += 1;
        curr->val = (char*)realloc(curr->val, curr->length + 1);
        if (curr->val == NULL) return NULL;

        //printf("%c ", *buffer);
        curr->val[curr->length - 1] = *buffer;
        curr->val[curr->length] = 0;
        ++buffer;
    }

    return buffer;
}

Node* PreorderStringConvert(const char* buffer)
{
    Node* root = (Node*)calloc(1, sizeof(Node));
    if (!root) return NULL;

    Node* curr = root;
    buffer += 1;

    while (*buffer != 0)
    {
        //printf("%s\n", buffer);
        buffer = sscan(curr, buffer);
        printf("%s\n", curr->val);
        
        RIGHT_BRACKET();
        LEFT_BRACKET();
        
        buffer += 1;
    }

    return root;
}

char* GetBuffer(const char* filename)
{
    struct stat file_info;
    stat(filename, &file_info);
    int size = file_info.st_size;
    FILE* tree = fopen(filename, "r");
    char* buffer = (char*)calloc(size, sizeof(char));
    if (buffer == NULL) return NULL;
    fread(buffer, sizeof(char), size, tree);

    fclose(tree);
    //printf("%s\n", buffer);

    return buffer;
}

Node* GetStringTreePreorder(const char* filename)
{
    return PreorderStringConvert(GetBuffer(filename));
}

void string_tree_destroy(Node* node)
{
    if (node != NULL) 
    {
        free(node->val);
	    string_tree_destroy(node->left);
        string_tree_destroy(node->right);
        free(node);
    }
}

void string_tree_print(Node* node, FILE* stream)
{
    fprintf(stream, "(");
    fprintf(stream, "%s", node->val);
    if (node->left != NULL) string_tree_print(node->left, stream);
    if (node->right != NULL) string_tree_print(node->right, stream);
    fprintf(stream, ")");
}

static long long g_node_id = 0;

static void print_tree_recursive(FILE* dump, Node* node, int is_root)
{
    if (node == NULL) return;

    int current_id = g_node_id++;

    /* Определяем тип узла: корень / лист / внутренний */
    const char* fillcolor;
    if (is_root)
        fillcolor = "orange";
    else if (node->left == NULL && node->right == NULL)
        fillcolor = "lightgreen";
    else
        fillcolor = "yellow";

    fprintf(dump,
        " node%d [label=\"%s\", shape=box, style=filled, fillcolor=%s];\n",
        current_id, node->val, fillcolor);

    if (node->left != NULL)
    {
        int left_id = g_node_id;

        fprintf(dump,
            " node%d -> node%d [label=\"YES\", color=blue, penwidth=1.5, dir=both];\n",
            current_id, left_id);

        print_tree_recursive(dump, node->left, 0);
    }

    if (node->right != NULL)
    {
        int right_id = g_node_id;

        fprintf(dump,
            " node%d -> node%d [label=\"NO\", color=blue, penwidth=1.5, dir=both];\n",
            current_id, right_id);

        print_tree_recursive(dump, node->right, 0);
    }
}

void TreePrintDot(Node* root, const char* filename)
{
    FILE* dump = fopen(filename, "w");
    if (dump == NULL) return;

    g_node_id = 0;

    fprintf(dump, "digraph Tree {\n");
    fprintf(dump, " rankdir=TB;\n");
    fprintf(dump, " splines=true;\n");
    fprintf(dump, " nodesep=0.5;\n");
    fprintf(dump, " ranksep=1.0;\n");
    fprintf(dump, " node [fontname=\"Helvetica\"];\n");
    fprintf(dump, " edge [fontname=\"Helvetica\", fontsize=10];\n");   /* шрифт подписей */

    print_tree_recursive(dump, root, 1);   /* root помечаем как корень */

    fprintf(dump, "}\n");
    fclose(dump);

    char cmd[256];
    const char* dot_path = "\"C:\\Program Files\\Graphviz\\bin\\dot.exe\"";
    snprintf(cmd, sizeof(cmd), "%s -Tpng %s -o Akinator.png", dot_path, filename);

    int result = system(cmd);
    if (result == 0) printf("Tree has been compiled into Akinator.png correctly\n");
    else printf("ERROR compiling tree graph\n");

    fclose(dump);
}


#endif
