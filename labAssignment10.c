#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[26]; // One for each lowercase letter
    int count; // To store the number of occurrences of the word
};

// Initializes a trie node
struct TrieNode *createTrieNode()
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    for (int i = 0; i < 26; ++i)
    {
        node->children[i] = NULL;
    }
    node->count = 0;
    return node;
}

// Inserts the word to the trie structure
void insert(struct TrieNode *root, char *word)
{
    struct TrieNode *current = root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    current->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct TrieNode *root, char *word)
{
    struct TrieNode *current = root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            return 0; // Word not found
        }
        current = current->children[index];
    }
    return current->count;
}

// Deallocates the trie nodes
void deallocateTrie(struct TrieNode *node)
{
    if (node == NULL)
    {
        return;
    }
    for (int i = 0; i < 26; ++i)
    {
        deallocateTrie(node->children[i]);
    }
    free(node);
}

int readDictionary(char *filename, char ***pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Unable to open file.\n");
        exit(1);
    }

    int numWords;
    fscanf(file, "%d", &numWords);
    *pInWords = (char **)malloc(numWords * sizeof(char *));
    
    for (int i = 0; i < numWords; ++i)
    {
        (*pInWords)[i] = (char *)malloc(100 * sizeof(char));
        fscanf(file, "%s", (*pInWords)[i]);
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char **inWords = NULL;

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", &inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode *root = createTrieNode();
    for (int i = 0; i < numWords; i++)
    {
        insert(root, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    deallocateTrie(root);
    
    // Deallocate memory for inWords
    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }
    free(inWords);

    return 0;
}
