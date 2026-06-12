#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(p) _mkdir(p)
#else
#include <sys/stat.h>
#define MKDIR(p) mkdir(p, 0755)
#endif

#define NOTES_DIR "notes"
#define INDEX_FILE "notes/index.txt"
#define LAST_ID_FILE "notes/last_id.txt"

static void ensure_notes_dir(void) {
    struct stat st;
    if (stat(NOTES_DIR, &st) != 0) {
        if (MKDIR(NOTES_DIR) != 0) {
            fprintf(stderr, "Failed to create %s: %s\n", NOTES_DIR, strerror(errno));
            exit(1);
        }
    }
}

static long get_next_id(void) {
    FILE *f = fopen(LAST_ID_FILE, "r+");
    long id = 0;
    if (!f) {
        f = fopen(LAST_ID_FILE, "w+");
        if (!f) return 1;
        fprintf(f, "0\n");
        fflush(f);
        rewind(f);
    }
    if (fscanf(f, "%ld", &id) != 1) id = 0;
    id += 1;
    freopen(LAST_ID_FILE, "w", f);
    if (f) {
        fprintf(f, "%ld\n", id);
        fclose(f);
    }
    return id;
}

static void append_index(long id, const char *title) {
    FILE *f = fopen(INDEX_FILE, "a");
    if (!f) {
        fprintf(stderr, "Failed to open index file: %s\n", strerror(errno));
        return;
    }
    fprintf(f, "%ld|%s\n", id, title);
    fclose(f);
}

static void remove_from_index(long id) {
    FILE *in = fopen(INDEX_FILE, "r");
    if (!in) return;
    FILE *out = fopen("notes/index.tmp", "w");
    if (!out) { fclose(in); return; }
    char line[4096];
    while (fgets(line, sizeof(line), in)) {
        long cur = 0;
        char *p = strchr(line, '|');
        if (p) {
            *p = '\0';
            cur = atol(line);
            *p = '|';
        }
        if (cur == id) continue;
        fputs(line, out);
    }
    fclose(in);
    fclose(out);
    remove(INDEX_FILE);
    rename("notes/index.tmp", INDEX_FILE);
}

static void cmd_add(const char *title) {
    ensure_notes_dir();
    long id = get_next_id();
    char path[1024];
    snprintf(path, sizeof(path), "notes/%ld.txt", id);
    FILE *f = fopen(path, "w");
    if (!f) { fprintf(stderr, "Failed to create note file: %s\n", strerror(errno)); return; }
    fprintf(f, "%s\n\n", title);
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        fputs(buffer, f);
    }
    fclose(f);
    append_index(id, title);
    printf("Added note %ld\n", id);
}

static void cmd_list(void) {
    FILE *f = fopen(INDEX_FILE, "r");
    if (!f) { printf("No notes yet.\n"); return; }
    char line[4096];
    while (fgets(line, sizeof(line), f)) {
        char *p = strchr(line, '|');
        if (p) {
            *p = '\0';
            long id = atol(line);
            char *title = p + 1;
            char *nl = strchr(title, '\n'); if (nl) *nl = '\0';
            printf("%ld: %s\n", id, title);
        }
    }
    fclose(f);
}

static void cmd_view(long id) {
    char path[1024];
    snprintf(path, sizeof(path), "notes/%ld.txt", id);
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "Note %ld not found.\n", id); return; }
    char buf[4096];
    while (fgets(buf, sizeof(buf), f)) fputs(buf, stdout);
    fclose(f);
}

static void cmd_delete(long id) {
    char path[1024];
    snprintf(path, sizeof(path), "notes/%ld.txt", id);
    if (remove(path) != 0) { fprintf(stderr, "Failed to delete note %ld\n", id); return; }
    remove_from_index(id);
    printf("Deleted note %ld\n", id);
}

static void print_usage(const char *prog) {
    printf("Usage: %s add <title>\n", prog);
    printf("       %s list\n", prog);
    printf("       %s view <id>\n", prog);
    printf("       %s delete <id>\n", prog);
}

int main(int argc, char **argv) {
    if (argc < 2) { print_usage(argv[0]); return 1; }
    const char *cmd = argv[1];
    if (strcmp(cmd, "add") == 0) {
        if (argc < 3) { fprintf(stderr, "add requires a title\n"); return 1; }
        cmd_add(argv[2]);
    } else if (strcmp(cmd, "list") == 0) {
        cmd_list();
    } else if (strcmp(cmd, "view") == 0) {
        if (argc < 3) { fprintf(stderr, "view requires id\n"); return 1; }
        long id = strtol(argv[2], NULL, 10);
        cmd_view(id);
    } else if (strcmp(cmd, "delete") == 0) {
        if (argc < 3) { fprintf(stderr, "delete requires id\n"); return 1; }
        long id = strtol(argv[2], NULL, 10);
        cmd_delete(id);
    } else {
        print_usage(argv[0]);
        return 1;
    }
    return 0;
}
