    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <ctype.h>

    #define PORT 8080
    #define MAX_BUF 2048
    #define USERS_FILE "users.txt"



void send_file(int client_fd, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("error opening file");
        return;
    }

    char buffer[MAX_BUF];
    while (fgets(buffer, sizeof(buffer), file)) {
        send(client_fd, buffer, strlen(buffer), 0);
    }

    fclose(file);
}

int user_exists(const char *username) {
    FILE *file = fopen(USERS_FILE, "r");
    if (!file) return 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, username, strlen(username)) == 0 && line[strlen(username)] == ':') {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}


void register_user(const char *username, const char *password) {
    FILE *file = fopen(USERS_FILE, "a");
    if (file) {
        fprintf(file, "%s:%s\n", username, password);
        fclose(file);
    }
}

int authenticate_user(const char *username, const char *password) {
    FILE *file = fopen(USERS_FILE, "r");
    if (!file) {
        perror("Error opening users file");
        return 0;
    }
    char line[256];
    char user[128], pass[128];
    while (fgets(line, sizeof(line), file)) {
    
        line[strcspn(line, "\r\n")] = 0; /*udalenie simvola new stroki*/
        
        if (sscanf(line, "%127[^:]:%127s", user, pass) == 2) {
            if (strcmp(user, username) == 0 && strcmp(pass, password) == 0) {
                fclose(file);
                return 1; 
            }
        }
    }
    fclose(file);
    return 0; 
}

void url_decode(char *dst, const char *src) {
    char a, b;
    while (*src) {
        if ((*src == '%') && ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b))) {
            if (a >= 'a') a -= 'a' - 'A';
            if (a >= 'A') a -= ('A' - 10);
            else a -= '0';
            if (b >= 'a') b -= 'a' - 'A';
            if (b >= 'A') b -= ('A' - 10);
            else b -= '0';
            *dst++ = 16 * a + b;
            src += 3;
        } else if (*src == '+') {
            *dst++ = ' ';
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}
void parse_post_data(const char *data, char *username, char *password) {
    char *username_start = strstr(data, "username=");
    char *password_start = strstr(data, "password=");

    if (!username_start || !password_start) {
        username[0] = '\0';
        password[0] = '\0';
        return;
    }

    username_start += strlen("username=");
    char *username_end = strchr(username_start, '&');
    if (username_end) {
        strncpy(username, username_start, username_end - username_start); /*username&*/
        username[username_end - username_start] = '\0';
    } else {
        strcpy(username, username_start);
    }

    password_start += strlen("password=");
    char *password_end = strchr(password_start, '&');
    if (password_end) {
        strncpy(password, password_start, password_end - password_start);
        password[password_end - password_start] = '\0';
    } else {
        strcpy(password, password_start);
    }
    
    username[strcspn(username, "\r\n")] = '\0';
    password[strcspn(password, "\r\n")] = '\0';

    url_decode(username, username);
    url_decode(password, password);
}


void handle_request(int client_fd, const char *request) {
    printf("request:\n%s\n", request);

    char username[128] = {0}, password[128] = {0};

    if (strstr(request, "GET /register") != NULL) {
        send_file(client_fd, "register.html");
    } else if (strstr(request, "GET /login") != NULL) {
        send_file(client_fd, "login.html");
    } else if (strstr(request, "POST /register") != NULL) {
        const char *data = strstr(request, "\r\n\r\n") + 4;
        parse_post_data(data, username, password);

        if (!user_exists(username)) {
            register_user(username, password);
            send_file(client_fd, "success.html");
        } else {
            send_file(client_fd, "failure.html");
        }
    } else if (strstr(request, "POST /login") != NULL) {
        const char *data = strstr(request, "\r\n\r\n") + 4;
        parse_post_data(data, username, password);

        if (authenticate_user(username, password)) {
            send_file(client_fd, "success.html");
        } else {
            send_file(client_fd, "failure.html");
        }
    } else {
        send_file(client_fd, "index.html");
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUF] = {0};
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failure");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failure");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listening failure");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("server running on %d\n", PORT);
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept error");
            continue;
        }
        read(client_fd, buffer, sizeof(buffer) - 1);
        handle_request(client_fd, buffer);
        close(client_fd);
    }
    close(server_fd);
    return 0;
}