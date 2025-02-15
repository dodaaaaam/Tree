#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 노드 구조체 정의
typedef struct Node {
    char* word;
    char* mean;
    struct Node* llink;
    struct Node* rlink;
} Node;

// 공백을 제거하는 함수
char* trim(char* str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

// 새 노드 생성
Node* createNode(char* word, char* mean) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->word = _strdup(word);
    newNode->mean = _strdup(mean);
    newNode->llink = NULL;
    newNode->rlink = NULL;
    return newNode;
}

// 이진 탐색 트리에 단어 삽입
Node* insert(Node* root, Node* newNode) {
    if (root == NULL) {
        return newNode;
    }

    if (strcmp(newNode->word, root->word) < 0) {
        root->llink = insert(root->llink, newNode);
    }
    else if (strcmp(newNode->word, root->word) > 0) {
        root->rlink = insert(root->rlink, newNode);
    }
    return root;
}

// 단어의 뜻 찾기
Node* findNode(Node* root, char* input) {
    if (root == NULL) {
        return NULL;
    }

    Node* foundNode = findNode(root->llink, input);

    if (foundNode != NULL) {
        return foundNode;
    }

    if (strcmp(root->word, input) == 0) {
        return root;
    }

    return findNode(root->rlink, input);
}


// 트리의 높이를 구하는 함수
int getHeight(Node* root) {
    if (root == NULL) {
        return 0;
    }
    int leftHeight = getHeight(root->llink);
    int rightHeight = getHeight(root->rlink);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// 트리에 저장된 노드의 수를 구하는 함수
int getNodeCount(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + getNodeCount(root->llink) + getNodeCount(root->rlink);
}


int main() {
    FILE* file = fopen("randdict.txt", "r");

    if (file == NULL) {
        perror("no file");
        return 0;
    }

    Node* root = NULL; // 루트 노드를 NULL로 초기화
    int wordNum = 0;

    char buffer[50];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // 개행 문자 제거

        char* token = strtok(buffer, ":");
        if (token != NULL) {
            char* word = trim(token);
            token = strtok(NULL, ":");
            char* mean = (token != NULL) ? trim(token) : "";

            Node* newNode = createNode(word, mean);

            root = insert(root, newNode); // 노드를 삽입하여 트리 갱신
            wordNum++;
        }
    }
    fclose(file);

    if (root == NULL) {
        return 1;
    }

    int height = getHeight(root);
    int nodeNum = getNodeCount(root);

    printf("단어 : %d개\n트리 전체 높이 : %d\n트리 노드 수 : %d개\n", wordNum, height, nodeNum);

    int over = 1;
    while (over) {
        char input[50];
        printf("검색할 단어를 입력하세요: ");
        scanf("%s", input);
        Node* foundNode = findNode(root, input);
        if (foundNode != NULL) {
            printf("의미: %s (레벨 : %d)\n", foundNode->mean, getHeight(foundNode));
        }
        else {
            printf("단어를 찾을 수 없습니다.\n");
        }
        printf("계속하려면 1, 그만하려면 0을 입력하세요 : ");
        scanf("%d", &over);
    }
    
    return 0;
}