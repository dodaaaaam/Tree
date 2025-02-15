#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ��� ����ü ����
typedef struct Node {
    char* word;
    char* mean;
    struct Node* llink;
    struct Node* rlink;
} Node;

// ������ �����ϴ� �Լ�
char* trim(char* str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

// �� ��� ����
Node* createNode(char* word, char* mean) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->word = _strdup(word);
    newNode->mean = _strdup(mean);
    newNode->llink = NULL;
    newNode->rlink = NULL;
    return newNode;
}

// ���� Ž�� Ʈ���� �ܾ� ����
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

// �ܾ��� �� ã��
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


// Ʈ���� ���̸� ���ϴ� �Լ�
int getHeight(Node* root) {
    if (root == NULL) {
        return 0;
    }
    int leftHeight = getHeight(root->llink);
    int rightHeight = getHeight(root->rlink);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Ʈ���� ����� ����� ���� ���ϴ� �Լ�
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

    Node* root = NULL; // ��Ʈ ��带 NULL�� �ʱ�ȭ
    int wordNum = 0;

    char buffer[50];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // ���� ���� ����

        char* token = strtok(buffer, ":");
        if (token != NULL) {
            char* word = trim(token);
            token = strtok(NULL, ":");
            char* mean = (token != NULL) ? trim(token) : "";

            Node* newNode = createNode(word, mean);

            root = insert(root, newNode); // ��带 �����Ͽ� Ʈ�� ����
            wordNum++;
        }
    }
    fclose(file);

    if (root == NULL) {
        return 1;
    }

    int height = getHeight(root);
    int nodeNum = getNodeCount(root);

    printf("�ܾ� : %d��\nƮ�� ��ü ���� : %d\nƮ�� ��� �� : %d��\n", wordNum, height, nodeNum);

    int over = 1;
    while (over) {
        char input[50];
        printf("�˻��� �ܾ �Է��ϼ���: ");
        scanf("%s", input);
        Node* foundNode = findNode(root, input);
        if (foundNode != NULL) {
            printf("�ǹ�: %s (���� : %d)\n", foundNode->mean, getHeight(foundNode));
        }
        else {
            printf("�ܾ ã�� �� �����ϴ�.\n");
        }
        printf("����Ϸ��� 1, �׸��Ϸ��� 0�� �Է��ϼ��� : ");
        scanf("%d", &over);
    }
    
    return 0;
}