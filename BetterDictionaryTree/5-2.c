#define _CRT_SECURE_NO_WARNINGS
#define MAX 50000
#define SWAP(x, y, t) ((t)=(x), (x)=(y), (y)=(t))

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// 배열에 저장할 word + mean
typedef struct element {
    char* word;
    char* mean;
} element;

// 노드 구조체 정의
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

// 배열을 이용해 균형 잡힌 트리 생성
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

// 트리에서 노드를 찾는 함수
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

    int index = 0;
    element* wordArr = (element*)malloc(MAX * sizeof(element));
    char buffer[50];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // 개행 문자 제거

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

    printf("단어 : %d개\n트리 전체 높이 : %d\n트리 노드 수 : %d개\n", index, height, nodeNum);

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
        printf("계속하려면 1, 그만하려면 0을 입력하세요: ");
        scanf("%d", &over);
    }

    // 메모리 해제
    for (int i = 0; i < index; i++) {
        free(wordArr[i].word);
        free(wordArr[i].mean);
    }
    free(wordArr);

    return 0;
}

