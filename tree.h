#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#define CHECK() (*buffer == '(' || *buffer == ')')
#define SKIP_SPACES()\
if (*buffer == ' ')\
{\
    ++buffer;\
    continue;\
}

#define LEFT_BRACKET()\
if (*buffer == '(')\
{\
    TreeNode_t* newnode = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));\
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
    TreeNode_t* newnode = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));\
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

typedef struct TreeNode_t
{
    int val;
    struct TreeNode_t* left;
    struct TreeNode_t* right;
    struct TreeNode_t* parent;
} TreeNode_t;

//int
void int_tree_destroy(TreeNode_t* tree);
TreeNode_t* tree_add(TreeNode_t* parent, TreeNode_t* tree, int x);
TreeNode_t* GetIntTreePreorder(const char* filename);
TreeNode_t* CreateIntNode(TreeNode_t* parent, int val);
TreeNode_t* PreorderIntConvert(const char* buffer);
void int_tree_print(TreeNode_t* tree, FILE* fstream);
void FillNils(TreeNode_t* tree);
void PrintIntLeaves(TreeNode_t* tree);


//universal
char* GetBuffer(const char* filename);
TreeNode_t* CreateLeave(TreeNode_t* newnode, TreeNode_t* curr);

TreeNode_t* CreateLeave(TreeNode_t* newnode, TreeNode_t* curr)
{
    newnode->parent = curr;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

void FillNils(TreeNode_t* tree)
{
    if (tree->left == NULL) 
    {
        if (tree->right == NULL) return;
        else 
        {
            tree->left = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
            if (tree->left == NULL) return;

            tree->left->val = NONE;
        }
    }

    else if (tree->right == NULL) 
    {
        tree->right = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
        if (tree->right == NULL) return;

        tree->right->val = NONE;
    }

    FillNils(tree->left);
    FillNils(tree->right);
}

TreeNode_t* PreorderIntConvert(const char* buffer)
{
    TreeNode_t* root = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (!root) return NULL;

    TreeNode_t* curr = root;
    buffer += 1;

    while (*buffer != 0)
    {
        int move = 0;
        SKIP_SPACES();
        //printf("%s\n", buffer);
        while (sscanf(buffer, "%d%n", &(curr->val), &move) == 0) 
        {
            buffer += 1;
            if (*buffer == 0) break;
        }
        if (*buffer == 0) break;
        buffer += move;
        
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
    fread(buffer, sizeof(char), size, tree);

    fclose(tree);
    //printf("%s\n", buffer);

    return buffer;
}

TreeNode_t* GetIntTreePreorder(const char* filename)
{
    return PreorderIntConvert(GetBuffer(filename));
}   

void PrintIntLeaves(TreeNode_t *node) 
{
    if (node == NULL) return;

    if (node->left == NULL && node->right == NULL) 
    {
        printf("%d ", node->val);
        return;
    }

    PrintIntLeaves(node->left);
    PrintIntLeaves(node->right);
}

TreeNode_t* CreateIntNode(TreeNode_t* parent, int val) 
{
    TreeNode_t* tree = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    if (tree == NULL) return NULL;
    
    tree->val = val;
    tree->left = NULL;
    tree->right = NULL;
    tree->parent = parent;
    return tree;
}

void int_tree_destroy(TreeNode_t* node)
{
    if (node != NULL) 
    {
	    int_tree_destroy(node->left);
        int_tree_destroy(node->right);
        free(node);
    }
}

void int_tree_print(TreeNode_t* node, FILE* stream)
{
    fprintf(stream, "(");
    fprintf(stream, "%d", node->val);
    if (node->left != NULL) int_tree_print(node->left, stream);
    if (node->right != NULL) int_tree_print(node->right, stream);
    fprintf(stream, ")");
}

TreeNode_t* tree_append(TreeNode_t* root, int x) 
{
    return tree_add(NULL, root, x);
}

TreeNode_t* tree_add(TreeNode_t* parent, TreeNode_t* tree, int x)
{
    if (tree == NULL)
    {
    	tree = (TreeNode_t*)calloc(1, sizeof(TreeNode_t));
    	if (tree == NULL) return NULL;

	    tree->val = x;
	    tree->left = NULL;
	    tree->right = NULL;
        tree->parent = parent;

	    return tree;
    }

    if (x < tree->val) tree->left = tree_add(tree, tree->left, x);
    else if (x > tree->val) tree->right = tree_add(tree, tree->right, x);

    return tree;
}

static int g_node_id = 0;

static void print_tree_recursive(FILE* dump, TreeNode_t* node)
{
    if (node == NULL) return;

    int current_id = g_node_id++;

    fprintf(dump, " node%d [label=\"%s\", shape=circle, style=filled, fillcolor=lightblue];\n", current_id, node->val);

    if (node->left != NULL)
    {
        int left_id = g_node_id; 
        
        fprintf(dump, " node%d -> node%d [color=blue, penwidth=1.5, dir=both];\n", current_id, left_id);
       
        print_tree_recursive(dump, node->left);
    }

    if (node->right != NULL)
    {
        int right_id = g_node_id;

        fprintf(dump, " node%d -> node%d [color=blue, penwidth=1.5, dir=both];\n", current_id, right_id);
        
        print_tree_recursive(dump, node->right);
    }
}

void TreePrintDot(TreeNode_t* root, const char* filename)
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

    print_tree_recursive(dump, root);

    fprintf(dump, "}\n");
    fclose(dump);

    char cmd[256];
    const char* dot_path = "\"C:\\Program Files\\Graphviz\\bin\\dot.exe\"";
    snprintf(cmd, sizeof(cmd), "%s -Tpng %s -o int_tree_print.png", dot_path, filename);
    
    int result = system(cmd);
    if (result == 0) printf("Tree has been compiled into tree.png correctly\n");
    else printf("ERROR compiling tree graph\n");
}

static unsigned int short_address(const void *p) {
    return (unsigned int)((uintptr_t)p & 0xFFFFFFFFu);
}

static const char* nz(const void *ptr, char *buf, size_t n) {
    if (ptr) snprintf(buf, n, "%08X", short_address(ptr));
    else   snprintf(buf, n, "(nil)");
    return buf;
}

static void dump_int_tree_recursive(FILE *dump, TreeNode_t *node)
{
    if (node == NULL) return;

    char k_self[16];
    snprintf(k_self, sizeof k_self, "%08X", short_address(node));

    char aParent[16], aLeft[16], aRight[16];
    nz(node->parent, aParent, sizeof aParent);
    nz(node->left,   aLeft, sizeof aLeft);
    nz(node->right,  aRight, sizeof aRight);

    fprintf(dump,
        "  n%s [shape=box, style=\"filled,rounded\", fillcolor=\"lightblue\", label=<\n"
        "    <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"2\">\n"
        "      <TR><TD ALIGN=\"LEFT\">val     = %d</TD></TR>\n"
        "      <TR><TD ALIGN=\"LEFT\">address = %s</TD></TR>\n"
        "      <TR><TD ALIGN=\"LEFT\">parent  = %s</TD></TR>\n"
        "      <TR><TD ALIGN=\"LEFT\">left    = %s | right = %s</TD></TR>\n"
        "    </TABLE>>];\n",
        k_self, node->val, k_self, aParent, aLeft, aRight);

    if (node->left) {
        fprintf(dump, "  n%s -> n%08X [label=\"L\", color=blue, penwidth=1.5, dir=both];\n",
                k_self, short_address(node->left));
        dump_int_tree_recursive(dump, node->left);
    }
    if (node->right) {
        fprintf(dump, "  n%s -> n%08X [label=\"R\", color=blue, penwidth=1.5, dir=both];\n",
                k_self, short_address(node->right));
        dump_int_tree_recursive(dump, node->right);
    }
}

void TreeDumpDot(TreeNode_t *root, const char *filename)
{
    FILE *dump = fopen(filename, "w");
    if (dump == NULL) return;

    fprintf(dump, "digraph Tree {\n");
    fprintf(dump, " rankdir=TB;\n");
    fprintf(dump, " splines=true;\n");
    fprintf(dump, " nodesep=0.5;\n");
    fprintf(dump, " ranksep=1.0;\n");
    fprintf(dump, " node [shape=plaintext, fontname=\"monospace\", fontsize=10];\n");
    fprintf(dump, " edge [fontname=\"monospace\", fontsize=9];\n");

    dump_int_tree_recursive(dump, root);

    fprintf(dump, "}\n");
    fclose(dump);

    char cmd[512];
    const char *dot_path = "\"C:\\Program Files\\Graphviz\\bin\\dot.exe\"";
    snprintf(cmd, sizeof cmd, "%s -Tpng %s -o int_tree_dump.png", dot_path, filename);

    int result = system(cmd);
    if (result == 0) printf("Tree has been compiled into tree.png correctly\n");
    else             printf("ERROR compiling tree graph\n");
}

#endif

//напиши прогу, которая создает дерево (как выше) из преордерной строки типа (50(20(10)(30))(60(52)(67))). Возвращает корень дерева.
