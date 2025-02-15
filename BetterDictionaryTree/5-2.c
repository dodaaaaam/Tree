#define _CRT_SECURE_NO_WARNINGS
#define MAX 50000
#define SWAP(x, y, t) ((t)=(x), (x)=(y), (y)=(t))

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// �迭�� ������ word + mean
typedef struct element {
    char* word;
    char* mean;
} element;

// ��� ����ü ����
typedef struct Node {
    char* word;
    char* mean;
    struct Node* llink;
    struct Node* rlink;
} Node;

void quick_sort(element* wordArr, int left, int right) {
    if (left < right) {
        int q = partition(wordArr, left, right);
        quick_sort(wordArr, left, q - 1);
        quick_sort(wordArr, q + 1, right);
    }
}

int partition(element* wordArr, int left, int right) {
    element pivot, temp;
    int low, high;

    low = left;
    high = right + 1;
    pivot = wordArr[left];

    do {
        do
            low++;
        while (low <= right && (strcmp(wordArr[low].word, pivot.word) < 0));
        do
            high--;
        while (high >= left && (strcmp(wordArr[high].word, pivot.word) > 0));
        if (low < high) SWAP(wordArr[low], wordArr[high], temp);
    } while (low < high);
    SWAP(wordArr[left], wordArr[high], temp);
    return high;
}

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

// �迭�� �̿��� ���� ���� Ʈ�� ����
Node* sortedArrayToBST(element* wordArr, int start, int end) {
    if (start > end) {
        return NULL;
    }

    int mid = start + (end - start) / 2;
    Node* node = createNode(wordArr[mid].word, wordArr[mid].mean);
    node->llink = sortedArrayToBST(wordArr, start, mid - 1);
    node->rlink = sortedArrayToBST(wordArr, mid + 1, end);
    return node;
}

// Ʈ������ ��带 ã�� �Լ�
Node* findNode(Node* root, char* input) {
    while (root != NULL) {
        int cmp = strcmp(root->word, input);
        if (cmp == 0) {
            return root;
        }
        else if (cmp > 0) {
            root = root->llink;
        }
        else {
            root = root->rlink;
        }
    }
    return NULL;
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

    int index = 0;
    element* wordArr = (element*)malloc(MAX * sizeof(element));
    char buffer[50];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // ���� ���� ����

        char* token = strtok(buffer, ":");
        if (token != NULL) {
            wordArr[index].word = _strdup(trim(token));
            token = strtok(NULL, ":");
            wordArr[index++].mean = (token != NULL) ? _strdup(trim(token)) : _strdup("");
        }
    }

    fclose(file);

    quick_sort(wordArr, 0, index - 1);

    Node* root = sortedArrayToBST(wordArr, 0, index - 1);

    if (root == NULL) {
        return 1;
    }

    int height = getHeight(root);
    int nodeNum = getNodeCount(root);

    printf("�ܾ� : %d��\nƮ�� ��ü ���� : %d\nƮ�� ��� �� : %d��\n", index, height, nodeNum);

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
        printf("����Ϸ��� 1, �׸��Ϸ��� 0�� �Է��ϼ���: ");
        scanf("%d", &over);
    }

    // �޸� ����
    for (int i = 0; i < index; i++) {
        free(wordArr[i].word);
        free(wordArr[i].mean);
    }
    free(wordArr);

    return 0;
}

