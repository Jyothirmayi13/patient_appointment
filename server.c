#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "avl.h"

#define PORT 8080
#define BUFFER_SIZE 4096
#define DATA_FILE "appointments.txt"

/* ── Global AVL root ── */
AVLNODE root = NULL;

/* ── Forward declarations ── */
void saveToFile();
void handleRequest(int client_fd);
void sendResponse(int fd, int status, const char *ctype, const char *body);
void routeGET(int fd, const char *path);
void routePOST(int fd, const char *path, const char *body);
void routeDELETE(int fd, const char *path);
void buildJSON(AVLNODE node, char *buf, int *first);

/* ════════════════════════════════════════
   FILE I/O
   ════════════════════════════════════════ */

void saveToFile() {
    FILE *fp = fopen(DATA_FILE, "w");
    if (!fp) { printf("Error saving file.\n"); return; }

    /* in-order traversal using a simple stack approach via recursion
       but we need a helper that takes fp */
    void writeNode(AVLNODE n) {
        if (!n) return;
        writeNode(n->left);
        fprintf(fp, "%d,%s,%s,%s,%s,%d\n",
            n->data.id,
            n->data.patientName,
            n->data.docName,
            n->data.date,
            n->data.timeSlot,
            n->data.priority);
        writeNode(n->right);
    }
    writeNode(root);
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) { printf("No existing data. Starting fresh.\n"); return; }

    Appointment appt;
    while (fscanf(fp, "%d,%49[^,],%49[^,],%14[^,],%9[^,\n],%d\n",
                  &appt.id,
                  appt.patientName,
                  appt.docName,
                  appt.date,
                  appt.timeSlot,
                  &appt.priority) == 6) {
        initQueue(&appt.history);
        root = insertNodeInAVL(root, appt);
    }
    fclose(fp);
    printf("Loaded appointments from %s\n", DATA_FILE);
}

/* ════════════════════════════════════════
   JSON BUILDER
   ════════════════════════════════════════ */

void buildJSON(AVLNODE node, char *buf, int *first) {
    if (!node) return;
    buildJSON(node->left, buf, first);

    char entry[512];
    if (!*first) strcat(buf, ",");
    *first = 0;

    snprintf(entry, sizeof(entry),
        "{\"id\":%d,\"name\":\"%s\",\"doctor\":\"%s\","
        "\"date\":\"%s\",\"slot\":\"%s\",\"priority\":%d}",
        node->data.id,
        node->data.patientName,
        node->data.docName,
        node->data.date,
        node->data.timeSlot,
        node->data.priority);
    strcat(buf, entry);

    buildJSON(node->right, buf, first);
}

/* ════════════════════════════════════════
   HTTP HELPERS
   ════════════════════════════════════════ */

void sendResponse(int fd, int status, const char *ctype, const char *body) {
    char header[512];
    const char *statusText = (status == 200) ? "OK" :
                             (status == 201) ? "Created" :
                             (status == 400) ? "Bad Request" :
                             (status == 404) ? "Not Found" : "Error";

    snprintf(header, sizeof(header),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, DELETE, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n\r\n",
        status, statusText, ctype, strlen(body));

    write(fd, header, strlen(header));
    write(fd, body, strlen(body));
}

/* ════════════════════════════════════════
   ROUTES
   ════════════════════════════════════════ */

/* GET /appointments → return all as JSON */
void routeGET(int fd, const char *path) {
    if (strcmp(path, "/appointments") == 0) {
        char *buf = calloc(1, 65536);
        strcat(buf, "[");
        int first = 1;
        buildJSON(root, buf, &first);
        strcat(buf, "]");
        sendResponse(fd, 200, "application/json", buf);
        free(buf);
    } else {
        sendResponse(fd, 404, "application/json", "{\"error\":\"Not found\"}");
    }
}

/* POST /appointments  body: id,name,doctor,date,slot,priority */
void routePOST(int fd, const char *path, const char *body) {
    if (strcmp(path, "/appointments") == 0) {
        Appointment appt;
        initQueue(&appt.history);

        if (sscanf(body, "%d,%49[^,],%49[^,],%14[^,],%9[^,],%d",
                   &appt.id,
                   appt.patientName,
                   appt.docName,
                   appt.date,
                   appt.timeSlot,
                   &appt.priority) == 6) {

            /* check duplicate */
            if (search(root, appt.id) != NULL) {
                sendResponse(fd, 400, "application/json",
                             "{\"error\":\"ID already exists\"}");
                return;
            }

            root = insertNodeInAVL(root, appt);
            enqueue(&(search(root, appt.id)->data.history), appt.id);
            saveToFile();
            sendResponse(fd, 201, "application/json", "{\"status\":\"added\"}");
        } else {
            sendResponse(fd, 400, "application/json", "{\"error\":\"Bad data format\"}");
        }
    } else {
        sendResponse(fd, 404, "application/json", "{\"error\":\"Not found\"}");
    }
}

/* DELETE /appointments/101 */
void routeDELETE(int fd, const char *path) {
    if (strncmp(path, "/appointments/", 14) == 0) {
        int id = atoi(path + 14);
        if (search(root, id) == NULL) {
            sendResponse(fd, 404, "application/json", "{\"error\":\"ID not found\"}");
            return;
        }
        root = deleteNodeInAVL(root, id);
        saveToFile();
        sendResponse(fd, 200, "application/json", "{\"status\":\"deleted\"}");
    } else {
        sendResponse(fd, 404, "application/json", "{\"error\":\"Not found\"}");
    }
}

/* ════════════════════════════════════════
   REQUEST HANDLER
   ════════════════════════════════════════ */

void handleRequest(int client_fd) {
    char buf[BUFFER_SIZE] = {0};
    read(client_fd, buf, BUFFER_SIZE - 1);

    /* Parse method and path */
    char method[10], path[256];
    sscanf(buf, "%9s %255s", method, path);

    printf("[%s] %s\n", method, path);

    /* Handle CORS preflight */
    if (strcmp(method, "OPTIONS") == 0) {
        sendResponse(client_fd, 200, "text/plain", "");
        return;
    }

    /* Find body (after \r\n\r\n) */
    char *body = strstr(buf, "\r\n\r\n");
    if (body) body += 4; else body = "";

    if (strcmp(method, "GET") == 0)         routeGET(client_fd, path);
    else if (strcmp(method, "POST") == 0)   routePOST(client_fd, path, body);
    else if (strcmp(method, "DELETE") == 0) routeDELETE(client_fd, path);
    else sendResponse(client_fd, 400, "application/json", "{\"error\":\"Unknown method\"}");
}

/* ════════════════════════════════════════
   MAIN
   ════════════════════════════════════════ */

int main() {
    loadFromFile();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port        = htons(PORT)
    };

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind"); return 1;
    }
    listen(server_fd, 10);

    printf("\n========================================\n");
    printf("  Patient Appointment Server running\n");
    printf("  http://localhost:%d\n", PORT);
    printf("  Open dashboard.html in your browser\n");
    printf("========================================\n\n");

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) { perror("accept"); continue; }
        handleRequest(client_fd);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
