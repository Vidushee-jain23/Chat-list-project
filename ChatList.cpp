#include <iostream>
#include <string>
#include <ctime>
using namespace std;

class Chat
{
private:
    string senderName;
    string receiverName;
    string message;
    time_t timestamp;
    bool read;

public:
    Chat(const string &sender, const string &receiver, const string &msg)
        : senderName(sender), receiverName(receiver), message(msg), timestamp(time(nullptr)), read(false) {}

    const string &getSenderName() const
    {
        return senderName;
    }

    const string &getReceiverName() const
    {
        return receiverName;
    }

    const string &getMessage() const
    {
        return message;
    }

    time_t getTimestamp() const
    {
        return timestamp;
    }

    bool isRead() const
    {
        return read;
    }

    void markAsRead()
    {
        read = true;
    }
};

struct AVLNode
{
    Chat chat;
    int height;
    int size;
    AVLNode *left;
    AVLNode *right;

    AVLNode(const Chat &chat) : chat(chat), height(1), size(1), left(nullptr), right(nullptr) {}
};

class WhatsAppChatList
{
private:
    AVLNode *root;
    AVLNode *deleteChatNode(AVLNode *node, int index)
    {
        if (node == nullptr)
        {
            return node;
        }

        int leftSubtreeSize = getSize(node->left);
        int currentChatIndex = leftSubtreeSize + 1;

        if (index == currentChatIndex)
        {
            if (node->left == nullptr && node->right == nullptr)
            {
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr)
            {
                AVLNode *temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                AVLNode *temp = node->left;
                delete node;
                return temp;
            }
            else
            {
                AVLNode *successor = findMinNode(node->right);
                node->chat = successor->chat;
                node->right = deleteChatNode(node->right, 1); 
            }
        }
        else if (index < currentChatIndex)
        {
            node->left = deleteChatNode(node->left, index);
        }
        else
        {
            node->right = deleteChatNode(node->right, index - currentChatIndex);
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalanceFactor(node);

        if (balance > 1 && getBalanceFactor(node->left) >= 0)
        {
            return rotateRight(node);
        }
        if (balance > 1 && getBalanceFactor(node->left) < 0)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && getBalanceFactor(node->right) <= 0)
        {
            return rotateLeft(node);
        }
        if (balance < -1 && getBalanceFactor(node->right) > 0)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    void MarkChatAsRead(AVLNode *node, int &index, int &currentIdx)
    {
        if (node == nullptr)
        {
            return;
        }

        MarkChatAsRead(node->left, index, currentIdx);
        currentIdx++;
        if (currentIdx == index)
        {
            node->chat.markAsRead();
            cout << "Chat marked as read." << endl;
        }
        MarkChatAsRead(node->right, index, currentIdx);
    }

    void searchChatsBySenderReceiverHelper(AVLNode *node, const string &name, int &index, bool &found)
    {
        if (node != nullptr)
        {
            searchChatsBySenderReceiverHelper(node->left, name, index, found);
            if (node->chat.getSenderName() == name || node->chat.getReceiverName() == name)
            {
                cout << "Index: " << index << endl;
                cout << "Sender: " << node->chat.getSenderName() << ", Receiver: " << node->chat.getReceiverName() << endl;
                cout << "Message: " << node->chat.getMessage() << endl;
                cout << endl;
                found = true;
            }
            index++;
            searchChatsBySenderReceiverHelper(node->right, name, index, found);
        }
    }

    // AVL tree functions
    int getHeight(AVLNode *node);
    int getSize(AVLNode *node);
    int getBalanceFactor(AVLNode *node);
    AVLNode *rotateLeft(AVLNode *node);
    AVLNode *rotateRight(AVLNode *node);
    AVLNode *insertNode(AVLNode *node, const Chat &chat);
    void inOrderTraversal(AVLNode *node);
    void deleteTree(AVLNode *node);
    AVLNode *findMinNode(AVLNode *node);

public:
    WhatsAppChatList() : root(nullptr) {}
    
    // Public functions for managing chats
    void addChat(const Chat &chat);
    void displayChats();
    void markChatAsRead(int index);
    void searchChatsBySenderReceiver(const string &name);
    void deleteChat(int index);

    // Destructor
    ~WhatsAppChatList();
};

// AVL tree functions implementation
int WhatsAppChatList::getHeight(AVLNode *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->height;
}

int WhatsAppChatList::getSize(AVLNode *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->size;
}

int WhatsAppChatList::getBalanceFactor(AVLNode *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode *WhatsAppChatList::rotateLeft(AVLNode *node)
{
    AVLNode *newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    newRoot->height = max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

    return newRoot;
}

AVLNode *WhatsAppChatList::rotateRight(AVLNode *node)
{
    AVLNode *newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    newRoot->height = max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

    return newRoot;
}

AVLNode *WhatsAppChatList::insertNode(AVLNode *node, const Chat &chat)
{
    if (node == nullptr)
    {
        return new AVLNode(chat);
    }

    if (chat.getTimestamp() < node->chat.getTimestamp())
    {
        node->left = insertNode(node->left, chat);
    }
    else if (chat.getTimestamp() > node->chat.getTimestamp())
    {
        node->right = insertNode(node->right, chat);
    }

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

    int balance = getBalanceFactor(node);

    if (balance > 1 && chat.getTimestamp() < node->left->chat.getTimestamp())
    {
        return rotateRight(node);
    }

    if (balance < -1 && chat.getTimestamp() > node->right->chat.getTimestamp())
    {
        return rotateLeft(node);
    }

    if (balance > 1 && chat.getTimestamp() > node->left->chat.getTimestamp())
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && chat.getTimestamp() < node->right->chat.getTimestamp())
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void WhatsAppChatList::inOrderTraversal(AVLNode *node)
{
    if (node == nullptr)
    {
        return;
    }

    inOrderTraversal(node->left);
    cout << "Sender: " << node->chat.getSenderName() << ", Receiver: " << node->chat.getReceiverName() << endl;
    cout << "Message: " << node->chat.getMessage() << endl;

    time_t timestamp = node->chat.getTimestamp();
    tm *timestampStruct = localtime(&timestamp);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timestampStruct);

    cout << "Timestamp: " << buffer << endl;
    cout << "Read: " << (node->chat.isRead() ? "Yes" : "No") << endl;
    cout << endl;

    inOrderTraversal(node->right);
}

AVLNode *WhatsAppChatList::findMinNode(AVLNode *node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->left == nullptr)
    {
        return node;
    }
    return findMinNode(node->left);
}

