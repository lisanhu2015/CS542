#include <stdbool.h>
#include <string.h>
#include "IOMngr.h"

FILE *source;
FILE *listing;
int lineno = 0;
int colno = 0;
char line[MAXLINE + 1];

bool newLine = true;

void processLine(char * line) {
    size_t len = strlen(line);
    if (len > 1 && len < MAXLINE && line[len - 1] != '\n') {
        line[len] = '\n';
        line[len + 1] = '\0';
    }
}

bool OpenFiles(const char *ASourceName, const char *AListingName) {
    source = fopen(ASourceName, "r");
    listing = stdout;

    if (AListingName != NULL) {
        listing = fopen(AListingName, "w");
    }

    if (source == NULL || listing == NULL) {
        return false;
    }

    return true;
}

void CloseFiles() {
    if (source != NULL) {
        fclose(source);
        source = NULL;
    }

    if (listing != NULL && listing != stdout) {
        fclose(listing);
        listing = NULL;
    }
}

char GetSourceChar() {
    char c;
    char * result;

    if (colno == 0) {
        lineno++;
        result = fgets(line, MAXLINE + 1, source);
        processLine(line);

        if (result == NULL) {
            return EOF;
        }

        if (listing != stdout) {
            fprintf(listing, "%-3d: %s", lineno, line);
        }
    }

    c = line[colno++];
    if (c == '\n') {
        colno = 0;
        newLine = true;
    }
    return c;
}

void WriteIndicator(int AColumn) {
    int i;
    if (listing == stdout && newLine) {
        fprintf(listing, "%-3d: %s", lineno, line);
        newLine = false;
    }
    for (i = 0; i < AColumn + 5; ++i) {
        fprintf(listing, " ");
    }
    fprintf(listing, "^\n");
}

void WriteMessage(const char *AMessage) {
    fprintf(listing, "%s\n", AMessage);
}

int GetCurrentLine() {
    return lineno;
}

int GetCurrentColumn() {
    return colno;
}
