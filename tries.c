/*
 * ENEL2DS 2026 - Practical: Tries
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

/* --- Trie node structure --- */

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int is_end;  /* 1 if this node marks the end of a word, 0 otherwise */
} TrieNode;


/* === Create a new trie node (provided) === */

TrieNode *create_node(void)
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node == NULL) {
        printf("Error: memory allocation failed.\n");
        exit(1);
    }
    node->is_end = 0;
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}


/* ============================================================
 * TODO 1: insert
 * ============================================================
 * Insert a word into the trie.
 *
 * Algorithm:
 *   1. Start at the root node.
 *   2. For each character in the word:
 *      a. Calculate its index: index = word[i] - 'a'
 *         (e.g., 'a' -> 0, 'b' -> 1, ..., 'z' -> 25)
 *      b. If children[index] is NULL, create a new node there.
 *      c. Move to children[index].
 *   3. After the last character, set is_end = 1.
 * ============================================================ */
void insert(TrieNode *root, char *word)
{
   TrieNode *current = root;

    int i;

    for (i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';

        if (current->children[index] == NULL) {
            current->children[index] = create_node();
        }

        current = current->children[index];
    }

    current->is_end = 1;  
}


/* ============================================================
 * TODO 2: search
 * ============================================================
 * Search for an exact word in the trie.
 *
 * Algorithm:
 *   1. Start at the root node.
 *   2. For each character in the word:
 *      a. Calculate its index: index = word[i] - 'a'
 *      b. If children[index] is NULL, the word is NOT in the
 *         trie -- return 0.
 *      c. Otherwise, move to children[index].
 *   3. After the last character, check is_end.
 *      - Return 1 if is_end == 1 (word exists).
 *      - Return 0 if is_end == 0 (only a prefix exists,
 *        not the full word).
 *
 * Think about this: if "apple" is in the trie, should
 * search for "app" return 1 or 0? Why?
 * ============================================================ */
int search(TrieNode *root, char *word)
{TrieNode *current = root;

    int i;

    for (i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';

        if (current->children[index] == NULL) {
            return 0;
        }

        current = current->children[index];
    }

    return current->is_end;
     
}


/* ============================================================
 * TODO 3: starts_with
 * ============================================================
 * Check if ANY word in the trie starts with the given prefix.
 *
 * This is very similar to search. The key difference:
 *   - search checks is_end at the final node.
 *   - starts_with does NOT check is_end.
 *
 * Return 1 if a word with this prefix exists, 0 otherwise.
 * ============================================================ */
int starts_with(TrieNode *root, char *prefix)
{
    TrieNode *current = root;

    int i;

    for (i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';

        if (current->children[index] == NULL) {
            return 0;
        }

        current = current->children[index];
    }

    return 1;
}


/* ============================================================
 * TODO 4: count_words
 * ============================================================
 * Count the total number of words stored in the trie.
 *
 * Algorithm (recursive):
 *   1. If the node is NULL, return 0.
 *   2. Start a count. If this node's is_end == 1, count = 1.
 *   3. For each of the 26 children, add count_words(child)
 *      to the count.
 *   4. Return the count.
 * ============================================================ */
int count_words(TrieNode *node)
{
    if (node == NULL) {
        return 0;
    }

    int count = 0;

    if (node->is_end == 1) {
        count = 1;
    }

    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        count += count_words(node->children[i]);
    }

    return count;
}


/* === Helper functions (provided) === */

void to_lowercase(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

void strip_newline(char *str)
{
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}


/* === Load words from file (provided) === */

int load_from_file(TrieNode *root, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file '%s'.\n", filename);
        return 0;
    }

    char word[100];
    int count = 0;
    while (fgets(word, sizeof(word), fp) != NULL) {
        strip_newline(word);
        to_lowercase(word);
        if (strlen(word) > 0) {
            insert(root, word);
            count++;
        }
    }

    fclose(fp);
    return count;
}


/* === Cleanup (provided) === */

void free_trie(TrieNode *node)
{
    if (node == NULL) {
        return;
    }

    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        free_trie(node->children[i]);
    }

    free(node);
}


/* === Main (provided) === */

int main(void)
{
    TrieNode *root = create_node();

    /* Load words from file */
    int loaded = load_from_file(root, "wordlist.txt");
    printf("Loaded %d words from file.\n", loaded);
    printf("Word count (from trie): %d\n\n", count_words(root));

    /* Interactive loop */
    char input[100];
    int choice;

    while (1) {
        printf("--- Trie Menu ---\n");
        printf("1. Insert a word\n");
        printf("2. Search for a word\n");
        printf("3. Check prefix\n");
        printf("4. Count words\n");
        printf("5. Exit\n");
        printf("Choice: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        choice = atoi(input);

        if (choice == 5) {
            break;
        }

        if (choice >= 1 && choice <= 3) {
            printf("Enter word/prefix: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
            strip_newline(input);
            to_lowercase(input);
        }

        switch (choice) {
            case 1:
                insert(root, input);
                printf("'%s' inserted.\n\n", input);
                break;

            case 2:
                if (search(root, input)) {
                    printf("'%s' found in the trie.\n\n", input);
                } else {
                    printf("'%s' NOT found.\n\n", input);
                }
                break;

            case 3:
                if (starts_with(root, input)) {
                    printf("Words with prefix '%s' exist.\n\n", input);
                } else {
                    printf("No words with prefix '%s'.\n\n", input);
                }
                break;

            case 4:
                printf("Total words stored: %d\n\n", count_words(root));
                break;

            default:
                printf("Invalid choice.\n\n");
                break;
        }
    }

    free_trie(root);
    printf("Trie freed. Goodbye.\n");

    return 0;
}