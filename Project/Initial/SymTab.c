/**
* Author: Sanhu Li
* Date & Time: 01/30/2015 01:57
* Description: Compiler assignment 1
*/
#include <string.h>
#include "SymTab.h"
#include "stdlib.h"

/**
* The hash function I use to make less collisions.
*
* Try to add up all the ASCII value of the chars in input
*/
unsigned int hash(const char *input) {
    unsigned int sum = 0;
    int i, len = (int) strlen(input);

    for (i = 0; i < len; ++i) {
        sum += input[i];
    }

    return sum;
}

struct SymTab *CreateSymTab(int Size) {
    struct SymTab *symTab;
    int i;

    if (Size < 0) {
        return NULL;
    }

    symTab = malloc(sizeof(struct SymTab));
    symTab->Size = Size;
    symTab->Contents = malloc(Size * sizeof(struct SymEntry *));

    for (i = 0; i < Size - 1; ++i) {
        symTab->Contents[i] = NULL;
    }

    return symTab;
}

void DestroySymTab(struct SymTab *ATable) {
    struct SymEntry *entry, *tmp;

    if (ATable == NULL) {
        return;
    }

    entry = FirstEntry(ATable);

    while (entry != NULL) {
        tmp = entry;
        entry = NextEntry(ATable, entry);
        free(tmp->Name);
//        Do not free the Attributes, because it may not be initialize yet
//        and the space may not be able to be freed
//        free(tmp->Attributes);
        free(tmp);
    }

    free(ATable->Contents);
    free(ATable);
}

bool EnterName(struct SymTab *ATable, const char *Name, struct SymEntry **AnEntry) {
    int index, size;
    struct SymEntry *tmp;
    //  Find the Entry with the Name:
    struct SymEntry *entry;

    if (ATable == NULL || Name == NULL || AnEntry == NULL) {
        return false;
    }

    size = ATable->Size;
    entry = FindName(ATable, Name);

    //      if found:
    //          return a pointer to the Entry as AnEntry parameter
    //          return false
    if (entry != NULL) {
        *AnEntry = entry;
        return false;
    }

    //      if not found:
    //          Get the hash of the Name to find the position of the bucket
    //          Create a new Entry
    //          Put the Entry in the first position
    //          return true
    index = hash(Name) % size;
    *AnEntry = malloc(sizeof(struct SymEntry));
    (*AnEntry)->Name = strdup(Name);
    (*AnEntry)->Attributes = NULL;
    tmp = ATable->Contents[index];
    ATable->Contents[index] = *AnEntry;
    (*AnEntry)->Next = tmp;
    return true;
}

struct SymEntry *FindName(struct SymTab *ATable, const char *Name) {
    //  Compute the hash for Name to get the index of the bucket
    int index = hash(Name) % ATable->Size;

    //  Go to that linked list and try to find an Entry with the name
    struct SymEntry *list = ATable->Contents[index];

    //  if no elements in the hash bucket
    if (list == NULL) {
        return false;
    }

    while (list != NULL) {
        //  found it
        if (!strcmp(list->Name, Name)) {
            return list;
        }
        list = list->Next;
    }
    //  if not found
    return NULL;
}

void SetAttr(struct SymEntry *AnEntry, void *Attributes) {
    AnEntry->Attributes = Attributes;
}

void *GetAttr(struct SymEntry *AnEntry) {
    return AnEntry->Attributes;
}

const char *GetName(struct SymEntry *AnEntry) {
    return AnEntry->Name;
}

struct SymEntry *FirstEntry(struct SymTab *ATable) {
    int i, size = ATable->Size;
    struct SymEntry *entry = NULL;

    if (size <= 0) {
        return NULL;
    }

    //  Search the SymTab to find the first element
    for (i = 0; i < size; ++i) {
        entry = ATable->Contents[i];
        if (entry != NULL) {
            return entry;
        }
    }
    return entry;
}

struct SymEntry *NextEntry(struct SymTab *ATable, struct SymEntry *AnEntry) {
    const char *Name = AnEntry->Name;
    int index, size;
    //  Try to find the Entry in the SymTab
    struct SymEntry *entry = FindName(ATable, Name);
    if (entry == NULL) {
        return NULL;
    }

    if (entry->Next != NULL) {
        return entry->Next;
    }

    //  Get the index of the bucket
    size = ATable->Size;
    index = hash(Name) % size;
    while (++index < size) {
        if (ATable->Contents[index] != NULL) {
            return ATable->Contents[index];
        }
    }
    return NULL;
}