void WhatsAppChatList::deleteTree(AVLNode *node)
{
    if (node == nullptr)
    {
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Public functions for managing chats
void WhatsAppChatList::addChat(const Chat &chat)
{
    root = insertNode(root, chat);
}

void WhatsAppChatList::displayChats()
{
    cout << endl << "WHATSAPP CONVERSATIONS:" << endl;
    inOrderTraversal(root);
}

void WhatsAppChatList::markChatAsRead(int index)
{
    int currentIdx = 0;
    MarkChatAsRead(root, index, currentIdx);
}

void WhatsAppChatList::searchChatsBySenderReceiver(const string &name)
{
    cout << "Search results for '" << name << "':" << endl;
    int index = 1;
    bool found = false;
    searchChatsBySenderReceiverHelper(root, name, index, found);
    if (!found)
    {
        cout << "No chats found with '" << name << "'." << endl;
    }
}

void WhatsAppChatList::deleteChat(int index)
{
    int treeSize = getSize(root);
    if (index >= 1 && index <= treeSize)
    {
        root = deleteChatNode(root, index);
        cout << "Chat deleted." << endl;
    }
    else
    {
        cout << "Invalid chat index." << endl;
    }
}

WhatsAppChatList::~WhatsAppChatList()
{
    deleteTree(root);
}

int main()
{
    WhatsAppChatList chatList;

    while (true)
    {
        cout << "1. Add Chat" << endl;
        cout << "2. Display Chats" << endl;
        cout << "3. Mark Chat as Read" << endl;
        cout << "4. Search Chats by Sender/Receiver Name" << endl;
        cout << "5. Delete Chat" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice == 1)
        {
            string senderName;
            cout << "Enter the name of the sender: ";
            getline(cin, senderName);

            string receiverName;
            cout << "Enter the name of the receiver: ";
            getline(cin, receiverName);

            string message;
            cout << "Enter the message: ";
            getline(cin, message);

            Chat chat(senderName, receiverName, message);
            chatList.addChat(chat);
        }
        else if (choice == 2)
        {
            chatList.displayChats();
        }
        else if (choice == 3)
        {
            cout << "Enter the chat index to mark as read: ";
            int index;
            cin >> index;
            cin.ignore(10000, '\n');
            chatList.markChatAsRead(index);
        }
        else if (choice == 4)
        {
            string name;
            cout << "Enter the sender/receiver name to search: ";
            getline(cin, name);
            chatList.searchChatsBySenderReceiver(name);
        }
        else if (choice == 5)
        {
            cout << "Enter the chat index to delete: ";
            int index;
            cin >> index;
            cin.ignore(10000, '\n');
            chatList.deleteChat(index);
        }
        else if (choice == 6)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    }

    return 0;
}
