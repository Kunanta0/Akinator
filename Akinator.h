#ifndef AKINATOR_H_INCLUDED
#define AKINATOR_H_INCLUDED
#include "StringTree.h"
#include <malloc.h>
#include <windows.h>

#define GREEN "\033[32m"
#define RED "\033[91m"
#define RESET "\033[0m"

#define ASK(prompt)\
do\
{\
    prompt;\
    fgets(buffer, 100, stdin);\
    buffer[strcspn(buffer, "\n")] = '\0';\
    printf("\n");\
} while(buffer[0] != 'y' && buffer[0] != 'n')
        
enum
{
    FINISH_GAME = 0,
    PLAY_AGAIN = 1
};

int Akinator(Node* root, const char* filename);

int Akinator(Node* root, const char* filename)
{
    Node* curr = root;
    char buffer[100] = {};

    while (curr->left != NULL && curr->right != NULL)
    {
        ASK(printf("Is your character %s?\n[y/n]\n", curr->val));

        if (*buffer == 'y') curr = curr->left;
        else curr = curr->right;

        *buffer = 0;
    }

    ASK(printf("I think it is " GREEN "%s" RESET"\n[y/n]\n", curr->val));

    if (*buffer == 'y') 
    {
        ASK(printf("I won again! Play one more?\n[y/n]\n"));

        if (buffer[0] == 'y') return PLAY_AGAIN;
        else return FINISH_GAME;
    }

    else 
    {
        ASK(printf("Congrats, you beat me! Do you want to add your character?\n[y/n]\n"));
        if (buffer[0] == 'n') return FINISH_GAME;

        else
        {
            FILE* tree = fopen(filename, "w");
            printf("Enter the characteristic, which your character has, but " RED "%s" RESET" ""doesn't have: ", curr->val);
            fgets(buffer, 100, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';

            Node* left = (Node*)calloc(1, sizeof(Node));
            left->left = NULL;
            left->right = NULL;

            Node* right = (Node*)calloc(1, sizeof(Node));
            right->left = NULL;
            right->right = NULL;

            right->val = (char*) calloc(curr->length, sizeof 1);
            if (right->val == NULL) return FINISH_GAME;

            right->val = strcpy(right->val, curr->val);

            curr->length = strlen(buffer);
            curr->val = (char*)calloc(strlen(buffer), sizeof 1);
            if (curr->val == NULL) return FINISH_GAME;
            curr->val = strcpy(curr->val, buffer);

            //printf("%s\n", buffer);

            curr->right = right;
            curr->left = left;
            curr = curr->left;

            printf("Enter the character: ");
            fgets(buffer, 100, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';        

            curr->length = strlen(buffer);
            curr->val = (char*)calloc(strlen(buffer), sizeof 1);
            curr->val = strcpy(curr->val, buffer);            

            string_tree_print(root, tree);

            fclose(tree);
            ASK(printf("Play one more?\n[y/n]\n"));

            if (buffer[0] == 'y') return PLAY_AGAIN;
            else return FINISH_GAME;
        }
    }
    return FINISH_GAME;
}

#endif
