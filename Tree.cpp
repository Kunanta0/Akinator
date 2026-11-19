#define NONE -1
#include <stdio.h>
//#include "tree.h"
#include "Akinator.h"

int main(int argc, char* argv[])
{
    /*int root_val = 0;
    printf("Enter the root value: ");
    scanf("%d", &root_val);
    TreeNode_t* root = CreateIntNode(NULL, root_val);

    printf("ROOT: %d\nEnter numbers, nan to quit: ", root->val);
    
    int n = 0;
    while (1)
    {
        if (scanf("%d", &n) == 0) break;

        tree_append(root, n);
    }

    FILE* test = fopen(argv[1], "w");
    FillNils(root);
    int_tree_print(root, test);
    fclose(test);

    TreeDumpDot(root, argv[2]);*/

    Node* root = GetStringTreePreorder(argv[1]);
    if (root == NULL) printf("gg\n");

    while(Akinator(root, argv[1]) == 1) continue;
    
    TreePrintDot(root, argv[2]);

    string_tree_destroy(root);
    
    return 0;
}

//argv[1] = test.txt
//argv[2] = dump.txt
//#define RED "\033[91m"вводим числа, дерево преордером записывается в test.txt, потом попадает в NewTree и дампим
