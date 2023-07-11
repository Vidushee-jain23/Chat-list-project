#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;


class Chat {
public:
    string senderName;
    string receiverName;
    string message;
};

class Node {
public:
    Chat chat;
    int height;
    Node* left;
    Node* right;

    Node(const Chat& chat) : chat(chat), height(1), left(nullptr), right(nullptr) {}
};

// Function to get the height of a node
int getHeight(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Function to update the height of a node
void updateHeight(Node* node) {
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

// Function to perform a right rotation
Node* rotateRight(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

// Function to perform a left rotation
Node* rotateLeft(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

// Function to balance the AVL tree
Node* balance(Node* node) {
    if (getHeight(node->left) - getHeight(node->right) > 1) {
        if (getHeight(node->left->left) >= getHeight(node->left->right))
            node = rotateRight(node);
        else {
            node->left = rotateLeft(node->left);
            node = rotateRight(node);
        }
    }
    else if (getHeight(node->right) - getHeight(node->left) > 1) {
        if (getHeight(node->right->right) >= getHeight(node->right->left))
            node = rotateLeft(node);
        else {
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
        }
    }
    return node;
}

// Function to insert a chat into the AVL tree
Node* insert(Node* node, const Chat& chat) {
    if (node == nullptr) {
        Node* newNode = new Node(chat);
        return newNode;
    }

    if (chat.senderName < node->chat.senderName)
        node->left = insert(node->left, chat);
    else
        node->right = insert(node->right, chat);

    updateHeight(node);
    node = balance(node);
    return node;
}

// Function to display the chats in the AVL tree (in-order traversal)
void displayChats(Node* node) {
    if (node == nullptr)
        return;

    displayChats(node->left);
    cout << "Sender: " << node->chat.senderName << ", Receiver: " << node->chat.receiverName
              <<endl<<"Message: " << node->chat.message << endl;
    displayChats(node->right);
}

// Function to free the memory allocated for the AVL tree
void destroyTree(Node* node) {
    if (node == nullptr)
        return;

    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

int main() {
    unordered_set<string> contacts;
    vector<Chat> chats;
    Node* root = nullptr;

    // Collect user's contacts
    cout << "Enter the names of the persons in your contact list (press Enter to finish):" << endl;
    while (true) {
        string contact;
        getline(cin, contact);
        if (contact == "stop")
            break;
        contacts.insert(contact);
    }

    // Display WhatsApp heading and contact names
    cout <<endl<< "WHATSAPP" << endl;
    for (const string& contact : contacts) {
        cout << contact << endl;
    }

    // Chat loop
    while (true) {
        string senderName;
        cout <<endl<<"Enter the name of the sender (press stop to finish):" << endl;
        getline(cin, senderName);
        if (senderName == "stop")
            break;
        if (contacts.find(senderName) == contacts.end()) {
            cout << "Invalid sender name. Please try again." << endl;
            continue;
        }

        string receiverName;
        cout <<endl<<"Enter the name of the receiver:" << endl;
        getline(cin, receiverName);
        if (contacts.find(receiverName) == contacts.end()) {
            cout << "Invalid receiver name. Please try again." << endl;
            continue;
        }

        string message;
        cout <<endl<< "Enter the message:" << endl;
        getline(cin, message);

        // Update the chat listChat chat;
        Chat conversation;
        conversation.senderName = senderName;
        conversation.receiverName = receiverName;
        conversation.message = message;
        chats.push_back(conversation);
        root = insert(root, conversation);
    }

    // Display all the conversations
    cout <<endl<<"WHATSAPP CONVERSATIONS:" << endl;
    displayChats(root);

    // Clean up the tree
    destroyTree(root);

    return 0;
}
