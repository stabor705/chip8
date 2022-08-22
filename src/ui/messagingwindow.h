#ifndef MESSAGINGWINDOW_H
#define MESSAGINGWINDOW_H

#include <string>
#include <vector>

class MessagingWindow {
public:
    void show(float width, float height);
    void add_message(const std::string &msg);
private:
    std::vector<std::string> messages;
};

#endif // MESSAGINGWINDOW_H
