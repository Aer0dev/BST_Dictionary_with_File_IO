#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <fstream>
using namespace std;

typedef struct Node {
    string word;
    string meaning;
    struct Node *left;
    struct Node *right;
}Node;

Node* insert(Node* node, string word, string meaning)
{
    if(node == nullptr) // 노드가 비어있다면 새로운 노드 생성
    {
        node = new Node;
        node->word = word;
        node->meaning = meaning;
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    else{       // 비어있지 않다면 res를 통해 위치 선정
        int res = word.compare(node->word);
        if(res > 0){
            node->right = insert(node->right, word, meaning);
        }
        else if(res < 0){
            node->left = insert(node->left, word, meaning);
        }
        return node;
    }
}

Node* findMinNode(Node* root){
    Node* tmp = root;
    while(tmp->left != nullptr)
        tmp = tmp->left;
    return tmp;
}

Node* deleteNode(Node* node, string str) {
    if (node == nullptr)
        return node;
    int res = node->word.compare(str);
    if (res > 0)
        node->left = deleteNode(node->left, str);
    else if (res < 0)
        node->right = deleteNode(node->right, str);
    else {
        Node* temp;
        if(node->left == nullptr){
            temp = node->right;
            free(node);
            return temp;
        }

        else if(node->right == nullptr){
            temp = node->left;
            free(node);
            return temp;
        }

        temp = findMinNode(node->right);
        node->word = temp->word;
        node->meaning = temp->meaning;
        node->right = deleteNode(node->right, temp->word);
    }
    return node;
}

void printLeafNodes(Node* root) {
    if (root == nullptr) return;

    if (root->left == nullptr && root->right == nullptr) 
        cout << root->word << " " << root->meaning << endl;
    
    printLeafNodes(root->left);
    printLeafNodes(root->right);
}

void findElement(Node* root, string str) {
    Node *temp;
    int flag = 0, res = 0;
    if (root == nullptr) {
            printf("Binary Search Tree is out of station!!\n");
            return;
    }
    temp = root;
    while (temp) {
        if ((res = temp->word.compare(str)) == 0) {
            cout << "Word   : " << str << "\t";
            cout << "Meaning: "<< temp->meaning;
            flag = 1;
            break;
        }
        temp = (res > 0) ? temp->left : temp->right;
    }
    if (!flag)
        printf("Search Element not found in Binary Search Tree\n");
    return;
}


void preorderTraversal(Node *node) {
    if(node == nullptr) return;
    
    cout << "Word: " << node->word << "\t";
    cout << "Meaning: "<< node->meaning;
    cout << "\n";
    
    preorderTraversal(node->left);
    preorderTraversal(node->right);	
}

void addlastline(string textToAdd){
        fstream file("engkor.dat", ios::in | ios::out | ios::ate);

    if (!file.is_open()) {
        cout << "파일을 열 수 없습니다." << endl;
        return;
    }

    // 파일 크기 확인
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();

    // 파일이 비어있지 않은 경우에만 빈 줄 추가
    if (fileSize > 0) {
        file << "\n" << textToAdd;
        cout << "파일 추가 완료" << endl;
    } else {
        file << textToAdd;
        cout << "파일 추가 완료 (파일이 비어있음)" << endl;
    }

    file.close();
  }


void deletespecificline(string textToDelete){
    string filename = "engkor.dat";
    
    // 임시 파일 이름
    string tempFilename = "temp.txt";

    ifstream inputFile(filename);
    ofstream tempFile(tempFilename);

    if (!inputFile.is_open() || !tempFile.is_open()) {
        cerr << "파일을 열 수 없습니다." << endl;
    }

    string line;

    while (getline(inputFile, line)) {
        // 특정 문자열을 포함하지 않는 줄만 새 파일에 쓰기
        if (line.find(textToDelete) == string::npos) {
                if(!line.empty())
                        tempFile << line << "\n";
        }
    }

    // 파일 닫기
    inputFile.close();
    tempFile.close();

    // 원본 파일 삭제
    if (remove(filename.c_str()) != 0) {
        cerr << "원본 파일을 삭제할 수 없습니다." << endl;   
    }
    // 임시 파일 이름을 원래 파일 이름으로 변경
    if (rename(tempFilename.c_str(), filename.c_str()) != 0) {
        cerr << "파일 이름을 변경할 수 없습니다." << endl;
    }
}  


int main(void) {
    Node *root = nullptr;
    ifstream readFile;
	readFile.open("engkor.dat");

	if (readFile.is_open()){
		while (!readFile.eof()) {       // 파일의 끝까지
			string tmp1;
			getline(readFile, tmp1);    // 한줄씩 읽어서 tmp1에 저장
			if (tmp1.empty()) continue; // 빈 줄이면 건너뜀
			int pos = tmp1.find('_');
            string KR = tmp1.substr(pos + 1);   // KR : _다음부터 끝까지
            string EN = tmp1.substr(0, pos);    // EN : 처음부터 _ 전까지
        	root = insert(root, EN, KR);        // 인서트한다
            cout << "EN: " << EN << "KR: " << KR <<"파일추가 성공\n";
		}
		readFile.close();
	}
	
        
	int ch;
	string word, meaning, temp;
	while (1) {
        cout << "\n1. Insertion\t2. Deletion\n";
        cout << "3. Searching\t4. Traversal\n";
        cout << "5. FindLeafNode\t6. Exit\nEnter ur choice:";
        cin >> ch;
        switch (ch) {
            case 1:
                cout << "Word to insert: ";
                cin >> word;
                cout << "Meaning:";
                cin >> meaning;
                root = insert(root, word, meaning);
                temp = word;            // 파일에 넣기위해 temp에 워드저장
                temp.push_back('_');    // 영어_
                temp += meaning;        // 영어_한국어
                addlastline(temp);      // 데이터파일에 추가함
                break;
            case 2:
                cout << "Enter the word to delete:";
                cin >> word;
                deleteNode(root, word);     
                deletespecificline(word);   // 파일에서 해당 워드 가지고 있는 라인 삭제함
                break;

            case 3:
                cout << "Enter the search word: ";
                cin >> word;
                findElement(root, word);
                break;

            case 4:
                preorderTraversal(root);
                break;

            case 5:
                printLeafNodes(root);
                break;
            case 6:
                exit(0);
            default:
                cout << "You have entered wrong option\n";
                break;
		}
	}
	return 0;
}
