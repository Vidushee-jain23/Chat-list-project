#include <iostream>
#include <string>
#include <list>
#include <iomanip>
#include <ctime>
using namespace std;

class Chat {
private:
    std::string senderName;
    std::string receiverName;
    std::string message;
    std::time_t timestamp;
    bool read;

public:
    Chat(const std::string& sender, const std::string& receiver, const std::string& msg)
        : senderName(sender), receiverName(receiver), message(msg), timestamp(std::time(nullptr)), read(false) {}

    const std::string& getSenderName() const {
        return senderName;
    }

    const std::string& getReceiverName() const {
        return receiverName;
    }

    const std::string& getMessage() const {
        return message;
    }

    std::time_t getTimestamp() const {
        return timestamp;
    }

    bool isRead() const {
        return read;
    }

    void markAsRead() {
        read = true;
    }
};

class WhatsAppChatList {
private:
    std::list<Chat> chats;

public:
    void addChat(const Chat& chat) {
        chats.push_back(chat);
    }

    void displayChats() {
        std::cout <<endl<<"WHATSAPP CONVERSATIONS:" << std::endl;
        int index = 1;
        for (auto& chat : chats) {
            std::cout << "Index: " << index << std::endl;
            std::cout << "Sender: " << chat.getSenderName() << ", Receiver: " << chat.getReceiverName() << std::endl;
            std::cout << "Message: " << chat.getMessage() << std::endl;

            std::time_t timestamp = chat.getTimestamp();
            std::tm* timestampStruct = std::localtime(&timestamp);
            char buffer[80];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timestampStruct);

            std::cout << "Timestamp: " << buffer << std::endl;
            std::cout << "Read: " << (chat.isRead() ? "Yes" : "No") << std::endl;
            std::cout << std::endl;

            index++;
        }
    }

    void markChatAsRead(int index) {
        if (index >= 1 && index <= chats.size()) {
            auto it = std::next(chats.begin(), index - 1);
            it->markAsRead();
            std::cout << "Chat marked as read." << std::endl;
        }
        else {
            std::cout << "Invalid chat index." << std::endl;
        }
    }

    void searchChatsBySenderReceiver(const std::string& name) {
        std::cout << "Search results for '" << name << "':" << std::endl;
        int index = 1;
        bool found = false;
        for (auto& chat : chats) {
            if (chat.getSenderName() == name || chat.getReceiverName() == name) {
                std::cout << "Index: " << index << std::endl;
                std::cout << "Sender: " << chat.getSenderName() << ", Receiver: " << chat.getReceiverName() << std::endl;
                std::cout << "Message: " << chat.getMessage() << std::endl;
                std::cout << std::endl;
                found = true;
            }
            index++;
        }
        if (!found) {
            std::cout << "No chats found with '" << name << "'." << std::endl;
        }
    }

    void deleteChat(int index) {
        if (index >= 1 && index <= chats.size()) {
            auto it = std::next(chats.begin(), index - 1);
            chats.erase(it);
            std::cout << "Chat deleted." << std::endl;
        }
        else {
            std::cout << "Invalid chat index." << std::endl;
        }
    }
};

int main() {
    WhatsAppChatList chatList;

    while (true) {
        std::cout << "1. Add Chat" << std::endl;
        std::cout << "2. Display Chats" << std::endl;
        std::cout << "3. Mark Chat as Read" << std::endl;
        std::cout << "4. Search Chats by Sender/Receiver Name" << std::endl;
        std::cout << "5. Delete Chat" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(10000, '\n');

        if (choice == 1) {
            std::string senderName;
            std::cout << "Enter the name of the sender: ";
            std::getline(std::cin, senderName);

            std::string receiverName;
            std::cout << "Enter the name of the receiver: ";
            std::getline(std::cin, receiverName);

            std::string message;
            std::cout << "Enter the message: ";
            std::getline(std::cin, message);

            Chat chat(senderName, receiverName, message);
            chatList.addChat(chat);
        }
        else if (choice == 2) {
            chatList.displayChats();
        }
        else if (choice == 3) {
            std::cout << "Enter the chat index to mark as read: ";
            int index;
            std::cin >> index;
            std::cin.ignore(10000, '\n');
            chatList.markChatAsRead(index);
        }
        else if (choice == 4) {
            std::string name;
            std::cout << "Enter the sender/receiver name to search: ";
            std::getline(std::cin, name);
            chatList.searchChatsBySenderReceiver(name);
        }
        else if (choice == 5) {
            std::cout << "Enter the chat index to delete: ";
            int index;
            std::cin >> index;
            std::cin.ignore(10000, '\n');
            chatList.deleteChat(index);
        }
        else if (choice == 6) {
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}

